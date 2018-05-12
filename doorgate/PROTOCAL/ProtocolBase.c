#include "ProtocolBase.h"
#include "debug.h"	

u8 password_default[] = {1,2,3,4,5}	

//ÃÅ½û°¬±´¶ûÐ­Òé
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
		return 1;
	}

	return 0;
}


int Ariber_CancelConfirm(void)
{
	LOG_INFO("Ariber_CancelConfirm\n");	
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
		
		return;
	}

	
}


int Ariber_GetSysTime()
{
	
}


int Ariber_SetSysTime()
{
	
}


int Ariber_SetVoiceMark()
{
	
}

int Ariber_PlayVoice()
{


}


int Ariber_GetVoiceMark()
{
	
}



int Ariber_GetFlashID()
{
	
}




