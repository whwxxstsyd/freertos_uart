/***************************************************************************** 
* 
* File Name : mp_uart_task.c 
* 
* Description: uart task Module 
* 
* Copyright (c) 2018 MakePower Co., Ltd. 
* All rights reserved. 
* 
* Author : Hetianqi
* 
* Date : 2018-4-25
*****************************************************************************/ 

#include "mp_idle_task.h"
#include "mp_osal_rtos.h"
#include "mp_uart.h"
#include "usart.h"	
#include "app.h"
#include "delay.h"	
#include "debug.h"	

#include "FreeRTOS.h"
#include "task.h"

	
static void tls_busy_task(void *data);
static void tls_idle_task(void *data);		


void tls_idle_init(void)
{
	xTaskCreate(tls_idle_task, 
				IDLE_TASK_NAME, 	
				IDLE_TASK_STACKSIZE, 
				NULL,		
				IDLE_TASK_PRIO, 	
				NULL);		


	xTaskCreate(tls_busy_task, 
				BUSY_TASK_NAME, 	
				BUSY_TASK_STACKSIZE, 
				NULL,		
				BUSY_TASK_PRIO, 
				NULL);		

}


static void tls_idle_task(void *data)
{
	while(1)
	{
		LOG_INFO("Here is the idle_task\n");	
		delay_ms(1000);		 	
	}
}


static void tls_busy_task(void *data)
{	
	while(1)
	{	
		LOG_INFO("Here is the busy_task\n");	
		delay_ms(1000);					
	}		
}


