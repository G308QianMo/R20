
#include "include.h"

/**
*@file    
*@author	������
*@version 1.0
*@date    2020.1.03
*@brief   uCOS��ײ���ֲ
**/
//includeĿ¼�°������ļ�#include "os_app_hooks.h"


//�������ȼ�


int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);  	//ʱ�ӳ�ʼ��
	uart_init(115200);  //���ڳ�ʼ��
	Init();         //��ʼ��

	
	OSInit(&err);		//��ʼ��UCOSIII
	
	OS_CRITICAL_ENTER();//�����ٽ���
	
	OS_APP_init(&err);  	//������ʼ����
	
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	
	OSStart(&err);  //����UCOSIII
								 
	while(1);
								 
}

//��ʼ������

