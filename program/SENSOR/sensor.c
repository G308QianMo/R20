#include "include.h"

__IO float global_gyro_angle_last = 0;
__IO float global_gyro_angle_now = 0;
__IO float global_gyro_angle = 0;
__IO int32_t global_gyro_pulse_x = 0;
__IO int32_t global_gyro_pulse_y = 0;
__IO EncodePointTypeDef global_gyro_location;
int global_DT35_dataA,global_DT35_dataB,global_DT35_dataC;
int16_t global_enc_x = 0, global_enc_y = 0;
double global_coordX = 0, global_coordY = 0;

/**
*@function GYRO_Get_Location_Param
*@param    void
*@brief    得到传感器的值(陀螺仪、编码器、激光)
*@retval   void
**/
void GYRO_Get_Location_Param(void)
{
	      uDataConvert32TypeDef temp;
        CanRxMsg RxMessage;
        if (CAN_GetITStatus(CAN_BUS, CAN_IT_FMP0) != RESET)
        {
            CAN_Receive(CAN_BUS, CAN_FIFO0, &RxMessage);
            switch (RxMessage.StdId)
            {
						//得到陀螺仪传回的角度值
            case GYRO_ID:						      //GYRO_ID==0x11			
                if (RxMessage.DLC == 4)		
                {
                    temp.u8_form[0] = RxMessage.Data[0];
                    temp.u8_form[1] = RxMessage.Data[1];
                    temp.u8_form[2] = RxMessage.Data[2];
                    temp.u8_form[3] = RxMessage.Data[3];
                    memcpy((void *)&global_gyro_angle_now, &temp.float_form, 4);
                }
								else if (RxMessage.DLC == 8)	//
                {
//										temp.u8_form[0] = RxMessage.Data[0];
//										temp.u8_form[1] = RxMessage.Data[1];
//										temp.u8_form[2] = RxMessage.Data[2];
//										temp.u8_form[3] = RxMessage.Data[3];
//										memcpy((void *)&GYRO_pulse_x, &temp.s32_form, 4);

//										temp.u8_form[0] = RxMessage.Data[4];
//										temp.u8_form[1] = RxMessage.Data[5];
//										temp.u8_form[2] = RxMessage.Data[6];
//										temp.u8_form[3] = RxMessage.Data[7];
//										memcpy((void *)&GYRO_pulse_y, &temp.s32_form, 4);
						
											global_gyro_location.x = (float)global_coordX*CODER_PARAM;
											global_gyro_location.y = (float)global_coordY*CODER_PARAM;
											global_gyro_location.angle = global_gyro_angle_now;
								
								
											global_gyro_pulse_y = TIM2->CNT;
											global_enc_y = global_gyro_pulse_y - 30000;
											global_gyro_pulse_x = TIM5->CNT;
											global_enc_x = global_gyro_pulse_x - 30000;

											TIM2->CNT = 30000;
											TIM5->CNT = 30000;

											global_gyro_angle = global_gyro_angle_now / 2 + global_gyro_angle_last / 2;
											global_enc_x = global_enc_x + (float)global_enc_x * my_cos(global_gyro_angle)/2 - (float)global_enc_y * my_sin(global_gyro_angle);//- 94.46;
											global_enc_y = global_enc_y + (float)global_enc_x * my_sin(global_gyro_angle)/2 + (float)global_enc_y * my_cos(global_gyro_angle);//- 224.62;

											global_gyro_location.x = (float)global_enc_x*CODER_PARAM + 221.7205f* my_cos(-global_gyro_angle + 75.08524f);
											global_gyro_location.y = (float)global_enc_y*CODER_PARAM + 221.7205f* my_sin(-global_gyro_angle + 75.08524f);

											global_gyro_angle_last = global_gyro_angle_now;
							
								}
                break;			
						case 0x13:		                 //global_DT35_dataA_ID
								temp.u8_form[0] = RxMessage.Data[0];
								temp.u8_form[1] = RxMessage.Data[1];
								temp.u8_form[2] = RxMessage.Data[2];
								temp.u8_form[3] = RxMessage.Data[3];
								memcpy((void *)&global_DT35_dataA, &temp.s32_form, 4);
								break;								
						case 0x15:		                //global_DT35_dataB_ID
								temp.u8_form[0] = RxMessage.Data[0];
								temp.u8_form[1] = RxMessage.Data[1];
								temp.u8_form[2] = RxMessage.Data[2];
								temp.u8_form[3] = RxMessage.Data[3];
								memcpy((void *)&global_DT35_dataB, &temp.s32_form, 4);
								break;								
						case 0x16:		                //global_DT35_dataC_ID
								temp.u8_form[0] = RxMessage.Data[0];
								temp.u8_form[1] = RxMessage.Data[1];
								temp.u8_form[2] = RxMessage.Data[2];
								temp.u8_form[3] = RxMessage.Data[3];
								memcpy((void *)&global_DT35_dataC, &temp.s32_form, 4);
						    break;
            default:
                break;
            }
						
								CAN_ClearITPendingBit(CAN_BUS, CAN_IT_FMP0);
        } 
}
