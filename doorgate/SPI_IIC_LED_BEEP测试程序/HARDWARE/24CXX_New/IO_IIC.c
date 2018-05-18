 
/*********************************************************************************************************
** Function name:          EEPROM	   
** Descriptions:          适合于AT24C02~AT24C1024                                                 
** input parameters:        无
** output parameters:       无      
** Returned value:          0	 
** Created by:		    刘玉刚
    
** Modified date:     2013.8.15     
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "IIC.h"
#ifdef __OPTIMIZE__

#endif	


unsigned char IIC_State;


static void  IIC_Wait(void)	
{	
	volatile u8 i;	

	for(i = 0;i<8;i++);		
}


		
void IIC_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	   	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);								
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);			

	IIC_SCK_HIGH();	
	IIC_SDA_HIGH();
}

void IIC_Start(void)
{	
	IIC_SDA_OUT();
	IIC_SCK_HIGH();
	IIC_SDA_HIGH();

	IIC_Wait();	
	//delay_us(4);
	
	IIC_SDA_LOW();

	IIC_Wait();
	//delay_us(4);
	
	IIC_SCK_LOW();
	
	IIC_Wait();
	//delay_us(4);
}


void IIC_Stop(void)
{
	IIC_SDA_OUT();

	IIC_SDA_LOW();
	IIC_Wait();
	//delay_us(4);
	IIC_SCK_HIGH();	
	IIC_Wait();
	//delay_us(4);
	IIC_SDA_HIGH();
	IIC_Wait();
	//delay_us(4);
}


void IIC_WriteByte(unsigned char Data)
{
	unsigned char i;
	IIC_SDA_OUT();
	for(i=0;i<8;i++)
	{
		if( (Data&0x80)==0x00 )
		{
			IIC_SDA_LOW();
		}
		else
		{
			IIC_SDA_HIGH();
		}
		IIC_SCK_HIGH();
		IIC_Wait();
		//delay_us(4);
		IIC_SCK_LOW();
		IIC_Wait();
		//delay_us(4);
		Data <<= 1;
	}

	IIC_SDA_IN();	//读取数据第九位-从机确认位
	IIC_SCK_HIGH();
	IIC_Wait();
	//delay_us(4);

	i = IIC_SDA_GET();
	if( i!=0x00 )
	{
		IIC_State = IIC_STATE_NOACK;
	}
	else
	{
		IIC_State = IIC_STATE_ACK;
	}

	IIC_SCK_LOW();
	IIC_Wait();
	//delay_us(4);
}



unsigned char IIC_ReadByte(unsigned char ACK)
{
	unsigned char i,Data,temp;
	IIC_SDA_IN();	

	Data = 0x00;

	for(i=0;i<8;i++)
	{
		Data <<= 1;
			
		IIC_SCK_HIGH();
		IIC_Wait();
		//delay_us(4);
		
		temp = IIC_SDA_GET();
			
		if( temp!=0x00 )
		{
			Data |= 0x01;
		}
		else
		{
			
		}
		IIC_SCK_LOW();	
		IIC_Wait();
		//delay_us(4);

	}

	IIC_SDA_OUT();	//主机读数据时，需要根据工作状况发送ACK
	
	if(ACK == IIC_SEND_ACK)
	{		
		IIC_SDA_LOW();
	}	
	else
	{
		IIC_SDA_HIGH();
	}
	IIC_SCK_HIGH();
	IIC_Wait();
	//delay_us(4);	
	IIC_SCK_LOW();	
	IIC_Wait();
	//delay_us(4);

	return Data;
}


