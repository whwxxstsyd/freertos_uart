#ifndef __PROTOCOLBASE_H
#define __PROTOCOLBASE_H	
#include "sys.h"	
#include "mp_protocol_hostif.h"

//定义数据返回的长度(单位:字节)
#define NO_RESP_DATA	0x00

#define GET_SYS_TIME_RTN_LEN	8
#define SET_SYS_TIME_RTN_LEN	0	


int Ariber_PasswordConfirm(struct tls_protocmd_token_t *tok,
        							u8 *res_resp, u32 *res_len);

int Ariber_CancelConfirm(void);	

int Ariber_ModifyPassword(struct tls_protocmd_token_t *tok,
        							u8 *res_resp, u32 *res_len);	

int Ariber_GetSysTime(u8 *res_resp);						

int Ariber_SetSysTime(struct tls_protocmd_token_t *tok,u8 *res_resp);

int Ariber_SetVoiceMark(void);

int Ariber_PlayVoice(void);

int Ariber_GetVoiceMark(void);

int Ariber_GetFlashID(void);	




#endif

