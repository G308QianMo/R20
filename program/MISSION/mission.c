#include "include.h"
extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

int print_flag=0;
int motorAngle=0;
void mission_temp(void)
{	   
        if(Read_KEY_1 == Bit_RESET)//����
		{			
            M3508_Pos_Velo_Set(3,350,360*4.85*7);//�����350RPM�ٶȻ����ȣ�����ת4.85Ȧ
            Beep_ms(500);
            delay_ms(500);
            while(1)
            {
                if(!LEG_REACH||Read_KEY_1 == Bit_RESET)//��λ���ر�ѹ�»���key1���������ֹͣ
                {
                    delay_ms(10);
                    if(!LEG_REACH||Read_KEY_1 == Bit_RESET)
                    {
                        M3508_Vel_Set(3,0);
                        Beep_ms(500);
                        RELAY = 1;//�������
                        break;
                    }
                }                
            }           
		}
		if(Read_KEY_2 == Bit_RESET)//���ߡ���
		{
            M3508_Pos_Velo_Set(3,-470,0);//���ȫ�ٷ���
            Beep_ms(500);
            delay_ms(1000);
            while(1)
            {
                if(0||Read_KEY_2 == Bit_RESET)//����key2��������ͷţ���
                {
                    delay_ms(10);
                    {
                       if(0||Read_KEY_2 == Bit_RESET)
                        Beep_ms(5000);
                        delay_ms(500);
                        RELAY = 0;
                        //������ͷ�
                        break; 
                    }
                    
                }                
            } 
		}       	 
}
