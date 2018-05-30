#ifndef MP_CARD_TASK_H
#define MP_CARD_TASK_H		
#include "sys.h"	
#include "ProtocolBase.h"			


#define MBOX_MSG_CARD_DEV1       1	
#define MBOX_MSG_CARD_DEV2       2

		
enum	
{
	SysState_REALTIME 		= 0,	//Õý³£×´Ì¬	
	SysState_CONFIG 		= 1,	//ÅäÖÃ×´Ì¬
	SysState_READCARD		= 2,	//¶Á¿¨×´Ì¬
	SysState_DEBUG			= 3,	//¶Á¿¨×´Ì¬	
};


typedef struct tls_card_dev
{
	CMD_0x4B_HANDLE_T *card_cmd;	
	
	tls_os_mailbox_t        rev_mailbox;			
	s16 (*card_rev_callback)(char *buf, u16 len);	
	
}tls_card_dev_t;

void tls_card_dev_init(void);				


#endif

