#include <string.h> 
#include "param.h"	
#include "Wiegand.h"	
#include "card.h"	
#include "debug.h"
#include "mp_osal_rtos.h"		


#define card_read_period M2T(20)		

static tls_os_timer_t card_read_timer;	

struct wiegand_reader Wiegand_Buf[CARD_DEV_NUM];//伟根缓冲区
				
	
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


static s8 wiegand_reader_process_26bits_code(struct wiegand_reader *wr)
{	
	u32 card;
	u8 parity;
	u8 i;	

	/* Check the parity */
	parity = even_parity(wr->bits, 1, 12);
	if (parity != get_bit(wr->bits, 0))	
		return FALSE;

	parity = odd_parity(wr->bits, 13, 24);
	if (parity != get_bit(wr->bits, 25))
		return FALSE;

	/* Read out the code */
	for (card = 0, i = 1; i <= 24; i++)
	{	
		card <<= 1;		
		if (get_bit(wr->bits, i))
			card |= 1;
	}

	return TRUE;	
}

		
static s8 wiegand_reader_process_66bits_code(struct wiegand_reader *wr)
{	
	u32 card[2]={0,0};	
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

	/*取得HID*/
	for (i = 1; i <= 32; i++) 
	{
		card[0] <<= 1;	
		
		if (get_bit(wr->bits, i))
			card[0] |= 1;		
	}

	/*取得PID*/
	for (i = 32; i <= 64; i++)	 	
	{	
		card[1] <<= 1;	
		
		if (get_bit(wr->bits, i))	
			card[1] |= 1;		
	}

	return TRUE;	
}
	



void wiegand_reader_on_word_timeout(tls_os_timer_t timer)	
{	
	struct wiegand_reader *wr;	

	int err = FALSE;	
	u8 i;

	for(i=0;i<CARD_DEV_NUM;i++)
	{	
		switch(wr->num_bits) 
		{	
			wr = &Wiegand_Buf[i];		
		
			case 66:	
				err = wiegand_reader_process_66bits_code(wr);		

				if(err == TRUE)
				{
						
				}
				
				break;
				
			default:
				LOG_INFO("The num bits is %d\n",wr->num_bits);	
				break;
		}

		wr->num_bits = 0;		
		//处理刷卡信息
		if(err == FALSE)	
		{
			
		}
	}
}


void wiegand_reader_data_pin_changed(u8 head_no, u8 pin, u8 state)
{			
	struct wiegand_reader *wr = Wiegand_Buf[head_no];				
	
	set_bit(&wr->data_pins, pin, state);
	
	switch (wr->data_pins & 3) 
	{		
		case 0: /* No reader */
			wr->num_bits = 0;
			
			//停止定时器	
			tls_os_timer_stop(card_read_timer);	
			
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
			
			//打开定时器
			tls_os_timer_start(card_read_timer);		
			
			return;	
	}
}



u8 wiegand_reader1_init(void)	
{				
	struct wiegand_reader *wr = &Wiegand_Buf[0];
	
	memset(wr, 0, sizeof(*wr));

	set_bit(&wr->data_pins, 0, WGReader_H1D0_Reader);
	set_bit(&wr->data_pins, 1, WGReader_H1D1_Reader);
}	



u8 wiegand_reader2_init(void)	
{		
	struct wiegand_reader *wr = &Wiegand_Buf[1];	
	
	memset(wr, 0, sizeof(*wr));
		
	set_bit(&wr->data_pins, 0, WGReader_H2D0_Reader);	
	set_bit(&wr->data_pins, 1, WGReader_H2D1_Reader);
}


	

void wiegand_timer_init(void)
{	
	tls_os_timer_create(&card_read_timer,  
							card_read_period,					 	
							pdTRUE,	 				
							NULL, 					
							wiegand_reader_on_word_timeout); 	
}




void wiegand_reader_init(void)
{
	//引脚初始化
	Card_Init();		

	wiegand_timer_init();	
	
	wiegand_reader1_init();	

	wiegand_reader2_init();

}



