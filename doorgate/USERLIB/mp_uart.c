#include <stdbool.h>	

#include "mp_uart.h"
#include "mp_mem.h"	
#include "usart.h"
#include "debug.h"	
#include "delay.h"
#include "param.h"		


struct tls_uart_port uart_port[2];	
static void tls_uart_tx_chars(struct tls_uart_port *port);


/**********************************************************************************************************
* Description: 	This function is used in interrupt.
* 
* Arguments  : 	callback		is the uart tx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/

//将串口接收到的数据放入缓冲区，在USART.c的中断中被调用
void tls_uart_put_into_buffer(int uart_no,u8 ch)		
{	
	struct tls_uart_port *port = &uart_port[uart_no];	
	struct tls_uart_circ_buf *recv = &port->recv;	
	
	if (CIRC_SPACE(recv->head, recv->tail, TLS_UART_RX_BUF_SIZE) <= 2) 
	{	
		port->icount.buf_overrun++;
		LOG_DEBUG("rx buf overrun\n");
		
	}		
	
	/* insert the character into the buffer */
	recv->buf[recv->head] = ch;
	recv->head = (recv->head + 1) & (TLS_UART_RX_BUF_SIZE - 1);
	port->icount.rx++;
}	


//串口接收到数据后执行回调函数，在USART.c文件中执行
void tls_uart_isr_rx_callback_x(int uart_no)				
{			
	struct tls_uart_port *port = &uart_port[uart_no];		
	struct tls_uart_circ_buf *recv = &port->recv;		
	
	if(port->rx_callback != NULL)
	{	
		port->rx_callback((char *)(recv->buf + recv->tail), (u16)port->icount.rx);
	}		
}	



//串口发送数据的时候，在中断中执行
void tls_uart_isr_tx(int uart_no)			
{
	struct tls_uart_port *port = &uart_port[uart_no];	

	tls_uart_tx_chars(port);
}


/**********************************************************************************************************
* Description: 	This function is used to fill tx buffer.
*
* Arguments  : 	port			is the uart port
* 				buf			is the user buffer.
*				count		is the user data length
*
* Returns    : 		
**********************************************************************************************************/
int tls_uart_write(struct tls_uart_port *port, char *buf, u32 count)
{
    struct tls_uart_circ_buf *xmit;
    int c;		
    int ret = 0;

    xmit = &port->xmit;		

    if (!xmit->buf)		
        return WM_FAILED;

    while (1) 
	{			
	
	    //c = CIRC_SPACE_TO_END(xmit->head, xmit->tail, TLS_UART_TX_BUF_SIZE);		
	    if (count < c)
	        c = count;
	    if (c <= 0)
	        break;		
	    MEMCPY((char *)(xmit->buf + xmit->head), buf, c);
	    xmit->head = (xmit->head + c) & (TLS_UART_TX_BUF_SIZE - 1);
	    buf += c;
	    count -= c;
	    ret += c;
    }

    return ret;
}



/**********************************************************************************************************
* Description: 	This function is used to start transfer data.
*
* Arguments  : 	port is the uart port
*
* Returns    : 	
**********************************************************************************************************/
void tls_uart_tx_chars_start(struct tls_uart_port *port)
{
    struct tls_uart_circ_buf *xmit = &port->xmit;
    int tx_count;
    u32 cpu_sr;
    
    /* send some chars */
    tx_count = 32;
    cpu_sr = tls_os_set_critical();

    while (!uart_circ_empty(xmit)) 	
	{			
		/*send the data*/
		Uart_SendByte(xmit->buf[xmit->tail]);			
		
        xmit->tail = (xmit->tail + 1) & (TLS_UART_TX_BUF_SIZE - 1);
    }	

	
    tls_os_release_critical(cpu_sr);
    return;
}




/**********************************************************************************************************
* Description: 	This function is used to continue transfer data.
*
* Arguments  : 	port is the uart port
*
* Returns    : 	
**********************************************************************************************************/
static void tls_uart_tx_chars(struct tls_uart_port *port)
{	
    struct tls_uart_circ_buf *xmit = &port->xmit;
    int tx_count = 64;
    int len;
	
    if (uart_circ_empty(xmit))
	{	
       // tls_uart_tx_disable(port);
		
		if(port->tx_sem)	
			tls_os_sem_release(port->tx_sem);	

	        return;	
    }

    while (!uart_circ_empty(xmit) && tx_count-- > 0) 
	{
        /*check if fifo is full*/
			

		/*send the data*/
		Uart_SendByte(xmit->buf[xmit->tail]);			
		
        xmit->tail = (xmit->tail + 1) & (TLS_UART_TX_BUF_SIZE - 1);
        port->icount.tx++;
    }

    len = uart_circ_chars_pending(xmit);
    if (len < WAKEUP_CHARS) 
	{
		if(port->tx_callback)
			port->tx_callback(port);
    }

#if 0
    if (uart_circ_empty(xmit))
        tls_uart_tx_disable(port);
#endif
}





/**********************************************************************************************************
* Description: 	This function is used to initial uart port.
*
* Arguments  : 	uart_no		is the uart number
*					uart_no	==	TLS_UART_0
*					uart_no	==	TLS_UART_1
*				opts			is the uart setting options,if this param is NULL,this function will use the default options.
* Returns    : 		WM_SUCCESS: init correctly;
*				WM_FAILED: error;			
* Notes      : 		When the system is initialized, the function has been called, so users can not call the function.
**********************************************************************************************************/
int tls_uart_port_init(int uart_no, tls_uart_options_t *opts)
{	
	struct tls_uart_port *port;
	int ret;
	char *bufrx,*buftx;
	tls_uart_options_t opt;

	if(TLS_UART_0 == uart_no)
	{	
		memset(&uart_port[0], 0, sizeof(struct tls_uart_port));
		port = &uart_port[0];
	}
	else if(TLS_UART_1 == uart_no)
	{
		memset(&uart_port[1], 0, sizeof(struct tls_uart_port)); 
		port = &uart_port[1];	
	}
	else
	{	
		return WM_FAILED;
	}

 	port->uart_no = uart_no;	

	if(NULL == opts)
	{	
		opt.baudrate = UART_BAUDRATE_B9600;		
		opt.charlength = USART_WordLength_8b;	
		opt.flow_ctrl = USART_HardwareFlowControl_None;
		opt.paritytype = USART_Parity_No;	
		opt.stopbits = USART_StopBits_1;
		STM32_USART_Config(uart_no,&opt);			
	}
	else
	{	
		STM32_USART_Config(uart_no,opts);		
	}	


	port->rxstatus = TLS_UART_RX_ENABLE;
		
	
#if 1		
	//接受缓冲区		
	bufrx = tls_mem_alloc(TLS_UART_RX_BUF_SIZE);
	if (!bufrx)	
	{	
		return WM_FAILED;	
	}	
	
	memset(bufrx, 0, TLS_UART_RX_BUF_SIZE);
 	port->recv.buf = (u8 *)bufrx;
 	port->recv.head = 0;
 	port->recv.tail = 0;


	//发送缓冲区	
 	buftx = tls_mem_alloc(TLS_UART_TX_BUF_SIZE);
	if (!buftx)
	{
		tls_mem_free(bufrx);
		return WM_FAILED;
	}
	memset(buftx, 0, TLS_UART_TX_BUF_SIZE);
	port->xmit.buf = (u8 *)buftx;
	port->xmit.head = 0;	
	port->xmit.tail = 0;

	port->tx_fifofull = 16;

#endif
	return WM_SUCCESS;
}





/**********************************************************************************************************
* Description: 	This function is used to register uart rx interrupt.
*
* Arguments  : 	callback		is the uart rx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/
void tls_uart_rx_register(u16 uart_no, s16 (*rx_callback)(char *buf, u16 len))
{	
	if(COM1 == uart_no)	
		uart_port[0].rx_callback = rx_callback;
	else if(COM2 == uart_no)	
		uart_port[1].rx_callback = rx_callback;
}
	
/**********************************************************************************************************
* Description: 	This function is used to register uart tx interrupt.
*
* Arguments  : 	callback		is the uart tx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/
void tls_uart_tx_register(u16 uart_no, s16 (*tx_callback)(struct tls_uart_port *port))
{
	if(COM1 == uart_no)	
		uart_port[0].tx_callback = tx_callback;
	else if(COM2 == uart_no)
		uart_port[1].tx_callback = tx_callback;
}

/**********************************************************************************************************
* Description: 	This function is used to copy circular buffer data to user buffer.
*
* Arguments  : 	buf		is the user buffer
*				bufsize	is the user buffer size
*				recv		is the uart rx circular buffer
*
* Returns    : 		copy data size
*
**********************************************************************************************************/
int tls_uart_copy_rxbuf(u8 *buf, int bufsize, struct tls_uart_circ_buf *recv)
{
	int data_cnt,buflen,bufcopylen;
	
	if(NULL == buf || NULL == recv)
		return WM_FAILED;

	data_cnt = CIRC_CNT(recv->head, recv->tail, TLS_UART_RX_BUF_SIZE);
	//TLS_DBGPRT_INFO("\ndata cnt=%d\n",data_cnt);
	if (data_cnt >= bufsize) {
		buflen = bufsize;
	}
	else
	{
    		buflen = data_cnt;
	}
	if ((recv->tail + buflen) > TLS_UART_RX_BUF_SIZE)
	{
		bufcopylen = (TLS_UART_RX_BUF_SIZE - recv->tail);
		memcpy(buf, recv->buf + recv->tail, bufcopylen);
		memcpy(buf + bufcopylen, recv->buf, buflen - bufcopylen);
	}
	else
	{
    	memcpy(buf, recv->buf + recv->tail, buflen);
	}
	recv->tail = (recv->tail + buflen) & (TLS_UART_RX_BUF_SIZE - 1);
	return buflen;
}




u8 *testbuf;
u16 flag = 0;
u16 datalen = 0;

#ifdef TEST_UART
/**********************************************************************************************************
* Description: 	This function is used to receive uart data.
*
* Arguments  : 	buf			is a buf for saving received data. 
*
* Returns    : 	Return received data length.
* Notes	     : 	
**********************************************************************************************************/
s16 tls_uart_rx_test(char *buf, u16 len)
{	
	if(datalen < (4096 - len))
	{	
		memcpy(testbuf + datalen, buf, len);
		flag = 1;
		datalen += len;
	}
	else
	{
		LOG_INFO("\rx buffer over\n");
	}
	return WM_SUCCESS;
}


void tls_uart_test(void)
{
	int len = 0;
	
	LOG_INFO("\nentry tls_uart_test\n");
	testbuf = tls_mem_alloc(4096);
  	if (!testbuf)
     		return;
	memset(testbuf, 0, 4096);
	tls_uart_cfg_user_mode();
	tls_user_uart_rx_register( tls_uart_rx_test);
#if 0	
	tls_user_uart_set_baud_rate(UART_BAUDRATE_B38400);
	tls_user_uart_set_stop_bits(TLS_UART_ONE_STOPBITS);
	tls_user_uart_set_parity(TLS_UART_PMODE_DISABLED);
#endif
	LOG_INFO("\nentry while\n");
	while(1)
	{
		//TLS_DBGPRT_INFO("\n==============flag=%d\n",flag);
	
		delay_ms(0x100);	
		if(flag)
		{	
			LOG_INFO("\n\rrx data len = %d\n%s\n\r",datalen,testbuf);
			
			//tls_uart_tx("\n\r",2);
			tls_uart_tx_sync(testbuf,datalen);
			len = tls_uart_tx_length();
			printf("\ntx len=%d\n",len);
			
			len = tls_uart_tx_length();
			printf("\ntx len 0=%d\n",len);

			//tls_uart_tx("\n\r",2);
			tls_uart_tx_sync("abcde",5);
			len = tls_uart_tx_length();
			printf("\ntx len 1=%d\n",len);
			
			flag = 0;
			datalen = 0;
		}
	}
	
}

#endif




/**********************************************************************************************************
* Description: 	This function is used to config uart port for user mode.
*
* Arguments  : 	
*
* Returns    : 	
* Notes      : 	If user want to use uart function,must call this function.
**********************************************************************************************************/
void tls_uart_cfg_user_mode(void)
{
	struct tls_uart_port *port = &uart_port[1];

	port->ifusermode = 1;
	memset(port->recv.buf, 0, TLS_UART_RX_BUF_SIZE);
	port->recv.head = 0;
	port->recv.tail = 0;
	port->icount.rx = 0;
	
	port->rx_callback_bk = port->rx_callback;
	port->rx_callback = NULL;
	
	port->tx_callback_bk = port->tx_callback;
	port->tx_callback = NULL;
}

/**********************************************************************************************************
* Description: 	This function is used to disable uart user mode.
*
* Arguments  : 	
*
* Returns    : 	
* Notes      : 	If user want to resume uart old function for AT commond,must call this function.
**********************************************************************************************************/
void tls_uart_disable_user_mode(void)
{
	struct tls_uart_port *port = &uart_port[1];

	port->ifusermode = 0;
	memset(port->recv.buf, 0, TLS_UART_RX_BUF_SIZE);
	port->recv.head = 0;
	port->recv.tail = 0;

	port->rx_callback = port->rx_callback_bk;
	port->rx_callback_bk = NULL;
	
	port->tx_callback = port->tx_callback_bk;
	port->tx_callback_bk = NULL;
}


static s16 tls_uart_tx_cb(struct tls_uart_port *port)
{	
	int ret_len = 0;
	
	if(NULL == port)
		return WM_FAILED;
	if(port->buf_len > 0)		//uart1,put remained data to buffer after buffer transfer completed
	{
	    	ret_len = tls_uart_write(port, port->buf_ptr, port->buf_len);
		//TLS_DBGPRT_INFO("\ntx cb write len=%d",ret_len);

		if(ret_len >= 0)
		{
			if(port->buf_len >= ret_len)
			{
				port->buf_len -= ret_len;	
				port->buf_ptr += ret_len;
			}
		}
	}
	
	return WM_SUCCESS;
}




/**********************************************************************************************************
* Description: 	This function is used to transfer data asynchronous.
*
* Arguments  : 	buf			is a buf for saving user data. 
* 				len			is the data length.
*
* Returns    : 		
* Notes		:	The function only start transmission, fill buffer in the callback function.
**********************************************************************************************************/
int tls_uart_tx(char *buf, u16 len)
{	
	struct tls_uart_port *port = &uart_port[1];
	int ret_len;

	if(NULL == buf || len < 1 || port->buf_len != 0)
		return WM_FAILED;

	port->icount.tx = 0;
	tls_uart_tx_register(TLS_UART_1, tls_uart_tx_cb);
	port->buf_ptr = buf;
	port->buf_len= len;
	
	ret_len = tls_uart_write(port, port->buf_ptr, port->buf_len);
	//TLS_DBGPRT_INFO("\ntx write len=%d",ret_len);
	if(ret_len >= 0)
	{
		if(port->buf_len >= ret_len)
		{	
			port->buf_ptr += ret_len;
			port->buf_len -= ret_len;
			//TLS_DBGPRT_INFO("\start uart1 tx\n");
			tls_uart_tx_chars_start(port);
		}
	}

	return WM_SUCCESS;
}

/**********************************************************************************************************
* Description: 	This function is used to judge data copy if finished.
*
* Arguments  : 	
* 				
*
* Returns    : 		TRUE    data copy finished
*                               FALSE  not finished
* Notes		:	
**********************************************************************************************************/
bool tls_uart_tx_finished(void)
{
    struct tls_uart_port *port = &uart_port[1];
    
    if(port->buf_len > 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

//Get UART1 tx's data length
int tls_uart_tx_length(void)
{
	struct tls_uart_port *port = &uart_port[1];
	
	return port->icount.tx;
}

/**********************************************************************************************************
* Description: 	This function is used to transfer data synchronous.
*
* Arguments  : 	buf			is a buf for saving user data. 
* 				len			is the data length.
*
* Returns    : 		
* Notes		:	
**********************************************************************************************************/
int tls_uart_tx_sync(char *buf, u16 len)
{
	struct tls_uart_port *port = &uart_port[1];
	u8 err;		
		
	err = tls_os_sem_create(port->tx_sem, 0);
	if (err != TLS_OS_SUCCESS) 
	{
		LOG_ERROR("\ntx sem create fail\n");
		return WM_FAILED;
	}

	tls_uart_tx(buf, len);

	tls_os_sem_acquire(port->tx_sem, 0);
	tls_os_sem_delete(port->tx_sem);
	port->tx_sem = NULL;
	
	return WM_SUCCESS;
}

int tls_uart_read(char *buf, u16 len)
{	
	return tls_uart_copy_rxbuf((u8 *)buf, len, &uart_port[1].recv);
}
/**********************************************************************************************************
* Description: 	This function is used to register uart rx interrupt.
*
* Arguments  : 	callback		is the uart rx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/
void tls_user_uart_rx_register(s16 (*rx_callback)(char *buf, u16 len))
{
	tls_uart_rx_register(TLS_UART_1, rx_callback);
}

/**********************************************************************************************************
* Description: 	This function is used to register uart tx interrupt.
*
* Arguments  : 	callback		is the uart tx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/
void tls_user_uart_tx_register(s16 (*tx_callback)(struct tls_uart_port *port))
{
	tls_uart_tx_register(TLS_UART_1, tx_callback);
}






static USART_TypeDef* COMX[]={USART1,USART2,USART3,UART4,UART5};

	
	
void STM32_USART_Config(int uart_no, tls_uart_options_t *opts) 
{ 			
	USART_InitTypeDef USART_InitStructure;

	USART_Cmd(COMX[uart_no], DISABLE);//关闭中断
	//管脚配置	
	if(uart_no==COM1) 
		STM32_USART1_GPIOConfig();
	else if(uart_no==COM2) 	
		STM32_USART2_GPIOConfig();
	else if(uart_no==COM3) 
		STM32_USART3_GPIOConfig();
	else if(uart_no==COM4)
		STM32_USART5_GPIOConfig();		

	USART_InitStructure.USART_BaudRate = opts->baudrate;		//波特率 2015060303
	USART_InitStructure.USART_WordLength = opts->charlength; 	//数据长度
	USART_InitStructure.USART_StopBits = opts->stopbits;		//停止位
	USART_InitStructure.USART_Parity = opts->paritytype; 		//校验位
	USART_InitStructure.USART_HardwareFlowControl = opts->flow_ctrl;	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使能发送和接收

	USART_Init(COMX[uart_no], &USART_InitStructure);  	
	USART_Cmd(COMX[uart_no], ENABLE);	//USART 使能开中断
	USART_ITConfig(COMX[uart_no],USART_IT_RXNE, ENABLE);   //使能串口接收中断 
	USART_ITConfig(COMX[uart_no],USART_IT_IDLE, ENABLE);   //使能串口接收完成中断
}















/* Return count in buffer.  */
int CIRC_CNT(u32 head,u32 tail,u16 size)
{
	return (((head) - (tail)) & ((size)-1));
}

/* Return space available, 0..size-1.  We always leave one free char
   as a completely full buffer has head == tail, which is the same as
   empty.  */
int CIRC_SPACE(u32 head,u32 tail,u16 size) 
{	
	return CIRC_CNT((tail),((head)+1),(size));
}

/* Return count up to the end of the buffer.  Carefully avoid
   accessing head and tail more than once, so they can change
   underneath us without returning inconsistent results.  */
int CIRC_CNT_TO_END(u32 head,u32 tail,u16 size) 
{
	int end = (size) - (tail); 
	int n = ((head) + end) & ((size)-1); 
	return (n < end ? n : end);	
}


	
/* Return space available up to the end of the buffer.  */
int CIRC_SPACE_TO_END(u32 head,u32 tail,u16 size) 
{
	int end = (size) - 1 - (head); 
	int n = (end + (tail)) & ((size)-1); 
	return (n <= end ? n : end+1);	
}

int CIRC_SPACE_TO_END_FULL(u32 head,u32 tail,u16 size)
{	
	int end = (size) - 1 - (head);
	int n = (end + (tail)) & ((size)-1);
	return (n < end ? n : end+1);
}	

	
int uart_circ_empty(struct tls_uart_circ_buf *circ)		
{	
	if(circ->head == circ->tail)
	{	
		return 1;
	}
	else
	{
		return 0;	
	}
}


int uart_circ_chars_pending(struct tls_uart_circ_buf *circ)		
{		
	return CIRC_CNT((circ)->head, (circ)->tail, TLS_UART_TX_BUF_SIZE);
}
	

