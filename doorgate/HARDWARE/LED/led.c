#include "FreeRTOS.h"
#include "task.h"

#include "led.h"	
#include "delay.h"
#include "param.h"	
#include "mp_osal_rtos.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void Led_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	//使能PB,PE端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_8); 					 //PB.5 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOE, &GPIO_InitStructure);				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_0); 					 //PE.5 输出高 
}

void My_Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				
	GPIO_SetBits(GPIOA,GPIO_Pin_8);							

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				
	GPIO_SetBits(GPIOD,GPIO_Pin_2);					
}
	 





#define		LED_RUN_MASK			(0x01<<0)
#define		LED_FUN_MASK			(0x01<<1)		
	
#define		LED_RUN_ON_MASK		(0x01<<4)
#define		LED_FUN_ON_MASK		(0x01<<5)

#define LED_RUN_PERIOD 	M2T(300)				
#define LED_FUN_PERIOD 	M2T(600)				


static u8 current_blink_led;
static u8 current_led_state;	
static u8 current_blink_frq;		

static tls_os_timer_t 	led_run_timer;	
static tls_os_timer_t 	led_fun_timer;	




void Led_Test(void)
{				
	hw_platform_start_led_blink(LED_RUN,LED_RUN_PERIOD);			
	hw_platform_start_led_blink(LED_FUN,LED_FUN_PERIOD);		
}	
	
/**
* @brief	LED控制接口
* @param[in]  unsigned int led	 LED_RED or  LED_GREEN  or LED_YELLOW
* @param[in]  unsigned int ctrl  0: close   1:open
* @return   none                 
*/
void hw_platform_led_ctrl(u16 led,u16 ctrl)
{
	if (led == LED_RUN)
	{	
		if (ctrl)
		{	
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		}
		else
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_8);			
		}
	}
	else if (led == LED_FUN)		
	{	
		if (ctrl)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
		}
		else	
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_0);	
		}
	}
}



static void led_run_blink_timer_hook(tls_os_timer_t timer)	
{			
	current_led_state ^= LED_RUN_MASK;
	hw_platform_led_ctrl(LED_RUN,current_led_state & LED_RUN_MASK);
}	

static void led_fun_blink_timer_hook(tls_os_timer_t timer)	
{	
	current_led_state ^= LED_FUN_MASK;			
	hw_platform_led_ctrl(LED_FUN,current_led_state & LED_FUN_MASK);
}


/**
 * @brief 开始LED闪烁指示
 * @param[in] unsigned int		led		
 * @param[in] unsigned short	delay	闪烁的时间间隔，也就是闪烁频率,单位10ms
 * @note 注意此接口只能让一个LED在闪烁，不能实现几个LED同时闪烁
*/
int hw_platform_start_led_blink(u16 led,u16 delay)
{		
	if (led == LED_RUN)
	{	
		if (current_led_state & LED_RUN_ON_MASK)
		{	
			current_led_state |= LED_RUN_MASK;
			tls_os_timer_change(led_run_timer,0);
		}	
		else
		{
			current_led_state |= (LED_RUN_MASK | LED_RUN_ON_MASK);
			tls_os_timer_create(&led_run_timer,  
									delay,			 	
									pdTRUE, 				
									NULL, 			
									led_run_blink_timer_hook); 

			tls_os_timer_start(led_run_timer);	
		}

		return TRUE;	
	}
	else if (led == LED_FUN)	
	{	
		if (current_led_state & LED_FUN_ON_MASK)
		{		
			current_led_state |= LED_FUN_MASK;
			tls_os_timer_change(led_fun_timer,0);	
		}
		else
		{		
			current_led_state |= (LED_FUN_MASK | LED_FUN_ON_MASK);
			tls_os_timer_create(&led_fun_timer, 		 
									delay,	 		
									pdTRUE, 					
									NULL, 		
									led_fun_blink_timer_hook); 	

			tls_os_timer_start(led_fun_timer);	
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * @brief 关闭正在闪烁的LED
*/
int hw_platform_stop_led_blink(u16 led)	
{			
	if(led == LED_RUN)	
	{	
		hw_platform_led_ctrl(LED_RUN,0);			

		if (current_led_state & LED_RUN_ON_MASK)
		{
			current_led_state &= ~LED_RUN_MASK;
			current_led_state &= ~LED_RUN_ON_MASK;
			tls_os_timer_stop(led_run_timer);
		}
						
		return TRUE;
	}
	else if (led == LED_FUN)	
	{
		hw_platform_led_ctrl(LED_FUN,0);			

		if (current_led_state & LED_FUN_ON_MASK)
		{
			current_led_state &= ~LED_FUN_MASK;
			current_led_state &= ~LED_FUN_ON_MASK;
			tls_os_timer_stop(led_fun_timer);	
		}
						
		return TRUE;
	}
	else
	{
		return FALSE;	
	}

}





