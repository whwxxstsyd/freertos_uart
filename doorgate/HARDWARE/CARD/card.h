#ifndef __CARD_H
#define __CARD_H	 
#include "sys.h"

//使用到的为GPIOD
#define GPIOReadHead_RCC_CLK			(RCC_APB2Periph_GPIOD)
//读卡头电源控制
#define GPIOReadHead_PORT				GPIOD	
#define GPIOReadHead_PIN				GPIO_Pin_12   	
#define GPIOReadHead_Open        		GPIO_SetBits(GPIOReadHead_PORT,GPIOReadHead_PIN)					
#define GPIOReadHead_Elapse    		GPIO_ResetBits(GPIOReadHead_PORT,GPIOReadHead_PIN)
 

//////////////////////////////////////////////////////////////
#define GPIOWGReader_OUT_RCC_CLK		(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD) 
/****************读卡头1***************************************/
#define WGReader_H1D0_PORT			  GPIOD
#define WGReader_H1D0_PIN			 (GPIO_Pin_8)
#define WGReader_H1D0_PORTSOURCE    GPIO_PortSourceGPIOD
#define WGReader_H1D0_PINSOURCE     GPIO_PinSource8
#define WGReader_H1D0_EXTLINE       EXTI_Line8
#define WGReader_H1D0_Reader		  GPIO_ReadInputDataBit(WGReader_H1D0_PORT,WGReader_H1D0_PIN)

#define WGReader_H1D1_PORT			  GPIOD
#define WGReader_H1D1_PIN			 (GPIO_Pin_9)
#define WGReader_H1D1_PORTSOURCE    GPIO_PortSourceGPIOD
#define WGReader_H1D1_PINSOURCE     GPIO_PinSource9
#define WGReader_H1D1_EXTLINE       EXTI_Line9
#define WGReader_H1D1_Reader		  GPIO_ReadInputDataBit(WGReader_H1D1_PORT,WGReader_H1D1_PIN)

/****************读卡头2***************************************/
#define WGReader_H2D0_PORT			  GPIOC
#define WGReader_H2D0_PIN			 (GPIO_Pin_12)
#define WGReader_H2D0_PORTSOURCE    GPIO_PortSourceGPIOC
#define WGReader_H2D0_PINSOURCE     GPIO_PinSource12
#define WGReader_H2D0_EXTLINE       EXTI_Line12	
#define WGReader_H2D0_Reader		  GPIO_ReadInputDataBit(WGReader_H2D0_PORT,WGReader_H2D0_PIN)

#define WGReader_H2D1_PORT			  GPIOD
#define WGReader_H2D1_PIN			 (GPIO_Pin_2)
#define WGReader_H2D1_PORTSOURCE    GPIO_PortSourceGPIOD
#define WGReader_H2D1_PINSOURCE     GPIO_PinSource2
#define WGReader_H2D1_EXTLINE       EXTI_Line2
#define WGReader_H2D1_Reader		  GPIO_ReadInputDataBit(WGReader_H2D1_PORT,WGReader_H2D1_PIN)

/****************读卡头3***************************************/
#define WGReader_H3D0_PORT			  GPIOD
#define WGReader_H3D0_PIN			 (GPIO_Pin_5)
#define WGReader_H3D0_PORTSOURCE    GPIO_PortSourceGPIOD
#define WGReader_H3D0_PINSOURCE     GPIO_PinSource5
#define WGReader_H3D0_EXTLINE       EXTI_Line5
#define WGReader_H3D0_Reader		  GPIO_ReadInputDataBit(WGReader_H3D0_PORT,WGReader_H3D0_PIN)

#define WGReader_H3D1_PORT			  GPIOD	
#define WGReader_H3D1_PIN			 (GPIO_Pin_6)
#define WGReader_H3D1_PORTSOURCE    GPIO_PortSourceGPIOD
#define WGReader_H3D1_PINSOURCE     GPIO_PinSource6
#define WGReader_H3D1_EXTLINE       EXTI_Line6
#define WGReader_H3D1_Reader		  GPIO_ReadInputDataBit(WGReader_H3D1_PORT,WGReader_H3D1_PIN)



void Card_Init(void);

			    
#endif
