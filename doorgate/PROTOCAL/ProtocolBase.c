#include "ProtocolBase.h"
#include "param.h"	
#include "BasicFunc.h"
#include "debug.h"	


u8 password_default[] = {1,2,3,4,5};

//门禁艾贝尔协议
int Ariber_PasswordConfirm(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
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
        char *res_resp, u32 *res_len)
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


int Ariber_GetSysTime(char *buf)
{	
	u8 param[8];

	//计算的时候字节数要变为ASCII码数
	u8 data_len = GET_SYS_TIME_RTN_LEN*2;

	//硬件获取时间	
	u16 year = 2018;
	u8 month=3,day=12,week=2,hour=22,min=30,sec=50;

	param[0] = HexToIntBCD(year/100);				
	CharToAsc(param[0],buf);			
	param[1] = HexToIntBCD(year%100);		
	CharToAsc(param[1],buf+2);	
	
	param[2] = HexToIntBCD(month);				
	CharToAsc(param[2],buf+4);	
	param[3] = HexToIntBCD(day);
	CharToAsc(param[3],buf+6);	
	param[4] = HexToIntBCD(week);		
	CharToAsc(param[4],buf+8);	
	param[5] = HexToIntBCD(hour);
	CharToAsc(param[5],buf+10);	
	param[6] = HexToIntBCD(min);	
	CharToAsc(param[6],buf+12);	
	param[7] = HexToIntBCD(sec);			
	CharToAsc(param[7],buf+14);						
	
	return data_len;
}



int Ariber_SetSysTime(void)
{
	return TRUE;
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



int Ariber_GetFlashID(void)
{
	return TRUE;		
}




