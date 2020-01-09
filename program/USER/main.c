#include "include.h"

extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

int main(void)
{
 Init();//≥ı ºªØ
 int LED_Flag = 0;
 int print_flag=0;
  while(1)
 {
	 Global_Target_X = 500;
	 Global_Target_Y = 0;
	 Global_Target_Angle = 0; 
	   
	 PID_Setup(1000,200,1000,-1000,
                      500,100,500,-500,
                      500,100,500,-500);  
	 
	 PID_Loop_X();  
	 PID_Loop_Y();
	 PID_Loop_angle();  
	 
	 Wheel_Spd_To_Motor();
	   
	 if(LED_Flag < 100)
		 LED0_ON;
	 else if(LED_Flag >=100 && LED_Flag < 200)
		 LED0_OFF;
	 else if(LED_Flag >= 200)
		 LED_Flag = 0;	 
	 
	 LED_Flag++;
	 
	 delay_ms(5);
 }
}

