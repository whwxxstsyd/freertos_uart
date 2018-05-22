#include "ProtocolBase.h"
#include "param.h"		
#include "param_base.h"	
#include "BasicFunc.h"
#include "debug.h"	
#include "led.h"	
	

u8 password_default[] = {1,2,3,4,5};

//门禁艾贝尔协议
int Ariber_PasswordConfirm(struct tls_protocmd_token_t *tok,
        u8 *res_resp, u32 *res_len)
{
	u8 password[5];
	u8 i;

	for(i=0;i<5;i++)
	{	
		password[i] = tok->arg[2+i];
	}

	if(memcmp(password,password_default,sizeof(password)) == 0)	
	{	
		return TRUE;
	}

	return FALSE;		
}


int Ariber_CancelConfirm(void)
{
	LOG_INFO("Ariber_CancelConfirm\n");	
	return TRUE;
}


int Ariber_ModifyPassword(struct tls_protocmd_token_t *tok,
        u8 *res_resp, u32 *res_len)
{
	u8 password[6];
	u8 i,sum;		
	
	for(i=0;i<6;i++)
	{		
		password[i] = tok->arg[2+i];
	}

	sum = password[0];
	for(i=1;i<5;i++)
	{
		sum = sum ^ password[i];
	}
	if(sum != password[i])
	{
			
		return FALSE;
	}

	return TRUE;
}


int Ariber_GetSysTime(u8 *res_resp)
{	
	u8 param[8];

	//计算的时候字节数要变为ASCII码数
	u8 resp_len;	
		
	//硬件获取时间	
	u16 year = 2015;	
	u8 month=4,day=30,week=4,hour=14,min=51,sec=46;	

	param[0] = HexToIntBCD(year/100);				
	CharToAsc(param[0],res_resp);			
	param[1] = HexToIntBCD(year%100);		
	CharToAsc(param[1],res_resp+2);	
	
	param[2] = HexToIntBCD(month);				
	CharToAsc(param[2],res_resp+4);	
	param[3] = HexToIntBCD(day);
	CharToAsc(param[3],res_resp+6);	
	param[4] = HexToIntBCD(week);			
	CharToAsc(param[4],res_resp+8);	
	param[5] = HexToIntBCD(hour);
	CharToAsc(param[5],res_resp+10);	
	param[6] = HexToIntBCD(min);	
	CharToAsc(param[6],res_resp+12);	
	param[7] = HexToIntBCD(sec);			
	CharToAsc(param[7],res_resp+14);							

	resp_len = GET_SYS_TIME_RTN_LEN*2;

	LED3 = !LED3;
	LED4 = !LED4;	
	
	return resp_len;	
}



int Ariber_SetSysTime(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 data_len;		
	
	u8 year_tmp[2];		
	u16 year;
	u8 month,day,week,hour,min,sec;
	
	year_tmp[0] = BCDToHex(tok->arg[2]);	
	year_tmp[1] = BCDToHex(tok->arg[3]);	

	year = year_tmp[0]*100+year_tmp[1];	
	month = BCDToHex(tok->arg[4]);							
	day   = BCDToHex(tok->arg[5]);
	week  = BCDToHex(tok->arg[6]);
	hour  = BCDToHex(tok->arg[7]);
	min   = BCDToHex(tok->arg[8]);
	sec   = BCDToHex(tok->arg[9]);

	LOG_INFO("Date:%d-%d-%d-%d-%d:%d:%d\n",year,month,day,week,hour,min,sec);	

	//回复消息为空

	//消息长度为0
	data_len = SET_SYS_TIME_RTN_LEN*2;			
	
	return data_len;	
}


int Ariber_SetVoiceMark(void)
{
	return TRUE;
}

int Ariber_PlayVoice(void)
{
	return TRUE;
}


int Ariber_GetVoiceMark(void)
{
	return TRUE;
}



int Ariber_GetFlashID(u8 *res_resp)	
{	
	u8 resp_len;	
	u8 Flash_ID = 0x4A;		
			
	CharToAsc(Flash_ID,res_resp);				
	resp_len = GET_FLASH_ID_RTN_LEN*2;				
	
	return resp_len;		
}

	
int Ariber_GetDeviceInfo(u8 *res_resp)
{
	u8 i;
	DEVICE_INFO *info;	
	u8 len_tmp[3];	
	u8 *str1,*str2;	

	Device_Info_Default();	
	info = Device_Info_Get();												
		
	len_tmp[0] =  strlen(info->SoftWare_Ver);	
	str1 = info->SoftWare_Ver;
	for(i=0;i<len_tmp[0];i++)	
	{		
		CharToAsc(*(str1),res_resp);	
		str1++;	
		res_resp = res_resp + 2;
	}

	len_tmp[1] =  12;
	for(i=len_tmp[0];i<len_tmp[1];i++)					
	{	
		CharToAsc(0x20,res_resp);
		res_resp = res_resp + 2;
	}  	
	
	len_tmp[2] =  strlen(info->Company_Name);		
	str2 = info->Company_Name;
	for(i=0;i<len_tmp[2];i++)		
	{	
		CharToAsc(*(str2),res_resp);		
		str2++;			
		res_resp = res_resp + 2;		
	}  	

	return (len_tmp[0]+len_tmp[1]+len_tmp[2])*2;				
}


	
u8 Week_list1[4][4]={
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
};

u8 Week_list2[4][4]={
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8}
};

u8 Week_list3[4][4]={
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1}
};

u8 Week_list4[4][4]={
	{8,7,6,5},
	{8,7,6,5},
	{8,7,6,5},
	{8,7,6,5},
	{8,7,6,5},
	{8,7,6,5}	
};

int Ariber_GetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	u8 i,j,tmp;
	u8 list_num;
	u8 (*list)[4];			

	list_num = tok->arg[2];//获得第几张表	

	//选取第几张表格
	switch(list_num)
	{		
		case 1:	
			list = Week_list1;
		break;

		case 2:
			list = Week_list2;
		break;
		
		case 3:
			list = Week_list3;
		break;

		case 4:
			list = Week_list4;				
		break;

		default:	

		break;
	}	

	LOG_INFO("GetWeek data dump:\n");		
	
	for(i=0;i<6;i++)
	{		
		for(j=0;j<4;j++)		
		{			
			tmp = HexToBCD(*(*(list+i)+j));			
			CharToAsc(tmp,res_resp);	
#if DEBUG_ON
			printf("%x %x ",*res_resp,*(res_resp+1));	
#endif		
			res_resp = res_resp + 2;
		}		
#if DEBUG_ON
		printf("\n");	
#endif
	}
	
	return GET_LIST_RTN_LEN*2;	
	
}



u8 time_list1[4][4]={
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4}	
};

u8 time_list2[4][4]={
	{11,12,13,14},
	{11,12,13,14},
	{11,12,13,14},
	{11,12,13,14}		
};

u8 time_list3[4][4]={
	{5,6,7,8},
	{5,6,7,8},	
	{5,6,7,8},
	{5,6,7,8}	
};

u8 time_list4[4][4]={
	{15,16,17,18},	
	{15,16,17,18},		
	{15,16,17,18},	
	{15,16,17,18}	
};


int Ariber_GetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	u8 i,j,tmp;
	u8 list_num;
	u8 (*list)[4];		

	list_num = tok->arg[2];//获得第几张表	
	
	LOG_INFO("list_num: %d\n",list_num);			

	LOG_INFO("WorkDay raw data dump:\n");

	//选取第几张表格
	switch(list_num)
	{		
		case 1: 	
			list = time_list1;
		break;

		case 2:
			list = time_list2;	
		break;
		
		case 3:
			list = time_list3;
		break;

		case 4:
			list = time_list4;			
		break;

		default:

		break;
	}		

		
	LOG_INFO("WorkDay real data dump:\n");	

	for(i=0;i<4;i++)
	{	
		for(j=0;j<4;j++)	
		{				
			tmp = HexToBCD(*(*(list+i)+j)); 	
			CharToAsc(tmp,res_resp);		
#if DEBUG_ON
			printf("%x %x ",*res_resp,*(res_resp+1));	
#endif
			res_resp = res_resp + 2;	
		}
#if DEBUG_ON
		printf("\n");
#endif
	}
	
	
	return GET_LIST_RTN_LEN*2;		
	
}


int Ariber_GetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	u8 i,j,tmp;
	u8 list_num;
	u8 (*list)[4];			

	list_num = tok->arg[2];//获得第几张表	

	LOG_INFO("RestDay data dump:\n");	
	
	//选取第几张表格
	switch(list_num)
	{	
		case 1: 
			list = time_list1;
		break;

		case 2:
			list = time_list2;
		break;
		
		case 3:
			list = time_list3;
		break;

		case 4:
			list = time_list4;			
		break;

		default:

		break;	
	}	


	for(i=0;i<4;i++)
	{	
		for(j=0;j<4;j++)	
		{				
			tmp = HexToBCD(*(*(list+i)+j)); 	
			CharToAsc(tmp,res_resp);			
#if DEBUG_ON
			printf("%x %x ",*res_resp,*(res_resp+1));	
#endif
			res_resp = res_resp + 2;
		}	
#if DEBUG_ON
		printf("\n");
#endif
	}	
		
	return GET_LIST_RTN_LEN*2;	
	
}







int Ariber_SetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	
}

int Ariber_SetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	
}	

int Ariber_SetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	
}

