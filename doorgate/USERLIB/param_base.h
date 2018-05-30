#ifndef __PARAM_BASE_H
#define __PARAM_BASE_H
#include "sys.h"	
#include "common.h"	


#define SOFTWARE_VERSION 		"V01.13"	
#define HARDWARE_VERSION 		"V01.00"
#define COMPANY_NAME 			"创力电子股份有限公司"
#define DEVICE_NAME 			"双门控制器"	

#define DEVICE_NUM				4
#define DEVICE_ADDR_DEFAULT 	1


//功能  : 系统配置参数   设计空间为20字节
typedef struct	DEVICE_INFO_t
{	
	u16 Device_Addr[DEVICE_NUM];						
	u8	*SoftWare_Ver;			
	u8  *HardWare_Ver;       		
	u8	*Company_Name;						
	u8	*Device_Name;	
	u32	User_ID;			
}DEVICE_INFO;			



void Device_Info_Default(void);		

DEVICE_INFO *Device_Info_Get(void);


u16 Device_Addr_Info_Get(u8 num);

void Device_Addr_Info_Set(u8 num,u16 addr);




#endif

