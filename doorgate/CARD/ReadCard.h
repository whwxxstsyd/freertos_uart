#ifndef __READCARD_H
#define __READCARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
#include "globe.h"
	
#if ReadCard_Open

#define ReadCardNum		4
typedef __packed struct
{
  IDCardStruct 	OldIDCard;				//刷卡卡号;
  
  u16   Delay_ms;
}ReadCardStruct;
extern 	ReadCardStruct	ReadCard[ReadCardNum];

//////////////////
//在定时器中使用
extern void ReadCard_TimerDeal(void);

extern void ReadCard_Init(u8 GpioOpen);
//功能描述  :系统处在正常状态 刷卡处理;在读卡状态下 刷卡上报
extern void ReadCard_Work(void);
	
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
