#include "mp_card_task.h"	
#include "wiegand.h"
#include "card.h"		
#include "app.h"	

#include "FreeRTOS.h"
#include "task.h"	


tls_card_t card_st[2];		


static void tls_card_task(void *data);


void tls_card_dev_init(void)		
{		
	tls_card_t *card;		

	wiegand_reader_init();

	tls_card_dev_open(CARD_READ_DEV1);		
}



void tls_card_dev_open(u32 card_dev)		
{
	tls_card_t *card;

	if(card_dev == CARD_READ_DEV1)
	{	
		card = &card_st[0];
		
		xTaskCreate(tls_card_dev1_task, 	
					CARD_DEV1_TASK_NAME, 	
					CARD_DEV1_TASK_STACKSIZE, 
					card,					
					CARD_DEV1_TASK_PRIO, 	
					NULL);			
	}
	else if(card_dev == CARD_READ_DEV2)	
	{
		card = &card_st[1];	
		
		xTaskCreate(tls_card_dev2_task, 		
					CARD_DEV2_TASK_NAME, 	
					CARD_DEV2_TASK_STACKSIZE, 
					card,			
					CARD_DEV2_TASK_PRIO, 	
					NULL);	
	}
	
	tls_os_mailbox_create(&card->rev_mailbox);	
}



	
static s16 tls_card_dev1_rev_cb(char *buf, u16 len)
{	
	tls_card_t *card = &card_st[0];	
		
	if(NULL == buf)	
		return WM_FAILED;		
	
	if(card->rev_mailbox)									
		tls_os_mailbox_send(card->rev_mailbox, (void *));	
		
	return WM_SUCCESS;
}

static s16 tls_card_dev2_rev_cb(char *buf, u16 len)
{			
	tls_card_t *card = &card_st[1];		
		
	if(NULL == buf)		
		return WM_FAILED;		
	
	if(card->rev_mailbox)								
		tls_os_mailbox_send(card->rev_mailbox, (void *));	
		
	return WM_SUCCESS;
}




	
static void tls_card_dev1_task(void *data)
{
    tls_card_t *card = (tls_card_t *)data;
    u32 *msg = NULL;			
    int err;		
	
	while(1)
	{	
		err = tls_os_mailbox_receive(card->rev_mailbox, (void *)msg, 0);
		
        if (err == 0) 
		{		
			LOG_INFO("!!!!!!CARD1_RECV!!!!!!\n");

			
					 
        } 
	}
}	

static void tls_card_dev2_task(void *data)
{
    tls_card_t *card = (tls_card_t *)data;
    u32 *msg = NULL;			
    int err;		
	
	while(1)	
	{		
		err = tls_os_mailbox_receive(card->rev_mailbox, (void *)msg, 0);
		
        if (err == 0) 
		{			
			LOG_INFO("!!!!!!CARD2_RECV!!!!!!\n");				
					 
        } 
	}

}


