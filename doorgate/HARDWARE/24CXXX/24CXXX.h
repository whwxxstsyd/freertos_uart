

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
 

#ifndef __24CXXX_H__
#define __24CXXX_H__
#include "sys.h"
//#include <stm32f10x_lib.h>
 
#include "IIC.h"
#include "delay.h"
#ifdef __OPTIMIZE__

#endif


//接口定义及操作    WP端口为PB5
//#define _24CXXX_WP_DDR		DDRB
//#define _24CXXX_WP_PORT		PORTB
#define _24CXXX_WP			5		//定义WP所处IO位置，如果>7 表示不使用WP

#if _24CXXX_WP < 8	
//#define _24CXXX_WP_OUT()	_24CXXX_WP_DDR |= (1<<_24CXXX_WP)
//#define _24CXXX_WP_HIGH()	_24CXXX_WP_PORT	|= (1<<_24CXXX_WP)
//#define _24CXXX_WP_LOW()	_24CXXX_WP_PORT	&= ~(1<<_24CXXX_WP)

#define _24CXXX_WP_OUT()	 {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=0X00300000;}//PB5为推挽输出
#define _24CXXX_WP_HIGH()	 GPIO_SetBits(GPIOB,GPIO_Pin_5)//PB5输出为高
#define _24CXXX_WP_LOW()	 GPIO_ResetBits(GPIOB,GPIO_Pin_5) //PB5输出为低
	
#else			

#define _24CXXX_WP_OUT()	asm("nop"); //一个时钟周期
#define _24CXXX_WP_HIGH()	asm("nop"); //一个时钟周期
#define _24CXXX_WP_LOW()	asm("nop"); //一个时钟周期

#endif	//#if _24CXXX_WP < 8
	
#define _24CXXX_WR_EN()	_24CXXX_WP_LOW()
#define _24CXXX_WR_DIS()	_24CXXX_WP_HIGH()


//期间类型列表
#define _24C01	1
#define _24C02	2
#define _24C04	3
#define _24C08	4
#define _24C16	5
#define _24C32	6
#define _24C64	7
#define _24C128	8
#define _24C256	9
#define _24C512	10
#define _24C1024	11


//*************************************************************************
//需要自定义修改区域
//设定芯片型号------
//#define DEV_24CXX_MODLE	_24C512		//定义器件类型
//#define DEV_24CXX_NUM	2			//定义器件数量 

#define DEV_24CXX_MODLE	_24C1024		//定义器件类型
#define DEV_24CXX_NUM	1			//定义器件数量

//*************************************************************************


//芯片基本信息
#if DEV_24CXX_MODLE==_24C01
	#define PAGE_SIZE		8
	#define PAGE_SIZE_REM	0x07
	#define MEM_SIZE		0x007f
#elif DEV_24CXX_MODLE==_24C02
	#define PAGE_SIZE		8
	#define PAGE_SIZE_REM	0x07
	#define MEM_SIZE		0x00ff
#elif DEV_24CXX_MODLE==_24C04
	#define PAGE_SIZE		16
	#define PAGE_SIZE_REM	0x0F
	#define MEM_SIZE		0x01ff
#elif DEV_24CXX_MODLE==_24C08
	#define PAGE_SIZE		16
	#define PAGE_SIZE_REM	0x0F
	#define MEM_SIZE		0x03ff
#elif DEV_24CXX_MODLE==_24C16
	#define PAGE_SIZE		16
	#define PAGE_SIZE_REM	0x0F
	#define MEM_SIZE		0x07ff
#elif DEV_24CXX_MODLE==_24C32
	#define PAGE_SIZE		32
	#define PAGE_SIZE_REM	0x1F
	#define MEM_SIZE		0x0fff
#elif DEV_24CXX_MODLE==_24C64
	#define PAGE_SIZE		32
	#define PAGE_SIZE_REM	0x1F
	#define MEM_SIZE		0x1fff
#elif DEV_24CXX_MODLE==_24C128
	#define PAGE_SIZE		64
	#define PAGE_SIZE_REM	0x3F
	#define MEM_SIZE		0x3fff
#elif DEV_24CXX_MODLE==_24C256
	#define PAGE_SIZE		64
	#define PAGE_SIZE_REM	0x3F
	#define MEM_SIZE		0x7fffUL
#elif DEV_24CXX_MODLE==_24C512
	#define PAGE_SIZE		128
	#define PAGE_SIZE_REM	0x7F
	#define MEM_SIZE		0xffffUL
#elif DEV_24CXX_MODLE==_24C1024
	#define PAGE_SIZE		256
	#define PAGE_SIZE_REM	0xFF
	#define MEM_SIZE		0x1ffffUL   //131071个字节
#endif


//最大寻址范围
#define MAX_ADDR	( (MEM_SIZE+1)*DEV_24CXX_NUM-1 )


//芯片常量
#define DEV_24CXX_MAIN_ADDR	0xA0


#define DEV_24CXX_READ		0x01
#define DEV_24CXX_WRITE	0x00


//变量声明
extern unsigned char HardAddr;

//函数声明
#define SetDevAddr(a)	HardAddr = (a)
#define GetDevAddr()	HardAddr
 

#if MAX_ADDR > 0xFFFF	//如果地址大于65535
extern void _24CXXX_WriteByte(unsigned long Addr,unsigned char Data);
extern unsigned char _24CXXX_ReadByte(unsigned long Addr);
extern void _24CXXX_WriteBytes(unsigned long StartAddr,unsigned char *Data,int DataNum);
extern void _24CXXX_ReadBytes(unsigned long StartAddr,unsigned char *Data,int DataNum);
extern void _24CXXX_AddrProcess(unsigned long Addr);
extern void _24CXXX_init(void);
#else
extern void _24CXXX_WriteByte(unsigned int Addr,unsigned char Data);
extern unsigned char _24CXXX_ReadByte(unsigned int Addr);
extern void _24CXXX_WriteBytes(unsigned int StartAddr,unsigned char *Data,int DataNum);
extern void _24CXXX_ReadBytes(unsigned int StartAddr,unsigned char *Data,int DataNum);
extern void _24CXXX_AddrProcess(unsigned long Addr) ;
extern void _24CXXX_init(void);
#endif

extern void _24CXXX_Test(void);



#endif
