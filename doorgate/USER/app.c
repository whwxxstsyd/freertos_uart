#include "mp_uart_task.h"	
#include "mp_idle_task.h"
#include "mp_memory_task.h"	
#include "mp_door_task.h"	
#include "mp_card_task.h"			

#include "mp_protocol_hostif.h"	
#include "mp_card_task.h"	


#include "app.h"

void app_init(void)
{		
	//串口数据交互(	底层)		
	tls_uart_init();				

	//串口数据交互(	应用层)		
	tls_hostif_init();			

	//数据存储任务
	tls_memory_init();			

	//门禁卡识别任务
	tls_card_init();	

	//门锁控制任务
	tls_door_init();		
		
	//空闲任务
	tls_idle_init();	
	
}



