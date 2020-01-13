#include "include.h"

extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

int main(void)
{
 Init();//初始化

  while(1)
 {
	mission_temp();
 }
}

