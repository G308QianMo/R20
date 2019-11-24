#ifndef __PROTOCOL_H
#define __PROTOCOL_H

void Send_Data(u8 *dataToSend , u8 length);//数据发送
void Data_Splitting_And_Sending(int * data,u8 length,u8 function_num);//数据拆分
void Data_Check(u8 *data,u8 num);//数据检查
void Data_Recieving(u8 data);//数据接收
void Data_Processing(u8 *data,u8 length);//数据处理
void Data_Rerequest(u8 erro_type);//数据重请求

#endif
