#ifndef MP_UART_H
#define MP_UART_H	
#include <stdbool.h>
#include "mp_osal_rtos.h"	


#define TLS_UART_RX_BUF_SIZE   128		
#define TLS_UART_TX_BUF_SIZE   128											
#define WAKEUP_CHARS          (TLS_UART_TX_BUF_SIZE/2)  //  256
	
#define MBOX_MSG_UART_RX       1
#define MBOX_MSG_UART_TX       2

/** baud rate definition */
#define UART_BAUDRATE_B600          600	
#define UART_BAUDRATE_B1200	      1200
#define UART_BAUDRATE_B1800         1800	
#define UART_BAUDRATE_B2400         2400	
#define UART_BAUDRATE_B4800         4800	
#define UART_BAUDRATE_B9600         9600	
#define UART_BAUDRATE_B19200        19200	
#define UART_BAUDRATE_B38400        38400	
#define UART_BAUDRATE_B57600        57600 
#define UART_BAUDRATE_B115200       115200	
#define UART_BAUDRATE_B230400       230400 
#define UART_BAUDRATE_B460800       460800 
#define UART_BAUDRATE_B921600       921600 
#define UART_BAUDRATE_B1000000      1000000 
#define UART_BAUDRATE_B1250000      1250000 
#define UART_BAUDRATE_B1500000      1500000 
#define UART_BAUDRATE_B2000000      2000000 

     
#if 0
/* Return count in buffer.  */
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/* Return space available, 0..size-1.  We always leave one free char
   as a completely full buffer has head == tail, which is the same as
   empty.  */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

/* Return count up to the end of the buffer.  Carefully avoid
   accessing head and tail more than once, so they can change
   underneath us without returning inconsistent results.  */
#define CIRC_CNT_TO_END(head,tail,size) \
		({int end = (size) - (tail); \
		  int n = ((head) + end) & ((size)-1); \
		  n < end ? n : end;})

	
/* Return space available up to the end of the buffer.  */
#define CIRC_SPACE_TO_END(head,tail,size) \
	({int end = (size) - 1 - (head); \
	  int n = (end + (tail)) & ((size)-1); \
	  n <= end ? n : end+1;})
	  	


#define CIRC_SPACE_TO_END_FULL(head,tail,size) \
	({int end = (size) - 1 - (head); \
	  int n = (end + (tail)) & ((size)-1); \
	  n < end ? n : end+1;})	


#define uart_circ_empty(circ)		((circ)->head == (circ)->tail)
#define uart_circ_chars_pending(circ)	\
	(CIRC_CNT((circ)->head, (circ)->tail, TLS_UART_TX_BUF_SIZE))


#endif


struct tls_uart_baud_rate {
    u32 baud_rate;
    u16 ubdiv;
    u16 ubdiv_frac;
};
	
/** uart number enum */
enum {	
	TLS_UART_0 = 0,
	TLS_UART_1 = 1,
};


/** flow control rx flag */
typedef enum TLS_UART_RX_FLOW_CTRL_FLAG {
    TLS_UART_RX_DISABLE,
    TLS_UART_RX_ENABLE,
} TLS_UART_RX_FLOW_CTRL_FLAG_T;


typedef enum TLS_UART_STATUS {
    TLS_UART_STATUS_OK,
    TLS_UART_STATUS_ERROR,
} TLS_UART_STATUS_T; 

/** operation mode */
typedef enum TLS_UART_MODE {
    TLS_UART_MODE_POLL,           /* uart operation mode: poll */
    TLS_UART_MODE_INT,            /* uart operation mode: interrupt mode */
} TLS_UART_MODE_T;

	
struct tls_uart_icount {
	u32	cts;
	u32	dsr;
	u32	rng;
	u32	dcd;
	u32	rx;
	u32	tx;
	u32	frame;
	u32	overrun;
	u32	parity;
	u32	brk;
	u32	buf_overrun;
};


/** uart parameters */
typedef struct tls_uart_options {
	/** Set baud rate of the UART  */
	u32 baudrate;	

	/** Number of bits to transmit as a character (5 to 8). */
	u16 charlength;	

	/** 
     * Parity type: TLS_UART_PMODE_DISABLED, 
     *              TLS_UART_PMODE_EVEN,
     *              TLS_UART_PMODE_ODD  
     */
	u16 paritytype;

    /**
     * Flow control type
     */
    u16 flow_ctrl;

	/**
     * Number of stop bits between two characters:
	 * true: 2 stop bits
	 * false: 1 stop bit
     */
	u16 stopbits;

} tls_uart_options_t;



typedef struct tls_uart_circ_buf {
    u8 *buf;
    u32 head;
    u32 tail;
} tls_uart_circ_buf_t;


 
typedef struct tls_uart_port {
    /** uart number: 0 or 1 */
    u32                         uart_no;
	
    /** uart config parameters */
    struct tls_uart_options     opts;    

    int fcStatus;	//flow ctrl status,0 flow control closed,1 flow control open
    enum TLS_UART_RX_FLOW_CTRL_FLAG rxstatus;

    /** uart tx fifo trigger level */
    u32                         tx_fifofull;


    /** uart statistics information */
	struct tls_uart_icount	    icount;			

    /** uart ring buffer */
    struct tls_uart_circ_buf    recv;
    struct tls_uart_circ_buf    xmit;

    u8 hw_stopped;

	tls_os_sem_t tx_sem;	

	u8	ifusermode;	/*if in user mode*/
	char	*buf_ptr;
	u16 buf_len;
	s16 (*rx_callback)(char *buf, u16 len);
	s16 (*rx_callback_bk)(char *buf, u16 len);
	 
	s16 (*tx_callback)(struct tls_uart_port *port);	
	s16 (*tx_callback_bk)(struct tls_uart_port *port);	
} tls_uart_port_t;	

struct tls_param_uart {
	u32 baudrate;
	u8 stop_bits;
	u8 parity;
	u8 flow;
	u8 charsize;
};


/**********************************************************************************************************
* Description: 	This function is used to act the callback
*
* Arguments  : 	
*				
* Returns    : 		
*				
* Notes      : 	
**********************************************************************************************************/

void tls_uart_isr_rx_callback_x(int uart_no);

/**********************************************************************************************************
* Description: 	This function is used to put the data into buffer
*
* Arguments  : 	
*					
* Returns    : 		
*				
* Notes      : 		
**********************************************************************************************************/

void tls_uart_put_into_buffer(int uart_no,u8 ch);


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

int tls_uart_port_init(int uart_no, tls_uart_options_t *opts);


/**********************************************************************************************************
* Description: 	This function is used to set uart rx status.
*
* Arguments  : 	uart_no		is the uart number
*					uart_no	==	TLS_UART_0
*					uart_no	==	TLS_UART_1
*				flag			
*					flag		==	TLS_UART_RX_DISABLE
*					flag 		==	TLS_UART_RX_ENABLE
*Notes:			if user disalbe the rx,uart will not receive data until user enable the rx.
**********************************************************************************************************/

void tls_set_uart_rx_status(int uart_no,int status);


/**********************************************************************************************************
* Description: 	This function is used to fill tx buffer.
*
* Arguments  : 	port			is the uart port
* 				buf			is the user buffer.
*				count		is the user data length
*
* Returns    : 		WM_FAILED: Write error
*				data length(Bytes) written into hw
**********************************************************************************************************/

int tls_uart_write(struct tls_uart_port *port, char *buf, u32 count);


/**********************************************************************************************************
* Description: 	This function is used to start transfer data.
*
* Arguments  : 	port			is the uart port
*
* Returns    : 	
**********************************************************************************************************/

void tls_uart_tx_chars_start(struct tls_uart_port *port);


/**********************************************************************************************************
* Description: 	This function is used to register uart rx interrupt.
*
* Arguments  : 	uart_no		TLS_UART_0 or TLS_UART_1
*				callback		is the uart rx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/

void tls_uart_rx_register(u16 uart_no, s16(*rx_callback)(char *buf, u16 len));


/**********************************************************************************************************
* Description: 	This function is used to register uart tx interrupt.
*
* Arguments  : 	callback		is the uart tx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/

void tls_uart_tx_register(u16 uart_no, s16 (*tx_callback)(struct tls_uart_port *port));


/**********************************************************************************************************
* Description: 	This function is used to transfer data asynchronous.
*
* Arguments  : 	buf			is a buf for saving user data. 
* 		len			is the data length.
*
* Returns    : 		WM_SUCCESS: TX ok;
*				WM_FAILED:	  error
* Notes		:	The function only start transmission, fill buffer in the callback function.
**********************************************************************************************************/
int tls_uart_tx(char *buf, u16 len);

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
bool tls_uart_tx_finished(void);

/**********************************************************************************************************
* Description: 	This function is used to get tx transfer data length.
*
* Arguments  :
*
* Returns    : 		length	
**********************************************************************************************************/
int tls_uart_tx_length(void);

/**********************************************************************************************************
* Description: 	This function is used to transfer data synchronous.
*
* Arguments  : 	buf			is a buf for saving user data. 
* 				len			is the data length.
*
* Returns    : 		WM_SUCCESS 	tx success
*				WM_FAILED		tx failed  	
* Notes		:	
**********************************************************************************************************/
int tls_uart_tx_sync(char *buf, u16 len);

/**********************************************************************************************************
* Description: 	This function is used to config uart port for user mode.
*
* Arguments  : 	
*
* Returns    : 	
* Notes      : 	If user want to use uart function,must call this function.
*			This function must be called before the register function.
**********************************************************************************************************/
void tls_uart_cfg_user_mode(void);

/**********************************************************************************************************
* Description: 	This function is used to disable uart user mode.
*
* Arguments  : 	
*
* Returns    : 	
* Notes      : 	If user want to resume uart old function for AT commond,must call this function.
**********************************************************************************************************/
void tls_uart_disable_user_mode(void);

/**********************************************************************************************************
* Description: 	This function is used to register uart rx interrupt.
*
* Arguments  : 	callback		is the uart rx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/
void tls_user_uart_rx_register(s16 (*rx_callback)(char *buf, u16 len));

/**********************************************************************************************************
* Description: 	This function is used to register uart tx interrupt.
*
* Arguments  : 	callback		is the uart tx interrupt call back function.
*
* Returns    : 
*
**********************************************************************************************************/
void tls_user_uart_tx_register(s16 (*tx_callback)(struct tls_uart_port *port));




//如果想串口中断接收，请不要注释以下宏定义
void STM32_USART_Config(int uart_no, tls_uart_options_t *opts);











	

int CIRC_CNT(u32 head,u32 tail,u16 size);	

int CIRC_SPACE(u32 head,u32 tail,u16 size);

int CIRC_CNT_TO_END(u32 head,u32 tail,u16 size); 

int CIRC_SPACE_TO_END(u32 head,u32 tail,u16 size); 

int CIRC_SPACE_TO_END_FULL(u32 head,u32 tail,u16 size);

int uart_circ_empty(struct tls_uart_circ_buf *circ);		

int uart_circ_chars_pending(struct tls_uart_circ_buf *circ);		

	

	
#endif 
