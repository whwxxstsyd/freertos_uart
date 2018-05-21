/**
 * @file  Timebase.c
 * @brief use TIM1 to generate a time base 
 * @version 1.0
 * @author joe
 * @date 2009年09月10日
 * @note
*/
#include "stm32f10x_lib.h" 

#include "TimeBase.h"
#include "JMemory.h"
#include <assert.h>


static TimerIsrHook	timer_isr_hook;

/**
 * @brief     us 软延时
 * @param[in] unsigned int time 延时参数
*/
void delay_us(unsigned int time)
{    
	unsigned int i=0;  
	while(time--)
	{
		i=8;  
		while(i--) ;    
	}
}

/**
 * @brief     ms软延时
 * @param[in] unsigned int time 延时参数
*/
void delay_ms(unsigned int time)
{    
	unsigned int i=0;  
	while(time--)
	{
		i=10255; 
		while(i--) ;    
	}
}
/**
 * @brief     初始化产生延时时基的计数器TIM2,设定计数器产生1ms的时基
 * @param[in] none
 * @param[out] none
 * @return none
 * @note   此初始化函数中调用了BSP_IntVectSet(BSP_INT_ID_TIM2, TIM2_UpdateISRHandler)这个函数，这个是设定TIM2的中断处理函数的
 *				 在移植的时候需要根据不同工程中设置中断处理函数的方法来稍作修改。       
*/
void TimeBase_Init(void)
{
	TIM_TimeBaseInitTypeDef						TIM_TimeBaseStructure;
	TIM_OCInitTypeDef							TIM_OCInitStructure;
	NVIC_InitTypeDef							NVIC_InitStructure;

	//初始化结构体变量
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

	/*开启相应时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  


	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler			= 1;      //72M的计数频率
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseStructure.TIM_Period			= (72000/2-1);      
	TIM_TimeBaseStructure.TIM_ClockDivision		= 0x0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 and 4 Configuration in Timing mode */
	TIM_OCInitStructure.TIM_OCMode				= TIM_OCMode_Timing;
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse				= 0x0;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* Enable the TIM2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel			= TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority	= 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd		= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//timer_isr_hook = (TimerIsrHook)0;
}

#if 0

/**
 * @brief 开启定时器
 * @param[in] TimerIsrHook hook_func		定时中断回调函数
*/
void start_timer(TimerIsrHook hook_func)
{
	if (timer_isr_hook)
	{
		if (timer_isr_hook == hook_func)
		{
			return;
		}
		else
		{
			while(1);		//error
		}
	}
	timer_isr_hook = hook_func;
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/* TIM counter enable */
	TIM_Cmd(TIM2, ENABLE);
}


/**
 * @brief 关闭定时器
*/
void stop_timer(void)
{
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	/* TIM counter enable */
	TIM_Cmd(TIM2, DISABLE);
	timer_isr_hook = (TimerIsrHook)0;
}


/**
 * @brief TIM2的溢出中断ISR
 * @param[in] none
 * @return none
 * @note  TIM2的中断服务函数调用
*/
void TIM2_UpdateISRHandler(void)
{    
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if (timer_isr_hook)
		{
			timer_isr_hook();
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
#endif



typedef struct v_timer_t
{
	unsigned char	mode;
	unsigned int	count;
	unsigned int	dly;
	TimerIsrHook	timer_hook;
	struct v_timer_t	*prev;
	struct v_timer_t	*next;
}V_TIMER;

struct 
{
	V_TIMER		*v_timer_tail;
	unsigned int	v_timer_cnt;
}v_timer_list;

/**
 * @brief 开启定时器
 * @param[in] unsigned char mode		V_TIMER_MODE_ONE_SHOT		or      V_TIMER_MODE_PERIODIC
 * @param[in] unsigned int dly			在V_TIMER_MODE_ONE_SHOT模式下，表示延时时间
 *										在V_TIMER_MODE_PERIODIC模式下，表示定时周期
 * @param[in] TimerIsrHook hook_func	表示超时一次需要执行的回调函数
 * @return 0：  堆空间不够
 *         else  返回虚拟定时器的句柄
 * @note 应用在调用此接口时不知道已经有多少个虚拟定时器挂在硬件定时器下，所以返回值相当于定时器句柄的作用
 *       由于此虚拟定时器是基于硬件定时器TIME2实现的，TIMER2初始化为1ms中断，所以定时的最小单位为1ms
*/
VTIMER_HANDLE start_timer(unsigned char mode,unsigned int dly, TimerIsrHook hook_func)
{
	V_TIMER	*p_vtimer;
	p_vtimer = (V_TIMER*)Jmalloc(sizeof(V_TIMER));
	if (p_vtimer == 0)
	{
		return 0;
	}
	p_vtimer->mode = mode;
	p_vtimer->dly = dly;
	p_vtimer->timer_hook = hook_func;
	p_vtimer->prev = 0;
	p_vtimer->next = 0;
	p_vtimer->count = 0;

	if (v_timer_list.v_timer_cnt == 0)
	{
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		/* TIM counter enable */
		TIM_Cmd(TIM2, ENABLE);
	}
	else
	{
		p_vtimer->prev = v_timer_list.v_timer_tail;
		v_timer_list.v_timer_tail->next = p_vtimer;
	}
	v_timer_list.v_timer_tail = p_vtimer;
	v_timer_list.v_timer_cnt ++;

	return (VTIMER_HANDLE)p_vtimer;
}

/**
 * @brief 判断虚拟定时器的句柄是否合法，检查给定的虚拟定时器句柄（实际是每个定时器控制块的指针）是否在定时器链表中
 * @param[in]  VTIMER_HANDLE v_timer_h
*/
static int check_vtimer_handle(VTIMER_HANDLE v_timer_h)
{
	V_TIMER	*p_vtimer;

	p_vtimer = v_timer_list.v_timer_tail;
	while (p_vtimer)
	{
		if (p_vtimer == (V_TIMER*)v_timer_h)
		{
			return 1;
		}
		p_vtimer = p_vtimer->prev;
	}

	return 0;
}

/**
 * @brief 重置某定时器
 * @param[in]  VTIMER_HANDLE v_timer_h
*/
int reset_timer(VTIMER_HANDLE v_timer_h,unsigned char mode,unsigned int dly, TimerIsrHook hook_func)
{
	if (check_vtimer_handle(v_timer_h) == 0)
	{
		return -1;
	}

	V_TIMER	*p_vtimer = (V_TIMER*)v_timer_h;

	p_vtimer->count = 0;
	p_vtimer->mode = mode;
	p_vtimer->dly = dly;
	p_vtimer->timer_hook = hook_func;

	return 0;
}

/**
 * @brief 关闭某定时器
 * @param[in]  VTIMER_HANDLE v_timer_h
*/
int stop_timer(VTIMER_HANDLE v_timer_h)
{
	if (check_vtimer_handle(v_timer_h) == 0)
	{
		return 0;
	}

	V_TIMER	*p_vtimer = (V_TIMER*)v_timer_h;

	if (p_vtimer->next)
	{
		p_vtimer->next->prev = p_vtimer->prev;
	}
	else
	{
		v_timer_list.v_timer_tail = p_vtimer->prev;
	}
	
	if(p_vtimer->prev)
	{
		p_vtimer->prev->next = p_vtimer->next;
	}

	v_timer_list.v_timer_cnt--;
	Jfree(p_vtimer);
	
	if (v_timer_list.v_timer_cnt == 0)
	{
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		/* TIM counter enable */
		TIM_Cmd(TIM2, DISABLE);
	}

	return 0;
}


/**
 * @brief 强制开启硬定时器
*/
void start_real_timer(void)
{
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/* TIM counter enable */
	TIM_Cmd(TIM2, ENABLE);
	return;
}

/**
 * @brief 强制关闭硬定时器
*/
void stop_real_timer(void)
{
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	/* TIM counter enable */
	TIM_Cmd(TIM2, DISABLE);
	return;
}

/**
 * @brief TIM2的溢出中断ISR
 * @param[in] none
 * @return none
 * @note  TIM2的中断服务函数调用
*/
void TIM2_UpdateISRHandler(void)
{    
	V_TIMER *p_vtimer;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		p_vtimer = v_timer_list.v_timer_tail;
		while (p_vtimer)
		{
			p_vtimer->count++;
			if (p_vtimer->count == p_vtimer->dly)
			{
				if (p_vtimer->mode == V_TIMER_MODE_ONE_SHOT)
				{
					if (p_vtimer->next)
					{
						p_vtimer->next->prev = p_vtimer->prev;
					}
					else
					{
						v_timer_list.v_timer_tail = p_vtimer->prev;
					}

					if(p_vtimer->prev)
					{
						p_vtimer->prev->next = p_vtimer->next;
					}

					v_timer_list.v_timer_cnt--;
					Jfree(p_vtimer);

					if (v_timer_list.v_timer_cnt == 0)
					{
						TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
						/* TIM counter enable */
						TIM_Cmd(TIM2, DISABLE);
					}
				}
				else
				{
					p_vtimer->count = 0;
				}

				if (p_vtimer->timer_hook)
				{
					p_vtimer->timer_hook();
				}
			}
			p_vtimer = p_vtimer->prev;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


//===============================================利用timer提供的接口实现延时功能================================
static int Delay_end_flag;
//利用timer实现延时功能时的中断钩子函数
void delay_func_hook(void)
{
	Delay_end_flag = 1;
}

/**
 * @brief 开始利用TIM2定时计数
 * @param[in] unsigned int nTime   单位ms
 * @return  none
*/
void StartDelay(unsigned short nTime)
{
	int	ret;
    Delay_end_flag = 0;
	ret = start_timer(V_TIMER_MODE_ONE_SHOT,nTime,delay_func_hook);
	assert(ret != 0);
}


/**
 * @brief 判断延时时间是否到
 * @param[in] none
 * @return 1: 定时到
 *        0: 定时未到
*/
unsigned char DelayIsEnd(void)
{
	return	Delay_end_flag; 
}
