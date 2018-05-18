#ifndef  __BEEP_H__
#define  __BEEP_H__
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define GPIO_BEEP_PORT			GPIOA
#define GPIO_BEEP_RCC_CLK		RCC_APB2Periph_GPIOA 
#define GPIO_BEEP_PIN			GPIO_Pin_12 	
#define	BEEP 					PAout(12)	

#define BEEP_OFF       			GPIO_SetBits(GPIO_BEEP_PORT,GPIO_BEEP_PIN)					
#define BEEP_ON					GPIO_ResetBits(GPIO_BEEP_PORT,GPIO_BEEP_PIN) 


void Beep_Test(void);
void Beep_Init(void);
	

#endif 

