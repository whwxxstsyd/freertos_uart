#include "mp_uart_task.h"	
#include "mp_idle_task.h"
#include "mp_cmdp_hostif.h"

#include "app.h"

void app_init(void)
{		
	tls_uart_init();	

	tls_hostif_init();		

	//¿ÕÏĞÈÎÎñ
	tls_idle_init();
}

