#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//定义使用TC方式引发发送中断
#define USART_STM32SendIT_TC 

//485方向管理
//使用到的为GPIOD
#define GPIO485Direction_RCC_CLK			RCC_APB2Periph_GPIOA

#define GPIO485Direction_PORT				GPIOA	
#define GPIO485Direction_PIN				GPIO_Pin_11   

//发送	
#define GPIO485Direction_Send()   			GPIO_SetBits(GPIO485Direction_PORT,GPIO485Direction_PIN)				
#define GPIO485Direction_Receive()   		GPIO_ResetBits(GPIO485Direction_PORT,GPIO485Direction_PIN)


//***********************串口信息********************************************************
#define 	COM1		   0
#define 	COM2		   1
#define 	COM3		   3
#define 	COM4		   4	

	
#define USART_REC_LEN  		200  	//定义最大接收字节数200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	
  	
//接收状态标记	
typedef struct _UART_REC
{
	u8 *RecData;
	u8 Rec_Over_Flag; 	
	u16 Len_Count;	
	u16 Rec_Len;	 
	u16 RxTimeout; 			
}UART_REC;	



void tls_uart_rx_disable(int uart_no);

void tls_uart_rx_enable(int uart_no);

void tls_uart_tx_disable(int uart_no);

void tls_uart_tx_enable(int uart_no);

void Uart_SendByte(u8 byte);	

void Uart_SendData(u8 *Buffer, u32 Length);

void STM32_USART1_GPIOConfig(void);

void STM32_USART2_GPIOConfig(void);

void STM32_USART3_GPIOConfig(void);

void STM32_USART5_GPIOConfig(void);	


u8 tls_get_usrt1_isr_count(void);


#endif


