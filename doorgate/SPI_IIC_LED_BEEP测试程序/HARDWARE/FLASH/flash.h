#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//W25X16 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/13 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

#define	SPI_FLASH_CS PAout(4)  //选中FLASH						 
////////////////////////////////////////////////////////////////////////////

	
#define STATUS_REG
#define STATUS_WEL  0x02 /**< Write-enable latch status 1 = device is write-enabled
                            0 = device is not write-enabled */
#define STATUS_WSE  0x04 /**< Write suspend-erase status 1 = erase suspended 0 =
                            erase is not suspended */
#define STATUS_WSP  0x08 /**< Write suspend-program status 1 = program suspended
                            0 = program is not suspended */
#define STATUS_WPLD 0x10 /**< Write protections lock-down status 1 = write
                            protection lock-down enabled 0 = write protection
                            lock-down disabled */
#define STATUS_SEC  0x20 /**< Security ID status 1 = security ID space locked
                            0 = security ID space not locked */
#define STATUS_RES  0x40 /**< Reserved for future use */
#define STATUS_BUSY 0x80 /**< Write operation status 1 = write in progress 0 =
                            no operation in progress */

	

#define CONFIGURATION_REG
#define CONFIGURATION_RES   0x01 /**< Reserved */
#define CONFIGURATION_IOC   0x02 /**< I/O configuration for SPI mode 1 = WP and
                                    HOLD pins disabled 0 = WP and HOLD enabled*/
#define CONFIGURATION_BPNV  0x08 /**< Block-protection volatility state 1 = no
                                   memory block permanently locked 0 = any block
                                    has been permanently locked */

#define CONFIGURATION_WPEN  0x80 /**< Write-protection pin (WP) enable 1 = enabled
                                    0 = disabled */


//指令表
#define INSTRUCTIONS
#define NOP         0x00 /**< No Operation */
#define RSTEN       0x66 /**< Reset Enable */
#define RST         0x99 /**< Reset Memory */
#define EQIO        0x38 /**< Enable Quad I/O */
#define RSTQIO      0xFF /**< Reset Quad I/O */
#define RDSR        0x05 /**< Read Status Register */
#define WRSR        0x01 /**< Write Status Register */
#define RDCR        0x35 /**< Read Configuration Register */
#define READ        0x03 /**< Read Memory */
#define HS_READ     0x0B /**< Read Memory at Higher Speed */
#define SQOR        0x6B /**< SPI Quad Output Read */
#define SQIOR       0xEB /**< SPI Quad I/O Read */
#define SDOR        0x3B /**< SPI Dual Output Read */
#define SDIOR       0xBB /**< SPI Dual I/O Read */
#define SB          0xC0 /**< Set Burst Length */
#define RBSQI       0x0C /**< SQI Read Burst with Wrap */
#define RBSPI       0xEC /**< SPI Read Burst with Wrap */
#define JEDECID     0x9F /**< JEDEC-ID Read */
#define QUAD_JID    0xAF /**< Quad I/O J-ID read */
#define SFDP        0x5A /**< Serial Flash Discoverable Parameters */
#define WREN        0x06 /**< Write Enable */
#define WRDI        0x04 /**< Write Disable */
#define SE          0x20 /**< Erase 4 KBytes of Memory Array */
#define BE          0xD8 /**< Erase 64, 32 or 8 KBytes of Memory Array */
#define CE          0xC7 /**< Erase Full Array */
#define PP          0x02 /**< Page Program */
#define SPI_QUAD    0x32 /**< SQI Quad Page Program */
#define WRSU        0xB0 /**< Suspends Program / Erase */
#define WRRE        0x30 /**< Resumes Program / Erase */
#define RBPR        0x72 /**< Read Block-Protection Register */
#define WBPR        0x42 /**< Write Block-Protection Register */
#define LBPR        0x8D /**< Lock Down Block-Protection Register */
#define NVWLDR      0xE8 /**< Non-Volatile Write Lock-Down Register */
#define ULBPR       0x98 /**< Global Block Protection Unlock */
#define RSID        0x88 /**< Read Security ID */
#define PSID        0xA5 /**< Program User Security ID Area */
#define LSID        0x85 /**< Lockout Security ID Programming */

#define START_PAGE_ADDRESS      0x010000
#define END_PAGE_ADDRESS        0x7FFFFF
#define FLASH_PAGE_SIZE         256
	
#define SST26_CE_CLR	GPIO_ResetBits(GPIOA,GPIO_Pin_4)			
#define SST26_CE_SET	GPIO_SetBits(GPIOA,GPIO_Pin_4)	
		

#define FLASH_2_CS PAout(4)	
#define	FLASH_2_WP PCout(4)
	

		
void SPI_Flash_Init(void);	
u8  SPI_Flash_ReadID(void);  	    //读取FLASH ID
u8	SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(u8 sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_Global_Block_Unlock( void );

#endif
















