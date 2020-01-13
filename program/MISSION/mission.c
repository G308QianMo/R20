#include "include.h"
extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

int print_flag=0;
int motorAngle=0;
void mission_temp(void)
{	   
        if(Read_KEY_1 == Bit_RESET)//收线
		{			
            M3508_Pos_Velo_Set(3,350,360*4.85*7);//电机按350RPM速度回收腿，至多转4.85圈
            Beep_ms(500);
            delay_ms(500);
            while(1)
            {
                if(!LEG_REACH||Read_KEY_1 == Bit_RESET)//限位开关被压下或按下key1，电机立刻停止
                {
                    delay_ms(10);
                    if(!LEG_REACH||Read_KEY_1 == Bit_RESET)
                    {
                        M3508_Vel_Set(3,0);
                        Beep_ms(500);
                        RELAY = 1;//电磁铁起动
                        break;
                    }
                }                
            }           
		}
		if(Read_KEY_2 == Bit_RESET)//放线、踢
		{
            M3508_Pos_Velo_Set(3,-470,0);//电机全速放线
            Beep_ms(500);
            delay_ms(1000);
            while(1)
            {
                if(0||Read_KEY_2 == Bit_RESET)//按下key2，电磁铁释放，踢
                {
                    delay_ms(10);
                    {
                       if(0||Read_KEY_2 == Bit_RESET)
                        Beep_ms(5000);
                        delay_ms(500);
                        RELAY = 0;
                        //电磁铁释放
                        break; 
                    }
                    
                }                
            } 
		}       	 
}
