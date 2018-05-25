#include "FreeRTOS.h"
#include "task.h"

/*add by hetianqi*/ 	
#include "app.h"	
#include "platform.h"	
#include "delay.h"	


int main(void)
{	
	//Initialize the platform.	
	Platform_Init();
	
	//∆ΩÃ®≤‚ ‘≥Ã–Ú
#if	TEST_OPEN	
	Platform_Test();		
#endif
		
	app_init();	
	
	vTaskStartScheduler();
}	
