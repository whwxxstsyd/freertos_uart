#include "card.h"
#include "delay.h"	

static void GPIO_WiegandPower_INIT(void);
static void GPIO_Wiegand_INIT(void);			


void Card_Init(void)	
{	
	GPIO_WiegandPower_INIT();
	
	GPIO_Wiegand_INIT();
	
	//初始化一个定时器:用于判断接收是否超时
	//GPIO_Wiegand_Timer();
}


void GPIO_Wiegand_Timer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;                       //定义结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;               //定义TIM结构体变量

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);          //使能TIM2外设
	TIM_DeInit(TIM2);	      

	//复位时钟TIM2，恢复到初始状态
	TIM_TimeBaseStructure.TIM_Period = 200;
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //TIM2时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //计数方式
	//  定时时间T计算公式：

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);               //初始化
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                         //清除标志

	//  中断方式下，使能中断源
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                     //使能中断源
	TIM_Cmd(TIM2,ENABLE);                                        //使能TIM2
	
	//  设置优先分级组
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //选择中断通道，库
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //启动此通道的中断
	NVIC_Init(&NVIC_InitStructure);                            //结构体初始化
}	


	
static void GPIO_WiegandPower_INIT(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	

	//读卡头电源开关		
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_12; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	

	CardRead1_PowerOn;	
	CardRead2_PowerOn;
}	


// 功能描述  : 伟根初始化
static void GPIO_Wiegand_INIT(void)	
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
		
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);


	/****************读卡头1***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//配置GPIO针脚

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2|EXTI_Line3;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	


	
	/****************读卡头2***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//配置GPIO针脚
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line3|EXTI_Line4;		
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	


	
	/***************** 外部中断使能设置 ***********************/		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  		
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 	
}






void EXTI2_IRQHandler(void)      
{
	if(WGReader_H1D0_Reader==0)	
	{			
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,0,0);		
	}
	else
	{
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,0,1);	
	}
	
	EXTI_ClearITPendingBit(EXTI_Line2);
}  



void EXTI3_IRQHandler(void)      
{
	if(WGReader_H1D1_Reader==0)	
	{				
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,1,0);
	}
	else
	{
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,1,1);	
	}

	
	if(WGReader_H2D0_Reader == 0)
	{	
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,0,0);		
	}
	else
	{
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,0,1);		
	}

	EXTI_ClearITPendingBit(EXTI_Line3);		
}  



void EXTI4_IRQHandler(void)      
{	
	if(WGReader_H2D1_Reader==0)		
	{		
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,1,0);	
	}
	else
	{	
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,1,1);			
	}	

	EXTI_ClearITPendingBit(EXTI_Line4);
}  



	
