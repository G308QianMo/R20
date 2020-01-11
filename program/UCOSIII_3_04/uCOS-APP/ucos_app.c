#include "include.h"
#include "ucos_app.h"
#include "os_app_hooks.h"

extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

/**
*@file    所有执行的任务
*@author	王梓帆
*@version 1.0
*@date    2020.1.11
*@brief   
**/

//led0任务函数
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		PCout(0)=0;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
		PCout(0)=1;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

//led1任务函数
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		PCout(1)=0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
		PCout(1)=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

//打印测试任务
void Print_task(void *p_arg)
{
	CPU_SR_ALLOC();
	OS_ERR err;
	p_arg = p_arg;
	static float float_num=0.01;
	while(1)
	{
		float_num+=0.01f;
		
		OS_CRITICAL_ENTER();	//进入临界区
		
		printf("TIM2的值为: %d\r\n",TIM2->CNT);
		printf("TIM1的值为: %d\r\n",TIM1->CNT);
		
		OS_CRITICAL_EXIT();		//退出临界区
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

void Wheel_Motor_task(void *p_arg)
	
{
		OS_ERR err;
	  p_arg = p_arg;

	while(1)
 {

	    ELMO_Velocity(500,500,-500,-500);
   		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
 }
}

/****定时器*****回调函数***/

//定时器1回调函数
void TIMR1_CallBack(void *p_tmr, void *p_arg)
{


}	

//定时器2回调函数
void TIMR2_CallBack(void *p_tmr, void *p_arg)	
{
	
	
	
}
	
	


