 
/*********************************************************************************************************
** Function name:          EEPROM	   
** Descriptions:          适合于AT24C02~AT24C1024                                                 
** input parameters:        无
** output parameters:       无      
** Returned value:          0	 
** Created by:		    刘玉刚
    
** Modified date:     2013.8.15     
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
	

#ifndef __IIC_H__
#define __IIC_H__
#include "sys.h"

//功能控制
#define __IO_IIC 1
#if __IO_IIC == 1

//端口定义及操作
 		
#define IIC_SCK_HIGH()    GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define IIC_SCK_LOW()     GPIO_ResetBits(GPIOB,GPIO_Pin_6)


/*端口设置  SDA:          PB7
           		     	 SCK(SCL):  PB6      */

#define IIC_SDA_OUT()    {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}
#define IIC_SDA_IN()     {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}

#define IIC_SDA_HIGH()   GPIO_SetBits(GPIOB,GPIO_Pin_7)	
#define IIC_SDA_LOW()    GPIO_ResetBits(GPIOB,GPIO_Pin_7)		
#define IIC_SDA_GET()    PBin(7)				


//常量定义
#define IIC_SEND_ACK		0x10	
#define IIC_SEND_NOACK		0x11

#define IIC_STATE_NOACK	0x20
#define IIC_STATE_ACK		0x21

	
//变量声明
extern unsigned char IIC_State;


//函数声明
#define IIC_StateGet()	IIC_State



extern void IIC_Init(void);                                  /*开发板IIC初始化*/
extern void IIC_Start(void);                                /*打开IIC*/
extern void IIC_Stop(void);                              /*关闭IIC*/
extern void IIC_WriteByte(unsigned char Data);          /* IIC写数据*/
extern unsigned char IIC_ReadByte(unsigned char ACK);  /* IIC读数据*/

#else


#endif	//#if __IO_IIC == 1





#endif	//#ifndef __IIC_H__
