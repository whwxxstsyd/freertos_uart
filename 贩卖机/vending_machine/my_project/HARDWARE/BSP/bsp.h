#ifndef __BSP_H
#define __BSP_H



/* 使能在源文件中使用uCOS-III的函数, 这里的源文件主要是指BSP驱动文件 */
#define  uCOS_EN      1

#if uCOS_EN == 1    
	#include "os.h"   

	#define  ENABLE_INT()      CPU_CRITICAL_EXIT()     /* 使能全局中断 */
	#define  DISABLE_INT()     CPU_CRITICAL_ENTER()    /* 禁止全局中断 */
#else
	/* 开关全局中断的宏 */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
#endif

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif



#include "bsp_uart_fifo.h"
#include "bsp_led.h"	
#include "bsp_beep.h"	
#include "bsp_key.h"



/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);


#endif

