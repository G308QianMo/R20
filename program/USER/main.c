
#include "include.h"

/**
*@file    
*@author	������
*@version 1.0
*@date    2020.1.11
*@brief   uCOS��ײ���ֲ
**/
//includeĿ¼�°������ļ�#include "os_app_hooks.h"
 //����Ӧ�ó�����UCOS_app�½��г�ʼ���ͱ�д


//�������ȼ�


int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);  	//ʱ�ӳ�ʼ��

	OSInit(&err);		//��ʼ��UCOSIII
	
	OS_CRITICAL_ENTER();//�����ٽ�����ע��:�����ٽ�����Ҫ����һ��CPU_SR_ALLOC();
	
	OS_APP_init(&err);  	//������ʼ����
	

	uart_init(115200);  //���Դ��ڳ�ʼ��
	Init();   //��ʼ��
	
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	

	OSStart(&err);  //����UCOSIII
	
	while (1){};//���ã���ɾ��
					 
}


