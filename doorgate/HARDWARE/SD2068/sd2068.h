#ifndef _DS2068_H_
#define _DS2068_H_
#include "sys.h"	
#include "stm32f10x_gpio.h"	

#define  READ_COMMAND			0x65
#define  WRITE_COMMAND			0x64
#define  ReviseTime				0x12

#define Second_Address			0x00
#define MINUTE_Address			0x01
#define HOUR_Address			0x02
#define WEEK_Address			0x03
#define DAY_Address				0x04
#define MONTH_Address			0x05
#define YEAR_Address			0x06

#define GPIO_SD2068_CLK   			RCC_APB2Periph_GPIOE

#define GPIO_SD2068_SDA_PORT   	GPIOE	
#define GPIO_SD2068_SDA_PIN       	GPIO_Pin_2

#define GPIO_SD2068_SCL_PORT   	GPIOE
#define GPIO_SD2068_SCL_PIN       	GPIO_Pin_3


#define  SD2068A_SCL_HIGH()     GPIO_SetBits(GPIO_SD2068_SCL_PORT, GPIO_SD2068_SCL_PIN)
#define  SD2068A_SCL_LOW()      GPIO_ResetBits(GPIO_SD2068_SCL_PORT, GPIO_SD2068_SCL_PIN) 
#define  SD2068A_SDA_HIGH()     GPIO_SetBits(GPIO_SD2068_SDA_PORT, GPIO_SD2068_SDA_PIN) 
#define  SD2068A_SDA_LOW()      GPIO_ResetBits(GPIO_SD2068_SDA_PORT, GPIO_SD2068_SDA_PIN) 
#define  SD2068A_SDA_GET()      GPIO_ReadInputDataBit(GPIO_SD2068_SDA_PORT, GPIO_SD2068_SDA_PIN)

#define BCD2DEC(X)				(((X&0x70)>>4)*10 + (X&0x0F))	//用于将BCD码转成十进制的宏
#define DEC2BCD(X)				((X/10)<<4 | (X%10))			//用于将十进制转成BCD码的宏



void  SD2068A_Initial(void);	
int  SD2068A_GetTime(u8 *TimeBuff);
int  SD2068A_SetTime(u8 *TimeBuff);

#endif

