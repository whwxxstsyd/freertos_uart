#include <string.h>		
#include "mp_card_task.h"
#include "param.h"	
#include "debug.h"	
#include "wiegand.h"
#include "card.h"			
#include "app.h"	

#include "FreeRTOS.h"
#include "task.h"	


tls_card_dev_t card_st[2];	

extern CMD_0x48_HANDLE_T CMD_0x48;	
extern CMD_0x49_HANDLE_T CMD_0x49;	
extern CMD_0x4A_HANDLE_T CMD_0x4A;
extern CMD_0x4B_HANDLE_T CMD_0x4B;
extern CMD_0x4C_HANDLE_T CMD_0x4C;	

	
	
static void tls_card_dev2_task(void *data);
static void tls_card_dev1_task(void *data);
static s16 tls_card_dev1_rev_cb(char *buf, u16 len);
static s16 tls_card_dev2_rev_cb(char *buf, u16 len);	
static void tls_card_dev_open(u32 card_dev);		
static void tls_card_rev_register(u16 card_no, s16 (*card_rev_callback)(char *buf, u16 len));



void tls_card_dev_init(void)		
{		
	tls_card_dev_t *card;		

	wiegand_reader_init();

	tls_card_dev_open(CARD_READ_DEV1);		
}


static void tls_card_dev_open(u32 card_dev)		
{	
	tls_card_dev_t *card;

	if(card_dev == CARD_READ_DEV1)
	{		
		card = &card_st[0];	
		memset(card,0,sizeof(tls_card_dev_t));		
		
		tls_card_rev_register(card_dev,tls_card_dev1_rev_cb);
			
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
		memset(card,0,sizeof(tls_card_dev_t));				
	
		tls_card_rev_register(card_dev,tls_card_dev2_rev_cb);
			
		xTaskCreate(tls_card_dev2_task, 		
					CARD_DEV2_TASK_NAME, 	
					CARD_DEV2_TASK_STACKSIZE, 
					card,			
					CARD_DEV2_TASK_PRIO, 	
					NULL);	
	}
	
	tls_os_mailbox_create(&card->rev_mailbox);	
}


	
static void tls_card_rev_register(u16 card_no, s16 (*card_rev_callback)(char *buf, u16 len))
{		
	if(CARD_READ_DEV1 == card_no)		
		card_st[0].card_rev_callback = card_rev_callback;		
	else if(CARD_READ_DEV2 == card_no)		
		card_st[1].card_rev_callback = card_rev_callback;
}

	
static s16 tls_card_dev1_rev_cb(char *buf, u16 len)
{	
	tls_card_dev_t *card = &card_st[0];	
		
	if(NULL == buf)	
		return WM_FAILED;		
		
	if(card->rev_mailbox)									
		tls_os_mailbox_send(card->rev_mailbox, (void *)MBOX_MSG_CARD_DEV1);	
		
	return WM_SUCCESS;
}

static s16 tls_card_dev2_rev_cb(char *buf, u16 len)
{			
	tls_card_dev_t *card = &card_st[1];		
		
	if(NULL == buf)		
		return WM_FAILED;		
	
	if(card->rev_mailbox)								
		tls_os_mailbox_send(card->rev_mailbox, (void *)MBOX_MSG_CARD_DEV2);	
		
	return WM_SUCCESS;
}






//根据上位机的参数重新设定
void tls_card_rebuild(tls_card_dev_t *card)	
{
	//高低字节置换
	if(card->card_cmd->param_0xE1.Card_VaildBit == Wiegand_Sequence_Reversal)	
	{
					
	}		
	//正常读取
	else
	{
		
	}
}


//获取得卡号任务
static void tls_card_dev1_task(void *data)
{
    tls_card_dev_t *card = (tls_card_dev_t *)data;	
    u32 *msg = NULL;			
    int err;		
	
	while(1)
	{	
		err = tls_os_mailbox_receive(card->rev_mailbox, (void *)msg, 0);
		
        if (err == 0) 	
		{			
			LOG_INFO("!!!!!!CARD1_RECV!!!!!!\n");
			
			//将卡的信息根据设定的进行读取
			tls_card_rebuild(card);	
			
			//卡号上报上位机	
			if(CMD_0x4B.param_0xE5.System_Sta == SysState_READCARD)	
			{
				//主动上报
				
			}
			//刷卡开门
			else
			{
				
			}
        } 
	}
}	

static void tls_card_dev2_task(void *data)
{	
    tls_card_dev_t *card = (tls_card_dev_t *)data;
    u32 *msg = NULL;			
    int err;		
	
	while(1)	
	{		
		err = tls_os_mailbox_receive(card->rev_mailbox, (void *)msg, 0);
		
        if (err == 0) 
		{			
			LOG_INFO("!!!!!!CARD2_RECV!!!!!!\n");				
		
			//根据设定的读卡模式进行读卡	
					 
        } 
	}

}



