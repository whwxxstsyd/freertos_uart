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



void Led_Test(void)
{	
	u8 i;
	u8 Blink_Num = 8;//快闪次数
	
	for(i=0;i<Blink_Num;i++)	
	{		
		delay_ms(1000);	
		LED0=!LED0;
		LED1=!LED1;			
	}
}	


static u8	current_blink_led;
static u8	current_led_state;
static u8   current_blink_frq;
static u16  current_timer_cnt;	
static u8 	led_running;

static tls_os_timer_t 	led_timer;	
#define TIMER_LED_PERIOD 	M2T(500) 		

	
/**
* @brief	LED控制接口
* @param[in]  unsigned int led	 LED_RED or  LED_GREEN  or LED_YELLOW
* @param[in]  unsigned int ctrl  0: close   1:open
* @return   none                 
*/
void hw_platform_led_ctrl(u16 led,u16 ctrl)
{
	if (led == LED0)
	{
		if (ctrl)
		{
			GPIO_ResetBits(GPIOF, GPIO_Pin_6);
		}
		else
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_6);
		}
	}
	else if (led == LED1)	
	{	
		if (ctrl)
		{
			GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		}
		else
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_7);
		}
	}
}



static void led_blink_timer_hook(tls_os_timer_t timer)	
{	
	current_timer_cnt++;
	if (current_timer_cnt == current_blink_frq*2*10)
	{	
		current_led_state ^= 0x01;
		hw_platform_led_ctrl(current_blink_led,current_led_state);
		current_timer_cnt = 0;
	}
}

/**
 * @brief 开始LED闪烁指示
 * @param[in] unsigned int		led		
 * @param[in] unsigned short	delay	闪烁的时间间隔，也就是闪烁频率,单位10ms
 * @note 注意此接口只能让一个LED在闪烁，不能实现几个LED同时闪烁
*/
int hw_platform_start_led_blink(u16 led,u16 delay)
{		
	if(!led_running)
	{
		current_timer_cnt = 0;
		current_blink_led = led;
		current_blink_frq = delay;
		current_led_state = 1;
		led_running = 1;
		hw_platform_led_ctrl(current_blink_led,current_led_state);
		
		tls_os_timer_create(&led_timer,  
							TIMER_LED_PERIOD , 
							pdTRUE, 		
							NULL, 
							led_blink_timer_hook); 
		
		tls_os_timer_start(led_timer);

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
int hw_platform_stop_led_blink(void)
{		
	if(led_running)	
	{
		tls_os_timer_stop(led_timer);
		tls_os_timer_delete(led_timer);	
		hw_platform_led_ctrl(current_blink_led,0);
		return TRUE;
	}
	else
	{
		return FALSE;		
	}
}


 
