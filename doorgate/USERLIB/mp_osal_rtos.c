#include "mp_osal_rtos.h"	
#include "usart.h"	
#include "debug.h"	
	
#if 0
tls_os_status_t tls_os_mutex_create(u8 prio, tls_os_mutex_t mutex);
tls_os_status_t tls_os_mutex_delete(tls_os_mutex_t mutex);
tls_os_status_t tls_os_mutex_acquire(tls_os_mutex_t mutex,u32 wait_time);
tls_os_status_t tls_os_mutex_release(tls_os_mutex_t mutex,u8 isrcount);


	
tls_os_status_t tls_os_sem_create(tls_os_sem_t tls_os_sem,u32 cnt);
tls_os_status_t tls_os_sem_delete(tls_os_sem_t tls_os_sem);	
tls_os_status_t tls_os_sem_acquire(tls_os_sem_t tls_os_sem,u32 wait_time);
tls_os_status_t tls_os_sem_release(tls_os_sem_t tls_os_sem,u8 isrcount);



tls_os_status_t tls_os_queue_create(tls_os_queue_t tls_os_queue,u32 queue_size);			
tls_os_status_t tls_os_queue_delete(tls_os_queue_t tls_os_queue);
tls_os_status_t tls_os_queue_send(tls_os_queue_t tls_os_queue,void *msg,	u8 isrcount);
tls_os_status_t tls_os_queue_receive(tls_os_queue_t tls_os_queue,void **msg,u32 wait_time,u8 isrcount);



tls_os_status_t tls_os_mailbox_create(tls_os_mailbox_t tls_os_mailbox);		
tls_os_status_t tls_os_mailbox_delete(tls_os_mailbox_t tls_os_mailbox);
tls_os_status_t tls_os_mailbox_send(tls_os_mailbox_t tls_os_mailbox,void *msg,u8 isrcount);
tls_os_status_t tls_os_mailbox_receive(tls_os_mailbox_t tls_os_mailbox,void **msg,u32 wait_time,u8 isrcount);



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


tls_os_status_t tls_os_mutex_release(tls_os_mutex_t mutex)
{
	u8 error;
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	isrcount = tls_get_usrt1_isr_count();
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

tls_os_status_t tls_os_sem_create(tls_os_sem_t *tls_os_sem,u32 cnt)
{
    tls_os_status_t os_status;		
	
	*tls_os_sem = xSemaphoreCreateCounting( configSEMAPHORE_INIT_VALUE, cnt);
	
    if (*tls_os_sem != NULL)
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


tls_os_status_t tls_os_sem_release(tls_os_sem_t tls_os_sem)
{
	u8 error;	
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	u8 isrcount = 0;

	isrcount = tls_get_usrt1_isr_count();
	if(isrcount > 0)	
	{
		error = xSemaphoreGiveFromISR(tls_os_sem, &pxHigherPriorityTaskWoken );
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







tls_os_status_t tls_os_queue_create(tls_os_queue_t *tls_os_queue,u32 queue_size)			
{	
    tls_os_status_t os_status;
	
	*tls_os_queue = xQueueCreate(queue_size, sizeof(void *));
	
    if (*tls_os_queue != NULL)
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




tls_os_status_t tls_os_queue_send(tls_os_queue_t tls_os_queue,void *msg)
{ 
	u8 error;	
	tls_os_status_t os_status;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	isrcount = tls_get_usrt1_isr_count();
	if(isrcount > 0)
	{
		error = xQueueSendFromISR( tls_os_queue, &msg, &pxHigherPriorityTaskWoken );	
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


	
tls_os_status_t tls_os_queue_receive(tls_os_queue_t tls_os_queue,void *msg,u32 wait_time)
{	
	u8 error;	
	tls_os_status_t os_status;	
	unsigned int xTicksToWait;	
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	if(0 == wait_time)				
		xTicksToWait = portMAX_DELAY;
	else
		xTicksToWait = wait_time;

	isrcount = tls_get_usrt1_isr_count();	
	if(isrcount > 0)
	{
		error = xQueueReceiveFromISR(tls_os_queue, msg, &pxHigherPriorityTaskWoken);
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












	
tls_os_status_t tls_os_mailbox_create(tls_os_mailbox_t *tls_os_mailbox)			
{	
    tls_os_status_t os_status;
		 		
	*tls_os_mailbox = xSemaphoreCreateBinary();	
	
    if (*tls_os_mailbox != NULL)
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;

    return os_status; 
}


tls_os_status_t tls_os_mailbox_delete(tls_os_mailbox_t tls_os_mailbox)
{
	vSemaphoreDelete(tls_os_mailbox);	

    return TLS_OS_SUCCESS; 	
}


tls_os_status_t tls_os_mailbox_send(tls_os_mailbox_t tls_os_mailbox,void *msg)	
{		
	u8 error;
	tls_os_status_t os_status;		
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;
		
	isrcount = tls_get_usrt1_isr_count();
	
	if(isrcount > 0)
	{	
		error = xSemaphoreGiveFromISR(tls_os_mailbox, &pxHigherPriorityTaskWoken);
	}		
	else
	{			
		error = xSemaphoreGive(tls_os_mailbox);		
	}		
		
    if (error == pdPASS)
        os_status = TLS_OS_SUCCESS;
    else {
        os_status = TLS_OS_ERROR;
    }	

    return os_status; 
}


tls_os_status_t tls_os_mailbox_receive(tls_os_mailbox_t tls_os_mailbox,
											void *msg,
										    u32 wait_time)
{
	u8 error;
	tls_os_status_t os_status;
	unsigned int xTicksToWait;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	if(0 == wait_time)				
		xTicksToWait = portMAX_DELAY;
	else	
		xTicksToWait = wait_time;	
		
	isrcount = tls_get_usrt1_isr_count();											
		
	if(isrcount > 0)
	{		
		error = xSemaphoreTakeFromISR(tls_os_mailbox,&pxHigherPriorityTaskWoken);		
	}
	else		
	{														
		error = xSemaphoreTake(tls_os_mailbox, xTicksToWait);
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
	








/*
************************************************************************************************************************
*                                                   CREATE A TIMER
*
* Description: This function is called by your application code to create a timer.
*
* Arguments  : timer	A pointer to an OS_TMR data structure.This is the 'handle' that your application 
*						will use to reference the timer created.
*
*		        callback      Is a pointer to a callback function that will be called when the timer expires.  The
*                               callback function must be declared as follows:
*
*                               void MyCallback (OS_TMR *ptmr, void *p_arg);
*
* 	             callback_arg  Is an argument (a pointer) that is passed to the callback function when it is called.
*
*          	   	 period        The 'period' being repeated for the timer.
*                               If you specified 'OS_TMR_OPT_PERIODIC' as an option, when the timer expires, it will
*                               automatically restart with the same period.
*
*			repeat	if repeat
*
*             	pname         Is a pointer to an ASCII string that is used to name the timer.  Names are useful for
*                               debugging.
*
*Returns    : TLS_OS_SUCCESS
*			TLS_OS_ERROR 
************************************************************************************************************************
*/


tls_os_status_t tls_os_timer_create(tls_os_timer_t *timer,	
										u32 period,
										bool repeat,
										const u8* Name,
										TimerCallbackFunction_t CallbackFunction)
{
    tls_os_status_t os_status;

	if(0 == period)
		period = 1;
	
#if configUSE_TIMERS
	
	*timer  = xTimerCreate(Name , 
						period , 
						repeat, 	
						NULL, 
						CallbackFunction); 
#endif

    if (*timer  != NULL)	
        os_status = TLS_OS_SUCCESS;
    else 
        os_status = TLS_OS_ERROR;
    
	return os_status;
		
}

/*
************************************************************************************************************************
*                                                   START A TIMER
*
* Description: This function is called by your application code to start a timer.
*
* Arguments  : timer          Is a pointer to an OS_TMR
*
************************************************************************************************************************
*/
void tls_os_timer_start(tls_os_timer_t timer)
{			
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	//isrcount = tls_get_isr_count();
	
	if(isrcount > 0)
	{
#if configUSE_TIMERS
		xTimerStartFromISR( timer, &pxHigherPriorityTaskWoken );
#endif	
	}
	else
	{
#if configUSE_TIMERS
		xTimerStart( timer, 0 );		//no block time
#endif
	}
}
/*
************************************************************************************************************************
*                                                   CHANGE A TIMER WAIT TIME
*
* Description: This function is called by your application code to change a timer wait time.
*
* Arguments  : timer          Is a pointer to an OS_TMR
*
*			ticks			is the wait time
************************************************************************************************************************
*/
void tls_os_timer_change(tls_os_timer_t timer, u32 ticks)
{	
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	if(0 == ticks)
		ticks = 1;
	
	//isrcount = tls_get_isr_count();
	
	if(isrcount > 0)
	{
#if configUSE_TIMERS
		xTimerChangePeriodFromISR( timer, ticks, &pxHigherPriorityTaskWoken );
		xTimerStartFromISR( timer, &pxHigherPriorityTaskWoken );
#endif
	}
	else
	{	
#if configUSE_TIMERS	
		xTimerChangePeriod( timer, ticks, 0 );
		xTimerStart( timer, 0 );
#endif
	}
}
/*
************************************************************************************************************************
*                                                   STOP A TIMER
*
* Description: This function is called by your application code to stop a timer.
*
* Arguments  : timer          Is a pointer to the timer to stop.
*
************************************************************************************************************************
*/
void tls_os_timer_stop(tls_os_timer_t timer)
{	
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
	u8 isrcount = 0;

	//isrcount = tls_get_isr_count();
	
	if(isrcount > 0)
	{
#if configUSE_TIMERS
		xTimerStopFromISR( timer, &pxHigherPriorityTaskWoken );
#endif

	}
	else
	{
#if configUSE_TIMERS
		xTimerStop( timer, 0 );
#endif
	}
}


/*
************************************************************************************************************************
*                                                   Delete A TIMER
*
* Description: This function is called by your application code to delete a timer.
*
* Arguments  : timer          Is a pointer to the timer to delete.
*
************************************************************************************************************************
*/
int tls_os_timer_delete(tls_os_timer_t timer)
{
	int ret = 0;	
	/* xTimer is already active - delete it. */
	ret = xTimerDelete(timer, 10);
	return ret;
}




#endif

	


