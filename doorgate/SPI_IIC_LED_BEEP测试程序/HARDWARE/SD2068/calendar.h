#ifndef _CALENDAR_H_
#define _CALENDAR_H_
#include "sys.h"	



#define TRUE				1
#define FALSE				0	

/**
* @brief 定义时间的数据结构                     
*/	
typedef struct TypedefDateTime_T
{
	u8	sec;
	u8	min;
	u8	hour;	
	u8  week;	
	u8  day;
	u8  month;	
	u8  year;
} TypedefDateTime;


void Calendar_Init(void);	

int check_time_format(TypedefDateTime *DateTime);	

int SetDateTime(TypedefDateTime *DateTime);	

int GetDateTime(TypedefDateTime *DateTime);			

void Systime2BCD(u8 * time_bcd);


#endif


