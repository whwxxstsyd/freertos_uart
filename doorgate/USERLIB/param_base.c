#include "param_base.h"	
#include "param.h"	

static DEVICE_INFO Device_Info;




void Device_Info_Default(void)		
{	
	u16 i;
		
	for(i=0;i<DEVICE_NUM;i++)
	{
		Device_Info.Device_Addr[i] = DEVICE_ADDR_DEFAULT+i;
	}
	
	Device_Info.SoftWare_Ver 		= 	SOFTWARE_VERSION;
	Device_Info.HardWare_Ver 		= 	HARDWARE_VERSION;	
	Device_Info.Company_Name 		= 	COMPANY_NAME;		
	Device_Info.Device_Name 		= 	DEVICE_NAME;	
	Device_Info.U_ID				= 	U_ID;
}


void Device_Info_Get(DEVICE_INFO *info)
{	
	info = &Device_Info;
}
	


u16 Device_Addr_Info_Get(u8 num)
{
	if(num > DEVICE_NUM)
	{
		return FALSE;
	}
	
	return Device_Info.Device_Addr[num];
}


void Device_Addr_Info_Set(u8 num,u16 addr)
{
	if(num > DEVICE_NUM)
	{
		return FALSE;		
	}

	Device_Info.Device_Addr[num] = addr;	
}
	

void

