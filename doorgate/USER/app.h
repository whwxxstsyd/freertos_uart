/***********************************************************************
* Filename      : app.h
* Version       : V1.00
* Programmer(s) : hetianqi
* Date			: 2018-04-24
***********************************************************************/
#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__
#include "sys.h"	


typedef struct _APP_MSG{
	u32 type;
	u32 param;	
}APP_MSG;	


typedef enum
{
	MSG_KEY = 0x01,
	MSG_LED,	
	MSG_WLAN,
	MSG_TIMER,
	MSG_SPI
}MSG_TYPE;	

	
#define MAX_STACK_SIZE 			512	
#define MIN_STACK_SIZE 			128		

	
#define IDLE_TASK_NAME 			"IDLE_TASK"
#define IDLE_TASK_PRIO				3						
#define IDLE_TASK_STACKSIZE 		MIN_STACK_SIZE
#define IDLE_QUEUE_SIZE			4	


#define BUSY_TASK_NAME 			"BUSY_TASK"
#define BUSY_TASK_PRIO				9			
#define BUSY_TASK_STACKSIZE 		MIN_STACK_SIZE
#define BUSY_QUEUE_SIZE			4		


#define CARD_DEV1_TASK_NAME 			"CARD_DEV1_TASK"	
#define CARD_DEV1_TASK_PRIO			4							
#define CARD_DEV1_TASK_STACKSIZE 		MIN_STACK_SIZE
#define CARD_DEV1_QUEUE_SIZE			4	


#define CARD_DEV2_TASK_NAME 			"CARD_DEV2_TASK"		
#define CARD_DEV2_TASK_PRIO			4						
#define CARD_DEV2_TASK_STACKSIZE 		MIN_STACK_SIZE
#define CARD_DEV2_QUEUE_SIZE			4	
	

#define UART0_TX_TASK_NAME 		"UART1_TX"
#define UART0_TX_TASK_PRIO			5
#define UART0_TX_TASK_STACKSIZE 	MAX_STACK_SIZE
#define UART0_TX_QUEUE_SIZE		4


#define UART0_RX_TASK_NAME 		"UART1_RX"
#define UART0_RX_TASK_PRIO			6	
#define UART0_RX_TASK_STACKSIZE 	MAX_STACK_SIZE	
#define UART0_RX_QUEUE_SIZE		4


#define UART1_TX_TASK_NAME 		"UART2_TX"
#define UART1_TX_TASK_PRIO			5
#define UART1_TX_TASK_STACKSIZE 	MAX_STACK_SIZE
#define UART1_TX_QUEUE_SIZE		4	


#define UART1_RX_TASK_NAME 		"UART2_RX"
#define UART1_RX_TASK_PRIO			6
#define UART1_RX_TASK_STACKSIZE 	MAX_STACK_SIZE
#define UART1_RX_QUEUE_SIZE		4




void app_init(void);


#endif


