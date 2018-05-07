#ifndef	__POSTPRODAPROFUNC_H
#define __POSTPRODAPROFUNC_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
	
#if ReadCard_Open
  
//时段表 数据初始化
extern void Init_TimeListSeg(u8 DoorNumber);
//星期时段-准进时段列表
extern void Init_SetWeekListSeg1(u32 EepAddr);

extern void Card_PostProtocol(void);
  
#endif
  
#ifdef __cplusplus
}
#endif

#endif


