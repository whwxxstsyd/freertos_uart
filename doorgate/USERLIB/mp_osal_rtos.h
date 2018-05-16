#ifndef MP_OSAL_RTOS_H
#define MP_OSAL_RTOS_H		

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	
#include "sys.h"		

#include "FreeRTOS.h"					
#include "task.h"			
#include "semphr.h"				
#include "timers.h"	
#include "FreeRTOSConfig.h"	
#include "temp.h"		
	

#define HZ        (configTICK_RATE_HZ)


#define tls_os_sem_t 		QueueHandle_t			
#define tls_os_queue_t 	QueueHandle_t
#define tls_os_mailbox_t 	xSemaphoreHandle	
#define tls_os_mutex_t 	QueueHandle_t
#define	tls_os_timer_t     xTimerHandle
			
#if 1

tls_os_status_t tls_os_mutex_create(u8 prio, tls_os_mutex_t mutex);
tls_os_status_t tls_os_mutex_delete(tls_os_mutex_t mutex);
tls_os_status_t tls_os_mutex_acquire(tls_os_mutex_t mutex,u32 wait_time);
tls_os_status_t tls_os_mutex_release(tls_os_mutex_t mutex);


	
tls_os_status_t tls_os_sem_create(tls_os_sem_t *tls_os_sem,u32 cnt);
tls_os_status_t tls_os_sem_delete(tls_os_sem_t tls_os_sem);	
tls_os_status_t tls_os_sem_acquire(tls_os_sem_t tls_os_sem,u32 wait_time);
tls_os_status_t tls_os_sem_release(tls_os_sem_t tls_os_sem);

	
tls_os_status_t tls_os_queue_create(tls_os_queue_t *tls_os_queue,u32 queue_size);			
tls_os_status_t tls_os_queue_delete(tls_os_queue_t tls_os_queue);
tls_os_status_t tls_os_queue_send(tls_os_queue_t tls_os_queue,void *msg);
tls_os_status_t tls_os_queue_receive(tls_os_queue_t tls_os_queue,void *msg,u32 wait_time);


		
tls_os_status_t tls_os_mailbox_create(tls_os_mailbox_t *tls_os_mailbox);		
tls_os_status_t tls_os_mailbox_delete(tls_os_mailbox_t tls_os_mailbox);
tls_os_status_t tls_os_mailbox_send(tls_os_mailbox_t tls_os_mailbox,void *msg);
tls_os_status_t tls_os_mailbox_receive(tls_os_mailbox_t tls_os_mailbox,void *msg,u32 wait_time);
	


u32 tls_os_get_time(void);	
u32 tls_os_set_critical(void);
void tls_os_release_critical(u32 cpu_sr);



		
tls_os_status_t tls_os_timer_create(tls_os_timer_t *timer,	
									u32 period,	
									bool repeat,
									const u8* Name,
									TimerCallbackFunction_t CallbackFunction);


void tls_os_timer_start(tls_os_timer_t timer);	


void tls_os_timer_change(tls_os_timer_t timer, u32 ticks);


void tls_os_timer_stop(tls_os_timer_t timer);


int tls_os_timer_delete(tls_os_timer_t timer);





#endif



#endif


