#include "mp_protocol_hostif.h"
#include "ProtocolBase.h"
#include "param_base.h"
#include "mp_mem.h"		
#include "protocol.h"
#include "BasicFunc.h"		
#include "calendar.h"	
#include "debug.h"	
#include "param.h"	
#include "common.h"


#if Post_Protocol_Open

static int post_protocmd_get_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)	
{		
	int ret; 
	TypedefDateTime DataTime;	
	
	ret = GetDateTime(&DataTime);
		
	if(ret)	
	{
		LOG_ERROR("get time err!!\n");
		return FALSE;
	}	
	else
	{	
		DateTime_Trans(res_resp,&DataTime);			
		return TRUE;	
	}

}


static int post_protocmd_set_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 buff[PROTOCMD_MAX_ARG];
	u8 i;	
	u16 arg_found = tok->arg_found;
		

	for(i=0;i<arg_found;i++)
	{
		buff[i] = tok->arg[i];
	}

	for(i=0;i<arg_found/2;i++)	
	{	
		TwoAscTOHex(buff[i],buff[i+1]);
	}
		
		
		
	if(check_time_format((TypedefDateTime *)buff))	
	{
		return FALSE;
	}

	SetDateTime((TypedefDateTime *)buff);	

	return TRUE;
}



static int post_protocmd_get_addr_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}	


static int post_protocmd_set_addr_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}

const u8 SysCreatedDate[12] = __DATE__;	
const u8 SysCreatedTime[8] = __TIME__;

static int post_protocmd_get_version_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	char temp2[4],temp3[12];		
	char *p;
	DEVICE_INFO *info;
	u8 i;
	u16 dev_addr;	
	u32 buflen;

	p = res_resp + 13;
	
	Device_Info_Get(info);
	
	dev_addr = info->Device_Addr[0];	
	
	u16ToAsc(dev_addr,temp2);	
	for(i=0;i<sizeof(temp2);i++)
	{	
		*p++ = temp2[i];
	}	
	
	MEMCPY(p, info->SoftWare_Ver, sizeof(info->SoftWare_Ver)-1);
	p += sizeof(info->SoftWare_Ver)-1;
	MEMCPY(p, info->HardWare_Ver, sizeof(info->HardWare_Ver)-1);
	p += sizeof(info->HardWare_Ver)-1;	
	MEMCPY(p, info->Company_Name, sizeof(info->Company_Name)-1);
	p += sizeof(info->Company_Name)-1;
	MEMCPY(p, info->Device_Name, sizeof(info->Device_Name)-1);
	p += sizeof(info->Device_Name)-1;		

	//这里解析设备唯一序列号还有点问题
	u32ToAsc(info->Uer_ID,temp3);				
	for(i=0;i<sizeof(temp3);i++)	
	{		
		*p++ = temp3[i];
	}			

	MEMCPY(p, SysCreatedDate, sizeof(SysCreatedDate));
	p += sizeof(SysCreatedDate);		

	MEMCPY(p, SysCreatedTime, sizeof(SysCreatedTime));
	p += sizeof(SysCreatedTime);			

	*res_len = 100;	

	return TRUE;
}


static int post_protocmd_set_configID_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}


static int post_protocmd_get_configID_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;	
}


static int post_protocmd_remote_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}



	



static int post_protocmd_get_ariber_ver_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}


static int post_protocmd_set_baudrate_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}


static int post_protocmd_authority_confirm_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{	
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF0)
	{
		return FALSE;
	}

	switch(cmd_type)
	{
		case CMD_0x48_GET_AUTHOR:	

			LOG_INFO(" CMD:0x48 Type:Set Date\n");	

			Ariber_PasswordConfirm(tok,res_resp,res_len);
			
			break;	

		case CMD_0x48_DEL_AUTHOR:

			LOG_INFO(" CMD:0x48 Type:Delete Author\n");

			Ariber_CancelConfirm();		
		
			break;

		case CMD_0x48_CHG_PASSWD:

			LOG_INFO(" CMD:0x48 Type:Change Password\n");

			Ariber_ModifyPassword(tok,res_resp,res_len);	
		
			break;

		default:
	
			break;
		
	}
	return TRUE;	
}
	

static int post_protocmd_set_sys_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF1)
	{
		return FALSE;
	}
	
	switch(cmd_type)
	{
		case CMD_0x49_SET_DATE:
	
			LOG_INFO("CMD:0x49 Type:Set Date\n");		
	
			//Ariber_SetSysTime();	

			break;

		default:

			break;
	}
	return TRUE;	
}

	
static int post_protocmd_get_sys_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF2)
	{	
		LOG_INFO("cmd_group error\n");		
		return FALSE;	
	}

	switch(cmd_type)
	{	
		case CMD_0x4A_GET_DATE:		
	
			LOG_INFO(" CMD:0x4A Type:Get Module Date\n");		
				
			//Ariber_GetSysTime();			

			break;

		case CMD_0x4A_GET_DOOR_STA:	

			LOG_INFO("0x4A CMD,Get Door State\n");		
			
			break;

		default:
		
			break;
	}
	
	
	
	return TRUE;
}


static int post_protocmd_set_guard_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF3)
	{
		return FALSE;
	}

	switch(cmd_type)
	{
		case CMD_0x4B_SET_VOICE:		

			LOG_INFO(" CMD:0x4B Type:Set Voice\n");	
			
			Ariber_SetVoiceMark();

			break;

		case CMD_0x4B_PLY_VOICE:			

			LOG_INFO(" CMD:0x4B Type:Play Voice\n"); 

			Ariber_PlayVoice();	

			break;

		default:

			break;
	}
	return TRUE;
}


static int post_protocmd_get_guard_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF4)
	{
		return FALSE;
	}	

	switch(cmd_type)
	{
		case CMD_0x4C_GET_VOICE:		

			LOG_INFO(" CMD:0x4C Type:Get Voice Mark\n");

			Ariber_GetVoiceMark();

			break;

		case CMD_0x4C_GET_FLASH_ID:			

			LOG_INFO(" CMD:0x4C Type:Get Falsh ID\n");
			
			Ariber_GetFlashID();
		
			break;

		default:

			break;
	}
	return TRUE;
}

#endif





//创立部分协议
#if Makepower_Protocol_Open

static int mp_protocmd_get_device_info_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;	
}


static int mp_protocmd_set_addr_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}


static int mp_protocmd_get_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}


static int mp_protocmd_set_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}


static int mp_protocmd_mem_dev_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return TRUE;
}

#endif

	









static struct tls_protocmd_t  protocmd_tbl[] = {			
#if Post_Protocol_Open	
	{ 0x4D, 0, post_protocmd_get_time_proc },				//获取时间
	{ 0x4E, 0, post_protocmd_set_time_proc },				//设置时间
	{ 0x50, 0, post_protocmd_get_addr_proc },				//获取地址
	{ 0xE3, 0, post_protocmd_set_addr_proc },				//设置地址
	{ 0x12, 0, post_protocmd_get_version_proc },			//获取版本
	{ 0xFB, 0, post_protocmd_set_configID_proc },			//设置多个参数
	{ 0xFC, 0, post_protocmd_get_configID_proc },			//获取多个参数
	{ 0x45, 0, post_protocmd_remote_proc },				//遥控设置
	
	{ 0x51, 0, post_protocmd_get_ariber_ver_proc },		//获取ariber的版本
	{ 0xA1, 0, post_protocmd_set_baudrate_proc },			//设置波特率
	{ 0x48, 0, post_protocmd_authority_confirm_proc },	//获取权限
	{ 0x49, 0, post_protocmd_set_sys_param_proc },		//设置系统参数
	{ 0x4A, 0, post_protocmd_get_sys_param_proc },		//获取系统参数
	{ 0x4B, 0, post_protocmd_set_guard_param_proc },		//设置门禁参数
	{ 0x4C, 0, post_protocmd_get_guard_param_proc },		//获取门禁参数
#endif 	
		
#if Makepower_Protocol_Open
	{ 0x11, 0, mp_protocmd_get_device_info_proc },	//获取设备信息
	{ 0xA1, 0, mp_protocmd_set_addr_proc },		//设置设备地址
	{ 0x13, 0, mp_protocmd_get_time_proc },		//获取时间
	{ 0xA3, 0, mp_protocmd_set_time_proc },		//设置时间
	{ 0x37, 0, mp_protocmd_mem_dev_proc }, 		//存储测试
#endif

};		


int protocmd_err_resp(char *buf, int err_code)
{
    int len;
    len = sprintf(buf, "+ERR=%d", -err_code);
    return len;
}


int protocmd_ok_resp(char *buf)
{	
    int len;
    len = sprintf(buf, "OK");	
    return len;
}

	
	
int LCHKSUM_LENID_CMP(u16 len_param)	
{
	u8 temp1 = (len_param >> 0)&0x0F;
	u8 temp2 = (len_param >> 4)&0x0F;
	u8 temp3 = (len_param >> 8)&0x0F;
	u8 temp4 = (len_param >> 12)&0x0F;	
		
	u8 sum = temp1 + temp2 + temp3;			
	
	sum = sum%16;
	
	sum = ((~sum)&0x0f) + 1;			

	
	if(sum == temp4)
	{
		return TRUE;	
	}
	return FALSE;
}

int VERSION_CHK(u16 len_param)	
{	
	return TRUE;	
}


int tls_protocmd_parse(struct tls_protocmd_token_t *tok, char *buf, u32 *res_len)
{	
    u16 remain_len=0;
	u8 lchksum,i=0,j=0,tmp,tmp1,tmp2;	
    char *buf_start = buf;	
	
	tok->VER = TwoAscTOHex(*(buf+1),*(buf+2));	//(1,2)
	//版本检查
	if(!VERSION_CHK(tok->VER))
	{
		return RTN_VER_ERR;		
	}						
	
	tok->ADDR	= 	TwoAscTOHex(*(buf+3),*(buf+4));//(3,4)
	tok->CID1	= 	TwoAscTOHex(*(buf+5),*(buf+6));//(5,6)
	tok->CID2	= 	TwoAscTOHex(*(buf+7),*(buf+8));//(7,8)	
	tok->LENGTH = 	TwoCharToInt(TwoAscTOHex(*(buf+9),*(buf+10)),TwoAscTOHex(*(buf+11),*(buf+12)));//(9,10,11,12)
		
	//数据校验监测
	if(!LCHKSUM_LENID_CMP(tok->LENGTH))		
	{				
		return RTN_CMDCHK_ERR;	
	}	

	tok->arg_found = (tok->LENGTH & 0xFFF)/2;		
	
	remain_len = tok->arg_found;	

	if(remain_len >= (PROTOCMD_MAX_ARG - 1))	
		return CMD_ERR_INV_PARAMS;		
				
	while (remain_len > 0)		
	{							
		tok->arg[j] = TwoAscTOHex(*(buf+13+i),*(buf+13+i+1));			
		j++;		
		i=i+2;				
		remain_len--;
	}

	LOG_INFO("Cmd_X = %x\n",tok->CID2); //执行的命令

	LOG_INFO("argv_cnt = %d\n",tok->arg_found);//参数个数
	
	LOG_INFO("ARGV = ");//参数数值

	for(j=0;j<tok->arg_found;j++)				
	{								
		printf("%x ",tok->arg[j]);						
	}	
	printf("\n");			
		
	return 0;		
}
	
	
int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp,u32 *res_len)
{		
	int err;
	struct tls_protocmd_t *protocmd, *match = NULL;

	/* look for AT CMD handle table */
	protocmd = protocmd_tbl;
		
	while(protocmd->name) 		
	{	
		if (protocmd->name == tok->CID2)		
		{			
			match = protocmd;
			break;
		}	
		protocmd++;			
	}


	/* at command handle */
	if (match) 
	{	
		//TLS_DBGPRT_INFO("command find: %s\n", atcmd->name);
		err = match->proc_func(tok, res_rsp, res_len);
	} 
	else 
	{	
		/* at command not found */
		*res_len = sprintf(res_rsp, "+ERR=-2");
			
		err = -CMD_ERR_UNSUPP;
	}

	return err;
}



/**
* @brief	对发送缓冲区进行填充
* @param[1]  u8 *buff			写入的缓冲区
* @param[2]  u8 *data			写入的数据
* @param[3]  u8 len			写入数据的长度
* @param[4]  u8 tear_flag		是否进行拆分	
* @return   none 
*/

void fill_buff(u8 *buff,u8 *data,u8 len,u8 tear_flag)	
{
	u8 i;
	
	if(tear_flag)
	{
		for(i=0;i<len;i++)
		{	
			*(buff + i) = Hi_HexToAsc(*(data + i));	
			*(buff + i + 1) = Low_HexToAsc(*(data + i));	
		}
		buff = buff + len*2;

	}
	else
	{
		for(i=0;i<len;i++)
		{	
			*(buff + i) = *(data + i);	
		}

		buff = buff + len;	
	}	
	
}








static void tls_protocol_add_head()	
#if 0
{
	char *p = buff;//指向数据头部
	
	u8 Ver  =  tok->VER;
	u8 Adr  =  tok->ADDR; 	
	u8 CID1 =  tok->CID1; 	
	u8 len1 = 0;
	u8 len2 = 0;		

	u16 Host_Adr = Adr + (CID1&0x0F)*0x100;
	
	//添加头
	fill_buff(p,'~',1,0);
	fill_buff(p,&Ver,1,1);		
	fill_buff(p,&Adr,1,1);
	
	if(Ver == 0x20)
	{	
		CID1 = 0xD0;
	}
	else
	{				
		CID1 = 0x80+((Host_Adr>>8)&0x0F);
	}
	
	fill_buff(p,&CID1,1,1);
	//两个字节的空	
	fill_buff(p,&len1,1,1);
	fill_buff(p,&len2,1,1);		

}

#else
{
		
}
#endif



static void tls_protocol_add_tail()
{	
	
}

static void tls_protocol_add_body()
{
	
}


int tls_protocol_rebuild(struct tls_protocmd_token_t *tok,u8 *buff,u32 *res_len)
{	
	tls_protocol_add_head();

	tls_protocol_add_body();
		
	tls_protocol_add_tail();
	
}

















void tls_protocol_init(void)			
{
	
}


















//创立协议
void makepower_protocol_deal(void)
{
		
}






























//邮电协议
void post_protocol_deal(void)
{
		
}














