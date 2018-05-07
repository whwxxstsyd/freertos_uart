#ifndef _READCARD_H_
#define _READCARD_H_

#ifdef __cplusplus
extern "C" {
#endif
 
#include "system.h"
#include "globe.h"
  
#define Password_buffer_len 20 
typedef __packed struct
{
	u8 state;
	signed char keyNumber;
	u8 right_flag;
	u8 end;
	u8 start;
	u8 count;
	u8 buffer[Password_buffer_len];
	u8 deal_buffer[Password_buffer_len];
  
  u16 Delay_ms;
}PasswordStruct;
extern PasswordStruct Password[4];

#define BeepPasswordError()  	Beep_SetInfo(3,200,200)	//√‹¬Î¥ÌŒÛ
#define BeepNoPassword()			Beep_SetInfo(5,200,200) //√‹¬Î±ªπÿ

extern void Keynum_Init(void);
extern void Keynum_Deal(u8 i,u8 Keynum,DoorParaStruct *DoorParaP,SysDataStruct *SysDataP);
extern void Keynum_TimerDeal(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
