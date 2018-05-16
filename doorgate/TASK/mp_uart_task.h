#ifndef MP_UART_TASK_H
#define MP_UART_TASK_H	

#include <stdbool.h>	

#include "mp_osal_rtos.h"		
#include "list_base.h"	
#include "mp_protocol_hostif.h"			


#define INS_SYNC_CHAR       0x01
#define INS_RICMD           0x02
#define INS_DATA            0x04


typedef struct tls_uart{
	struct tls_uart_port *uart_port;
	/** uart rx semaphore, notify receive a char */
	tls_os_mailbox_t         rx_mailbox;			
	/** uart tx semaphore, notify tx empty */
	tls_os_mailbox_t         tx_mailbox;

	bool	rx_idle;
	u8	inputstate;
	struct dl_list	tx_msg_pending_list;	

	    /** 
	     * tx callbak, notify user application tx complete, 
	     * user can use it, write new data to uart for transmit
	     */
	void (*tx_cb)(struct tls_uart *uart);
	u16 sksnd_cnt;
} tls_uart_t;

void tls_uart_init(void);


#endif


