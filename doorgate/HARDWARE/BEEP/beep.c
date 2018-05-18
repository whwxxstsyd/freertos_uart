#include "beep.h"
#include "delay.h"	

static void GPIO_BEEP_INIT(void);


void Beep_Init(void)
{	
	GPIO_BEEP_INIT();
	
	BEEP_OFF;
}

static void GPIO_BEEP_INIT(void)
{	
	//蜂鸣器
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clocks ,所有使用到得GPIO口的RCC时钟
	RCC_APB2PeriphClockCmd( GPIO_BEEP_RCC_CLK, ENABLE);
	
	//PE_15端口初始化为开漏输出	RUN
	GPIO_InitStructure.GPIO_Pin =  GPIO_BEEP_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(GPIO_BEEP_PORT, &GPIO_InitStructure);
}


//蜂鸣器测试程序
void Beep_Test(void)
{
	u8 i;
	u8 Beep_Num = 3;//快闪次数

	for(i=0;i<Beep_Num;i++)
	{
		BEEP_ON;
		delay_xms(300);	
		BEEP_OFF;
		delay_xms(300);	
	}


}



void hw_platform_beep_ctrl(unsigned short delay,unsigned int beep_freq)
{
	int i;
	for (i = 0; i < (delay*beep_freq)/2000;i++)
	{		
		BEEP_ON;
		delay_us(1000000/beep_freq);	
		BEEP_OFF;		
		delay_us(1000000/beep_freq);
	}
}



