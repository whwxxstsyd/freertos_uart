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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "app.h"	
#include "delay.h"
#include "usart.h"	
#include "param.h"		
#include "mp_uart.h"
#include "debug.h"
#include "list_base.h"					
#include "mp_uart_task.h"	
#include "mp_cmdp_hostif.h"	
	
	
#define      UART0_TX_TASK_STK_SIZE          256 
#define      UART0_RX_TASK_STK_SIZE          300 
#define      UART1_TX_TASK_STK_SIZE          256 
#define      UART1_RX_TASK_STK_SIZE          300 

extern struct tls_uart_port uart_port[2];
struct tls_uart uart_st[2];


#define UART_NET_SEND_DATA_SIZE      512
char uart_net_send_data[UART_NET_SEND_DATA_SIZE];


void uart_rx(struct tls_uart *uart);
void uart_tx(struct tls_uart *uart);
struct tls_uart *tls_uart_open(u32 uart_no);





static s16 tls_uart1_task_rx_cb(char *buf, u16 len);
static s16 tls_uart2_task_rx_cb(char *buf, u16 len);	
static s16 tls_uart1_task_tx_cb(struct tls_uart_port *port);
static s16 tls_uart2_task_tx_cb(struct tls_uart_port *port);
static void tls_uart_1_rx_task(void *data);	
static void tls_uart_1_tx_task(void *data);
static void tls_uart_2_rx_task(void *data);
static void tls_uart_2_tx_task(void *data);




static void uart_send_tx_msg(u8 hostif_mode, struct tls_hostif_tx_msg *tx_msg, bool is_event)
{
	
}

	
void tls_uart_init(void)
{	
    struct tls_uart *uart;	
    struct tls_hostif *hif = tls_get_hostif();
    struct tls_param_uart uart_cfg;
		
    memset(uart_st, 0, 2 * sizeof(struct tls_uart));
	
    /* setting uart */			
    if(WM_SUCCESS  != tls_uart_port_init(COM1, NULL))
		return;
	
#if 1	
	/*start the uart task*/		
    uart = tls_uart_open(COM1); 	
    if(NULL == uart)	
		return;		
		
    hif->uart_send_tx_msg_callback = uart_send_tx_msg;	

#endif	 

}


static xSemaphoreHandle temp; 

struct tls_uart *tls_uart_open(u32 uart_no)	
{
    struct tls_uart *uart;
    char *stk;

    if (uart_no == COM1) 
	{			
		uart = &uart_st[0];	
		memset(uart,0,sizeof(struct tls_uart));
		uart->uart_port = &uart_port[0];		
		
		tls_uart_rx_register(uart_no, tls_uart1_task_rx_cb);
		tls_uart_tx_register(uart_no, tls_uart1_task_tx_cb);
    } 
	else if (uart_no == COM2) 	
	{			
		uart = &uart_st[1];
		memset(uart,0,sizeof(struct tls_uart));
		uart->uart_port = &uart_port[1];		
		
		tls_uart_rx_register(uart_no, tls_uart2_task_rx_cb);
		tls_uart_tx_register(uart_no, tls_uart2_task_tx_cb);
    } 
	else
	{	
        return NULL;
	}
	
    dl_list_init(&uart->tx_msg_pending_list);			
		
    if (uart_no == COM1) 
	{		
		
		/*
		xTaskCreate(tls_uart_1_tx_task, 
					UART0_TX_TASK_NAME, 
					UART0_TX_TASK_STACKSIZE, 
					uart, 	
					UART0_TX_TASK_PRIO, 
					NULL);			

		
		*/	


		xTaskCreate(tls_uart_1_rx_task, 	
					UART0_RX_TASK_NAME, 
					UART0_RX_TASK_STACKSIZE, 
					uart,				
					UART0_RX_TASK_PRIO, 
					NULL);	

					

    } 
	else
	{
		
		xTaskCreate(tls_uart_2_tx_task, 
					UART1_TX_TASK_NAME, 
					UART1_TX_TASK_STACKSIZE, 
					uart, 	
					UART1_TX_TASK_PRIO, 
					NULL);	


		xTaskCreate(tls_uart_2_rx_task, 	
					UART1_RX_TASK_NAME, 
					UART1_RX_TASK_STACKSIZE, 
					uart,		 		
					UART1_RX_TASK_PRIO, 
					NULL);	

	}	

		
    tls_os_mailbox_create(&uart->rx_mailbox);			
    tls_os_mailbox_create(&uart->tx_mailbox);
	
    return uart;	
}


static s16 tls_uart1_task_rx_cb(char *buf, u16 len)
{	
	struct tls_uart *uart = &uart_st[0];
		
	if(NULL == buf)	
		return WM_FAILED;		

	LOG_INFO("nihao weiwsad\n");		
	
	if(uart->rx_mailbox)					
		tls_os_mailbox_send(uart->rx_mailbox, (void *)MBOX_MSG_UART_RX);
		
	return WM_SUCCESS;
}

static s16 tls_uart2_task_rx_cb(char *buf, u16 len)
{
	struct tls_uart *uart = &uart_st[1];
	
	if(NULL == buf)	
		return WM_FAILED;
	
	if(uart->rx_mailbox)						
		tls_os_mailbox_send(uart->rx_mailbox, (void *)MBOX_MSG_UART_RX);
	
	return WM_SUCCESS;	
}

static s16 tls_uart1_task_tx_cb(struct tls_uart_port *port)
{
	struct tls_uart *uart = &uart_st[0];
	
	if(NULL == port)	
		return WM_FAILED;	
		
	if(uart->tx_mailbox)	
		tls_os_mailbox_send(uart->tx_mailbox, (void *)MBOX_MSG_UART_TX);
	
	return WM_SUCCESS;
}

static s16 tls_uart2_task_tx_cb(struct tls_uart_port *port)
{
	struct tls_uart *uart = &uart_st[1];
	
	if(NULL == port)
		return WM_FAILED;

	if(uart->tx_mailbox)		
	    tls_os_mailbox_send(uart->tx_mailbox, (void *)MBOX_MSG_UART_TX);
	
	return WM_SUCCESS;	
}







static void tls_uart_1_rx_task(void *data)
{	
    struct tls_uart *uart = (struct tls_uart *)data;
    int err;	
    u32 *msg = NULL;

    for ( ;; )	
	{												
        err = tls_os_mailbox_receive(uart->rx_mailbox, (void *)msg, 0) ;			
		
        if (!err) 
		{	
			LOG_INFO("!!!!!!Uart1 Recv Data!!!!!!\n");	
            uart_rx(uart);									
        } 	
    }
}


static void tls_uart_1_tx_task(void *data)
{
    struct tls_uart *uart = (struct tls_uart *)data;
    u32 *msg = NULL;
    int err;		

	LOG_INFO("!!!tls_uart_1_tx_task start!!!\n");
	
    for ( ;; ) 	
	{					
        err = tls_os_mailbox_receive(uart->tx_mailbox, (void *)msg, 0) ;
        if (err == 0) 
		{		
            uart_tx(uart);		 
        } 
    }
}






static void tls_uart_2_rx_task(void *data)
{
    struct tls_uart *uart = (struct tls_uart *)data;    
    u32 *msg;
    int err;

	LOG_INFO("!!!tls_uart_2_rx_task start!!!\n");

    for ( ;; ) 
	{		
        err = tls_os_mailbox_receive(uart->rx_mailbox, (void *)msg, 0) ;
        if (!err) 
		{	
			LOG_INFO("!!!!!!Uart2 Recv Data!!!!!!\n");
            uart_rx(uart);
        } 
    }
}

static void tls_uart_2_tx_task(void *data)
{	
    struct tls_uart *uart = (struct tls_uart *)data;
    u32 *msg;
    int err;	

	LOG_INFO("!!!tls_uart_2_tx_task start!!!\n");

    for ( ;; ) 	
	{				
        err = tls_os_mailbox_receive(uart->tx_mailbox, (void *)msg, 0) ;
        if (err == 0) 
		{				
            uart_tx(uart); 
        } 	
    } 
}

	


static void parse_data(struct tls_uart *uart)
{
	struct tls_uart_circ_buf *recv = &uart->uart_port->recv;
	u8 *atcmd_start = NULL;

	u8 Version,CID1,CID2;
	u16 Addr;
	u8 hostif_uart_type;

	while ((CIRC_CNT(recv->head, recv->tail, TLS_UART_RX_BUF_SIZE) >= 2) && (atcmd_start == NULL))
	{		
		
		if((recv->buf[recv->tail] == '#'))	
		{		
			atcmd_start = &recv->buf[recv->tail];
			recv->tail = (recv->tail + 1) & (TLS_UART_RX_BUF_SIZE - 1);	
			
			Version = TwoAscTOHex(recv->buf[recv->tail+1],recv->buf[recv->tail+2]);
			Addr = TwoAscTOHex(recv->buf[recv->tail+3],recv->buf[recv->tail+4]);	
			CID1 = TwoAscTOHex(recv->buf[recv->tail+5],recv->buf[recv->tail+6]);

			//获得实际地址
			Addr += (CID1&0x0F)*0x100;	
			CID1 &= 0xF0;			



			if((CID1==DEVICE_TYPE_1)||(CID1==DEVICE_TYPE_2)||(CID1==DEVICE_TYPE_2))	
			{	
				//校验	
			}

			

			if (uart->uart_port->uart_no == TLS_UART_0)
            {
                hostif_uart_type = HOSTIF_UART0_CMD;	
            }
            else
            {	
                hostif_uart_type = HOSTIF_UART1_CMD;
            }



			atcmd_start = NULL; 
		}
		else
		{

		}
	
	}
	
}

void uart_rx(struct tls_uart *uart)
{	
	parse_data(uart);
}




void uart_tx(struct tls_uart *uart)	
{
	struct tls_hostif *hif = tls_get_hostif();
	struct tls_hostif_tx_msg *tx_msg;
	u32 cpu_sr;
	int write_cnt; 
		
	cpu_sr = tls_os_set_critical();
	
	while (!dl_list_empty(&uart->tx_msg_pending_list))
	{	
        tx_msg = dl_list_first(&uart->tx_msg_pending_list,
        struct tls_hostif_tx_msg, list);
		tls_os_release_critical(cpu_sr);

		switch (tx_msg->type) 
		{
				
		}

		cpu_sr = tls_os_set_critical();
	}
	tls_os_release_critical(cpu_sr);
}

