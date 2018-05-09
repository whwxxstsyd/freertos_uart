#ifndef _CALENDAR_H_
#define _CALENDAR_H_
#include "sys.h"	


/**
* @brief 定义时间的数据结构                     
*/	
typedef struct 
{
	u8	sec;
	u8	min;
	u8	hour;
	u8   week;
	u8   day;
	u8   month;
	u8   year;
} TypedefDateTime;


void Calendar_Init(void);	

int SetDateTime(TypedefDateTime *DateTime);	

int GetDateTime(TypedefDateTime *DateTime);			

void Systime2BCD(u8 * time_bcd);


#endif


