#include "include.h"

/**
*@file    basial_move
*@author  HYH
*@version 1.0
*@date    2020/1/4
*@brief   基本底层控制
**/

static int32_t Static_Wheel_Spd[4] = {0};
int32_t Stop_flag = 0;

/**
*@function Move_To_Point_Set
*@param    X  X坐标
           Y  Y坐标
           Alpha  车身角度
           Speed  速度
*@brief    以Alpha为角度，Speed为速度，向(X,Y)走，也可以用(X,Y)表示方向向量
*@retval   NULL
**/
void Move_To_Point_Set(int32_t X, int32_t Y, float Alpha, int32_t Speed)
{
	
    int32_t Spd[4] = {0};
	int64_t Dir = 0,
	        Vx = 0,
	        Vy = 0;
	Alpha = Alpha + OFFSET_ANG;
	Dir = my_sqrt(square(X) + square(Y));
	if(Dir == 0)
	{
		Vx = 0;
		Vy = 0;
	}
	else 
	{
		Vx = X * ((double)Speed / (double)Dir);
		Vy = Y * ((double)Speed / (double)Dir);
	}
	
	Spd[0] = my_cos(Alpha-135)*Vx + my_sin(Alpha-135)*Vy;
	Spd[1] = my_cos(Alpha-45)*Vx + my_sin(Alpha-45)*Vy;
	Spd[2] = my_cos(Alpha+45)*Vx + my_sin(Alpha+45)*Vy;
	Spd[3] = my_cos(Alpha+135)*Vx + my_sin(Alpha+135)*Vy;
		

	
    Static_Wheel_Spd[0] += Spd[0];
    Static_Wheel_Spd[1] += Spd[1];
    Static_Wheel_Spd[2] += Spd[2];
    Static_Wheel_Spd[3] += Spd[3];
}

/**
*@function MOVE_As_Circle
*@param    Speed  速度
*@brief    以Speed为速度原地转
*@retval   NULL
**/
void MOVE_As_Circle(int32_t Speed)
{
	int32_t Spd[4] = {0};
	
	Spd[0] = Speed;
	Spd[1] = Speed;
	Spd[2] = Speed;
	Spd[3] = Speed;
	
	Static_Wheel_Spd[0] += Spd[0];
    Static_Wheel_Spd[1] += Spd[1];
    Static_Wheel_Spd[2] += Spd[2];
    Static_Wheel_Spd[3] += Spd[3];
}

/**
*@function Wheel_Spd_To_Motor
*@param    NULL
*@brief    将Static_Wheel_Spd中累加后的速度给到电机
*@retval   NULL
**/
void Wheel_Spd_To_Motor(void)
{
	int32_t Max_Spd = 0;
	float Percent = 0;
	int32_t Spd[4] = {0};
	
	Spd[0] = Static_Wheel_Spd[0] * 3.21;
    Spd[1] = Static_Wheel_Spd[1] * 3.21;
    Spd[2] = Static_Wheel_Spd[2] * 3.21;
    Spd[3] = Static_Wheel_Spd[3] * 3.21;
	
	Clear_Static_Wheel_Spd();
	
	Max_Spd = my_max(my_max(my_max(my_abs(Spd[0]),my_abs(Spd[1])),my_abs(Spd[2])),my_abs(Spd[3]));

	//限速
	if (Max_Spd > MAX_SPEED)
    {
        Percent = (double)MAX_SPEED / (double)Max_Spd;
        Spd[0] *= Percent;
        Spd[1] *= Percent;
        Spd[2] *= Percent;
        Spd[3] *= Percent;
    }
	
	if (Stop_flag == 1)
    {
        STOP();
    }
    else
    {
        ELMO_Velocity(Spd[0], Spd[1], Spd[2], Spd[3]);
    }
	
	Print(Spd,30);
	
}

/**
*@function Clear_Static_Wheel_Spd
*@param    NULL
*@brief    清除Clear_Static_Wheel_Spd中速度，方便再次累加
*@retval   NULL
**/
void Clear_Static_Wheel_Spd(void)
{
    memset(Static_Wheel_Spd, 0, sizeof(Static_Wheel_Spd));
}

/**
*@function STOP
*@param    NULL
*@brief    STOP
*@retval   NULL
**/
void STOP(void)
{
	int32_t Spd[4] = {0};
	
	Clear_Static_Wheel_Spd();

	ELMO_Velocity(Spd[0], Spd[1], Spd[2], Spd[3]);
}


extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;
extern EncodePointTypeDef global_gyro_location;

/**
*@function Print
*@param    Spd  就是各电机速度
           Print_Frequency  发送数据频率，越大发的越慢
*@brief    发送各项信息
*@retval   NULL
**/
void Print(int32_t Spd[4], int Print_Frequency)
{
	int Print_Flag = 0;
	
	Print_Flag++; 
	
	if(Print_Flag == Print_Frequency)
	{
		printf("v1:  %d\tv2:  %d\tv3:  %d\tv4:  %d\t\r\n",Spd[0],Spd[1],Spd[2],Spd[3]);
		printf("%d\t%d\t%d\r\n",
		       (int)global_gyro_location.x,(int)global_gyro_location.y,(int)global_gyro_location.angle);
		printf("targetX:  %d\ttargetY1:  %d\ttargetA:  %d\t\r\n",
		        Global_Target_X,Global_Target_Y,Global_Target_Angle);
	}
}
