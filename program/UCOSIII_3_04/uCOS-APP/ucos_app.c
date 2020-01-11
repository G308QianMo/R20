#include "include.h"
#include "ucos_app.h"
#include "os_app_hooks.h"

extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;
//led0������
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		PCout(0)=0;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
		PCout(0)=1;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}

//led1������
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		PCout(1)=0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
		PCout(1)=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}

//�����������
void float_task(void *p_arg)
{
	CPU_SR_ALLOC();
	OS_ERR err;
	p_arg = p_arg;
	static float float_num=0.01;
	while(1)
	{
		float_num+=0.01f;
		OS_CRITICAL_ENTER();	//�����ٽ���
		printf("float_num��ֵΪ: %.4f\r\n",float_num);
		OS_CRITICAL_EXIT();		//�˳��ٽ���
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}

void Wheel_Motor_task(void *p_arg)
	
{
		OS_ERR err;
	  p_arg = p_arg;

	while(1)
 {
   		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
 }
}
	
	


