#include <string.h>	
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "spi.h"
#include "flash.h"
#include "24CXXX.h"	

#include "calendar.h"
#include "beep.h"	

#include "sst26vf032b.h"

#include "demo_spi_flash_fatfs.h"

#include "bsp.h"		


//ALIENTEK Mini STM32开发板范例代码20
//SPI实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
   	
//要写入到W25Q64的字符串数组
const u8 *TEXT_Buffer = "Hetianqi";						
const u8 *IIC_Buffer  = "IIC TEST";			

	
u8 Flash_ID[3];	

u8 datatemp[15];	 
u8 IIC_datatemp[15];	  

TypedefDateTime DateTime;	
	

static void  Wait(void)
{
	volatile u8 i;

	for(i = 0;i<8;i++);
}

 u8 ID;  

 int main(void)
 { 
	u8 key;	
	u16 i=0;		
	u32 start;	
	UART_REC *rec;	
	u16 text_size,IIC_text_size;		
	
	uint8_t read;
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
					
	bsp_Init();	

	
					
	//delay_init();	    	 //延时函数初始化	  	
	
	//uart_init(9600);	 	//串口初始化为9600		
			
	//LED_Init();		  		//初始化与LED连接的硬件接口			
	
	SST26_Init();	//SPI FLASH 初始化		

	SPI_Flash_Global_Block_Unlock();		

	ID = SPI_Flash_ReadID();			

	SPI_Flash_Erase_Sector(0);		

	SST26_Test();			
			
#if 0	
	
	_24CXXX_init();												
	Calendar_Init();	
	Beep_Init();			
			
	memset(datatemp,0,sizeof(datatemp));		
	memset(IIC_datatemp,0,sizeof(IIC_datatemp));	
						
	text_size 	  = strlen(TEXT_Buffer);					
	IIC_text_size = strlen(IIC_Buffer);
			
	start = 0;	//FLASH 大小为4M字节		
	
				

				
		
					
						
				
		
	GetDateTime(&DateTime);			

#endif		
	
	DemoFatFS();						


	bsp_StartAutoTimer(1, 500); 	

	while(1)
	{	
		bsp_Idle(); 	/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */
				
		if(bsp_CheckTimer(1))
		{	
			bsp_LedToggle(1);
			bsp_LedToggle(2);	
		}			
	}	

		
}





