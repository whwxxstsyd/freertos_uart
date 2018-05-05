#ifndef __PostProtocolCommon_H
#define __PostProtocolCommon_H

#include "system.h"

//全球唯一序列号

//门参数 数据初始化
void Init_DoorPara(u8 DoorNumber);

//广播读命令，获取设备信息
extern void Send_MPJ12(void);
// 功能块 49 命令
extern void Ariber_SetSysParaAndRecord(void);
// 功能块  : 4A 命令
extern void Ariber_GetSysParaAndRecord(void);

// 功能描述  : 邮电部协议处理 共用
extern void Common_PostProtocol(void);

#endif

