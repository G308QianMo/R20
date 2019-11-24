#include "include.h"

/**
*@file    init.c
*@author
*@version 1.0
*@date    2019.11.21
*@brief   进行硬件初始化
**/



/**
 *@function Init
 *@param    无
 *@brief    用于所有硬件初始化管理
 *@retval   无
**/
void Init(void)
{
	GPIO_INIT();
	Periph_Init();
}
/**
 *@function GPIO_INIT
 *@param    无
 *@brief    用于IO口初始化管理
 *@retval   无
**/
void GPIO_INIT(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

#if USE_LED
	//************************LED_INIT************************//
	//LED: PC0 PC1 PC2 PC3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//LED对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不上拉也不下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);//GPIO设置高，灯灭
#endif

#if USE_BEEP
	//************************Beep_INIT************************//
	//Beep: PA4	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//BEEP对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不上拉也不下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//GPIO置低，蜂鸣器关闭
#endif

#if USE_KEY
	//************************KEY_INIT************************//
	//KEY: PC13 PC14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13; //KEY1 KEY2对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif

#if USE_CAN_1
	//************************CAN1_INIT************************//
	//CAN1_RX: PA11 
	//CAN1_TX: PA12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能PORTA时钟	                   											 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA11,PA12

	//引脚复用映射配置
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); 	//GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 	//GPIOA12复用为CAN1
#endif

#if USE_CAN_2
	//************************CAN2_INIT************************//
	//CAN2_RX: PB12 
	//CAN2_TX: PB13
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//初始化GPIOB时钟

	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化PB12,PB13

	//引脚复用映射配置
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); 	//GPIOB12复用为CAN2 RX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 	//GPIOB13复用为CAN2 TX
#endif

#if USE_USART_2
	//************************USART2_INIT************************//
	//USART2_TX: PA2
	//USART2_RX: PA3

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART2时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2,PA3,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2，PA3

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3复用为USART2
#endif

#if USE_USART_3
	//************************USART3_INIT************************//
	//USART3_TX: PB10
	//USART3_RX: PB11

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//使能USART3时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11和GPIOB10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIOB11，和GPIOB10

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //GPIOB10复用为USART3
#endif

#if USE_UART_4
	//************************UART4_INIT************************//
	//UART4_TX: PC10
	//UART4_RX: PC11

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);//使能UART4时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOC11和GPIOC10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIOB11，和GPIOB10

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); //GPIOC11复用为UART4
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4); //GPIOC10复用为UART4
#endif

#if USE_UART_5
	//************************UART5_INIT************************//
	//UART5_TX: PC12
	//UART5_RX: PD2

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//使能UART5时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOC12初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);  //GPIOD2复用为UART4 RX
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //GPIOAC12复用为UART4 TX
#endif

#if USE_USART_6
	//************************USART6_INIT************************//
	//USART6_TX: PC6
	//USART6_RX: PC7

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);//使能USART6时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PC6,PC7,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //GPIOC6复用为USART6
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //GPIOC7复用为USART6
#endif





}

/**
 *@function Periph_Init
 *@param    无
 *@brief    用于外设初始化管理
 *@retval   无
**/
void Periph_Init(void)
{
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	USART_InitTypeDef USART_InitStructure;

#if USE_CAN_1
	//************************CAN1_INIT************************//
	//CAN1_RX: PA11 
	//CAN1_TX: PA12
	//通讯速率1M
	//过滤器0，不拦截任何报文（掩码 0x000）
	//报文存储于FIFO0

	//CAN单元设置，
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM = DISABLE;		//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM = DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = ENABLE;		//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP = DISABLE;		//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //模式设置为普通模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; 			//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;			//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = 3;  	//分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);   		//初始化CAN1 

	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	  					//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;				//标识符和掩码都需要左移三位，由寄存器分配决定
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;					//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				//标识符和掩码都需要左移三位，由寄存器分配决定
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;			//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; 					//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);
#endif

#if USE_CAN_2
	//************************CAN2_INIT************************//
	//CAN2_RX: PB12 
	//CAN2_TX: PB13
	//通讯速率1M
	//过滤器15，不拦截任何报文（掩码 0x000）  **特别地：CAN2的过滤器必须从14开始
	//报文存储于FIFO1

	//CAN单元设置
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM = DISABLE;		//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM = DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = ENABLE;		//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP = DISABLE;		//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //模式设置为普通模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; 			//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;			//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = 3;  	//分频系数(Fdiv)为brp+1	
	CAN_Init(CAN2, &CAN_InitStructure);   		//初始化CAN2 

	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 15;	  					//过滤器15,can2的过滤器必须在14以上
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;					//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;			//过滤器1关联到FIFO1
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; 					//激活过滤器1
	CAN_FilterInit(&CAN_FilterInitStructure);
#endif


#if USE_USART_2
	//************************USART2_INIT************************//
	//USART2_TX: PA2
	//USART2_RX: PA3
	//波特率：默认115200（宏定义于 USART2_BAUDRATE）
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;//默认115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式;
	USART_Init(USART2, &USART_InitStructure); //初始化串口
	USART_Cmd(USART2, ENABLE);  //使能串口
#endif

#if USE_USART_3
	//************************USART3_INIT************************//
	//USART3_TX: PB10
	//USART3_RX: PB11
	//波特率：默认115200（宏定义于 USART3_BAUDRATE）
	USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	USART_Cmd(USART3, ENABLE);                    //使能串口
#endif

#if USE_UART_4
	//************************UART4_INIT************************//
	//UART4_TX: PC10
	//UART4_RX: PC11
	//波特率：默认115200（宏定义于 UART4_BAUDRATE）
	USART_InitStructure.USART_BaudRate = UART4_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口3
	USART_Cmd(UART4, ENABLE);  	//使能串口
#endif

#if USE_UART_5
	//************************UART5_INIT************************//
	//UART5_TX: PB10
	//UART5_RX: PB11
	//波特率：默认115200（宏定义于 UART5_BAUDRATE）
	USART_InitStructure.USART_BaudRate = UART5_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(UART5, ENABLE);
#endif

#if USE_USART_6
	//************************USART6_INIT************************//
	//USART6_TX: PC6
	//USART6_RX: PC7
	//波特率：默认115200（宏定义于 USART6_BAUDRATE）
	USART_InitStructure.USART_BaudRate = USART6_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口3
	USART_Cmd(USART6, ENABLE);                   //使能串口
#endif








}


