#ifndef __WIEGAND_H
#define __WIEGAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"	


#if 0
#define WIEGAND_BYTE_NUM 5
#else
#define WIEGAND_BYTE_NUM 9	
#endif



typedef enum Wiegand_Sequence_Type_t
{
	Wiegand_Sequence_Normal 		= 0x00,//正常
	Wiegand_Sequence_Reversal 	= 0x01,//倒序
	Wiegand_Sequence_NOT			= 0x02,//取反
  Wiegand_Sequence_HighStart 		= 0x04,//从高（低）开始取指定位数
}Wiegand_Sequence_Type;


	
typedef enum CardDevice_State_t
{	
	CardDev_IDLE 		= 0, 	//空闲状态	
	CardDev_RECEIVE	= 1,	//正在接收	
	CardDev_FINISH		= 2,	//接收完毕	
}CardDevice_State;			


struct wiegand_reader {
	u8 bits[WIEGAND_BYTE_NUM]; /* For up to 34 bits */
	u8 num_bits;	
	u8 data_pins;		

}wiegand_reader_T;		

	

void wiegand_reader_data_pin_changed(u8 head_no, u8 pin, u8 state);

void wiegand_reader_init(void);

static void wiegand_reader_on_word_timeout(void *context);


		
#endif 
