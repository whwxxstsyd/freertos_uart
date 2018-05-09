#include "protocol.h"
#include "calendar.h"	
#include "debug.h"	
#include "param.h"


#if Post_Protocol_Open

static int post_protocmd_get_time_proc(struct tls_atcmd_token_t *tok,
								       	 		char *res_resp, u32 *res_len)
{
	TypedefDateTime DataTime;	
	
	if(GetDateTime(&DataTime))
	{
		LOG_ERROR("get data err!!\n");
		return FALSE;
	}

	DataTime.year	
	DataTime.month	
	DataTime.week
	DataTime.day
	DataTime.hour
	DataTime.min
	DataTime.sec

		
}


static int post_protocmd_set_time_proc()
{
	u8 

	SetDateTime
}



static int post_protocmd_get_addr_proc()
{
	
}


static int post_protocmd_set_addr_proc()
{
	
}

static int post_protocmd_get_version_proc()
{
	
}


static int post_protocmd_set_configID_proc()
{
	
}


static int post_protocmd_get_configID_proc()
{
	
}


static int post_protocmd_remote_proc()
{
	
}


static int post_protocmd_get_ariber_ver_proc()
{
	
}


static int post_protocmd_set_baudrate_proc()
{
	
}


static int post_protocmd_authority_confirm_proc()
{
	
}


static int post_protocmd_set_sys_param_proc()
{
	
}


static int post_protocmd_get_sys_param_proc()
{
	
}


static int post_protocmd_set_guard_param_proc()
{
	
}


static int post_protocmd_get_guard_param_proc()
{
	
}

#endif



#if Makepower_Protocol_Open

static int mp_protocmd_get_device_info_proc()
{
	
}


static int mp_protocmd_set_addr_proc()
{
	
}


static int mp_protocmd_get_time_proc()
{
	
}


static int mp_protocmd_set_time_proc()
{
	
}


static int mp_protocmd_mem_dev_proc()
{
	
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
	{ 0x11, 0, mp_protocmd_get_device_info_proc },		//获取设备信息
	{ 0xA1, 0, mp_protocmd_set_addr_proc },		//设置设备地址
	{ 0x13, 0, mp_protocmd_get_time_proc },		//获取时间
	{ 0xA3, 0, mp_protocmd_set_time_proc },		//设置时间
	{ 0x37, 0, mp_protocmd_mem_dev_proc }, 		//存储测试
#endif

}


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

	
static int protocmd_nop_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{	
    if (!tok->arg_found && (tok->op == ATCMD_OP_NULL)) 
	{	
        *res_len = protocmd_ok_resp(res_resp);	
    } 
	else 
	{
        *res_len = protocmd_err_resp(res_resp, CMD_ERR_OPS);
    }

    return 0;
}


int tls_protocmd_parse(struct tls_protocmd_token_t *tok, char *buf, u32 len)
{
	
}
	
	
int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp, u32 *res_len)
{
	int err;
	struct tls_protocmd_t *protocmd, *match = NULL;

	if (strlen(tok->name) == 0) 
	{
		err = protocmd_nop_proc(tok, res_rsp, res_len);
		return err;
	}

	/* look for AT CMD handle table */
	protocmd = protocmd_tbl;
	while (protocmd->name) 
	{		
		if (strcmp(protocmd->name, tok->name) == 0)
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
















void tls_protocol_init(void)			
{
	
}


















//创立协议
void makepower_protocol_deal(void)
{
		
}






























//邮电协议
void post__protocol_deal(void)
{
	
}














