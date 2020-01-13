#include "include.h"
uint32_t P_liu = 500, I_Liu = 0, D_Liu = 100;
extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X, Global_Target_Y, Global_Target_Angle;
int16_t Move_liu = 0;
int main(void)
{
 Init();//初始化

  while(1)
 {
	mission_temp();
 }
}

