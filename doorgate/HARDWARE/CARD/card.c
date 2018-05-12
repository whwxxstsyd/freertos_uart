#include "card.h"
#include "delay.h"
#include "wiegand.h"


static void GPIO_WiegandPower_INIT(void);
static void GPIO_Wiegand_INIT(void);			


void Card_Init(void)	
{	
	GPIO_WiegandPower_INIT();
	GPIO_Wiegand_INIT();
}


static void GPIO_WiegandPower_INIT(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	//时钟使能
	RCC_APB2PeriphClockCmd(GPIOReadHead_RCC_CLK, ENABLE);

	//读卡头电源开关	
	GPIO_InitStructure.GPIO_Pin =  GPIOReadHead_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
	GPIO_Init(GPIOReadHead_PORT, &GPIO_InitStructure);
}	


// 功能描述  : 伟根初始化
static void GPIO_Wiegand_INIT(void)	
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

  RCC_APB2PeriphClockCmd(GPIOWGReader_OUT_RCC_CLK|RCC_APB2Periph_AFIO, ENABLE);
	/****************读卡头1***************************************/	
	GPIO_InitStructure.GPIO_Pin = WGReader_H1D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H1D1_PORT, &GPIO_InitStructure);	//配置GPIO针脚
	GPIO_EXTILineConfig(WGReader_H1D1_PORTSOURCE, WGReader_H1D1_PINSOURCE);//配置EXTI线 使中断线和IO针脚线连接上
	
	GPIO_InitStructure.GPIO_Pin = WGReader_H1D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H1D0_PORT, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(WGReader_H1D0_PORTSOURCE, WGReader_H1D0_PINSOURCE);
	
	/****************读卡头2***************************************/	
	GPIO_InitStructure.GPIO_Pin = WGReader_H2D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H2D1_PORT, &GPIO_InitStructure);	//配置GPIO针脚
	GPIO_EXTILineConfig(WGReader_H2D1_PORTSOURCE, WGReader_H2D1_PINSOURCE);//配置EXTI线 使中断线和IO针脚线连接上
	
	GPIO_InitStructure.GPIO_Pin = WGReader_H2D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H2D0_PORT, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(WGReader_H2D0_PORTSOURCE, WGReader_H2D0_PINSOURCE);

  /****************读卡头3***************************************/	
	GPIO_InitStructure.GPIO_Pin = WGReader_H3D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H3D1_PORT, &GPIO_InitStructure);	//配置GPIO针脚
	GPIO_EXTILineConfig(WGReader_H3D1_PORTSOURCE, WGReader_H3D1_PINSOURCE);//配置EXTI线 使中断线和IO针脚线连接上
	
	GPIO_InitStructure.GPIO_Pin = WGReader_H3D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H3D0_PORT, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(WGReader_H3D0_PORTSOURCE, WGReader_H3D0_PINSOURCE);
	//**********将EXTI线上出现下降沿产生中断************
	EXTI_InitStructure.EXTI_Line = WGReader_H1D1_EXTLINE|WGReader_H1D0_EXTLINE|
	                               WGReader_H2D1_EXTLINE|WGReader_H2D0_EXTLINE|
                                   WGReader_H3D1_EXTLINE|WGReader_H3D0_EXTLINE;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			 //设置 EXTI 线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//线路的状态
	EXTI_Init(&EXTI_InitStructure);	
	/***************** 外部中断使能设置 ***********************/		
	/* Enable the EXTI8、9 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
	
	// Enable the EXTI12 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
		
	// Enable the EXTI2 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
}




	
void EXTI9_5_IRQHandler(void)   
{
	if(EXTI_GetITStatus(EXTI_Line8)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
		if(WGReader_H1D0_Reader==0)
		{	
			wiegand_reader_d0(0,0);
		}		
		else
		{
			wiegand_reader_d0(0,1);
		}
		
	}
	else if(EXTI_GetITStatus(EXTI_Line9)!= RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line9);
		if(WGReader_H1D1_Reader==0)
		{		
			wiegand_reader_d0(1,0);
		}
		else
		{
			wiegand_reader_d0(1,1);
		}
	}  	

			
	if(EXTI_GetITStatus(EXTI_Line5)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		if(WGReader_H3D0_Reader==0)
		{
			wiegand_reader_d3(0,0);
		}
		else
		{
			wiegand_reader_d3(0,1);
		}
	}	
	else if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		if(WGReader_H3D1_Reader==0)
		{
			wiegand_reader_d3(1,0);
		}
		else
		{	
			wiegand_reader_d3(1,1);	
		}
	}  	
}

void EXTI15_10_IRQHandler(void)   
{	
	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);
		if(WGReader_H2D0_Reader==0)
		{		
			wiegand_reader_d2(0,0);
		}
		else
		{
			wiegand_reader_d2(0,1);
		}
	} 
}  

void EXTI2_IRQHandler(void)      
{
	if(EXTI_GetITStatus(EXTI_Line2)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
		if(WGReader_H2D1_Reader==0)	
		{		
			wiegand_reader_d2(1,0);
		}
		else
		{
			wiegand_reader_d2(1,1);	
		}
	} 
	
}  

