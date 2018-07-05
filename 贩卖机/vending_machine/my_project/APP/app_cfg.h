/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/
#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_ENABLED


#define  BSP_CFG_LED_SPI2_EN                        DEF_ENABLED       /* Enable/disable LEDs on SPI port.                     */
#define  BSP_CFG_LED_PIOC_EN                        DEF_ENABLED       /* Enable/disable PIOC LEDs.                            */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/

#define  APP_TASK_START_NAME                          "AppTaskStart"
#define  APP_TASK_GUI_NAME                            "AppTaskGUI"
#define  APP_TASK_USER_IF_NAME                        "AppTaskUserIF"
#define  APP_TASK_COM_NAME                            "AppTaskCom"
#define  APP_TASK_UPDATE_NAME                         "AppTaskGUIUpdate"


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/
/* 启动任务 */
#define  APP_TASK_START_PRIO                               1
/* 触摸和JOYSTICK任务 */
#define  APP_TASK_USER_IF_PRIO                             4
/* LED任务 */
#define  APP_TASK_COM_PRIO								   3

/* GUI图片更新任务 */
#define  APP_TASK_UPDATE_PRIO							   2

/* GUI显示任务 */
#define  APP_TASK_GUI_PRIO               (OS_LOWEST_PRIO - 3)
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                         1024
#define  APP_TASK_GUI_STK_SIZE                           1024
#define  APP_TASK_USER_IF_STK_SIZE                       1024
#define  APP_TASK_COM_STK_SIZE							 1024
#define  APP_TASK_UPDATE_STK_SIZE						 1024

#define  OS_CPU_EXCEPT_STK_SIZE    1024      



/*
*********************************************************************************************************
*                                    BSP CONFIGURATION: RS-232
*********************************************************************************************************
*/

#define  BSP_CFG_SER_COMM_SEL             			BSP_SER_COMM_UART_02
#define  BSP_CFG_TS_TMR_SEL                    		2


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#if 0
#define  TRACE_LEVEL_OFF                       		0
#define  TRACE_LEVEL_INFO                      		1
#define  TRACE_LEVEL_DEBUG                     		2
#endif

#define  APP_TRACE_LEVEL                			TRACE_LEVEL_INFO
#define  APP_TRACE                      			BSP_Ser_Printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)



#endif

