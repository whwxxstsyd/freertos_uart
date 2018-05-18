#include "sd2068.h"

static int SD2068A_WriteTimeOn(void);
static int  SD2068A_WriteTimeOff(void);

static void  SD2068A_PortInitial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(GPIO_SD2068_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIO_SD2068_SDA_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_OD;//开漏输出
	GPIO_Init(GPIO_SD2068_SDA_PORT, &GPIO_InitStructure);	
}

/* 将SDA设为输入					*/
static void  SD2068A_SDA_IN(void)
{
	GPIO_InitTypeDef					GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/* 将SDA设为输出					*/
static void  SD2068A_SDA_OUT(void)
{		
	GPIO_InitTypeDef					GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_OD;//开漏输出
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}



void  SD2068A_Initial(void)
{
	SD2068A_PortInitial();		
}


static void  SD2068A_I2CWait(void)
{
	volatile u8 i;

	for(i = 0;i<8;i++);
}


/***SCL处于高电平，SDA由高变低构成一个开始条件****/

static int  SD2068A_I2CStart(void)
{
	SD2068A_SDA_OUT();
	SD2068A_SDA_HIGH();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	if(!SD2068A_SDA_GET())  //SDA线为低电平则总线忙，退出
	{
		return -1;
	}
	SD2068A_SDA_LOW();
	SD2068A_I2CWait();
	while(SD2068A_SDA_GET())//SDA线为高电平则总线忙，退出
	{
		return -1;
	}
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	return 0;
}

/******关闭I2C总线******************/
static void  SD2068A_I2CStop(void)
{
	SD2068A_SDA_OUT();

	SD2068A_SDA_LOW();
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	SD2068A_SDA_HIGH();
}

/********发送ACK*****/
static void  SD2068A_I2CAck(void)
{
	SD2068A_SDA_OUT();
	SD2068A_SDA_LOW();
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	SD2068A_SCL_LOW();

}

/******没有ACK*******/
static void  SD2068A_I2CNoAck(void)
{
	SD2068A_SDA_OUT();
	SD2068A_SDA_HIGH();
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	SD2068A_SCL_LOW();

}

/*************读取ACK信号************返回1 有 ACK，0 = 无ACK***/
static int SD2068A_I2CWaitAck(void)
{
	u8 ErrTime = 255;

	SD2068A_SCL_LOW();
	SD2068A_SDA_HIGH();
	SD2068A_SDA_IN();

	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	while(SD2068A_SDA_GET())
	{
		ErrTime--;
		if(!ErrTime)
		{
			SD2068A_SCL_LOW();
			return -1;
		}
	}
	SD2068A_SCL_LOW();
	return 0;
}

/************MCU向SD2068发送一个字节*************/
static void  SD2068A_I2CSendByte(u8 WriteData)  //数据从高位到低位//
{
	volatile int x;
	u8 i = 8;
	while(i--)
	{
		SD2068A_SCL_LOW();

		for(x=5;x>0;x--);

		if(WriteData & 0x80)
		{
			SD2068A_SDA_HIGH();
		}
		else
		{
			SD2068A_SDA_LOW();
		}
		SD2068A_I2CWait();
		SD2068A_SCL_HIGH();
		SD2068A_I2CWait();
		WriteData<<=1;
	}
	SD2068A_SCL_LOW();
}

/*********MCU从SD2068读入一字节*********/
static u8  SD2068A_I2CReceiveByte(void) //数据从高位到低位//
{
	u8  ReadData = 0;
	u8	i = 8;

	SD2068A_SDA_HIGH();			//设置SDA 为输入
	while(i--)
	{
		ReadData<<=1;
		SD2068A_SCL_LOW();
		SD2068A_I2CWait();
		SD2068A_I2CWait();
		SD2068A_SCL_HIGH();
		SD2068A_I2CWait();
		SD2068A_I2CWait();
		if(SD2068A_SDA_GET())
		{
			ReadData |= 0x01;
		}
	}
	SD2068A_SCL_LOW();
	return ReadData;
}

/******读SD2068实时数据寄存器******/
/**
 * @brief 从2068中读取时间
 * @param[in] 
 * @return 
 * @note 
 */
static int SD2068A_I2CReadDate(u8 *TimeBuff)
{
	u8 n;
	if(SD2068A_I2CStart() != 0)
	{
		return -1;
	}
	SD2068A_I2CSendByte(READ_COMMAND);

	if(SD2068A_I2CWaitAck() != 0)
	{
		SD2068A_I2CStop();
		return -1;
	}

	for(n = 0;n<7;n++)
	{
		TimeBuff[n] = SD2068A_I2CReceiveByte();

		if( n != 6)
		{
			SD2068A_I2CAck();
		}
	}
	SD2068A_I2CNoAck();
	SD2068A_I2CStop();
	return 0;
}

/******写SD2068实时数据寄存器******/
static int SD2068A_I2CWriteDate(u8* WriteData)
{
	SD2068A_WriteTimeOn();

	if(SD2068A_I2CStart() != 0)
	{
		return -1;
	}

	SD2068A_I2CSendByte(WRITE_COMMAND);

	if( 0 != SD2068A_I2CWaitAck())
	{
		SD2068A_I2CStop();
		return -1;
	}

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(Second_Address);  //设置写起始地址
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[0]));  //Second
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[1]));  //minute
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[2]) | 0x80);  //hour,24小时制
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[3]));  //week
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[4]));  //DAY
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[5]));  //MONTH
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[6]));  //year
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();

	SD2068A_I2CStart();
	SD2068A_I2CSendByte(WRITE_COMMAND);
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(ReviseTime);//设置调整时间地址
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x00);//零数字调整寄存器 
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();
	SD2068A_WriteTimeOff();
	return 0;
}

/******写SD2068允许程序******/
static int SD2068A_WriteTimeOn(void)
{
	if(0 != SD2068A_I2CStart())
	{ 
		return -1;
	}

	SD2068A_I2CSendByte(WRITE_COMMAND);

	if( 0 != SD2068A_I2CWaitAck())
	{
		SD2068A_I2CStop();
		return -1;
	}

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x10);//设置写地址10H,CTR2
	SD2068A_I2CWaitAck();

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x80); //置WRTC1 = 1
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();

	SD2068A_I2CStart();
	SD2068A_I2CSendByte(WRITE_COMMAND);
	SD2068A_I2CWaitAck();

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x0f);//设置写地址0FH ,CTR1
	SD2068A_I2CWaitAck();

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x84);//置WRTC2，WRTC3 = 1
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();
	return 0;
}

/******写SD2068禁止程序******/
static int  SD2068A_WriteTimeOff(void)
{
	if(0 != SD2068A_I2CStart())
	{
		return -1;
	}
	SD2068A_I2CSendByte(WRITE_COMMAND);
	if( 0 != SD2068A_I2CWaitAck())
	{
		SD2068A_I2CStop();
		return -1;
	}
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x0f);//设置写地址0FH ,CTR1
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0X00);//置WRTC2，WRTC3 = 0
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x00);//置WRTC1=0(10H地址)
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();
	return 0;
}

/**
 * @brief 获取RTC中的时间,秒:分:时:周:日:月:年
 * @param[in] 
 * @return 0 获取时间成功
 *        -1 获取时间失败
 * @note 输出:
 */
int  SD2068A_GetTime(u8 *TimeBuff)
{
	if(SD2068A_I2CReadDate(TimeBuff) != 0)
	{
		return -1;
	}

	// BCD转10进制
	TimeBuff[0]				= BCD2DEC(TimeBuff[0]);	// 秒
	TimeBuff[1]				= BCD2DEC(TimeBuff[1]);	// 分
	TimeBuff[2]				= BCD2DEC(TimeBuff[2]);	// 时
	TimeBuff[3]				= BCD2DEC(TimeBuff[3]);	// 周
	TimeBuff[4]				= BCD2DEC(TimeBuff[4]);	// 日
	TimeBuff[5]				= BCD2DEC(TimeBuff[5]); 	// 月
	TimeBuff[6]				= BCD2DEC(TimeBuff[6]); 	// 年

	return 0;

}

/**
 * @brief 设置RTC中的时间
 * @param[in] u8 *TimeBuff 格式 秒:分:时:周:日:月:年
 * @return 0 设置时间成功
 *        -1 设置时间失败
 * @note 输入：秒分时，周，日月年，十进制
 */
int  SD2068A_SetTime(u8 *TimeBuff)
{
//	TimeBuff[0] = SD2068_Time.Second ;
//	TimeBuff[1] = SD2068_Time.Minute ;
//	TimeBuff[2] = SD2068_Time.Hour ;
//	TimeBuff[3] = SD2068_Time.Week ;
//	TimeBuff[4] = SD2068_Time.Day ;
//	TimeBuff[5] = SD2068_Time.Month ;
//	TimeBuff[6] = SD2068_Time.Year ;

	return SD2068A_I2CWriteDate(TimeBuff);

}


