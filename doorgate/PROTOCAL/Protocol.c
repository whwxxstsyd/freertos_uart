#include "ProtocolBase.h"
#include "mp_mem.h"		
#include "protocol.h"
#include "BasicFunc.h"		
#include "calendar.h"	
#include "debug.h"			
#include "common.h"	



CMD_0x48_HANDLE_T CMD_0x48;
CMD_0x49_HANDLE_T CMD_0x49;	
CMD_0x4A_HANDLE_T CMD_0x4A;
CMD_0x4B_HANDLE_T CMD_0x4B;
CMD_0x4C_HANDLE_T CMD_0x4C;


#if Post_Protocol_Open

static int post_protocmd_get_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)	
{		
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];
	
	if(cmd_group != 0xF2)
	{			
		LOG_INFO("cmd_group error\n");		
		return RTN_UNVALID_DATA;			
	}
	
	switch(cmd_type)
	{		
		case CMD_0x4D_GET_DATE:		
								
			*res_len = Ariber_GetSysTime(res_resp);	
			
			return RTN_NORMAL;
				
			break;	

		default:
		
			break;
	}
	
	return RTN_UNVALID_DATA;

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

	return RTN_NORMAL;
}



static int post_protocmd_get_addr_proc(struct tls_protocmd_token_t *tok,
        										 char *res_resp, u32 *res_len)
{
	*res_len = 0;

	return RTN_NORMAL;
}	


static int post_protocmd_set_addr_proc(struct tls_protocmd_token_t *tok,
        										char *res_resp, u32 *res_len)
{
	u8 Device_Addr = tok->arg[0];
	
	if((Device_Addr & 0x00ff) == 0x0000 || (Device_Addr & 0x00ff) == 0x00ff )
	{	
		return RTN_UNVALID_DATA;  
	}	

	//保存地址
	tok->ADDR = Device_Addr;	

	//存储到EEPROM	


	*res_len = 0;

	return RTN_NORMAL;	
}



const u8 SysCreatedDate[12] = __DATE__;	
const u8 SysCreatedTime[8] = __TIME__;

static int post_protocmd_get_version_proc(struct tls_protocmd_token_t *tok,
        											char *res_resp, u32 *res_len)
{
	

	return RTN_NORMAL;
}

		
static int post_protocmd_set_configID_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	//

	return RTN_NORMAL;
}


static int post_protocmd_get_configID_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;	
}


static int post_protocmd_remote_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;

	
}



	



static int post_protocmd_get_ariber_ver_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	
	*res_len = Ariber_GetDeviceInfo(res_resp);		
	
	return RTN_NORMAL;
}


static int post_protocmd_set_baudrate_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 baudrate_rev = tok->arg[0];
	u16 baudrate;	

	switch(baudrate_rev)
	{
		case 3:
			baudrate = UART_BAUDRATE_B4800;
			break;		

		case 4:
			baudrate = UART_BAUDRATE_B9600;
			break;

		case 5:
			baudrate = UART_BAUDRATE_B19200;	
			break;	

		case 7:		
			baudrate = UART_BAUDRATE_B57600;	
			break;	

		default:
			return RTN_UNKNOW_CMD;
			break;
	}

	*res_len = 0;	

	return RTN_NORMAL;
}




static int post_protocmd_authority_confirm_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{	
	CMD_0x48_HANDLE_T *cmd = CMD_0x48;	
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF0)
	{
		return RTN_UNKNOW_CMD;
	}

	switch(cmd_type)
	{
		case CMD_0x48_GET_AUTHOR:	

			LOG_INFO(" CMD:0x48 Type:Set Date\n");	
			
			Ariber_GetAuthority(tok,res_resp,res_len,cmd);
			
			break;	

		case CMD_0x48_DEL_AUTHOR:

			LOG_INFO(" CMD:0x48 Type:Delete Author\n");

			Ariber_CancelConfirm(cmd);		
		
			break;

		case CMD_0x48_CHG_PASSWD:

			LOG_INFO(" CMD:0x48 Type:Change Password\n");

			Ariber_ModifyPassword(tok,res_resp,res_len,cmd);			
		
			break;

		default:	
	
			break;
		
	}
	return RTN_NORMAL;	
}
	



static int post_protocmd_set_sys_param_proc(struct tls_protocmd_token_t *tok,
        												char *res_resp, 
        												u32 *res_len)
{
	CMD_0x49_HANDLE_T *cmd = &CMD_0x49;
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF1)
	{	
		return RTN_UNKNOW_CMD;
	}
	
	switch(cmd_type)
	{	
		case CMD_0x49_CORRECT_TIME:

			LOG_INFO("CMD_0x49_SET_WORK_DAY_IN\n");	

			*res_len = Ariber_SetSysTime(tok,res_resp,cmd);			
			
			break;	

		case CMD_0x49_SET_WORK_DAY_IN:
		
			LOG_INFO("CMD_0x49_SET_WORK_DAY_IN\n");					
			
			break;

		case CMD_0x49_SET_REST_DAY_IN:
	
			LOG_INFO("CMD_0x49_SET_REST_DAY_IN\n");			
			
			break;	
	
		case CMD_0x49_SET_WEEK_AUTH_IN:
	
			LOG_INFO("CMD_0x49_SET_WEEK_AUTH_IN\n");		
			
			break;	

		case CMD_0x49_USER_AUTHORIZE:
	
			LOG_INFO("CMD_0x49_USER_AUTHORIZE\n");		
			
			break;

		case CMD_0x49_USER_UNAUTHORIZE:		
	
			LOG_INFO("CMD_0x49_USER_UNAUTHORIZE\n");		
			
			break;
			
		case CMD_0x49_REST_DAY_IN_WEEK:	
			
			LOG_INFO("CMD_0x49_REST_DAY_IN_WEEK\n");		
				
			break;

		case CMD_0x49_ADD_HOLIDAY:	
			
			LOG_INFO("CMD_0x49_ADD_HOLIDAY\n");		
				
			break;

		case CMD_0x49_DEL_HOLIDAY:	
			
			LOG_INFO("CMD_0x49_DEL_HOLIDAY\n");		
				
			break;

		case CMD_0x49_OPEN_DOOR:	
			
			LOG_INFO("CMD_0x49_OPEN_DOOR\n");		
				
			break;

		case CMD_0x49_RECORD_MEM_ADDR:		
			
			LOG_INFO("CMD_0x49_RECORD_MEM_ADDR\n");		
				
			break;

		case CMD_0x49_SET_PARAM:		
			
			LOG_INFO("CMD_0x49_SET_PARAM\n");		
				
			break;


		default:

			break;
	}
	return RTN_NORMAL;
}



static int post_protocmd_get_sys_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	CMD_0x4A_HANDLE_T *cmd = &CMD_0x4A;
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];
	
	if(cmd_group != 0xF2)
	{	
		LOG_INFO("cmd_group error\n");		
		return RTN_UNKNOW_CMD;	
	}
		
	switch(cmd_type)
	{	
		case CMD_0x4A_GET_WORK_DAY_IN:		

			LOG_INFO("CMD_0x4A_GET_WORK_DAY_IN\n");				
			
			*res_len = Ariber_GetWorkDayPermitList(tok,res_resp,cmd);	
			
			break;

		case CMD_0x4A_GET_REST_DAY_IN:			
			
			LOG_INFO("CMD_0x4A_GET_REST_DAY_IN\n");	
			
			*res_len = Ariber_GetRestDayPermitList(tok,res_resp,cmd);	
			
			break;

		case CMD_0x4A_GET_WEEK_IN:				
			
			LOG_INFO("CMD_0x4A_GET_WEEK_IN\n");		
			
			*res_len = Ariber_GetWeekPermitList(tok,res_resp,cmd);		
			
			break;

		default:	
		
			break;
	}
	
	return RTN_NORMAL;
}




static int post_protocmd_set_guard_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	CMD_0x4B_HANDLE_T *cmd = &CMD_0x4B;	
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];			

	if(cmd_group != 0xF3)				
	{						
		return RTN_UNKNOW_CMD;
	}

	switch(cmd_type)
	{
		case CMD_0x4B_SET_HANDLE_POS:		

			LOG_INFO("CMD_0x4B_SET_VOICE_MARK\n");		

			*res_len = Ariber_SetHandlePos(tok,res_resp,cmd);

			break;

		case CMD_0x4B_SET_CARD_BIT:			

			LOG_INFO("CMD_0x4B_SET_CARD_BIT\n");		

			*res_len = Ariber_SetCardBit(tok,res_resp,cmd);	
				
			break;

		case CMD_0x4B_SET_ARMY_PARAM:		

			LOG_INFO("CMD_0x4B_SET_ARMY_PARAM\n");	
			
			*res_len = Ariber_SetArmyParam(tok,res_resp,cmd);	
		
			break;
			
		case CMD_0x4B_SET_ALARM_PARAM:		
			
			LOG_INFO("CMD_0x4B_SET_ALARM_PARAM\n");	

			*res_len = Ariber_SetAlarmParam(tok,res_resp,cmd);	
				
			break;

		case CMD_0x4B_SET_ARMYING_STA:		
				
			LOG_INFO("CMD_0x4B_SET_ARMYING_STA\n");	
			
			*res_len = Ariber_SetArmyingSta(tok,res_resp,cmd);	
			
			break;

		case CMD_0x4B_SET_SYS_STA:			
			
			LOG_INFO("CMD_0x4B_SET_SYS_STA\n");	

			*res_len = Ariber_SetSystemSta(tok,res_resp,cmd);	
				
			break;

		case CMD_0x4B_SET_SWITCH_OUT:		
			
			LOG_INFO("CMD_0x4B_SET_SWITCH_OUT\n");	

			*res_len = Ariber_SetSwitchOut(tok,res_resp,cmd);	
				
			break;

		case CMD_0x4B_SET_VOICE_MARK:		
			
			LOG_INFO("CMD_0x4B_SET_VOICE_MARK\n");	
					
			*res_len = Ariber_SetVoiceMark(tok,res_resp,cmd);
			
			break;		
					
		case CMD_0x4B_SET_IO_SWITCH:			
				
			LOG_INFO("CMD_0x4B_SET_IO_SWITCH\n");	

			*res_len = Ariber_SetIOSwitchParam(tok,res_resp,cmd);	
				
			break;

		default:
			
			*res_len = 0;
			
			*res_resp = 0;

			return RTN_UNKNOW_CMD;		
				
			break;	
	}

	
	return RTN_NORMAL;
}




static int post_protocmd_get_guard_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	CMD_0x4C_HANDLE_T *cmd = &CMD_0x4C;	
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];
	
	if(cmd_group != 0xF4)
	{	
		return RTN_UNKNOW_CMD;
	}	

	switch(cmd_type)
	{
		case CMD_0x4C_GET_HANDLE_POS:			

			LOG_INFO("CMD_0x4C_GET_HANDLE_POS\n");

			*res_len = Ariber_GetHandlePos(tok,res_resp,cmd);		

			break;

		case CMD_0x4C_GET_CARD_BIT:			

			LOG_INFO("CMD_0x4C_GET_CARD_BIT\n");

			*res_len = Ariber_GetCardBit(tok,res_resp,cmd);	

			break;

		case CMD_0x4C_GET_ARMY_PARAM:				

			LOG_INFO("CMD_0x4C_GET_ARMY_OFF_STA\n");

			*res_len = Ariber_GetArmyParam(tok,res_resp,cmd);
	
			break;

		case CMD_0x4C_GET_ALARM_PARAM:			

			LOG_INFO("CMD_0x4C_GET_ALARM_PARAM\n");

			*res_len = Ariber_GetAlarmParam(tok,res_resp,cmd);	

			break;


		case CMD_0x4C_GET_ARMY_ON_STA:			

			LOG_INFO("CMD_0x4C_GET_ARMY_ON_STA\n");

			*res_len = Ariber_GetArmyingSta(tok,res_resp,cmd);	

			break;

		case CMD_0x4C_GET_DIY_PARAM:			

			LOG_INFO("CMD_0x4C_GET_DIY_PARAM\n");

			*res_len = Ariber_GetUserDefinedData(tok,res_resp,cmd);	

			break;

		case CMD_0x4C_GET_VOICE_STA:			

			LOG_INFO("CMD_0x4C_GET_VOICE_STA\n");

			*res_len = Ariber_GetVoiceMaskSta(tok,res_resp,cmd);	

			break;

		case CMD_0x4C_GET_SWITCH_PARAM:			

			LOG_INFO("CMD_0x4C_GET_SWITCH_PARAM\n");

			*res_len = Ariber_GetSwitchConfig(tok,res_resp,cmd);

			break;

		case CMD_0x4C_GET_ARMY_CONFIG:					

			LOG_INFO("CMD_0x4C_ARMY_PARAM\n");

			*res_len = Ariber_GetArmyConfigParam(tok,res_resp,cmd);	

			break;

		
		case CMD_0x4C_GET_USER_PWD:			
		
			LOG_INFO("CMD_0x4C_GET_USER_PWD\n");
			
			*res_len = Ariber_GetUserPassword(tok,res_resp,cmd);		
				
			break;

		
		case CMD_0x4C_GET_FLASH_ID:			
		
			LOG_INFO("CMD_0x4C_GET_FLASH_ID\n");
			
			*res_len = Ariber_GetFlashID(tok,res_resp,cmd);			
			
			break;

		case CMD_0x4C_GET_DOOR_LOG:			
		
			LOG_INFO("CMD_0x4C_GET_DOOR_LOG\n");		
				
			*res_len = Ariber_GetDoorLog(tok,res_resp,cmd);				
		
			break;
					
		default:

			break;
	}
	return RTN_NORMAL;
}

#endif





//创立部分协议
#if Makepower_Protocol_Open

static int mp_protocmd_get_device_info_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_set_addr_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_get_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_set_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_mem_dev_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
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
	u8 i=0,j=0;			
	
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
	
	LOG_INFO("Cmd = %x\n",tok->CID2); //执行的命令
	
	LOG_INFO("argv_cnt = %d\n",tok->arg_found);//参数个数
	
	LOG_INFO("ARGV = ");//参数数值

#if DEBUG_ON
			
	for(j=0;j<tok->arg_found;j++)				
	{								
		printf("%x ",tok->arg[j]);						
	}	
	printf("\n");

#endif 
			
		
	return 0;		
}
	
	
int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp,u32 *res_len)
{			
	int err = 0;	
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
		err = match->proc_func(tok, res_rsp, res_len);
	} 
	else 
	{				
		err = RTN_CID2_ERR;
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

void fill_buff(u8 **buff,u8 data,u8 tear_flag)		
{
	/*
	函数传递参数时,会做一份拷贝,只保留了传入参数的值,
	也就说容器变了,但是内容没有变。
	指针地址的变化就是因为这个“容器”变了，
	但是指针所指向的那块的地址是没变的。
	函数在调用完就会销毁那份拷贝。
	*/
	
	if(tear_flag)
	{		
		**buff = Hi_HexToAsc(data);	
		(*buff)++;
		**buff = Low_HexToAsc(data);		
		(*buff)++;	
	}
	else
	{	
		**buff = data;
		(*buff)++;	
	}	
}






static void tls_protocol_add_head(tls_respon_head *resp_h,u8 *buff,u32 *res_len)	
{	
	u8 *p = buff;//指向数据头部
	u8 Head =  resp_h->SOI;					
	u8 Ver  =  resp_h->VER;
	u8 Adrr =  resp_h->ADR; 	
	u8 CID1 =  resp_h->CID1; 	
	
	//RTN和Length根据前面的收到的消息做具体的回复
	u8 RTN = resp_h->RTN;					
	u16 LEN = resp_h->LENGTH;			
	
	u8 Len_H  = (u8)(LEN>>8);				
	u8 Len_L  = (u8)(LEN);			
		
	u16 Host_Adr = Adrr + (CID1&0x0F)*0x100;	
		
	LOG_INFO("RTN %x\n",RTN);					
			
	//添加头
	fill_buff(&p,Head,0);				
	fill_buff(&p,Ver,1);						
	fill_buff(&p,Adrr,1);	
	fill_buff(&p,CID1,1);
	fill_buff(&p,RTN,1);						
	//长度
	fill_buff(&p,Len_H,1);				
	fill_buff(&p,Len_L,1);							
				
	*res_len = *res_len + PROTOCOL_HEAD_LEN;	
}



static void tls_protocol_add_body(void)
{
	
}


static void tls_protocol_add_tail(tls_respon_head *resp_h,u8 *buff,u32 *res_len)
{	
	u8 *p = buff;	
	u16 i=0,chk_sum=0,data_cnt=0;
	u8 tail = PROTOCOL_TAIL;			
	u8 tempSum[2]={0};		
	
	p++;//去掉头		
	
	data_cnt = resp_h->BODY_LEN + (PROTOCOL_HEAD_LEN - 1);//有效信息的长度				
		
	LOG_INFO("data dump\n");			

	for(i=0;i<data_cnt;i++) 				
	{				
		LOG_INFO("No.%d data:%x,chksum:%x\n",i,*p,chk_sum);							
		chk_sum += *p++;		
	}			
	LOG_INFO("No.%d data:%x,chksum:%x\n",i,*p,chk_sum);	
	
	chk_sum = ~chk_sum+1;			

	LOG_INFO("tail chksum= %x\n",chk_sum);				
	
	tempSum[0] = (u8)(chk_sum>>8);		
	tempSum[1] = (u8)(chk_sum);					

	fill_buff(&p,tempSum[0],1);
	fill_buff(&p,tempSum[1],1);	
	fill_buff(&p,tail,0);	

	*res_len = *res_len + PROTOCOL_TAIL_LEN;		
}

u16 tls_token_resp_len(u32 *res_len)
{		
	u16 len = *res_len,ret = 0;	//得到回复命令长度
	u8 chk_sum = 0;
	u8 chk_tmp[3]={0};
	
	if(len == 0)	
	{	
		return 0x0000;		
	}		
	else
	{			
		LOG_INFO("token resp len=%d\n",len);	
		
		chk_tmp[0] = (len>>8)&0x0F;  //len的倒数第三四位
		chk_tmp[1] = (len>>4)&0x0F;  //len的倒数第二四位
		chk_tmp[2] = len&0x0F;//len的最低四位		

		chk_sum = (chk_tmp[0]+chk_tmp[1]+chk_tmp[2])%16;
			
		LOG_INFO("chk_sum phrase1 = %x\n",chk_sum);		
		
		chk_sum = ~chk_sum;								

		LOG_INFO("chk_sum phrase2 = %x\n",chk_sum);				

		chk_sum = (chk_sum&0x0F) + 1;				

		LOG_INFO("chk_sum phrase3 = %x\n",chk_sum);		

		ret = ret + (u16)chk_sum;	
	
		LOG_INFO("ret phrase4 = %x\n",ret);					
	
		ret = (ret<<12) + len;									
		
		LOG_INFO("ret phrase5 = %x\n",ret);			

		return ret;	
	}
}

//这个函数中处理RTN和LENGTH这些对应不同情况下的数值
static void tls_token_reunion(tls_respon_head *respon_h,
									struct tls_protocmd_token_t *tok,
									u8 err,u32 *res_len)			
{			
	u8  RTN  = err;								
	u16 Length  = tls_token_resp_len(res_len);//根据命令发送不同长度的回复
		
	LOG_INFO("The data len = %x\n",Length);				
	
	respon_h->SOI 			= 	PROTOCOL_HEAD;		
	respon_h->VER			= 	tok->VER;
	respon_h->ADR 			= 	tok->ADDR;
	respon_h->CID1 			=	tok->CID1;
	respon_h->RTN			=	RTN;		
	respon_h->LENGTH		=	Length;			
	respon_h->HEAD_LEN		=	PROTOCOL_HEAD_LEN;	//发送的ASCII数
	respon_h->BODY_LEN		= 	(Length&0x0FFF);	//发送的ASCII数
	respon_h->TAIL_LEN		=	PROTOCOL_TAIL_LEN;	//发送的ASCII数
}



int tls_protocol_rebuild(struct tls_protocmd_token_t *tok,
							 u8  *buff,
							 u32 *res_len,
							 u8 err)			
{	
	tls_respon_head respon_head;	

	tls_token_reunion(&respon_head,tok,err,res_len);							

	tls_protocol_add_head(&respon_head,buff,res_len);	
		
	tls_protocol_add_tail(&respon_head,buff,res_len);
	
}






//创立协议
void makepower_protocol_deal(void)
{
		
}


//邮电协议
void post_protocol_deal(void)
{
	
}


	
void tls_protocol_init(void)	
{
	CMD_0x48.param_0xE0.Handle_Pos[0] = 0;
	CMD_0x48.param_0xE0.Handle_Pos[1] = 0;
		
}











