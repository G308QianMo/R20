#include "include.h"
#include "ucos_init.h"
#include "os_app_hooks.h"

#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define LED0_TASK_PRIO		50
//�����ջ��С	
#define LED0_STK_SIZE 		128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//�������ȼ�
#define LED1_TASK_PRIO		51
//�����ջ��С	
#define LED1_STK_SIZE 		128
//������ƿ�
OS_TCB Led1TaskTCB;
//�����ջ	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *p_arg);

//�������ȼ�
#define Print_TASK_PRIO		60
//�����ջ��С
#define Print_STK_SIZE		128
//������ƿ�
OS_TCB	PrintTaskTCB;
//�����ջ
__align(8) CPU_STK	Print_TASK_STK[Print_STK_SIZE];
//������
void Print_task(void *p_arg);

#define 	 Wheel_Motor_TASK_PRIO		4
//�����ջ��С
#define Wheel_Motor_STK_SIZE		128
//������ƿ�
OS_TCB	Wheel_MotorTaskTCB;
//�����ջ
__align(8) CPU_STK	Wheel_Motor_TASK_STK[Wheel_Motor_STK_SIZE];
//������
void Wheel_Motor_task(void *p_arg);

/***�����ʱ��****/

OS_TMR 	TIMR1;		//��ʱ��1
OS_TMR	TIMR2;		//��ʱ��2
void TIMR1_CallBack(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void TIMR2_CallBack(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����


void OS_APP_init(OS_ERR  *p_err)
{
	OS_ERR err;
	
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ��������÷�ΧΪ1~OS_CFG_PRIO_MAX-1,0��OS_CFG_PRIO_MAXΪϵͳ�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ��ʣ��ջ�ռ�С��10%�ʹﵽջ������ȡ�
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&p_err);				//��Ÿú�������ʱ�ķ���ֵ
								 
	//������ʱ������
				 
								 
								 
}

void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ�������ʼ��������
	
						//������ʱ��1
	OSTmrCreate((OS_TMR		*)&TIMR1
								 ,		//��ʱ��1
                (CPU_CHAR	*)"TIMR1",		//��ʱ������
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)TIMR1_CallBack,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����					
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&TIMR2,		
                (CPU_CHAR	*)"TIMR2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)TIMR2_CallBack,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);	
	//����LED0����
	OSTaskCreate(  (OS_TCB 	* )&Led0TaskTCB,		
				         (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//����LED1����
	OSTaskCreate(  (OS_TCB 	* )&Led1TaskTCB,		
				         (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
				 
	//���������������
	OSTaskCreate(  (OS_TCB 	* )&PrintTaskTCB,		
				         (CPU_CHAR	* )"Print test task", 		
                 (OS_TASK_PTR )Print_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Print_TASK_PRIO,     	
                 (CPU_STK   * )&Print_TASK_STK[0],	
                 (CPU_STK_SIZE)Print_STK_SIZE/10,	
                 (CPU_STK_SIZE)Print_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		
								 
	//���������������						 
								 	OSTaskCreate(  (OS_TCB 	* )&Wheel_MotorTaskTCB,		
				         (CPU_CHAR	* )"Wheel_Motor test task", 		
                 (OS_TASK_PTR )Wheel_Motor_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Wheel_Motor_TASK_PRIO,     	
                 (CPU_STK   * )&Wheel_Motor_TASK_STK[0],	
                 (CPU_STK_SIZE)Wheel_Motor_STK_SIZE/10,	
                 (CPU_STK_SIZE)Wheel_Motor_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
  								

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����
								 
	OSTmrStart(&TIMR1,&err);	//������ʱ��1
	OSTmrStart(&TIMR2,&err);	//������ʱ��2
	
	/****�����ʱ��*****/

 
	//�ɸ�����Ҫ��ʱ�رջ��߿�����ʱ������
//	OSTmrStop(&TIMR1,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��1
//	OSTmrStop(&TIMR2,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��2	
								 
	OS_CRITICAL_EXIT();	//�˳��ٽ����������������
}




