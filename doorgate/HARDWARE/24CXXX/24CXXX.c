#include <string.h>	
#include "24CXXX.h"
#include "delay.h"
#include "IIC.h"	
#include "led.h"	
//如果器件是_24C1024，通过HardAddr的BIT0=0，或者=1，来访问高64K和低64K；设置BIT1=0，=1，访问不同芯片
//对于其他器件，通过设置HardAddr，选择不同的芯片


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
unsigned char HardAddr;
  
extern void _24CXXX_AddrProcess(unsigned long Addr) ;

void _24CXXX_init(void)
{		
	IIC_Init();
	_24CXXX_WP_OUT();
	_24CXXX_WR_DIS();  //wp引脚输出为高
	HardAddr = 0x0000;
}

const u8 *EEPROM_Test = "24CXXX Test";			
u8 _24CXXX_Buf[15];	 	
		
static u8 _24CXXX_Check(void)
{			
	u8 temp;
	temp=_24CXXX_ReadByte(255);   
	
	if(temp==0X55)
		return 0; 	   
	else
	{	
		_24CXXX_WriteByte(255,0X55);
		temp=_24CXXX_ReadByte(255);	  
		if(temp==0X55)	
			return 0;
	}	
	return 1;											  
}

void _24CXXX_Test(void)		
{		
	u16 len;
	u32 start;	

	len = strlen(EEPROM_Test)	;	
	start = 0;
	
	while(_24CXXX_Check())		
	{			
		delay_xms(500);	
		LED1=!LED1;					
	}
	_24CXXX_WriteBytes(0,(u8*)EEPROM_Test,len);			

	delay_xms(500);	

	_24CXXX_ReadBytes(0,_24CXXX_Buf,len);		

	delay_xms(500);	
}


//地址处理函数，主要处理多芯片时，对于地址的处理
#if DEV_24CXX_MODLE == _24C1024
	#if MAX_ADDR > 0xFFFF	//如果地址大于65535
	void _24CXXX_AddrProcess(unsigned long Addr)
	#else
	void _24CXXX_AddrProcess(unsigned int Addr)
	#endif	//#if MAX_ADDR > 0xFFFF
	{
		if( Addr>( (MEM_SIZE+1)/2-1 ) )	//如果地址大于65535
		{
			Addr /= (MEM_SIZE+1)/2;
		}
		else
		{
			Addr = 0x00;
		}

		HardAddr = Addr;
	}
#else
	#if DEV_24CXX_NUM > 1	//如果使用多个芯片
		#if MAX_ADDR > 0xFFFF		//如果地址大于65535
		void _24CXXX_AddrProcess(unsigned long Addr)
		#else
		void _24CXXX_AddrProcess(unsigned int Addr)
		#endif	//#if MAX_ADDR > 0xFFFF
		{
			if( Addr> MEM_SIZE )	//如果地址大于单芯片地址范围
			{
				Addr /= MEM_SIZE+1;
			}
			else
			{
				Addr = 0x00;
			}

			HardAddr = Addr;
		}
	#else	//如果使用单个芯片
		#if MAX_ADDR > 0xFFFF		//如果地址大于65535
		void _24CXXX_AddrProcess(unsigned long Addr)
		#else
		void _24CXXX_AddrProcess(unsigned int Addr)
		#endif	//#if MAX_ADDR > 0xFFFF
		{
			HardAddr = 0x00;
		}
	#endif	//#if DEV_24CXX_NUM > 1
#endif	//#if DEV_24CXX_MODLE==_24C1024


#if MAX_ADDR > 0xFFFF	//如果地址大于65535
void _24CXXX_WriteByte(unsigned long Addr,unsigned char Data)
#else
void _24CXXX_WriteByte(unsigned int Addr,unsigned char Data)
#endif	//#if MAX_ADDR > 0xFFFF
{
	_24CXXX_AddrProcess(Addr);
	Addr &= MEM_SIZE;	//注：如果是C1024，与后可能数据大于65535，但是写入地址时会被移位和强制转换消除
	_24CXXX_WR_EN();
	IIC_Start();
	IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE);
	IIC_WriteByte( ((unsigned char)(Addr>>8)) );
	IIC_WriteByte( ((unsigned char)Addr) );
	IIC_WriteByte( Data );
	IIC_Stop();
	_24CXXX_WR_DIS();
}

#if MAX_ADDR > 0xFFFF	//如果地址大于65535
unsigned char _24CXXX_ReadByte(unsigned long Addr)
#else
unsigned char _24CXXX_ReadByte(unsigned int Addr)
#endif	//#if MAX_ADDR > 0xFFFF
{
	unsigned char temp;
	_24CXXX_AddrProcess(Addr);
	Addr &= MEM_SIZE;			
		
	IIC_Start();
	IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE);
	IIC_WriteByte( ((unsigned char)(Addr>>8)) );
	IIC_WriteByte( ((unsigned char)Addr) );
	IIC_Start();
	IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_READ);
	temp = IIC_ReadByte(IIC_SEND_NOACK);
	IIC_Stop();		
	return temp;
}



//---------------------------------------------------------------------------
//输入参数：地址，数据指针，写入数据个数（<32768）
//---------------------------------------------------------------------------
#if MAX_ADDR > 0xFFFF	//如果地址大于65535
void _24CXXX_WriteBytes(unsigned long StartAddr,unsigned char *Data,int DataNum)
#else
void _24CXXX_WriteBytes(unsigned int StartAddr,unsigned char *Data,int DataNum)
#endif	//#if MAX_ADDR > 0xFFFF
{
	unsigned char Num;
	unsigned int i;
	unsigned int y;
#if MAX_ADDR > 0xFFFF	//如果地址大于65535
	unsigned long yy;
#else	
	unsigned int yy;
#endif	//#if MAX_ADDR > 0xFFFF

	_24CXXX_AddrProcess(StartAddr);
	yy = StartAddr & MEM_SIZE;
	
	y = yy&PAGE_SIZE_REM;	//=StartAddr/PAGE_SIZE
	y = PAGE_SIZE - y;				//要写的第一页中有多少个数据

	_24CXXX_WR_EN();
	
	if( y <= PAGE_SIZE )	//写第一页（可能不到一页，以使页对齐）
	{
		IIC_Start();
		IIC_WriteByte( DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE );
		IIC_WriteByte( ((unsigned char)(yy>>8)) );
		IIC_WriteByte( ((unsigned char)yy) );
		for(i=0;i<y;i++)
		{
			if( i>=DataNum )
			{
				break;
			}
			IIC_WriteByte( *(Data++) );
			
		}
	}
	IIC_Stop();
	delay_ms(10);

	DataNum -= y;		//还有多少数据未写
	i = 0;
	Num = 0;			//用于记录已经写了多少页
	while( DataNum>0 )	//数据未写完
	{
		if( i == 0 )
		{
			yy = StartAddr+y+Num*PAGE_SIZE;
			_24CXXX_AddrProcess(yy);
			yy = yy & MEM_SIZE;
			IIC_Start();	
			IIC_WriteByte( DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE );
			IIC_WriteByte( ((unsigned char)(yy>>8)) );
			IIC_WriteByte( ((unsigned char)(yy)) );
		}
		IIC_WriteByte( *(Data++) );
		
		i++;
		if(i>=PAGE_SIZE )	//如果一页写完
		{
			i = 0;
			Num++;
			IIC_Stop();
			delay_ms(10);
		}
		DataNum--;
	}
	if(i!=0x0000)	//如果写结束时，并未到达页的最后，则需要补上停止位
	{
		IIC_Stop();
		delay_ms(10);
	}
	
	_24CXXX_WR_DIS();
}
	


//---------------------------------------------------------------------------
//输入参数：地址，数据指针，读出数据个数（<32768）
//---------------------------------------------------------------------------
#if MAX_ADDR > 0xFFFF	//如果地址大于65535
void _24CXXX_ReadBytes(unsigned long StartAddr,unsigned char *Data,int DataNum)
#else
void _24CXXX_ReadBytes(unsigned int StartAddr,unsigned char *Data,int DataNum)
#endif	//#if MAX_ADDR > 0xFFFF
{
	unsigned char Num;
	unsigned int i;
	unsigned int y;
#if MAX_ADDR > 0xFFFF	//如果地址大于65535
	unsigned long yy;
#else
	unsigned int yy;
#endif	//#if MAX_ADDR > 0xFFFF

	_24CXXX_AddrProcess(StartAddr);
	yy = StartAddr & MEM_SIZE;

	y = yy&PAGE_SIZE_REM;	//=StartAddr/PAGE_SIZE
	y = PAGE_SIZE - y;				//要写的第一页中有多少个数据

	if( y <= PAGE_SIZE )	//写第一页（可能不到一页，以使页对齐）
	{
		IIC_Start();
		IIC_WriteByte( DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE );
		IIC_WriteByte( ((unsigned char)(StartAddr>>8)) );
		IIC_WriteByte( ((unsigned char)StartAddr) );
		IIC_Start();
		IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_READ);
		for(i=0;i<y;i++)
		{	
			if( i>=DataNum )
			{
				break;
			}
			if( (i==DataNum-1)||(i==y-1) )	//如果是该页的最后一个数据，则NOACK
				*Data = IIC_ReadByte(IIC_SEND_NOACK);
			else
				*Data = IIC_ReadByte(IIC_SEND_ACK);
			Data++;
			
		}
	}
	IIC_Stop();	

}

