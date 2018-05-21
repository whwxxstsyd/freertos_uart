#ifndef _TIMEBASE_H_
#define _TIMEBASE_H_


//定时器定时模式
#define V_TIMER_MODE_ONE_SHOT		1
#define V_TIMER_MODE_PERIODIC		2

typedef unsigned int VTIMER_HANDLE;		

typedef void (*TimerIsrHook)(void);

void delay_us(unsigned int time);
void delay_ms(unsigned int time);
void TimeBase_Init(void);
void TIM2_UpdateISRHandler(void);

//void start_timer(TimerIsrHook hook_func);
//void stop_timer(void);

VTIMER_HANDLE start_timer(unsigned char mode,unsigned int dly, TimerIsrHook hook_func);
int stop_timer(VTIMER_HANDLE v_timer_h);
int reset_timer(VTIMER_HANDLE v_timer_h,unsigned char mode,unsigned int dly, TimerIsrHook hook_func);

void StartDelay(unsigned short nTime);
unsigned char DelayIsEnd(void);

void start_real_timer(void);
void stop_real_timer(void);

#endif