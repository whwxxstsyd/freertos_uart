 #include "Wiegand.h"	
#include "mp_card_task.h"	

WIEGAND_PARAM Wiegand_Buf[CARD_DEVICE_NUM];//伟根缓冲区
		

static inline u8 get_bit(u8 *data, u8 idx)
{
	return (data[idx >> 3] >> (idx & 7)) & 1;
}


static inline void set_bit(u8 *data, u8 idx, u8 val)
{
	if (val)	
		data[idx >> 3] |= 1 << (idx & 7);
	else
		data[idx >> 3] &= ~(1 << (idx & 7));
}


static u8 even_parity(u8 *data, u8 from, u8 to)	
{
	u8 p;	
	
	for (p = 0; from <= to; from++)
		p += get_bit(data, from);

	return p & 1;
}


static u8 odd_parity(u8 *data, u8 from, u8 to)	
{
	return !even_parity(data, from, to);
}

		
static s8 wiegand_reader_process_66bits_code(WIEGAND_PARAM *wr)
{
	u32 card;	
	u8 parity;
	u8 i;			

	/* Check the parity */
	parity = even_parity(wr->bits, 1, 12);
	if (parity != get_bit(wr->bits, 0))
		return -1;

	parity = odd_parity(wr->bits, 13, 24);
	if (parity != get_bit(wr->bits, 25))
		return -1;

	/* Read out the code */
	for (card = 0, i = 1; i <= 24; i++) {
		card <<= 1;
		if (get_bit(wr->bits, i))
			card |= 1;
	}
	return 0;
}

//中断里面调用
void wiegand_data_rec(u8 dev_no,u8 bit)	
{
	WIEGAND_PARAM *pWiegandParam = &Wiegand_Buf[dev_no];		

	
	
}




// 功能描述  : 获取伟根通信的卡号
// 参   数1  : No使用的刷卡头编号 0开始
// 参   数2  : *IDCardNo 卡号存储的指针
u8 Wiegand_Receive(u8 No)
{	
	u8 Flag=0; 	
	if(WiegandRota[No].State== CardDevice_FINISH)
	{ 		
		if(Wiegand_ReadJudge(No))
		{ 					   
			Flag=1;
		}
    	else
		{
			if((WiegandRota[No].wiegand[0] & 0xfc) == 0x00)
			{
				Flag=2;
				WiegandRota[No].KeyNumber = WiegandRota[No].Even<<3 | WiegandRota[No].wiegand[0]<<1 | WiegandRota[No].Odd;
			}
		}
		WiegandRota[No].State= CardDevice_IDLE; 				
	}		
	return Flag;
}
// 功能描述  : 对读卡头获取的卡号信息处理
// 参   数1  : No使用的刷卡头编号 0开始
// 参   数2  : *IDCardNo 卡号存储的指针
// 备    注	 ：这里根据SysConfig.Wiegand_Sequence系统配置处理信息
void Wiegand_DealData(u8 No,u8 *IDCardNo,DoorParaStruct *DoorParaPointer)
{
	int8_t	i,j,End,Ta;
	u8 temp[8]={0};

	End=(int8_t)WiegandRota[No].Pos-1;//伟根接收数据最后一个字节的位置
	if(DoorParaPointer->Wiegand_Sequence&Wiegand_Sequence_Reversal)
	{//倒序
		for(j=0,i=End/2;j<=i;j++)
		{
		   Ta=WiegandRota[No].wiegand[j];
		   WiegandRota[No].wiegand[j]=WiegandRota[No].wiegand[End-j];
		   WiegandRota[No].wiegand[End-j]=Ta;
		}  		   
	}
	if(DoorParaPointer->Wiegand_Sequence&Wiegand_Sequence_NOT)
	{//取反
		for(i=0;i<=End;i++)
		{			  
		   WiegandRota[No].wiegand[i]=~WiegandRota[No].wiegand[i];		   
		}  	
	} 
	//取卡号
  if(DoorParaPointer->Wiegand_Sequence&Wiegand_Sequence_HighStart)
  {
    for(i=0,j=0;(i<=End)&&(j<DoorParaPointer->Wiegand_CardBits);i++,j++)
    {
      temp[j]=WiegandRota[No].wiegand[i];//高位在前
    }
  }
  else
  {
    for(i=End,j=0;(i>=0)&&(j<DoorParaPointer->Wiegand_CardBits);i--,j++)
    {
      temp[j]=WiegandRota[No].wiegand[i];//低位在前
    }
    for(i=0;i<=(j-1)/2;i++)//取反
		{
		   Ta=temp[i];
		   temp[i]=temp[j-1-i];
		   temp[j-1-i]=Ta;
		}  		   
  }
  //卡号有效位
  if(DoorParaPointer->Wiegand_CardBits>=4)
  {
    for(i=0;i<4;i++)
    {
       *IDCardNo++=temp[i];
    }
  }
  else
  {
    for(i=4;i>DoorParaPointer->Wiegand_CardBits;i--)
    {
      *IDCardNo++=0;
    }
    for(i=0;i<DoorParaPointer->Wiegand_CardBits;i++)
    {
      *IDCardNo++=temp[i];
    }
  }
}



void wiegand_reader_data_pin_changed(struct WIEGAND_PARAM *wr,
				    							uint8_t pin, uint8_t state)
{

	set_bit(&wr->data_pins, pin, state);

	switch (wr->data_pins & 3) {
	case 0: /* No reader */
		wr->num_bits = 0;
		timer_deschedule(&wr->word_timeout);
		wiegand_reader_event(wr, WIEGAND_READER_ERROR, -ENODEV);
		return;
	case 1: /* 1 bit */
		if (wr->num_bits < sizeof(wr->bits) * 8)
			set_bit(wr->bits, wr->num_bits, 1);
		return;
	case 2: /* 0 bit */
		if (wr->num_bits < sizeof(wr->bits) * 8)
			set_bit(wr->bits, wr->num_bits, 0);
		return;
	case 3: /* Inter bit */
		wr->num_bits++;
		timer_schedule_in(&wr->word_timeout, WORD_TIMEOUT);
		return;
	}
}



static void wiegand_reader_d0(uint8_t pin_state, void *context)
{
	struct WIEGAND_PARAM *wr = context;

	wiegand_reader_data_pin_changed(wr, 0, pin_state);
}	


static void wiegand_reader_d1(uint8_t pin_state, void *context)
{
	struct WIEGAND_PARAM *wr = context;

	wiegand_reader_data_pin_changed(wr, 0, pin_state);
}


static void wiegand_reader_d2(uint8_t pin_state, void *context)
{
	struct WIEGAND_PARAM *wr = context;

	wiegand_reader_data_pin_changed(wr, 0, pin_state);
}

	
static void wiegand_reader_d3(uint8_t pin_state, void *context)
{	
	struct WIEGAND_PARAM *wr = context;

	wiegand_reader_data_pin_changed(wr, 0, pin_state);
}


void wiegand_reader_init(WIEGAND_PARAM *wg_buf)
{		
	memset(wg_buf, 0, sizeof (*wg_buf));
}



