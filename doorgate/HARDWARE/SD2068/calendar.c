/**
* @file		calendar.c
* @brief	终端实时时钟维护的模块
* @author   joe
* @version  v1.00
* @date     2009.10.15
* @note    
*/

#include "sd2068.h"	
#include "calendar.h"
#include "param.h"	
#include "BasicFunc.h"		


/**
* @brief  系统的RTC功能是否OK的标记
*/
unsigned int	RTC_OK_flag;

/**
* @brief      Initializes calendar application.
* @param[in]  none
* @param[out] none
* @return     none
* @note                    
*/
void Calendar_Init(void)
{
	TypedefDateTime curDateTime;
		
	RTC_OK_flag = 1;		
	
	SD2068A_Initial();

	SD2068A_GetTime((u8*)&curDateTime);
	
	if (curDateTime.year + 2000 > 2200)
	{	
		curDateTime.sec		= 0x00;	
		curDateTime.min		= 0x00;
		curDateTime.hour	= 0x00;
		curDateTime.week	= 0x00;	
		curDateTime.day		= 0x01;
		curDateTime.month	= 0x01;	
		curDateTime.year	= 2010 - 2000;
		
		// 设置时间
		SD2068A_SetTime((u8*)&curDateTime);		//设置初始时间为2010.01.01 00 00 00

		// 立即获取时间
		SD2068A_GetTime((u8*)&curDateTime);

		// 时间不对，即为RTC故障
		if(curDateTime.year + 2000 != 2010 )
		{	
			RTC_OK_flag = 0;		//置位RTC故障的标记
		}	
	}
}


/**
* @brief      简单的检查系统日期时间的格式
* @param[in]  none
* @param[out] none
* @return     none
* @note    没有检查闰年 月大月小的问题     
*/
int check_time_format(TypedefDateTime *DateTime)
{	
	if (DateTime->sec >= 60)
	{
		return -1;
	}

	if (DateTime->min >= 60)
	{
		return -1;
	}

	if (DateTime->hour >= 24)
	{
		return -1;
	}

	if (DateTime->day > 31)
	{
		return -1;
	}	

	if (DateTime->month > 12)
	{
		return -1;
	}

	return 0;

}

/**
* @brief      如果RTC获取的时间格式有问题，构造Dummy时间
* @param[in]  none
* @param[out] none
* @return     none
* @note    没有检查闰年 月大月小的问题     
*/
static int dummy_time_format(TypedefDateTime *DateTime)	
{	
	while(DateTime->sec >= 60)
	{
		DateTime->sec -= 60;
		DateTime->min++;
	}

	while(DateTime->min >= 60)
	{
		DateTime->min -= 60;
		DateTime->hour++;
	}

	while(DateTime->hour >= 24)
	{
		DateTime->hour -= 24;
		DateTime->day++;
	}

	while(DateTime->day > 31)
	{
		DateTime->day -= 31;
		DateTime->month++;
	}	

	while(DateTime->month > 12)
	{
		DateTime->month -= 12;
		DateTime->year++;
	}

	return 0;

}


/**
* @brief      设置系统日期时间
* @param[in]  u8 * timebuf  输入的新的时间缓冲区
* @param[out] none
* @return     none
* @note       
*       输入的年份需要在实际年份的基础上减去2000
*/
int SetDateTime(TypedefDateTime *DateTime)
{	
	if(SD2068A_SetTime((u8*)DateTime))	
	{	
		if (SD2068A_SetTime((u8*)DateTime))	
		{
			RTC_OK_flag = 0;		//置位RTC功能异常标记
			return FALSE;
		}
	}	
			
	return TRUE;
}

/**
* @brief     获取系统日期时间
* @param[in]  none
* @param[out] none
* @return     none
* @note                    
*/
int GetDateTime(TypedefDateTime *DateTime)	
{	
	u8 cnt = 0;
	
	do 
	{	
		if (SD2068A_GetTime((u8 *)DateTime))
		{	
			if (SD2068A_GetTime((u8 *)DateTime))
			{		
				RTC_OK_flag = 0;
			}
		}

		cnt++;	
	} while (check_time_format(DateTime) && (cnt < 4));


	if (cnt == 4)
	{	
		dummy_time_format(DateTime);
		return FALSE;	
	}
	
	return TRUE;	
}




void DateTime_Trans(u8 *resp,TypedefDateTime *DataTime)
{		
	u16 temp;

	temp = DataTime->year + 2000;	

	CharToAsc(temp>>8,resp);	
	CharToAsc(temp,resp++);	
	CharToAsc(DataTime->month,resp++);
	CharToAsc(DataTime->day,resp++);	
	CharToAsc(DataTime->week,resp++);
	CharToAsc(DataTime->hour,resp++);
	CharToAsc(DataTime->min,resp++);	
	CharToAsc(DataTime->sec,resp++);	
	
}

