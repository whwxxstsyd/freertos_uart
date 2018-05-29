#ifndef MP_CARD_TASK_H
#define MP_CARD_TASK_H		
#include "sys.h"	


enum
{
	Wiegand_Sequence_Normal 		= 0x00,//正常
	Wiegand_Sequence_Reversal 	= 0x01,//倒序
	Wiegand_Sequence_NOT			= 0x02,//取反
  Wiegand_Sequence_HighStart 		= 0x04,//从高（低）开始取指定位数
};

enum	
{
	SysState_REALTIME 		= 0,	//正常状态	
	SysState_CONFIG 		= 1,	//配置状态
	SysState_READCARD		= 2,	//读卡状态
	SysState_DEBUG			= 3,	//读卡状态	
};


//4B命令对卡的操作
typedef struct tls_card_cmd_4b
{
	u8 Card_ValidBit;
	u8 Card_EndianSet;	
}tls_card_cmd_4b_t;	



typedef struct tls_card_dev
{
	tls_card_cmd_4b_t *card_cmd;


	CMD_0x4B_HANDLE_T *card_cmd;	

	 
	tls_os_mailbox_t        rev_mailbox;			
	s16 (*card_rev_callback)(char *buf, u16 len);	
	
}tls_card_dev_t;

void tls_card_dev_init(void);				


#endif

