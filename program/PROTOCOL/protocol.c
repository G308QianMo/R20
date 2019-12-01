#include "include.h"

/**
*@file    protocol.c
*@author  蒋林杰
*@version 1.0
*@date    2019.11.23
*@brief   用于不同芯片之间的数据交流
**/

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
#define elmo_delay 200

unsigned char u8_data_to_send[100];	//数据发送缓存
static unsigned char static_u8_RxBuffer[100];//接收数据缓存
static int static_int_Processed_Data[50];//处理后数据缓存

//该协议的各帧的定义
/*主机到从机发送帧
0     0x ee //帧头
1     0x ef //帧头
2     0x xx //功能字：1：自检
                 2：速度环
                 3：位置环
3     0x xx //帧长度
4~n-3 0x xx //数据帧
n-2   0x xx //sum校验和
n-1   0x fe //帧尾
n     0x ff //帧尾
*/


/**
 *@function Send_Data
 *@param    unsigned char *data 需传送至外部的数据
            unsigned char u8_length 需传送至外部的数据的个数
 *@brief    用于将数据传输出去API，可更改内部的数据传出方式
 *@retval   无
**/
void Send_Data(unsigned char *data , unsigned char u8_length)
{
	for(unsigned char i = 0;i < u8_length;i ++)
	{
    USART_SendData(USART1,data[i]);
		delay_us(90);
	}
}

/**
 *@function Data_Splitting_And_Sending
 *@param    int *data 需拆分的数据
            unsigned char u8_length 需拆分数据的个数
            unsigned char function_num 确定当前拆分数据组的功能字
 *@brief    将需传输的数据拆分成定义的格式，并添加入相关信息
 *@retval   无
**/
void Data_Splitting_And_Sending(int * data,unsigned char u8_length,unsigned char function_num)
{ unsigned char u8_cnt = 0;//计数
	unsigned char u8_sum = 0;//校验和
	u8_data_to_send[u8_cnt ++] = 0xee;//帧头
	u8_data_to_send[u8_cnt ++] = 0xef;//帧头
	
	u8_data_to_send[u8_cnt ++] = function_num;//功能字
	u8_data_to_send[u8_cnt ++] = u8_length*2+4;//数据长度

	for(int i = 0;i < u8_length;i ++)//数据拆分
	{	
		u8_data_to_send[u8_cnt ++] = BYTE1(data[i]);
		u8_data_to_send[u8_cnt ++] = BYTE0(data[i]);
	}

	for(unsigned char i = 0;i < u8_cnt;i ++)
	{
		u8_sum += u8_data_to_send[i];//校验和
	}

	u8_data_to_send[u8_cnt ++] = u8_sum;//得到校验和
	
	u8_data_to_send[u8_cnt ++] = 0xfe;//帧尾
	u8_data_to_send[u8_cnt ++] = 0xff;//帧尾

	Send_Data(u8_data_to_send,u8_cnt);//数据发送
}

/**
 *@function Data_Check
 *@param    unsigned char *data 接收到的数据
            unsigned char num 接收到的数据的个数
 *@brief    将由Data_Recieving函数接收到的数据进行相关信息检查，若无误则进行下一步数据处理，调用 Data_Processing函数
             若有误，根据错误类型反馈调用 Data_Rerequest给数据发送端进行数据的再次请求
 *@retval   无
**/
void Data_Check(unsigned char *data,unsigned char num)
{ unsigned char Check_flag =0;
  unsigned char u8_sum = 0;

	if(data[0]     != 0xee || data[1]       != 0xef) Check_flag = 1;//帧头错误
	if(data[num-2] != 0xfe || data[num - 1] != 0xff) Check_flag = 2;//帧尾错误

	for(unsigned char i = 0; i < data[3]; i++)//获得接收数据的校验和
	{
		u8_sum += data[i];
	}

	if(u8_sum != data[num - 3]) Check_flag = 3;//校验和错误
	
       if(Check_flag == 0)//若接收不出现错误，则进行下一步数据处理
	{
			Data_Processing(static_u8_RxBuffer,num);//调用数据处理函数
	}
	else//数据接收出现错误,根据不同的返回值确定错误类型
	{
		Data_Rerequest(Check_flag);//若出现数据错误，调用数据再请求函数
	}
}

/**
 *@function Data_Recieving
 *@param    unsigned char data 来自于接收中断的数据
 *@brief    按步骤接收数据，接收完成后调用Data_Check函数进行数据检查，检查无误后进行数据处理
 *@retval   无
**/
static unsigned char static_u8_data_len = 0,static_u8_data_cnt = 0;
static unsigned char state = 0;
void Data_Recieving(unsigned char data)//数据接收，无需做任何改动
{

	if(state == 0&&data == 0xee)//帧头1
	{
		state = 1;
		static_u8_RxBuffer[0] = data;
	}
	else if(state == 1&&data == 0xef)//帧头2
	{
		state = 2;
		static_u8_RxBuffer[1] = data;
	}
	else if(state == 2 && data < 0xff)//功能帧
	{
		state = 3;
		static_u8_RxBuffer[2] = data;
	}
	else if(state == 3 && data < 100)//数据长度
	{
		state = 4;
		static_u8_RxBuffer[3] = data;
		static_u8_data_len = data - 4;
		static_u8_data_cnt = 0;
	}
	else if(state == 4 && static_u8_data_len > 0)//接收数据
	{
		static_u8_data_len --;
		static_u8_RxBuffer[4 + static_u8_data_cnt ++] = data;
		if(static_u8_data_len == 0)
			 state = 5;
	}
	else if(state == 5)//接收校验和
	{
		state = 6;
		static_u8_RxBuffer[ 4 + static_u8_data_cnt ++] = data;
	}else if(state == 6 && data == 0xfe)//接收帧尾1
	{
		state = 7;
		static_u8_RxBuffer[4 + static_u8_data_cnt ++] = data;
	}
	else if(state == 7 && data == 0xff)//接收帧尾2
	{
		state = 0;
		static_u8_RxBuffer[4 + static_u8_data_cnt ++] = data;
	  Data_Check(static_u8_RxBuffer, 4 + static_u8_data_cnt);//数据处理
	}
	else
		state = 0;
}

/**
 *@function Data_Processing
 *@param    unsigned char *data 来自于接收中断的数据
            unsigned char length 接收到的数据的长度
 *@brief    将接收无误后的数据，根据功能字进行相应的数据处理
 *@retval   无
**/
void Data_Processing(unsigned char *data,unsigned char length)
{
	switch(data[2])//data[2]为该帧数据的功能字帧
	{
		case 0x10:
			for(unsigned char i = 0; i < (data[3] - 4) / 2; i ++)
		{
			static_int_Processed_Data[i]=(int)(data[4 + i * 2] << 8 | data[4 + i * 2 + 1]);
		}
		 break;
	}
}

/**
 *@function Data_Rerequest
 *@param    unsigned char erro_type 定义好的错误类型
 *@brief    若接收的数据有误，则发送数据再次请求
 *@retval   无
**/
void Data_Rerequest(unsigned char erro_type)
{
	unsigned char u8_sum=0;

	u8_data_to_send[0] = 0xee;
	u8_data_to_send[1] = 0xef;//帧头
	
	u8_data_to_send[2] = 0xe0|erro_type;//功能字
	u8_data_to_send[3] = 0;//数据长度

	for(unsigned char i = 0; i < 3;i ++)
	{
		u8_sum += u8_data_to_send[i];
	}

	u8_data_to_send[3] = u8_sum;//得到校验和
	u8_data_to_send[4] = 0xfe;//帧尾
	u8_data_to_send[5] = 0xff;//帧尾

	Send_Data(u8_data_to_send, 6);//发送数据请求
}

CanTxMsg TxMessage;
u8 CAN1_SendMsg(u8* msg, u32 TX_STD_ID)
{
    u8 mbox;
    u16 i = 0;
    TxMessage.StdId = TX_STD_ID;				 //使用的标准ID		 
    TxMessage.IDE = CAN_ID_STD;					 //标准模式
    TxMessage.RTR = CAN_RTR_DATA;				 //发送的是数据
    TxMessage.DLC = 8;							     //数据长度为2字节
    for (i = 0; i<8; i++)
    {
        TxMessage.Data[i] = msg[i];
    }
    mbox = CAN_Transmit(CAN1, &TxMessage);
    i = 0;
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i<0XFFF))i++;	//等待发送结束
    if (i >= 0XFFF)return 1;//发送失败
    return 0;		         //发送成功
}


void ELMO_Single_Enable(uint32_t elmo_id)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE0;
    Tx_message.Data[1] = 0x55;
    Tx_message.Data[2] = 0x55;
    Tx_message.Data[3] = 0x55;
    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x0E;
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Single_Init(uint32_t elmo_id)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE1;
    Tx_message.Data[1] = 0x55;
    Tx_message.Data[2] = 0x55;
    Tx_message.Data[3] = 0x55;
    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x1E;
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Single_Disenable(uint32_t elmo_id)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE2;
    Tx_message.Data[1] = 0x55;
    Tx_message.Data[2] = 0x55;
    Tx_message.Data[3] = 0x55;
    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x2E;
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Single_STOP(uint32_t elmo_id)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE3;
    Tx_message.Data[1] = 0x55;
    Tx_message.Data[2] = 0x55;
    Tx_message.Data[3] = 0x55;
    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x3E;
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Single_BEGIN(uint32_t elmo_id)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE4;
    Tx_message.Data[1] = 0x55;
    Tx_message.Data[2] = 0x55;
    Tx_message.Data[3] = 0x55;
    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x4E;
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
	CAN1_SendMsg(Tx_message.Data,can_id);
 
}


void ELMO_Single_Velocity(uint32_t elmo_id,int32_t v1)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE5;
//    Tx_message.Data[1] = 0x55;
//    Tx_message.Data[2] = 0x55;
    Tx_message.Data[3] = 0x55;
    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x5E;
	
	if(v1<0)
    {
        v1=-v1;
        v1=v1|0x8000;
        Tx_message.Data[1] = (u8)((v1&0xff00) >> 8);
        Tx_message.Data[2] = (u8)(v1&0x00ff);
    }
    else
    {
        Tx_message.Data[1] = (u8)((v1&0xff00) >> 8);
        Tx_message.Data[2] = (u8)(v1&0x00ff);
    }
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Single_PTP_PA(uint32_t elmo_id,int32_t p1)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE6;
//    Tx_message.Data[1] = 0x55;
//    Tx_message.Data[2] = 0x55;
//    Tx_message.Data[3] = 0x55;
//    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x6E;
	
	if(p1<0)
    {
        p1=-p1;
        p1=p1|0x80000000;
        Tx_message.Data[1] = (u8)((p1&0xff000000) >> 24);
        Tx_message.Data[2] = (u8)((p1&0x00ff0000) >> 16);
		Tx_message.Data[3] = (u8)((p1&0x0000ff00) >> 8);
		Tx_message.Data[4] = (u8)(p1&0x000000ff);
    }
    else
    {
        Tx_message.Data[1] = (u8)((p1&0xff000000) >> 24);
        Tx_message.Data[2] = (u8)((p1&0x00ff0000) >> 16);
		Tx_message.Data[3] = (u8)((p1&0x0000ff00) >> 8);
		Tx_message.Data[4] = (u8)(p1&0x000000ff);
    }
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Single_PTP_PR(uint32_t elmo_id,int32_t p2)
{
    uint32_t can_id = 0x000;
	
    CanTxMsg Tx_message;
    
//    Tx_message.IDE = CAN_ID_STD;    //标准帧
//    Tx_message.RTR = CAN_RTR_DATA;  //数据帧
//    Tx_message.DLC = 0x08;          //帧长度为8

	can_id = elmo_id;
    
//    Tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    Tx_message.Data[0] = 0xE7;
//    Tx_message.Data[1] = 0x55;
//    Tx_message.Data[2] = 0x55;
//    Tx_message.Data[3] = 0x55;
//    Tx_message.Data[4] = 0x55;
    Tx_message.Data[5] = 0x55;
    Tx_message.Data[6] = 0x55;
    Tx_message.Data[7] = 0x7E;
	
	if(p2<0)
    {
        p2=-p2;
        p2=p2|0x80000000;
        Tx_message.Data[1] = (u8)((p2&0xff000000) >> 24);
        Tx_message.Data[2] = (u8)((p2&0x00ff0000) >> 16);
		Tx_message.Data[3] = (u8)((p2&0x0000ff00) >> 8);
		Tx_message.Data[4] = (u8)(p2&0x000000ff);
    }
    else
    {
        Tx_message.Data[1] = (u8)((p2&0xff000000) >> 24);
        Tx_message.Data[2] = (u8)((p2&0x00ff0000) >> 16);
		Tx_message.Data[3] = (u8)((p2&0x0000ff00) >> 8);
		Tx_message.Data[4] = (u8)(p2&0x000000ff);
    }
    
//    can_tx_success_flag = 0;
//    CAN_Transmit(CAN1,&Tx_message);
//    
//    CAN_Time_Out = 0;
//    while(can_tx_success_flag == 0)
//    {
//        CAN_Delay_Us(1);
//        CAN_Time_Out++;
//        if(CAN_Time_Out>100)
//        {
//            break;
//        }
//    }
	CAN1_SendMsg(Tx_message.Data,can_id);
}


void ELMO_Enable(void)
{
	ELMO_Single_Enable(elmo1_id);
	delay_us(elmo_delay);
	ELMO_Single_Enable(elmo2_id);
	delay_us(elmo_delay);
	ELMO_Single_Enable(elmo3_id);
	delay_us(elmo_delay);
	ELMO_Single_Enable(elmo4_id);
	delay_us(elmo_delay);
}


void ELMO_Init(void)
{
	ELMO_Single_Init(elmo1_id);
	delay_us(elmo_delay);
	ELMO_Single_Init(elmo2_id);
	delay_us(elmo_delay);
	ELMO_Single_Init(elmo3_id);
	delay_us(elmo_delay);
	ELMO_Single_Init(elmo4_id);//后期需加delay
	delay_us(elmo_delay);
}


void ELMO_Disenable(void)
{
	ELMO_Single_Disenable(elmo1_id);
	delay_us(elmo_delay);
	ELMO_Single_Disenable(elmo2_id);
	delay_us(elmo_delay);
	ELMO_Single_Disenable(elmo3_id);
	delay_us(elmo_delay);
	ELMO_Single_Disenable(elmo4_id);
	delay_us(elmo_delay);
}


void ELMO_STOP(void)
{
	ELMO_Single_STOP(elmo1_id);
	delay_us(elmo_delay);
	ELMO_Single_STOP(elmo2_id);
	delay_us(elmo_delay);
	ELMO_Single_STOP(elmo3_id);
	delay_us(elmo_delay);
	ELMO_Single_STOP(elmo4_id);	
	delay_us(elmo_delay);
}


void ELMO_BEGIN(void)
{
	ELMO_Single_BEGIN(elmo1_id);
	delay_us(elmo_delay);
	ELMO_Single_BEGIN(elmo2_id);
	delay_us(elmo_delay);
	ELMO_Single_BEGIN(elmo3_id);
	delay_us(elmo_delay);
	ELMO_Single_BEGIN(elmo4_id);	
	delay_us(elmo_delay);
}


void ELMO_Velocity(int32_t v1,int32_t v2,int32_t v3,int32_t v4)
{
	ELMO_Single_Velocity(elmo1_id,v1);	
	delay_us(elmo_delay);
	ELMO_Single_Velocity(elmo2_id,v2);	
	delay_us(elmo_delay);
	ELMO_Single_Velocity(elmo3_id,v3);	
	delay_us(elmo_delay);
	ELMO_Single_Velocity(elmo4_id,v4);
	delay_us(elmo_delay);
}

