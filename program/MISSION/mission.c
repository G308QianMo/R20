#include "include.h"
extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

int print_flag=0;
int motorAngle=0;
void mission_temp(void)
{
//    Global_Target_X = 500;
//	Global_Target_Y = 0;
//	Global_Target_Angle = 0; 
//	   
//	PID_Setup(1000,200,1000,-1000,
//                      500,100,500,-500,
//                      500,100,500,-500);  
	 
//	 PID_Loop_X();  
//	 PID_Loop_Y();
//	 PID_Loop_angle();  
//	 
//	 Wheel_Spd_To_Motor();
	   
        if(Read_KEY_1 == Bit_RESET)
		{
			motorAngle+=15*7;
            if(motorAngle>360*5*7)
            {
                motorAngle=360*5*7;
            }
            M3508_Pos_Velo_Set(5,200,motorAngle);
		}
		if(Read_KEY_2 == Bit_RESET)
		{
			motorAngle-=15*7;
            if(motorAngle<-360*5*7)
            {
                motorAngle=-360*5*7;
            }
            M3508_Pos_Velo_Set(5,-200,motorAngle);
		}
        
	 delay_ms(100);
}
