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

	LED3 = !LED3;
	LED4 = !LED4;	
	
	return ASCII_LEN(GET_SYS_TIME_RTN_LEN);	
}



int Ariber_SetSysTime(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
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
	
	return ASCII_LEN(SET_SYS_TIME_RTN_LEN);		
}

	

	
int Ariber_GetDeviceInfo(u8 *res_resp)
{
	u8 i;
	u8 resp_len;	
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

	resp_len = (len_tmp[0]+len_tmp[1]+len_tmp[2]);

	return ASCII_LEN(resp_len);					
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
	
	return ASCII_LEN(GET_LIST_RTN_LEN);	
	
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
	
	return ASCII_LEN(GET_LIST_RTN_LEN);		
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
		
	return ASCII_LEN(GET_LIST_RTN_LEN);	
	
}







int Ariber_SetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);
}

int Ariber_SetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);	
}	

int Ariber_SetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);			
}





//4B命令下的子命令


//设置左右把手
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
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);
}

//设置卡有效位
int Ariber_SetCardBit(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Card_ValidBit = tok->arg[2]; //卡有效位
	u8 Card_EndianSet = tok->arg[3];//字节序设置
	
	LOG_INFO("Card_ValidBit:&d\n",Card_ValidBit);		
	LOG_INFO("Card_EndianSet:&d\n",Card_EndianSet);		
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);
}

//设置门禁撤布防参数
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
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);
}


//设置报警参数
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


	return ASCII_LEN(SET_SUCCEED_RTN_LEN);		
}


//设置设布防的状态
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
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);		
}	


//设置系统状态
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
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);			
}


//设置开关量输出控制
int Ariber_SetSwitchOut(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);			
}	


//设置屏蔽语音
int Ariber_SetVoiceMark(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Door_Num,Mask_Bit[2],pos = 2;
	u16 Voice_Mask;
	
	Door_Num = tok->arg[pos++];
	Mask_Bit[0] = tok->arg[pos++];
	Mask_Bit[1] = tok->arg[pos++];	
	Voice_Mask = TwoCharToInt(Mask_Bit[0],Mask_Bit[1]);
	
	LOG_INFO("Voice_Mask = %d\n",Voice_Mask);	
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);		
}


//输入输出开关量配置	
int Ariber_SetIOSwitchParam(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
		
	
	return ASCII_LEN(SET_SUCCEED_RTN_LEN);		
}

/*******End ********/




/*4C命令下的子命令*/	

//读取左右把手
int Ariber_GetHandlePos(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 pos = HANDLE_RIGHT;//读取把手的方向
	u8 handle_pos[2];//获取方向

	handle_pos[0] = 0;
	CharToAsc(handle_pos[0],res_resp);
	res_resp = res_resp + 2;	
	
	handle_pos[1] = pos;	
	CharToAsc(handle_pos[1],res_resp);			
		
	return ASCII_LEN(GET_HANDLE_POS_RTN_LEN);
}

//读取卡有效位
int Ariber_GetCardBit(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Card_ValidBit = 4; //卡有效位		
	u8 Card_EndianSet = 0;//字节序设置

	CharToAsc(Card_ValidBit,res_resp);
	res_resp = res_resp + 2;
	
	CharToAsc(Card_EndianSet,res_resp);
	
	return ASCII_LEN(GET_CARD_BIT_RTN_LEN);	
}

//读取门禁撤布防参数
int Ariber_GetArmyParam(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{	
	u16 Army_Type = 2;	
	u16 Arming_Time = 6;	

	u16ToAsc(Army_Type,res_resp);					
	res_resp = res_resp + 4;	
	
	u16ToAsc(Arming_Time,res_resp);
	
	return ASCII_LEN(GET_ARMY_PARAM_RTN_LEN);			
}

//读取报警参数
int Ariber_GetAlarmParam(struct tls_protocmd_token_t *tok,u8 *res_resp)
{
	u16 Alarm_Arming_Time;
	u8 Alarm_Mask,Alarm_RevokeByKey;		

	Alarm_Arming_Time = 5;
	Alarm_Mask = 1;
	Alarm_RevokeByKey = 1;

	u16ToAsc(Alarm_Arming_Time,res_resp);
	res_resp = res_resp +4;
	
	CharToAsc(Alarm_Mask,res_resp);
	res_resp = res_resp +2;
	
	CharToAsc(Alarm_RevokeByKey,res_resp);		

	return ASCII_LEN(GET_ALARM_PARAM_RTN_LEN);	
}

//读取设布防的状态
int Ariber_GetArmyingSta(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 i;
	u8 Armying_Flag = 1;
	u8 Armying_Type = 4;
	u8 Armying_Tim_Num = 48;
	u8 Armying_N_Byte = 1;
	u8 Armying_Tim[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	u8 Armying_Man = 0;
	u8 Armying_Condition = 0;
	u8 LianDong[8] = {0,0,3,5,0,0,0,0};	


	CharToAsc(Armying_Flag,res_resp); 	
	res_resp = res_resp +2;	

	CharToAsc(Armying_Type,res_resp); 	
	res_resp = res_resp +2;	

	CharToAsc(Armying_Tim_Num,res_resp); 	
	res_resp = res_resp +2;	

	CharToAsc(Armying_N_Byte,res_resp); 	
	res_resp = res_resp +2;		

	for(i=0;i<6;i++)
	{
		CharToAsc(Armying_Tim[i],res_resp);	
		res_resp = res_resp +2; 	
	}

	CharToAsc(Armying_Man,res_resp); 	
	res_resp = res_resp +2;		

	CharToAsc(Armying_Condition,res_resp); 	
	res_resp = res_resp +2;		

	for(i=0;i<8;i++)
	{	
		CharToAsc(LianDong[i],res_resp);	
		res_resp = res_resp +2;		 	
	}

	return ASCII_LEN(GET_ARMYING_STA_RTN_LEN);	
}


//获取自定义数据
int Ariber_GetUserDefinedData(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	
	
	return ASCII_LEN(GET_USER_DEF_DATA_RTN_LEN);	
}


//读取屏蔽语音状态
int Ariber_GetVoiceMaskSta(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 Door_Num;
	u16 Voice_Mask;

	Door_Num = 0;
	Voice_Mask = 0xFFFF;

	CharToAsc(Door_Num,res_resp); 	
	res_resp = res_resp +2;

	u16ToAsc(Voice_Mask,res_resp);	
	
	return ASCII_LEN(GET_VOICE_MASK_STA_RTN_LEN);	
}


//读取输入输出开关量配置
int Ariber_GetSwitchConfig(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 i,j,k;
	u8 resp_len;
	u8 Sw_In_Num = 8;
	SWITCH_IN_PARAM_T Sw_In_Param[8];		

	u8 Sw_Out_Num = 8;
	u8 expr_param_num = 3;	
	SWITCH_OUT_PARAM_T Sw_Out_Param[8];		

	CharToAsc(Sw_In_Num,res_resp); 		
	res_resp = res_resp +2;		
	
	for(i=0;i<Sw_In_Num;i++)
	{	
		Sw_In_Param[i].state = 105;	
		CharToAsc(Sw_In_Param[i].state,res_resp); 	
		res_resp = res_resp +2;		

		Sw_In_Param[i].sec = 10;	
		u16ToAsc(Sw_In_Param[i].state,res_resp); 	
		res_resp = res_resp +4;		
		
		Sw_In_Param[i].rele_sta	= 2;			
		CharToAsc(Sw_In_Param[i].rele_sta,res_resp);	 	
		res_resp = res_resp +2;		
	}

	resp_len = Sw_In_Num*3+1;

	CharToAsc(Sw_Out_Num,res_resp); 	
	res_resp = res_resp +2;		

	for(i=0;i<Sw_Out_Num;i++)
	{	
		Sw_Out_Param[i].state = 1;		
		CharToAsc(Sw_Out_Param[i].state,res_resp); 	
		res_resp = res_resp +2; 		

		Sw_Out_Param[i].argv.num = 4;
		CharToAsc(Sw_Out_Param[i].argv.num,res_resp); 	
		res_resp = res_resp +2; 	

		k = 1;
		
		for(j=0;j<Sw_Out_Param[i].argv.num;j++)
		{		
			Sw_Out_Param[i].argv.param[j] = k++;		
			CharToAsc(Sw_Out_Param[i].argv.num,res_resp); 	
			res_resp = res_resp +2;	 	
			
			if(j != (Sw_Out_Param[i].argv.num-1))	
			{		
				Sw_Out_Param[i].argv.sign[j] = '&'; 
				CharToAsc(Sw_Out_Param[i].argv.num,res_resp);	
				res_resp = res_resp +2; 
			}	
		}		
	}		
	
	resp_len = resp_len + Sw_Out_Num*(2+Sw_Out_Param[i].argv.num*2-1) + 1;
		
	return resp_len;			
}


//读取门禁撤布防配置参数
int Ariber_GetArmyConfigParam(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	
	
	return ASCII_LEN(GET_ARMY_CONFIG_RTN_LEN);		
}


//读取flash id
int Ariber_GetFlashID(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{			
	u8 Flash_ID = 0x4A;		
			
	CharToAsc(Flash_ID,res_resp);								
	
	return ASCII_LEN(GET_FLASH_ID_RTN_LEN);				
}
	

//读取门禁对应日志
int Ariber_GetDoorLog(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 MainCmd,ViceCmd;

	//获取主命令和副命令
	MainCmd = tok->arg[2];
	ViceCmd = tok->arg[3];

	switch(MainCmd)
	{
		case 0x01:
			
			switch(ViceCmd)
			{			
				case 1:

					LOG_INFO("Add_Card_OPT\n");
					
				break;

				case 2:

					LOG_INFO("Del_Card_OPT\n");

				break;

				case 3:

					LOG_INFO("Reset_Card_Lib\n");
					
				break;

				case 4:
					
					LOG_INFO("Add_Cards_OPT\n");	
					
				break;

				case 5:	

					LOG_INFO("Read_Card_Lib\n");		

				break;

				default:

				break;
			}

			
		break;

		case 0x02:

		break;

		case 0x03:
			switch(ViceCmd)
			{	
				case 1:

				break;

				case 2:

				break;

				default:

				break;
			}

		break;

		case 0x04:

		break;

		case 0x05:	
			
			switch(ViceCmd)
			{
				case 1:

				break;

				case 2:

				break;

				case 3:

				break;

				case 4:		

				break;

				default:	

				break;
			}

		break;

		default:

		break;
	}
	
	
	return ASCII_LEN(GET_DOOR_LOG_RTN_LEN);	
}


//读取用户密码
int Ariber_GetUserPassword(struct tls_protocmd_token_t *tok,u8 *res_resp)	
{
	u8 code_group;
	u8 code_Rtn[8];
	u8 code_tmp[3];

	code_group = tok->arg[2];//获得组号
	
	code_Rtn[0] = 1;
	CharToAsc(code_Rtn[0],res_resp); 
	res_resp = res_resp +2;
	
	code_Rtn[1] = 1;	
	CharToAsc(code_Rtn[1],res_resp); 
	res_resp = res_resp +2;
	
	//密码
	code_Rtn[2] = 12;	
	code_tmp[0] = HexToIntBCD(code_Rtn[2]);	
	CharToAsc(code_tmp[0],res_resp); 	
	res_resp = res_resp +2;

	code_Rtn[3] = 34;
	code_tmp[1] = HexToIntBCD(code_Rtn[3]);
	CharToAsc(code_tmp[1],res_resp); 
	res_resp = res_resp +2;

	code_Rtn[4] = 56;	
	code_tmp[2] = HexToIntBCD(code_Rtn[4]);
	CharToAsc(code_tmp[2],res_resp); 	
	res_resp = res_resp +2;	

	//备用
	code_Rtn[5] = 0;		
	CharToAsc(code_Rtn[5],res_resp); 		
	res_resp = res_resp +2; 

	code_Rtn[6] = 0;	
	CharToAsc(code_Rtn[6],res_resp); 	
	res_resp = res_resp +2; 	

	code_Rtn[7] = 0;	
	CharToAsc(code_Rtn[7],res_resp);	 	
	
	
	return ASCII_LEN(GET_USER_PASS_RTN_LEN);	
}


/*******End ********/


