#include "include.h"
#include "ucos_init.h"
#include "os_app_hooks.h"

#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define LED0_TASK_PRIO		50
//任务堆栈大小	
#define LED0_STK_SIZE 		128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//任务优先级
#define LED1_TASK_PRIO		51
//任务堆栈大小	
#define LED1_STK_SIZE 		128
//任务控制块
OS_TCB Led1TaskTCB;
//任务堆栈	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *p_arg);

//任务优先级
#define Print_TASK_PRIO		60
//任务堆栈大小
#define Print_STK_SIZE		128
//任务控制块
OS_TCB	PrintTaskTCB;
//任务堆栈
__align(8) CPU_STK	Print_TASK_STK[Print_STK_SIZE];
//任务函数
void Print_task(void *p_arg);

#define 	 Wheel_Motor_TASK_PRIO		4
//任务堆栈大小
#define Wheel_Motor_STK_SIZE		128
//任务控制块
OS_TCB	Wheel_MotorTaskTCB;
//任务堆栈
__align(8) CPU_STK	Wheel_Motor_TASK_STK[Wheel_Motor_STK_SIZE];
//任务函数
void Wheel_Motor_task(void *p_arg);

/***软件定时器****/

OS_TMR 	TIMR1;		//定时器1
OS_TMR	TIMR2;		//定时器2
void TIMR1_CallBack(void *p_tmr, void *p_arg); 	//定时器1回调函数
void TIMR2_CallBack(void *p_tmr, void *p_arg);	//定时器2回调函数


void OS_APP_init(OS_ERR  *p_err)
{
	OS_ERR err;
	
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级，可设置范围为1~OS_CFG_PRIO_MAX-1,0与OS_CFG_PRIO_MAX为系统保留优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位，剩余栈空间小于10%就达到栈极限深度。
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&p_err);				//存放该函数错误时的返回值
								 
	//创建定时器任务
				 
								 
								 
}

void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区，开始创建任务
	
						//创建定时器1
	OSTmrCreate((OS_TMR		*)&TIMR1
								 ,		//定时器1
                (CPU_CHAR	*)"TIMR1",		//定时器名字
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)TIMR1_CallBack,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码					
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&TIMR2,		
                (CPU_CHAR	*)"TIMR2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)TIMR2_CallBack,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);	
	//创建LED0任务
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
				 
	//创建LED1任务
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
				 
	//创建浮点测试任务
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
								 
	//创建电机测试任务						 
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
								 
  								

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务
								 
	OSTmrStart(&TIMR1,&err);	//开启定时器1
	OSTmrStart(&TIMR2,&err);	//开启定时器2
	
	/****软件定时器*****/

 
	//可根据需要随时关闭或者开启定时器任务
//	OSTmrStop(&TIMR1,OS_OPT_TMR_NONE,0,&err);	//关闭定时器1
//	OSTmrStop(&TIMR2,OS_OPT_TMR_NONE,0,&err);	//关闭定时器2	
								 
	OS_CRITICAL_EXIT();	//退出临界区，创建任务完成
}




