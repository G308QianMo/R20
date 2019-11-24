#include "include.h"

static void GYRO_Reset_Module(void);
/**
*@function GYRO_Init
*@param    void
*@brief    �����ǽ��ж�ν��г�ʼ������ʼ������ֵ��Ƕ�ֵΪ��
*@retval   void
**/
void GYRO_Init(void)
{

    GYRO_Reset_Module(); //��λ
	
    GYRO_Set_Angle(0);   //д���ֵ 
    GYRO_Set_Position(0, 0);
	
    delay_ms(20);        //��ʱ
	
    GYRO_Set_Angle(0);   //�ٴ�д���ֵ  
    GYRO_Set_Position(0, 0);
	
    delay_ms(20);        //��ʱ
	
    GYRO_Set_Angle(0);   //�ٴ�д���ֵ  
    GYRO_Set_Position(0, 0);
}

/**
*@function GYRO_Set_Angle
*@param    angle���趨�Ƕ�ֵ
*@brief    ���������ǽǶ�ֵ����ʼ��ʱ�Ƕȹ���ʹ��
*@retval   void
**/
void GYRO_Set_Angle(float angle)
{
    uDataConvert32TypeDef temp;
    CanTxMsg TxMessage;
    temp.float_form = angle;
    TxMessage.StdId = POS_CID;	  //
    TxMessage.DLC = 4;						//
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.Data[0] = temp.u8_form[0];
    TxMessage.Data[1] = temp.u8_form[1];
    TxMessage.Data[2] = temp.u8_form[2];
    TxMessage.Data[3] = temp.u8_form[3];
    CAN_Transmit(CAN_BUS, &TxMessage);
}

/**
*@function GYRO_Set_Position
*@param    pos_x���趨x����ֵ��pos_y���趨y����ֵ
*@brief    ��������������ֵ����ʼ������ֵ����ʱʹ��
*@retval   void
**/
void GYRO_Set_Position(int32_t pos_x, int32_t pos_y)
{
    CanTxMsg TxMessage;
    int32_t xx = 0, yy = 0;
    xx = -(int32_t)(pos_x* 12.8318);
    yy = -(int32_t)(pos_y* 12.5318);	
    TxMessage.StdId = POS_CID;				//??? ID
    TxMessage.DLC = 8;//??? 8
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.Data[0] = (u8)(xx >> 0);
    TxMessage.Data[1] = (u8)(xx >> 8);
    TxMessage.Data[2] = (u8)(xx >> 16);
    TxMessage.Data[3] = (u8)(xx >> 24);
    TxMessage.Data[4] = (u8)(yy >> 0);
    TxMessage.Data[5] = (u8)(yy >> 8);
    TxMessage.Data[6] = (u8)(yy >> 16);
    TxMessage.Data[7] = (u8)(yy >> 24);
    CAN_Transmit(CAN_BUS, &TxMessage);
}

/**
*@function GYRO_Reset_Module
*@param    void
*@brief    �����������λ
*@retval   void
**/
static void GYRO_Reset_Module(void)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = POS_CID;		
    TxMessage.DLC = 2;					
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.Data[0] = 0x55;
    TxMessage.Data[1] = 0xff;
    CAN_Transmit(CAN_BUS, &TxMessage);
}

