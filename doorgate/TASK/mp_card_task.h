#ifndef MP_CARD_TASK_H
#define MP_CARD_TASK_H		
#include "sys.h"	



typedef struct tls_card{

	tls_os_mailbox_t        rev_mailbox;			
	s16 (*card_rev_callback)(char *buf, u16 len);	
	
} tls_card_t;

void tls_card_init(void);			


#endif

