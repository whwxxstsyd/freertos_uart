#ifndef __USART_H
#define __USART_H
#include <stdio.h>	
#include "sys.h" 	

#define countof(a) (sizeof(a) / sizeof(*(a)))

#define USART_REC_LEN  200	
	
//接收状态标记	
typedef struct _UART_REC
{	
	u8 Len_Count;
	u8 Rec_Len; 		
	u8 Rec_Over_Flag; 		
	u8 RxTimeout; 		
	u8 USART_RX_BUF[USART_REC_LEN];	
}UART_REC;
		

UART_REC *Get_Usart(void);	

void uart_init(u32 bound);


#endif


