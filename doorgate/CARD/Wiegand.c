#include <string.h> 
#include "param.h"	
#include "Wiegand.h"		
#include "mp_card_task.h"	



WIEGAND_PARAM Wiegand_Buf[CARD_DEVICE_NUM];//伟根缓冲区
		

static u8 get_bit(u8 *data, u8 idx)
{
	return (data[idx >> 3] >> (idx & 7)) & 1;
}


static void set_bit(u8 *data, u8 idx, u8 val)
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
	parity = even_parity(wr->bits, 1, 32);
	if (parity != get_bit(wr->bits, 0))
		return FALSE;

	parity = odd_parity(wr->bits, 33, 64);	
	if (parity != get_bit(wr->bits, 65))	
		return FALSE;

	/* Read out the code */
	for (card = 0, i = 1; i <= 64; i++) {
		card <<= 1;	
		if (get_bit(wr->bits, i))
			card |= 1;	
	}

	//执行card的动作	
	//wiegand_reader_event(wr, WIEGAND_READER_EVENT_CARD, card);

	
	return TRUE;	
}



static void wiegand_reader_on_word_timeout(void *context)
{
	WIEGAND_PARAM *wr = context;	
	int err = FALSE;		
		
	switch(wr->num_bits)	
	{	
		case 26:
			break;
				
		case 66:				
			//err = wiegand_reader_process_66bits_code(wr);
			break;

		default:	
			break;
	}	
	
	//wr->num_bits = 0;	
	
	//if (!err)		
		//wiegand_reader_event(wr, WIEGAND_READER_ERROR, err);
}


	

//中断里面调用	
static void wiegand_reader_data_pin_changed(WIEGAND_PARAM *pWiegandParam,u8 pin,u8 state)			
{
	set_bit(&pWiegandParam->data_pins, pin, state);
	
	switch (pWiegandParam->data_pins & 3)
	{	
		case 0://读卡器设备错误	
			pWiegandParam->num_bits = 0;		
			//关闭定时器
			//timer_deschedule(&wr->word_timeout);
			//处理事件
			//wiegand_reader_event(wr, WIEGAND_READER_ERROR, -ENODEV);
			return;
			
		case 1: /* 1 bit */
			if (pWiegandParam->num_bits < sizeof(pWiegandParam->bits) * 8)
				set_bit(pWiegandParam->bits, pWiegandParam->num_bits, 1);
			return;
			
		case 2: /* 0 bit */
			if (pWiegandParam->num_bits < sizeof(pWiegandParam->bits) * 8)
				set_bit(pWiegandParam->bits, pWiegandParam->num_bits, 0);
			return;	
			
		case 3: /* Inter bit */
			pWiegandParam->num_bits++;	
			//开启定时器
			//timer_schedule_in(&pWiegandParam->word_timeout, WORD_TIMEOUT);
			return;

	}

	
}




void wiegand_reader_d0(u8 pin,u8 state)
{
	WIEGAND_PARAM *wr = &Wiegand_Buf[0];		
	
	wiegand_reader_data_pin_changed(wr, pin, state);
}	


void wiegand_reader_d1(u8 pin,u8 state)
{
	WIEGAND_PARAM *wr = &Wiegand_Buf[1];		

	wiegand_reader_data_pin_changed(wr, pin, state);
}


void wiegand_reader_d2(u8 pin,u8 state)
{
	WIEGAND_PARAM *wr = &Wiegand_Buf[2];			

	wiegand_reader_data_pin_changed(wr, pin, state);
}

	
void wiegand_reader_d3(u8 pin,u8 state)
{			
	WIEGAND_PARAM *wr = &Wiegand_Buf[3];		

	wiegand_reader_data_pin_changed(wr, pin, state);
}


	


void wiegand_reader_init(void)	
{				
	memset(Wiegand_Buf, 0, sizeof (*Wiegand_Buf));		
}



