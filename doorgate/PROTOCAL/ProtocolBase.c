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



int Ariber_IO_Switch()
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


	
u8 Week_list1[6][4]={
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
	{1,2,3,4},
};

u8 Week_list2[6][4]={
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8},
	{5,6,7,8}
};

u8 Week_list3[6][4]={
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1},
	{4,3,2,1}
};

u8 Week_list4[6][4]={	
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





//4B命令下的子命令

int Ariber_SetHandlePos(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 handle_pos[2];//获取方向

	handle_pos[0] = tok->arg[2];//获取的第一个字节	
	handle_pos[1] = tok->arg[3];//获取到把手的方向

	if(handle_pos[1] == HANDLE_RIGHT)
	{
		LOG_INFO("HANDLE_RIGHT\n ");
	}
	else
	{
		LOG_INFO("HANDLE_LEFT\n ");	
	}
	
	return SET_SUCCEED_RTN_LEN;
}


int Ariber_SetCardBit(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Card_ValidBit = tok->arg[2]; //卡有效位
	u8 Card_EndianSet = tok->arg[3];//字节序设置
	
	LOG_INFO("Card_ValidBit:&d\n",Card_ValidBit);		
	LOG_INFO("Card_EndianSet:&d\n",Card_EndianSet);		
	
	return SET_SUCCEED_RTN_LEN;
}

int Ariber_SetArmyParam(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Army_Style = tok->arg[2];//撤布防+布防类型	
	u8 Army_Period = tok->arg[3];//布防时间(单位:分钟)

	u8 Armying_Type = Army_Style & 0x01;//布防类型
	u8 Army_Type = Army_Style & 0x02;//撤防和布防御

	LOG_INFO("Army_Period:%d\n",Army_Period);				

	if(Armying_Type == DEVICE_ARMING)	
	{
		LOG_INFO("Armying_Type: DEVICE_ARMING\n");	
	}
	else
	{
		LOG_INFO("Armying_Type: DEVICE_UN_ARMING\n");			
	}

	if(Army_Type == DEVICE_AUTO_ARMING)
	{
		LOG_INFO("Army_Type: DEVICE_AUTO_ARMING\n");	
	}
	else
	{
		LOG_INFO("Army_Type: DEVICE_MAN_ARMING\n");		
	}	
	
	return SET_SUCCEED_RTN_LEN;
}

int Ariber_SetAlarmParam(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u16 Alarm_Arming_Time;
	u8 Alarm_Mask,Alarm_RevokeByKey,Alarm_TimeTmp[2];	

	Alarm_TimeTmp[0] = tok->arg[2];
	Alarm_TimeTmp[1] = tok->arg[3];
	Alarm_Mask = tok->arg[4];
	Alarm_RevokeByKey = tok->arg[5];	
	
	Alarm_Arming_Time = TwoCharToInt(Alarm_TimeTmp[0],Alarm_TimeTmp[1]);	

	LOG_INFO("Alarm_Arming_Time:%d\n",Alarm_Arming_Time);

	if(Alarm_Mask == ALARM_MASK_ON)
	{
		LOG_INFO("ALARM_MASK_ON\n");
	}
	else
	{
		LOG_INFO("ALARM_MASK_OFF\n");	
	}

	
	if(Alarm_RevokeByKey == ALARM_REVOKE_BY_KEY)	
	{
		LOG_INFO("ALARM_REVOKE_BY_KEY\n");
	}	
	else
	{
		LOG_INFO("ALARM_REVOKE_NO_KEY\n");	
	}


	return SET_SUCCEED_RTN_LEN;	
}

int Ariber_SetArmyingSta(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 i,pos = 2;
	u8 Armying_Flag,Armying_Type,Armying_Tim_Num,Armying_Tim[6],Armying_N_Byte;
	u8 Armying_Man,Armying_Condition;
	u8 LianDong[8];

	Armying_Flag = tok->arg[pos++];
	Armying_Type = tok->arg[pos++];
	Armying_Tim_Num = tok->arg[pos++];
	Armying_N_Byte = tok->arg[pos++];	

	for(i=0;i<6;i++)
	{
		Armying_Tim[i] = tok->arg[pos++];
	}

	Armying_Man = tok->arg[pos++];
	Armying_Condition = tok->arg[pos++];

	for(i=0;i<8;i++)
	{			
		LianDong[i] = tok->arg[pos++];
	}
	
	LOG_INFO("Armying_Info\n");	
	LOG_INFO("1.%d,2.%d,3.%d\n",Armying_Flag,Armying_Type,Armying_Tim_Num);	
	LOG_INFO("4.%d,5.%d,6.%d\n",Armying_N_Byte,Armying_Man,Armying_Condition);

	LOG_INFO("LianDong:"); 
	for(i=0;i<7;i++)
	{	
#if DEBUG_ON	
		printf("%x ",LianDong[i]);	
#endif
	}
	
#if DEBUG_ON
	printf("\n");		
#endif

	return SET_SUCCEED_RTN_LEN;	
}

int Ariber_SetSystemSta(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 System_Sta,pos = 2;

	System_Sta = tok->arg[pos++];

	switch(System_Sta)
	{
		case 0:
			LOG_INFO("Normal Condition\n"); 

			break;

		case 1:
			LOG_INFO("Setting Condition\n"); 

			break;

		case 2:
			LOG_INFO("Card reading Condition\n"); 	

			break;

		case 3:
			LOG_INFO("Debug Condition\n"); 	

			break;

		default:		

			break;
	}
	
	return SET_SUCCEED_RTN_LEN;		
}

int Ariber_SetSwitchOut(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	
	
	return SET_SUCCEED_RTN_LEN;		
}

int Ariber_SetVoiceMark(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Door_Num,Mask_Bit[2],pos = 2;
	u16 Voice_Mask;
	
	Door_Num = tok->arg[pos++];
	Mask_Bit[0] = tok->arg[pos++];
	Mask_Bit[1] = tok->arg[pos++];	
	Voice_Mask = TwoCharToInt(Mask_Bit[0],Mask_Bit[1]);
	
	LOG_INFO("Voice_Mask = %d\n",Voice_Mask);	
	
	return SET_SUCCEED_RTN_LEN;		
}
	
int Ariber_SetIOSwitchParam(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	
	
	return SET_SUCCEED_RTN_LEN;	
}



