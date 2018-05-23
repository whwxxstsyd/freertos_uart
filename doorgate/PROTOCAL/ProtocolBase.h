#ifndef __PROTOCOLBASE_H
#define __PROTOCOLBASE_H	
#include "sys.h"	
#include "mp_protocol_hostif.h"

//定义数据返回的长度(单位:字节)
#define NO_RESP_DATA	0x00
#define SET_SUCCEED_RTN_LEN   0	

#define GET_FLASH_ID_RTN_LEN  1	


#define GET_SYS_TIME_RTN_LEN	8
#define SET_SYS_TIME_RTN_LEN	0	

 
#define GET_LIST_RTN_LEN		16	


//布防和撤防
typedef enum HANDLE_POS
{
	HANDLE_RIGHT = 2,
	HANDLE_LEFT = 1,	
}HANDLE_POS_T; 	


//布防和撤防
typedef enum ARMY_TYPE
{
	DEVICE_ARMING = 1,
	DEVICE_UN_ARMING = 0,
	DEVICE_AUTO_ARMING = 0,
	DEVICE_MAN_ARMING = 1,	
}ARMY_TYPE_T; 	


//自动布防和手动布防
typedef enum ALARM_TYPE
{
	ALARM_MASK_ON = 1,
	ALARM_MASK_OFF = 0,	
	ALARM_REVOKE_BY_KEY = 1,	
	ALARM_REVOKE_NO_KEY = 0,	
}ALARM_TYPE_T; 	


int Ariber_PasswordConfirm(struct tls_protocmd_token_t *tok,
        							u8 *res_resp, u32 *res_len);

int Ariber_CancelConfirm(void);	

int Ariber_ModifyPassword(struct tls_protocmd_token_t *tok,
        							u8 *res_resp, u32 *res_len);	

int Ariber_GetSysTime(u8 *res_resp);						

int Ariber_SetSysTime(struct tls_protocmd_token_t *tok,u8 *res_resp);



int Ariber_PlayVoice(void);

int Ariber_GetVoiceMark(void);

int Ariber_GetFlashID(u8 *res_resp);



int Ariber_GetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);

int Ariber_GetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);

int Ariber_GetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);



	

int Ariber_SetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);

int Ariber_SetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);
 
int Ariber_SetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);
 



//4B命令下面的指令
int Ariber_SetHandlePos(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetCardBit(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetArmyParam(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetAlarmParam(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetArmyingSta(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetSystemSta(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetSwitchOut(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetVoiceMark(struct tls_protocmd_token_t *tok,u8 *res_resp);	

int Ariber_SetIOSwitchParam(struct tls_protocmd_token_t *tok,u8 *res_resp);	



#endif

