#include "include.h"

/**
*@file    init.c
*@author  
*@version 1.0
*@date    2019.11.21
*@brief   进行硬件初始化
**/

/**
 *@function Init
 *@param    无
 *@brief    用于所有硬件初始化管理
 *@retval   无
**/
void Init(void)
{
	GPIO_INIT();
	Periph_Init();
}
/**
 *@function GPIO_INIT
 *@param    无
 *@brief    用于IO口初始化管理
 *@retval   无
**/
void GPIO_INIT(void)
{
	
}

/**
 *@function Periph_Init
 *@param    无
 *@brief    用于外设初始化管理
 *@retval   无
**/
void Periph_Init(void)
{
	
}


