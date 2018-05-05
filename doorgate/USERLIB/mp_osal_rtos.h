#ifndef MP_OSAL_RTOS_H
#define MP_OSAL_RTOS_H		

#include <stdio.h>
#include <stdlib.h>
#include "sys.h"		

#include "FreeRTOS.h"					
#include "task.h"			
#include "semphr.h"				
#include "FreeRTOSConfig.h"	

#include "temp.h"		
	

#define tls_os_sem_t 		QueueHandle_t			
#define tls_os_queue_t 	QueueHandle_t
#define tls_os_mailbox_t 	xSemaphoreHandle	
#define tls_os_mutex_t 	QueueHandle_t
	
		
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

#else


		
#if (1 == configUSE_MUTEXES)

tls_os_status_t tls_os_mutex_create(u8 prio, tls_os_mutex_t mutex)
{
    tls_os_status_t os_status;

	mutex = xSemaphoreCreateMutex();	

    if (mutex != NULL)
        os_status = TLS_OS_SUCCESS;
    else 	
        os_status = TLS_OS_ERROR;

    return os_status; 
}


tls_os_status_t tls_os_mutex_delete(tls_os_mutex_t mutex)
{
	vSemaphoreDelete(mutex);

    return TLS_OS_SUCCESS; 
}


tls_os_status_t tls_os_mutex_acquire(tls_os_mutex_t mutex,u32 wait_time)
{
    u8 error;
    tls_os_status_t os_status;
	unsigned int time;

	if(0 == wait_time)
		time = portMAX_DELAY;
	else
		time = wait_time;
		
	error = xSemaphoreTake( mutex, time);	
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
}


tls_os_status_t tls_os_mutex_release(tls_os_mutex_t mutex,u8 isrcount)
{
	u8 error;
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	if(isrcount > 0)
	{
		error = xSemaphoreGiveFromISR( mutex, &pxHigherPriorityTaskWoken );
		if((pdTRUE == pxHigherPriorityTaskWoken) && (1 == isrcount))
		{
			vTaskSwitchContext();
		}
	}
	else
	{
		error = xSemaphoreGive( mutex );
	}
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
}

#endif













#if (1 == configUSE_COUNTING_SEMAPHORES)

tls_os_status_t tls_os_sem_create(tls_os_sem_t tls_os_sem,u32 cnt)
{
    tls_os_status_t os_status;		
		
	tls_os_sem = xSemaphoreCreateCounting( configSEMAPHORE_INIT_VALUE, cnt);
	
    if (tls_os_sem != NULL)
        os_status = TLS_OS_SUCCESS;
    else 	
        os_status = TLS_OS_ERROR;

    return os_status; 
}

tls_os_status_t tls_os_sem_delete(tls_os_sem_t tls_os_sem)	
{
	vSemaphoreDelete(tls_os_sem);
	
	return TLS_OS_SUCCESS; 		
}

tls_os_status_t tls_os_sem_acquire(tls_os_sem_t tls_os_sem,u32 wait_time)
{
    u8 error;	
    tls_os_status_t os_status;
	unsigned int time;

	if(0 == wait_time)
		time = portMAX_DELAY;
	else	
		time = wait_time;
	
	error = xSemaphoreTake(tls_os_sem, time);
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;	

    return os_status;
}


tls_os_status_t tls_os_sem_release(tls_os_sem_t tls_os_sem,u8 isrcount)
{
	u8 error;	
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	
	if(isrcount > 0)
	{
		error = xSemaphoreGiveFromISR(tls_os_sem, &pxHigherPriorityTaskWoken );
		if((pdTRUE == pxHigherPriorityTaskWoken) && (1 == isrcount))
		{		
			vTaskSwitchContext();
		}	
	}
	else
	{	
		error = xSemaphoreGive(tls_os_sem);
	}
		
	if (error == pdPASS)
		os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
	
}



#endif







tls_os_status_t tls_os_queue_create(tls_os_queue_t tls_os_queue,u32 queue_size)			
{	
    tls_os_status_t os_status;
	
	tls_os_queue = xQueueCreate(queue_size, sizeof(void *));
	
    if (tls_os_queue != NULL)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
}


tls_os_status_t tls_os_queue_delete(tls_os_queue_t tls_os_queue)
{
	vQueueDelete(tls_os_queue);		
	
    return TLS_OS_SUCCESS; 
}




tls_os_status_t tls_os_queue_send(tls_os_queue_t tls_os_queue,void *msg,	u8 isrcount)
{ 
	u8 error;	
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	if(isrcount > 0)
	{
		error = xQueueSendFromISR( tls_os_queue, &msg, &pxHigherPriorityTaskWoken );	
		if((pdTRUE == pxHigherPriorityTaskWoken) && (1 == isrcount))
		{
			vTaskSwitchContext();	
		}
	}
	else
	{
		error = xQueueSend( tls_os_queue, &msg, 0 );
	}
	
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
}



tls_os_status_t tls_os_queue_receive(tls_os_queue_t tls_os_queue,void **msg,u32 wait_time,u8 isrcount)
{
	u8 error;	
	tls_os_status_t os_status;
	unsigned int xTicksToWait;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	if(0 == wait_time)				
		xTicksToWait = portMAX_DELAY;
	else
		xTicksToWait = wait_time;
	
	if(isrcount > 0)
	{
		error = xQueueReceiveFromISR(tls_os_queue, msg, &pxHigherPriorityTaskWoken);
		if((pdTRUE == pxHigherPriorityTaskWoken) && (1 == isrcount))
		{
			vTaskSwitchContext();
		}
	}
	else
	{	
		error = xQueueReceive( tls_os_queue, msg, xTicksToWait );
	}

    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status;
}












	
tls_os_status_t tls_os_mailbox_create(tls_os_mailbox_t tls_os_mailbox)			
{
    tls_os_status_t os_status;
	
	tls_os_mailbox = xQueueCreate(1, sizeof(void *));
	
    if (tls_os_mailbox != NULL)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
}


tls_os_status_t tls_os_mailbox_delete(tls_os_mailbox_t tls_os_mailbox)
{
	vQueueDelete(tls_os_mailbox);

    return TLS_OS_SUCCESS; 	
}


tls_os_status_t tls_os_mailbox_send(tls_os_mailbox_t tls_os_mailbox,
											void *msg,
											u8 isrcount)
{
	u8 error;
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	
	if(isrcount > 0)
	{
		error = xQueueSendFromISR( tls_os_mailbox, &msg, &pxHigherPriorityTaskWoken );
		if((pdTRUE == pxHigherPriorityTaskWoken) && (1 == isrcount))
		{
			vTaskSwitchContext();
		}
	}	
	else
	{	
		error = xQueueSend( tls_os_mailbox, &msg, 0 );
	}
	
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else {
        os_status = TLS_OS_ERROR;
    }

    return os_status; 
}


tls_os_status_t tls_os_mailbox_receive(tls_os_mailbox_t tls_os_mailbox,
											void **msg,
										    u32 wait_time,	
										    u8 isrcount)
{
	u8 error;
	tls_os_status_t os_status;
	unsigned int xTicksToWait;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	if(0 == wait_time)				
		xTicksToWait = portMAX_DELAY;
	else	
		xTicksToWait = wait_time;

	if(isrcount > 0)
	{
		error = xQueueReceiveFromISR(tls_os_mailbox, msg, &pxHigherPriorityTaskWoken);
		if((pdTRUE == pxHigherPriorityTaskWoken) && (1 == isrcount))
		{
			vTaskSwitchContext();
		}
	}
	else
	{		
		error = xQueueReceive( tls_os_mailbox, msg, xTicksToWait );
	}
	
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status;
}











u32 tls_os_get_time(void)
{	
    return xTaskGetTickCountFromISR();
}


u32 tls_os_set_critical(void)
{
	vPortEnterCritical();
    return 1;
}	

void tls_os_release_critical(u32 cpu_sr)
{
    vPortExitCritical();	
}
	


#endif


#endif


