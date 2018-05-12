#include "mp_card_task.h"	
#include "wiegand.h"
#include "card.h"		
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"	


static void tls_card_task(void *data);


void tls_card_init(void)		
{
		
	wiegand_reader_init();

	
	xTaskCreate(tls_card_task, 	
				CARD_TASK_NAME, 	
				CARD_TASK_STACKSIZE, 
				NULL,		
				CARD_TASK_PRIO, 	
				NULL);			
}




	
static void tls_card_task(void *data)
{
	u8 i; 	
	while(1)
	{			
		for(i=0;i<CARD_DEVICE_NUM;i++)	
		{
				
		}
	}
}

