#ifndef __INIT_H
#define __INIT_H

void Init(void);
void GPIO_INIT(void);//IO口初始化
void Periph_Init(void);//外设初始化\

#define 	USE_LED			1
#define 	USE_BEEP		1
#define 	USE_KEY			1
#define		USE_CAN_1		1
#define		USE_CAN_2		1
#define 	USE_USART_1		1
#define 	USE_USART_2		1
#define 	USE_USART_3		1
#define 	USE_UART_4		1
#define 	USE_UART_5		1
#define 	USE_USART_6		1


#if USE_LED
//************************LED************************//
#define LED0_ON GPIO_ResetBits(GPIOC, GPIO_Pin_0)	//开灯
#define LED1_ON GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define LED2_ON GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define LED3_ON GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#define LED0_OFF GPIO_SetBits(GPIOC, GPIO_Pin_0)	//关灯
#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define LED3_OFF GPIO_SetBits(GPIOC, GPIO_Pin_3)

#endif

#if USE_BEEP

//************************Beep************************//
#define BEEP_ON  GPIO_SetBits(GPIOA, GPIO_Pin_4)	//打开蜂鸣器
#define BEEP_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_4)	//关闭蜂鸣器

#endif

#if USE_KEY
//************************KEY************************//
//返回值： 按下： (uint8_t)Bit_RESET
//		   松开： (uint8_t)Bit_SET
#define Read_KEY_1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define Read_KEY_2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)
#endif

#if USE_USART_2
//************************USART2************************//
#define USART2_BAUDRATE 115200		//USART2波特率
#endif

#if USE_USART_3
//************************USART3************************//
#define USART3_BAUDRATE 115200		//USART3波特率
#endif

#if USE_UART_4
//************************UART4************************//
#define UART4_BAUDRATE 115200		//UART4波特率
#endif

#if USE_UART_5
//************************UART5************************//
#define UART5_BAUDRATE 115200		//UART5波特率
#endif

#if USE_USART_6
//************************USART6************************//
#define USART6_BAUDRATE 115200		//USART6波特率
#endif

#endif
