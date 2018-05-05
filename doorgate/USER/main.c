#include "FreeRTOS.h"
#include "task.h"

/*add by hetianqi*/ 	
#include "app.h"	
#include "platform.h"	
#include "delay.h"	


int main(void)
{	
	//Initialize the platform.	
	platformInit();			
			
	app_init();		
	
	vTaskStartScheduler();
}	
