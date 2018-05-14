#ifndef __PROTOCOLBASE_H
#define __PROTOCOLBASE_H	
#include "sys.h"	
#include "mp_protocol_hostif.h"


int Ariber_PasswordConfirm(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len);

int Ariber_CancelConfirm(void);

int Ariber_ModifyPassword(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len);

int Ariber_GetSysTime(void);	

int Ariber_SetSysTime(void);

int Ariber_SetVoiceMark(void);

int Ariber_PlayVoice(void);

int Ariber_GetVoiceMark(void);

int Ariber_GetFlashID(void);	




#endif

