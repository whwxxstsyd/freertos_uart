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


#define MINIMAL_STACK_SIZE 		128

#define IDLE_TASK_NAME 			"IDLE_TASK"
#define IDLE_TASK_PRIO				3						
#define IDLE_TASK_STACKSIZE 		MINIMAL_STACK_SIZE
#define IDLE_QUEUE_SIZE			4	

#define BUSY_TASK_NAME 			"BUSY_TASK"
#define BUSY_TASK_PRIO				9						
#define BUSY_TASK_STACKSIZE 		MINIMAL_STACK_SIZE
#define BUSY_QUEUE_SIZE			4		


#define CARD_TASK_NAME 			"CARD_TASK"	
#define CARD_TASK_PRIO				3						
#define CARD_TASK_STACKSIZE 		MINIMAL_STACK_SIZE
#define CARD_QUEUE_SIZE			4	
	


#define UART0_TX_TASK_NAME 		"UART1_TX"
#define UART0_TX_TASK_PRIO			8
#define UART0_TX_TASK_STACKSIZE 	MINIMAL_STACK_SIZE
#define UART0_TX_QUEUE_SIZE		4


#define UART0_RX_TASK_NAME 		"UART1_RX"
#define UART0_RX_TASK_PRIO			7
#define UART0_RX_TASK_STACKSIZE 	MINIMAL_STACK_SIZE
#define UART0_RX_QUEUE_SIZE		4


#define UART1_TX_TASK_NAME 		"UART2_TX"
#define UART1_TX_TASK_PRIO			6
#define UART1_TX_TASK_STACKSIZE 	MINIMAL_STACK_SIZE
#define UART1_TX_QUEUE_SIZE		4

#define UART1_RX_TASK_NAME 		"UART2_RX"
#define UART1_RX_TASK_PRIO			5
#define UART1_RX_TASK_STACKSIZE 	MINIMAL_STACK_SIZE
#define UART1_RX_QUEUE_SIZE		4




void app_init(void);


#endif


