/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{

	switch (pdrv) {

		case FS_SPI_FLASH :
			bsp_InitSFlash();
			return RES_OK;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

	switch (pdrv)
	{
		case FS_SPI_FLASH :
			stat = 0;
			return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{

	switch (pdrv) {

		case FS_SPI_FLASH :
			sf_ReadBuffer(buff, sector << 12, count<<12);
			return RES_OK;

	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{

	switch (pdrv) {
		
		case FS_SPI_FLASH :
			{
				uint8_t i;
				
				for(i = 0; i < count; i++)
				{
					sf_WriteBuffer((uint8_t *)buff, sector << 12, 4096);	
				}
				
				return RES_OK;
			}
	
	}
	
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	
	switch (pdrv) {
		
	case FS_SPI_FLASH :
		switch(cmd)
		{
			/* SPI Flash不需要同步 */
			case CTRL_SYNC :  
				return RES_OK;
			
			/* 返回SPI Flash扇区大小 */
			case GET_SECTOR_SIZE:
				*((WORD *)buff) = 4096;  
				return RES_OK;
			
			/* 返回SPI Flash扇区数 */
			case GET_SECTOR_COUNT:
				*((DWORD *)buff) = 2048;    
				return RES_OK;
			
			/* 下面这两项暂时未用 */
			case GET_BLOCK_SIZE:   
				return RES_OK;
			
			case CTRL_ERASE_SECTOR:
				return RES_OK;       
		}
	}
	return RES_PARERR;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2013-01-01 00:00:00 */

	return	  ((DWORD)(2013 - 1980) << 25)	/* Year = 2013 */
			| ((DWORD)1 << 21)				/* Month = 1 */
			| ((DWORD)1 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
}

