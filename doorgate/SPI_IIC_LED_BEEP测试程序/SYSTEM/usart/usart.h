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
		

/* RS485芯片发送使能GPIO, PB2 */
#define RCC_RS485_TXEN 	 	RCC_APB2Periph_GPIOA
#define PORT_RS485_TXEN  		GPIOA	
#define PIN_RS485_TXEN	 		GPIO_Pin_11	
	
#define RS485_RX_EN()	GPIO_ResetBits(PORT_RS485_TXEN,PIN_RS485_TXEN)					
#define RS485_TX_EN()	GPIO_SetBits(PORT_RS485_TXEN,PIN_RS485_TXEN)							
	

UART_REC *Get_Usart(void);	

void uart_init(u32 bound);

void USART_Send(uint8_t *Buffer, uint32_t Length);


#endif


