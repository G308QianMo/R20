#include "include.h"

extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

int main(void)
{
 Init();//≥ı ºªØ
 int LED_Flag = 0;
  while(1)
 {
     
     mission_temp(); 
     delay_ms(100);
 }
}

