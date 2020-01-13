#include "include.h"
uint32_t P_liu = 500, I_Liu = 0, D_Liu = 100;
extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X, Global_Target_Y, Global_Target_Angle;
int16_t Move_liu = 0;
int main(void)
{
	Init();//≥ı ºªØ
	int LED_Flag = 0;
	int print_flag = 0;
	Global_Target_Y = 0;
	Global_Target_Angle = 0;
	while (1)
	{

		PID_Setup(1000, 200, 1000, -1000,
			P_liu, D_Liu, 3000, -3000,
			500, 100, 1000, -1000);

		PID_Loop_X();
		PID_Loop_Y();
		PID_Loop_angle();

		Wheel_Spd_To_Motor();

		if (LED_Flag < 100)
			LED0_ON;
		else if (LED_Flag >= 100 && LED_Flag < 200)
			LED0_OFF;
		else if (LED_Flag >= 200)
			LED_Flag = 0;

		LED_Flag++;

		//delay_ms(5);
	}
}

