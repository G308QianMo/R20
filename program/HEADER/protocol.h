#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "include.h"
void Send_Data(u8 *dataToSend , u8 length);//数据发送
void Data_Splitting_And_Sending(int * data,u8 length,u8 function_num);//数据拆分
void Data_Check(u8 *data,u8 num);//数据检查
void Data_Recieving(u8 data);//数据接收
void Data_Processing(u8 *data,u8 length);//数据处理
void Data_Rerequest(u8 erro_type);//数据重请求

#define elmo1_id 0xF1
#define elmo2_id 0xF2
#define elmo3_id 0xF3
#define elmo4_id 0xF4

void ELMO_Delay(int8_t time);

void ELMO_Single_Enable(uint32_t elmo_id);
void ELMO_Single_Init(uint32_t elmo_id);
void ELMO_Single_Disenable(uint32_t elmo_id);
void ELMO_Single_STOP(uint32_t elmo_id);
void ELMO_Single_BEGIN(uint32_t elmo_id);
void ELMO_Single_Velocity(uint32_t elmo_id,int32_t v1);
void ELMO_Single_PTP_PA(uint32_t elmo_id,int32_t p1);
void ELMO_Single_PTP_PR(uint32_t elmo_id,int32_t p2);

void ELMO_Enable(void);
void ELMO_Init(void);
void ELMO_Disenable(void);
void ELMO_STOP(void);
void ELMO_BEGIN(void);
void ELMO_Velocity(int32_t v1,int32_t v2,int32_t v3,int32_t v4);

#endif
