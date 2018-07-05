#include "delay.h"  	
#include "includes.h"
#include "bsp.h"	


/*
**************************************************************************
*                              任务堆栈
**************************************************************************
*/

/* 定义每个任务的堆栈空间，app_cfg.h文件中宏定义栈大小 */
static OS_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
static OS_STK AppTaskGUIStk[APP_TASK_GUI_STK_SIZE];	
static OS_STK AppTaskCOMStk[APP_TASK_COM_STK_SIZE];	
static OS_STK AppTaskUpdateStk[APP_TASK_UPDATE_STK_SIZE];	




int main(void)
{ 			
	INT8U  err;

	/* 初始化"uC/OS-II"内核 */
	OSInit();
  
	/* 创建一个启动任务（也就是主任务）。启动任务会创建所有的应用程序任务 */
	OSTaskCreateExt(AppTaskStart,	/* 启动任务函数指针 */
                    (void *)0,		/* 传递给任务的参数 */
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1], /* 指向任务栈栈顶的指针 */
                    APP_TASK_START_PRIO,	/* 任务的优先级，必须唯一，数字越低优先级越高 */
                    APP_TASK_START_PRIO,	/* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&AppTaskStartStk[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    APP_TASK_START_STK_SIZE, /* 任务栈大小 */
                    (void *)0,	/* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       	      （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */


	/* 指定任务的名称，用于调试。这个函数是可选的 */
	OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)APP_TASK_START_NAME, &err);

	/*ucosII的节拍计数器清0    节拍计数器是0-4294967295*/ 
	OSTimeSet(0);	

	/* 启动多任务系统，控制权交给uC/OS-II */
	OSStart();

}





/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 这是一个启动任务，在多任务系统启动后，必须初始化滴答计数器(在BSP_Init中实现)
*             实现触摸和按键检测。
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：1
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg)
{	
	uint8_t  ucCount = 0;
	uint8_t  ucCount1 = 0;
    /* 仅用于避免编译器告警，编译器不会产生任何目标代码 */	
    (void)p_arg;   
	 
	/* BSP 初始化。 BSP = Board Support Package 板级支持包，可以理解为底层驱动。*/
	bsp_Init();      

	/* 检测CPU能力，统计模块初始化。该函数将检测最低CPU占有率 */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif	
		
	/* 创建应用程序的任务 */
	AppTaskCreate();

	/* 任务主体，必须是一个死循环 */
	while (1)     
	{   
		//矩阵键盘扫描
		
	
		ucCount++;
		if(ucCount == 5)
		{
			ucCount = 0;
			//单个键盘扫描
			bsp_KeyScan();
		}	
		
		OSTimeDly(1);										
	}      
}





void watch_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0; 
 	u8 t=0;	   
 	u8 rerreturn=0; 
	u8 res; 
	u8 key;
 	pdata=pdata;
	while(1)
	{			     
		t++;	 	 
		if((t%60)==0)//900ms左右检测1次
		{ 
			//SD卡在位检测
			if((DCMI->CR&0X01)==0)//摄像头不工作的时候,才可以查询SD卡
			{
				OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
				res=SD_GetState();	//查询SD卡状态
				OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断) 
				if(res==0XFF)
				{
					gui_phy.memdevflag&=~(1<<0);//标记SD卡不在位 
					OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
					SD_Init();			//重新检测SD卡 
					OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断) 
				}else if((gui_phy.memdevflag&(1<<0))==0)//SD不在位?
				{
					f_mount(fs[0],"0:",1);		//重新挂载sd卡
					gui_phy.memdevflag|=1<<0;	//标记SD卡在位了		
				} 
			}
			//U盘在位检测   
			if(usbx.hdevclass==1)
			{
				if((gui_phy.memdevflag&(1<<2))==0)//U盘不在位?
				{
					fs[2]->drv=2;
					f_mount(fs[2],"2:",1);		//重新挂载U盘
					gui_phy.memdevflag|=1<<2;	//标记U盘在位	
				}  
			}else gui_phy.memdevflag&=~(1<<2);	//U盘被拔出了  
			//SIM900A检测
			sim900a_status_check();  
		}  	
		//sim900a_cmsgin_check();					//来电/短信 监测
		if(usbx.mode==USBH_MSC_MODE)			//U盘模式,才处理
		{
			while((usbx.bDeviceState&0XC0)==0X40)//USB设备插入了,但是还没连接成功,猛查询.
			{
				usbapp_pulling();	//轮询处理USB事务
				delay_us(1000);		//不能像HID那么猛...,U盘比较慢
			}
			usbapp_pulling();//处理USB事务
		}
		delay_ms(10);
	}
}




/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	
}

