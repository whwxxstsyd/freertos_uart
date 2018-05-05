#include "usart.h"	
#include "string.h"
#include "mp_uart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}


//__use_no_semihosting was requested, but _ttywrch was
_ttywrch(int ch)
{
	ch = ch;	
} 	


#endif 

	
u8 intr_counter = 0;		


static void USART_RS485IO(void);
	


//功能描述  : STM32串口管脚配置
void STM32_USART1_GPIOConfig(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//  使能时钟

	/*串口1使用GPIOA的9,10脚*/ 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);//  使能时钟  
	//USART1 TX	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//USART1 Rx (PA.10)输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//使能相应中断，	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//RS485通信方向管控脚	
	USART_RS485IO();

	intr_counter = 0;
}




void STM32_USART2_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
		
	/*串口2重映射使用GPIOD的5,6脚*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  使能时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);//  使能时钟
	GPIO_PinRemapConfig(GPIO_Remap_USART2 , ENABLE);//重映射	
	//usart2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
	//USART2 Rx  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	//使能相应中断，
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}



void STM32_USART3_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/*串口3完全映射使用GPIOD的8,9脚*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  使能时钟 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);//  使能时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3 , ENABLE); //全映射  
	//usart3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
	//USART3 Rx  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  

	//使能相应中断，
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}	



void STM32_USART5_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); 

	/*串口3使用GPIOB的10,11脚*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);//  使能时钟
	//USAR5 TX	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	//USAR5 Rx  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  使能时钟  
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												  
	NVIC_Init(&NVIC_InitStructure);		

}


//功能块  : 串口RS485方向控制脚通信 配置
void USART_RS485IO(void)
{      
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//============USART1 RS485/422DIR引脚方向设置
	RCC_APB2PeriphClockCmd(GPIO485Direction_RCC_CLK, ENABLE);
	//读端口初始化为推挽输出 
	GPIO_InitStructure.GPIO_Pin =  GPIO485Direction_PIN; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIO485Direction_PORT, &GPIO_InitStructure); 
	GPIO485Direction_Receive();    //收状态	
}  



void Uart_SendByte(u8 byte) //发送1字节数据
{
	while(!((USART1->SR)&(1<<7)));
	USART1->DR = byte;	 	
}

	
void Uart_SendData(u8 *Buffer, u32 Length)
{
	while(Length != 0)
	{
		while(!((USART1->SR)&(1<<7)));//等待发送完
		USART1->DR = *Buffer; 
		Buffer++;
		Length--;
	}
}



static USART_TypeDef* COMX[]={USART1,USART2,USART3,UART4,UART5};	
	
void tls_uart_rx_disable(int uart_no)
{
	USART_ITConfig(COMX[uart_no],USART_IT_RXNE, DISABLE);  	
}

void tls_uart_rx_enable(int uart_no)
{	
   	USART_ITConfig(COMX[uart_no],USART_IT_RXNE, ENABLE);   
}

void tls_uart_tx_disable(int uart_no)
{
	USART_ITConfig(COMX[uart_no],USART_IT_TC, DISABLE);
}
	
void tls_uart_tx_enable(int uart_no)
{	
 	USART_ITConfig(COMX[uart_no],USART_IT_TC, ENABLE);	
}


 
#if EN_USART1_RX   //如果使能了接收


void USART1_IRQHandler(void)               //串口1中断服务程序
{			
	//全局判断是否有中断发生
		

	//发送中断	
	if( USART_GetITStatus(USART1,USART_IT_TXE) )
	{		
		USART_ClearITPendingBit(USART1,USART_IT_TXE);	

		//完成中断发送
		intr_counter = 0;	
	} 

	
	//接收中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{		
		intr_counter= 1;	
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);				
				
		tls_uart_put_into_buffer(COM1,USART_ReceiveData(USART1));	
	} 
	//接受不定长的数据	
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{			
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
			
		USART_ReceiveData(USART1);			

		tls_uart_isr_rx_callback_x(COM1);			
		
		//完成中断接受
		intr_counter = 0;
	}			



}


u8 tls_get_usrt1_isr_count(void)
{	
	return intr_counter;
}	


#endif	




#if EN_USART2_RX	

void USART2_IRQHandler(void)    
{
		
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{		
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);		
	
		//tls_uart_put_into_buffer(COM2,USART_ReceiveData(USART2));	
				
	} 
	//接受不定长的数据
	else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{		
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
		USART_ReceiveData(USART2);			
			
		//tls_uart_isr_rx_callback_x(COM2);				
	}	

	
}

#endif	


