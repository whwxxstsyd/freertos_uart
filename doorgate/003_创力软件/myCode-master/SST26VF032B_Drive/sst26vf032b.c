/**
******************************************************************************
* @file    sst26vf032b.c
* @author  LL
* @version V0.0.0
* @date    2017-06-07
* @brief   
******************************************************************************
******************************************************************************
***/

#include "sst26vf032b.h"
#include "SPIX.h"

#define SST26_DELAY_TIME 160000

static uint8_t SST26_BY(void);// 检测忙
static void SSTCmd1(uint8_t cmd);//写一个SST命令
//static void SSTCmd2(uint8_t cmd,uint8_t data);//写一个命令后接一个数据
//static void SSTCmd4(uint8_t cmd,uint8_t *addr);//写一个命令后再写3个地址字节
//static uint8_t SSTCmdb1b(uint8_t cmd);//写一个SST命令，返回1字节数据
static uint8_t* SSTCmd4bs(uint8_t cmd,uint8_t* addr,uint8_t* data,uint32_t no);//写1个命令字，3个地址字节，返回多个字节
static void SST26_WREN(void);//允许写功能
static void SST26_WRDI(void);//屏蔽写功能


static uint8_t SST_ReadWriteByte(uint8_t data)
{
    return SST26SPI_ReadWriteByte(data);
}
/*****************************************
*函数名:SSTCmd1/2/4
*功能:写一个SST命令/写一个命令后接一个数据/写一个命令后再写3个数据
*注意事项:这是一个完整的单命令操作，不返回
*****************************************/
void SSTCmd1(uint8_t cmd)//写一个SST命令
{
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(cmd);
    SST26_CE_SET;//禁止片选
}
void SSTCmd2(uint8_t cmd,uint8_t data)//写一个命令后接一个数据
{
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(cmd);
    SST_ReadWriteByte(data);
    SST26_CE_SET;//禁止片选
}
void SSTCmd4(uint8_t cmd,uint8_t *addr)//写一个命令后再写3个地址字节
{
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(cmd); //首命令
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr);
    SST26_CE_SET;//禁止片选
}
/****************************************
*函数名:SSTCmdb1b/SSTCmd4bs
*功能:写一个SST命令，返回1字节数据/写1个命令字，3个地址字，返回多个字节
*更多使用在读出上的
****************************************/
uint8_t SSTCmdb1b(uint8_t cmd)//写一个SST命令，返回1字节数据
{
    uint8_t tmp;
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(cmd);
    tmp=SST_ReadWriteByte(0xFF);
    SST26_CE_SET;//禁止片选
    return(tmp);
}
uint8_t* SSTCmd4bs(uint8_t cmd,uint8_t* addr,uint8_t* data,uint32_t no)//写1个命令字，3个地址字节，返回多个字节
{
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(cmd); //首命令
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr);
    for(;no>0;no--)
    {
        *data++=SST_ReadWriteByte(0xFF);
    }
    SST26_CE_SET;//禁止片选
    return(data);
}
/***************************************
  SST26_WREN  允许写功能
***************************************/
void SST26_WREN(void)
{
    SSTCmd1(0x06);
}
/***********************************
  SST26_WRDI  屏蔽写功能
***********************************/
void SST26_WRDI(void)
{
    SSTCmd1(0x04);
}
/**********************************
  Read_StatusRegister 读状态寄存器
**********************************/
uint8_t Read_StatusRegister()
{
	uint8_t byte = 0;
    SST26_CE_CLR;//使能片选
	SST_ReadWriteByte(0x05);		/* send RDSR command */
	byte = SST_ReadWriteByte(0xFF);		/* receive byte */
    SST26_CE_SET;//禁止片选
	return byte;
}

/**********************************
  SST26_BY  检测忙,返回0，在忙状态，1，非忙状态
**********************************/
uint8_t SST26_BY(void)
{
    uint8_t i=0;
	uint8_t sta;
    uint32_t rty = SST26_DELAY_TIME;

	do
	{
		sta = Read_StatusRegister();
	}while (((sta & 0x80) == 0x80)&&(rty--));	/* waste time until not busy */
       
    if(0 == rty)
        i=0x00;
    else
        i=0x01;
	
    return i;
}

/***********************************
  SST26_WRSR  写状态寄存器
***********************************/
void SST26_WRSR(uint8_t data)
{
    SST26_WREN();
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(0x01);
    SST_ReadWriteByte(0xFF);
    SST_ReadWriteByte(data);
    SST26_CE_SET;//禁止片选
}

/***********************************
  SST26_RDCR 读配置寄存器
***********************************/
uint8_t SST26_RDCR(void)
{
    uint8_t byte = 0;
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(0x35);
    byte = SST_ReadWriteByte(0xFF);
    SST26_CE_SET;//禁止片选
    return byte;
}

/***********************************
  SST26_ULBRP 全局解锁
***********************************/
void SST26_ULBRP(void)
{
    SST26_WREN();
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(0x98);
    SST26_CE_SET;//禁止片选
    SST26_WRDI();
}

/**********************************
 * SST26_ReadJedecID 读取SST的JedecID 
**********************************/
uint32_t SST26_ReadJedecID(void)
{
    uint32_t temp;
    temp = 0;
    
    SST26_CE_CLR;//使能片选
	SST_ReadWriteByte(0x9F);			/* send JEDEC ID command (9Fh) */
    temp = (temp | SST_ReadWriteByte(0xFF)) << 8;	/* receive byte */
	temp = (temp | SST_ReadWriteByte(0xFF)) << 8;	
	temp = (temp | SST_ReadWriteByte(0xFF)); 		/* temp value = 0xBF2642 */
	SST26_CE_SET;//禁止片选

	return temp;
}
/**********************************
 * 擦除指定的扇区
**********************************/
void SST26_SectorErase(uint32_t addr,uint8_t capacity)
{
    
    uint8_t ad[3];
    ad[0]=(addr>>16)&0xFF;
    ad[1]=(addr>>8)&0xFF;
    ad[2]=addr&0xFF;
    
    SST26_WREN();
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(capacity);
    SST_ReadWriteByte(ad[0]);
    SST_ReadWriteByte(ad[1]);
    SST_ReadWriteByte(ad[2]);
    SST26_CE_SET;//禁止片选
    SST26_BY();
    SST26_WRDI();
}
/***********************************
 * SST26_4KByteSectorErase  擦除4KB扇区 
 * 用的是4kb大小 假如地址在0~4095 之间那么
 * 这之间的地址都会擦除
 * 当然我给 4096 的话4096到4096+4095 之间都会擦除
***********************************/
void SST26_4KByteSectorErase(uint32_t addr)
{
    SST26_SectorErase(addr,0x20);
}

/***********************************
 * SST26_8KByteSectorErase  擦除8KB扇区 
 * 用的是8kb大小 假如地址在0~8191 之间那么
 * 这之间的地址都会擦除
 * 当然我给 8193 的话8192到8192+8191 之间都会擦除
***********************************/
void SST26_8KByteSectorErase(uint32_t addr)
{
    SST26_SectorErase(addr,0xD8);
}


/***********************************
 * SST26_32KByteSectorErase  擦除32KB扇区 
 * 用的是32kb大小 假如地址在0~32767 之间那么
 * 这之间的地址都会擦除
 * 当然我给 32768 的话32768到32768+32767 之间都会擦除
***********************************/
void SST26_32KByteSectorErase(uint32_t addr)
{
    SST26_SectorErase(addr,0xD8);
}
/***********************************
 * SST26_64KByteSectorErase  擦除64KB扇区 
 * 用的是64kb大小 假如地址在0~65535 之间那么
 * 这之间的地址都会擦除
 * 当然我给 65536 的话65536到65536+65535 之间都会擦除
***********************************/
void SST26_64KByteSectorErase(uint32_t addr)
{ 
    SST26_SectorErase(addr,0xD8);
}

/**********************************
 * SST26_ChipErase 擦除整片
**********************************/
void SST26_ChipErase(void)
{
    SST26_WREN();
    SSTCmd1(0xC7);
    SST26_BY();
    SST26_WRDI();
}

/***********************************
 * SST26_PageProgram 页编程，一次编程一个整页256字节
 * SST26VF032B器件只有页编程，可以编程1-256字节
 * 不足256字节时，部分编程，超过256字节时只编程256字节 
 编程成功返回 1，不成功返回0
***********************************/
uint8_t SST26_PageProgram(uint32_t addr,const uint8_t* p_data,uint32_t no)
{
	uint16_t i;
	
	if (no != 256) return 0;//一次编程256个字节
    
    SST26_WREN();//使能写
    
    SST26_CE_CLR;//使能片选
	SST_ReadWriteByte(0x02); 			/* send Byte Program command */
	SST_ReadWriteByte((addr >> 16) & 0xFF);	/* send 3 address bytes */
	SST_ReadWriteByte((addr >> 8) & 0xFF);
	SST_ReadWriteByte(addr & 0xFF);
	for (i=0;i<256;i++)
	{	
		SST_ReadWriteByte(p_data[i]);		/* send byte to be programmed */
	}  
    SST26_CE_SET;//禁止片选
   
    if (0==SST26_BY()) return 0;
    SST26_WRDI();//WP保护
    
   return 1; 
}


/***********************************
 * SST26_Write 写多个字节
 * SST26VF032B器件只有页编程，可以编程1-256字节
 * 不足256字节时，部分编程，超过256字节时只编程256字节 
 编程成功返回 1，不成功返回0
***********************************/
uint8_t SST26_Write(uint32_t addr,uint8_t* p_data,uint32_t no)
{
	uint16_t i;
    if (no == 0) return 0;
	if (no > 256) return 0;//最多编程256个字节
    
    SST26_WREN();//使能写
    
    SST26_CE_CLR;//使能片选
	SST_ReadWriteByte(0x02); 			/* send Byte Program command */
	SST_ReadWriteByte((addr >> 16) & 0xFF);	/* send 3 address bytes */
	SST_ReadWriteByte((addr >> 8) & 0xFF);
	SST_ReadWriteByte(addr & 0xFF);
	for (i=0;i<no;i++)
	{	SST_ReadWriteByte(p_data[i]);		/* send byte to be programmed */
	}  
    SST26_CE_SET;//禁止片选
   
    if (0==SST26_BY()) return 0;
    SST26_WRDI();//WP保护
    
   return 1; 
}

/***********************************
 * SST26_EraseWrite 先擦除再编程，写多个页
 * SST26VF032B器件只有页编程，可以编程1-256字节
 * 不足256字节时，部分编程，超过256字节时只编程256字节 
 * 编程成功返回 1，不成功返回0
***********************************/
uint8_t SST26_EraseWrite(uint32_t addr,const uint8_t* p_data,uint32_t no)
{
	uint16_t temp,temp1;
	uint16_t startbz;//编程第一个块的大小
	uint16_t endbz;//编程结束块的大小
	uint16_t bz;//整数块的大小
	uint32_t i;
    uint32_t temp2;
	uint32_t addrs;
	uint8_t  bufft[4096];
    
    if (no == 0) return 0;//没有数据
	if (0!= (no%256)) return 0;//数据个数不是整数页
	if ((addr%256)) return 0;//地址不是页对齐的

	temp= (addr%4096); 
	if (temp!=0)//起始地址不是4Kbytes块对齐的
	{
		temp1 = 4096-temp;
		if (no<=temp1)//写入的数据小于当前块的剩余大小
		{
			startbz = no;
			bz = 0;
			endbz = 0;
		}
		else //跨块了
		{
			startbz = temp1;
			bz = (no - startbz)/4096;
			bz *= 4096;
			endbz = no - startbz - bz;
		}
	}
	else //起始地址是页对齐的
	{
		if (no<4096)//小于一个块
		{
			startbz = no;
			bz = 0;
			endbz = 0;
		}
		else
		{
			startbz = 0;
			bz = no/4096;
			bz *= 4096;
			endbz = no - bz;
		}
	}
    
    /*
	由于写数据时需要写入的地址内容为0xFF,所以写入之前，要先擦除操作。
	因为擦除的最小单位是4KBytes,而写入的是扇区一般是512bytes,所以写入之前需要先读出数据
	然后修改之后再写入进去。
	*/

	if (startbz)
	{
		//需要读出数据，在写回
        addrs = addr & (~0xFFF);
        SST26_LRead(addrs,bufft, 4096);
        for(i=0; i<startbz; i++)
        {
            bufft[i+ addr-addrs] = p_data[i];
        }
        SST26_4KByteSectorErase(addrs);
        for(i=0; i<16; i++)
        {
            if (0==SST26_PageProgram(addrs+i*256,bufft+i*256,256)) break;
        }
        if (i<16) return 0;
       
		addr += startbz;
	}
    temp2 = 0; 
	while (bz--)
	{
		//不需要读出数据
        addrs = addr & (~0xFFF);
        SST26_4KByteSectorErase(addrs);
        for(i=0; i<16; i++)
        {
            if (0==SST26_PageProgram(addrs+i*256,p_data+startbz+temp2+i*256,256)) break;
        }
        if (i<16) return 0;
		addr += 4096;
        bz -= 4096;
        temp2 += 4096;
	}
    
    if (endbz)
	{
		//需要读出数据，在写回
        addrs = addr & (~0xFFF);
        SST26_LRead(addrs,bufft, 4096);
        for(i=0; i<startbz; i++)
        {
            bufft[i] = p_data[i];
        }
        SST26_4KByteSectorErase(addrs);
        for(i=0; i<16; i++)
        {
            if (0==SST26_PageProgram(addrs+i*256,bufft+i*256,256)) break;
        }
        if (i<16) return 0;
	}
    
   return 1; 
}

/*************************************
 * SST26_HRead 高速读 对于后续带5的芯片，可调用此函数读（从指定地址读出指定的字节）
*************************************/
void SST26_HRead(uint32_t addr,uint8_t* p_data,uint32_t no)
{
    SST26_CE_CLR;//使能片选
    SST_ReadWriteByte(0x0b);
    SST_ReadWriteByte(addr>>16);
    SST_ReadWriteByte(addr>>8);
    SST_ReadWriteByte(addr);
    SST_ReadWriteByte(0xff);
    for(;no>0;no--)
        *p_data++=SST_ReadWriteByte(0xFF);
    SST26_CE_SET;//禁止片选
}
/****************************************
SST26_LRead  低速读（从指定地址读出指定的字节）
****************************************/
void SST26_LRead(uint32_t addr,uint8_t* p_data,uint32_t no)
{
    uint8_t ad[3];
    ad[0]=(addr>>16)&0xff;
    ad[1]=(addr>>8)&0xff;
    ad[2]=addr&0xff;
    SSTCmd4bs(0x03,ad,p_data,no);
}
/****************************************
SST26_LReadByte  低速读一个字节
****************************************/
uint8_t SST26_LReadByte(uint32_t addr)
{
	uint8_t byte = 0;	
    
    SST26_CE_CLR;//使能片选
	SST_ReadWriteByte(0x03); 		/* read command */
	SST_ReadWriteByte(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SST_ReadWriteByte(((addr & 0xFFFF) >> 8));
	SST_ReadWriteByte(addr & 0xFF);
	byte = SST_ReadWriteByte(0xFF);
	SST26_CE_SET;//禁止片选
	return byte;			/* return one byte read */
}

/****************************************
SST26_Init
初始化SST26接口
****************************************/
void SST26_Init(void)
{
	SST26SPI_Initialization();
}

/****************************************
SST26_Deinit
关闭SST26接口
****************************************/
void SST26_Deinit(void)
{
	SST26SPI_DeInitialization();
}


/*******************************************************************************
* Function Name  : SST26_Test
* Description    : 测试函数                 
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-06-08
* Author         : LL
*******************************************************************************/
void SST26_Test(void)
{	
    uint16_t i;
    uint8_t buf[300];
    uint8_t bufd[300];
    for(i=0; i<300; i++)
    {
        buf[i] = i+1;
        bufd[i] = 0;
    }
    
    buf[0] = 'a';
    SST26_4KByteSectorErase(0);
    //SST26_ChipErase();
    if (0==SST26_Write(0,buf,256))
    {
        
    }
    SST26_LRead(0,bufd,256);
   
	
}
