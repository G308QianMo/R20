#include "include.h"

/**
*@file    basial_move
*@author  黄彦皓
*@version ver1.0
*@date    2019-12-1
*@brief   基础运行代码
**/

static int32_t wheel_spd[4] = {0};
extern s16 targetX, targetY1, targetA;
extern u8 Sircle_flag;
extern int DT35_DataA,DT35_DataB,DT35_DataC;//????
void ClearWheelSpeed(void);

void ClearWheelSpeed(void)
{
    memset(wheel_spd, 0, sizeof(wheel_spd));
}
/**
 * @function    : rotate_speed
 * @Description : ????exp_v??,?????,?????
**/
void Camera_rotate_speed(int32_t exp_v)
{
    int32_t spd[4] = { 0 };

    spd[0] = exp_v;
    spd[1] = exp_v;
    spd[2] = 0;
    spd[3] = 0;

    //???????????
    wheel_spd[0] += spd[0];
    wheel_spd[1] += spd[1];
    wheel_spd[2] += spd[2];
    wheel_spd[3] += spd[3];
}



void rotate_speed(int32_t exp_v)
{
    int32_t spd[4] = {0};

    spd[0] = exp_v;
    spd[1] = exp_v;
    spd[2] = exp_v;
    spd[3] = exp_v;

    //???????????
    wheel_spd[0] += spd[0];
    wheel_spd[1] += spd[1];
    wheel_spd[2] += spd[2];
    wheel_spd[3] += spd[3];
}

/**
 * @function    : linear_speed
 * @Description : ???exp_v???(x,y)?????,alpha????????y???????
**/
void linear_speed(int32_t x, int32_t y, float alpha, int32_t exp_v)
{
    double spd_gain = 1;
    int32_t spd[4] = {0};
    int64_t Vx = x,
            Vy = y,
            V  = 0;
    alpha = alpha + OFFSET_ANG; //?????????
    V  = my_sqrt(square(Vx) + square(Vy));
    if (V == 0) spd_gain = 0;
    else spd_gain = (double)exp_v / (double)V;
    Vx = x * spd_gain;
    Vy = y * spd_gain;

    /*?????*/
//    spd[0] = -Vy * my_sin(60 + alpha ) - Vx * my_cos(60 + alpha);
//    spd[1] = Vy * my_sin(60 - alpha ) - Vx * my_cos(60 - alpha );
//    spd[3] = Vy * my_sin(alpha) + Vx * my_cos(alpha );//2017.11.15.19.35 ?????????????
		/*?????*/
	  spd[0] = my_cos(alpha-135)*Vx + my_sin(alpha-135)*Vy;
	  spd[1] = my_cos(alpha-45)*Vx + my_sin(alpha-45)*Vy;
	  spd[2] = my_cos(alpha+45)*Vx + my_sin(alpha+45)*Vy;
	  spd[3] = my_cos(alpha+135)*Vx + my_sin(alpha+135)*Vy;
		

    //???????????
    wheel_spd[0] += spd[0];
    wheel_spd[1] += spd[1];
    wheel_spd[2] += spd[2];
    wheel_spd[3] += spd[3];
}

/**
 * @function    :
 * @Description : ??????elmo
 * ??,???spd0 1 3
**/
extern EncodePointTypeDef GYRO_Location;
int32_t Stop_flag=0;
extern int32_t Task_Go_Flag;;
int32_t spd[4] = {0};
int32_t Vxx=0,Vyy=0;
#define K_angle 0x00
u8 Print_Flag1 = 0;
void SetMotorSpeed(void)
{
    int32_t max_spd;		    //????
    float   percent = 0.0;	    //????
    //*??????????  ????EPOS?????r/min  ????????J=21?,?????125mm
    //??n=wheel_spd[]*60*J/(R*2*3.14)   ????n=wheel_spd[]*60*21/(125*3.14)=wheel_spd[]*3.21*/
    spd[0] = wheel_spd[0] * 3.21;
    spd[1] = wheel_spd[1] * 3.21;
    spd[2] = wheel_spd[2] * 3.21;
    spd[3] = wheel_spd[3] * 3.21;

    /*??????????  ????EPOS?????r/min  ????????J=12?,?????125mm
    ??n=wheel_spd[]*60*J/(R*2*3.14)   ????n=wheel_spd[]*60*12/(125*3.14)=wheel_spd[]*1.83*/
//    spd[0] = wheel_spd[0] * 1.83;
//    spd[1] = wheel_spd[1] * 1.83;
//    spd[2] = wheel_spd[2] * 1.83;
//    spd[3] = wheel_spd[3] * 1.83;
	
	
    ClearWheelSpeed();
}