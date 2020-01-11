
#include "include.h"

/**
*@file    
*@author	王梓帆
*@version 1.0
*@date    2020.1.03
*@brief   uCOSⅢ底层移植
**/
//include目录下包含此文件#include "os_app_hooks.h"


//任务优先级


int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);  	//时钟初始化
	uart_init(115200);  //串口初始化
	Init();         //初始化

	
	OSInit(&err);		//初始化UCOSIII
	
	OS_CRITICAL_ENTER();//进入临界区
	
	OS_APP_init(&err);  	//创建开始任务
	
	OS_CRITICAL_EXIT();	//退出临界区	 
	
	OSStart(&err);  //开启UCOSIII
								 
	while(1);
								 
}

//开始任务函数

