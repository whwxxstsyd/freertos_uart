#include <string.h>	
#include "flash.h" 
#include "led.h"	
#include "spi.h"
#include "delay.h"   



//初始化SPI FLASH的IO口
void SPI_Flash_Init(void)	
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		
	GPIO_SetBits(GPIOC,GPIO_Pin_4);		
	
	SPI1_Init();		   //初始化SPI
				
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);	//设置为18M时钟,高速模式
	//SPI_FLASH_TYPE=SPI_Flash_ReadID();//读取FLASH ID.
}  	


const u8 *Flash_Test = "Flash Test";			
u8 Flash_Buf[15];		 


void SPI_Flash_Test(void)	
{			
	u16 len;
	u32 start;	

	len = strlen(Flash_Test)	;		
	start = 16*4*1024;	//FLASH 大小为4M字节		

	while(SPI_Flash_ReadID() != 0x26)	
	{				
		delay_xms(500);		
		LED0=!LED0;	
	}
		
	SPI_Flash_Write((u8*)Flash_Test,start,len);	

	delay_xms(500);	
	
	SPI_Flash_Read(Flash_Buf,start,len);		

	delay_xms(500);	
}

//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   	
	SPI_FLASH_CS=0;                            //使能器件   
	SPI1_ReadWriteByte(RDSR);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	SPI_FLASH_CS=1;                            //取消片选     
	return byte;   
} 	
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	u8 dummy_byte = 0x00;	
	
	SPI_Flash_Wait_Busy();	

	SPI_FLASH_CS=0; 	                        //使能器件   
	SPI1_ReadWriteByte(WRSR);   				//发送写取状态寄存器命令    
//	SPI1_ReadWriteByte(dummy_byte);	
	SPI1_ReadWriteByte(sr);               	   //写入一个字节  
	SPI_FLASH_CS=1;                            //取消片选     	      
}   
//SPI_FLASH写使能	
//将WEL置位   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(WREN);      			//发送写使能  
	SPI_FLASH_CS=1;                            //取消片选     	      
} 		
//SPI_FLASH写禁止	
//将WEL清零  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(WRDI);     //发送写禁止指令    
	SPI_FLASH_CS=1;                            //取消片选     	      
} 			    
//读取芯片ID W25X16的ID:0XEF14
u8 SPI_Flash_ReadID(void)
{	
	u8 partialInfo[3];
		  		
	SPI_FLASH_CS=0;				    
	SPI1_ReadWriteByte(JEDECID);//发送读取ID命令	     	 			   
	partialInfo[0] = SPI1_ReadWriteByte(0xFF);  	
	partialInfo[1] = SPI1_ReadWriteByte(0xFF);	 
	partialInfo[2] = SPI1_ReadWriteByte(0xFF);	 	
	SPI_FLASH_CS=1;				
	
	return partialInfo[1];			
}   		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{
 	u16 i;    		
	SPI_Flash_Wait_Busy();
		
	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(READ);         			//发送读取命令   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   	
    for(i=0;i<NumByteToRead;i++)
	{ 		
        pBuffer[i]=SPI1_ReadWriteByte(0xFF);   //循环读数  
    }
	SPI_FLASH_CS=1;                            //取消片选     	      
}  



//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   

void flash_hal_write_address(u32 address)
{    
    u8 addr_hi = (u8)((address)>>16);	
    u8 addr_mi = (u8)((address)>>8);
    u8 addr_lo = (u8)(address);		

    SPI1_ReadWriteByte( addr_hi );
    SPI1_ReadWriteByte( addr_mi );
    SPI1_ReadWriteByte( addr_lo );	
    
    return;
}
	

void flash_hal_write( u8 *buffer,u16 count )
{
    while( count-- )
    {	
        SPI1_ReadWriteByte( *( buffer++ ) );          
    }

    return;
}



void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{	
	u16 i; 
	
	SPI_Flash_Wait_Busy();

	SPI_FLASH_Write_Enable();

	SPI_FLASH_CS=0; 
	SPI1_ReadWriteByte(PP);	
	
	if( NumByteToWrite <= FLASH_PAGE_SIZE)	
	{
		SPI1_ReadWriteByte((u8)((WriteAddr)>>16));  //发送24bit地址    
		SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
		SPI1_ReadWriteByte((u8)WriteAddr); 	 

	    for(i=0;i<NumByteToWrite;i++)
		{ 		
			SPI1_ReadWriteByte(pBuffer[i]);   //循环读数  
	    }
	}
	else
	{
		u8 i;		
		u8 repeat = NumByteToWrite / FLASH_PAGE_SIZE;
		u8 repeat_over = NumByteToWrite % FLASH_PAGE_SIZE;
		
		for ( i = 0; i < repeat; i++ )
		{
			flash_hal_write_address( WriteAddr + ( i * FLASH_PAGE_SIZE ) );
			flash_hal_write( pBuffer + ( i * FLASH_PAGE_SIZE ),FLASH_PAGE_SIZE );
		}		
		if ( repeat_over )
		{
			flash_hal_write_address( WriteAddr + ( repeat * FLASH_PAGE_SIZE ) );
			flash_hal_write( pBuffer + ( repeat * FLASH_PAGE_SIZE ), repeat_over );
		}
	}
	SPI_FLASH_CS=1; 	
	
	return;
}




//擦除整个芯片
//整片擦除时间:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待时间超长...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   	
  	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(CE);        //发送片擦除命令  
	SPI_FLASH_CS=1;                            //取消片选     	      
	SPI_Flash_Wait_Busy();   				   //等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   		
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   	
  	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(SE);      				//发送扇区擦除指令 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   	
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	SPI_FLASH_CS=1;                            //取消片选     	      
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
}  
//等待空闲
void SPI_Flash_Wait_Busy(void)   
{   	
	while (SPI_Flash_ReadSR()&STATUS_BUSY);   // 等待BUSY位清空
}  



void SPI_Flash_Global_Block_Unlock( void )
{	
    SPI_FLASH_Write_Enable();
    SPI_FLASH_CS=0; 
    SPI1_ReadWriteByte( ULBPR );	
    SPI_FLASH_CS=1; 			

    return;
}






/*

	注意:
	
		Flash 的编程原理都是只能将 1 写为 0，
	而不能将 0 写为 1。所以在 Flash 编程之前，
	必须将对应的块擦除，
	而擦除的过程就是把所有位都写为 1 的过程，
	块内的所有字节变为 0xFF。
	
*/














