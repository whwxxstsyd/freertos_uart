#ifndef	__PostProtocolDeal_H
#define __PostProtocolDeal_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"

//RTN返回的值
enum
{
  RTN_NORMAL          = 0x00,     //正常回复
  RTN_VER_ERR         = 0x01,     //版本不符合
  RTN_CMDCHK_ERR      = 0x02,     //命令校验和CHKSUM失败
  RTN_PARACHAK_ERR    = 0x03,     //参数部分的累加和校验失败
  RTN_CID2_ERR        = 0x04,     //无效的CID2
  RTN_UNKNOW_CMD      = 0x05,     //命令格式错误
  RTN_UNVALID_DATA    = 0x06,     //数据部分有无效数据
  RTN_NOACCESS        = 0x07,     //无权限进行访问
  RTN_PASSWORD_WRONG  = 0xE0,     //权限校验密码不正确
  RTN_MODPASSWORD_ERR = 0xE1,     //对密码修改失败
  RTN_MEMORY_FULL     = 0xE2,     //存储空间已满
  RTN_MODPARA_ERR     = 0xE3,     //修改内部参数失败不成功
  RTN_MEMORY_EMPTY    = 0xE4,     //存储空间已空
  RTN_NOHAVE_INFO     = 0xE5,     //无对应信息项
  RTN_HAVESAME_USER   = 0xE6,     //重复设定相同的用户，门禁将保持不变
  RTN_BluetoothPasswod_Error = 0xE9,	//蓝牙密码错误 
}; 
/*************数据结构*************************/
typedef __packed struct
{	
	u16 lLenth:4;
	u16 mLenth:4;
	u16 hLenth:4;
	u16 lChksum:4;
}Packed_data;
typedef __packed union
{	
	u16  			  word;
	Packed_data	div;
}Taglchksum;

extern u8   RTN;
extern u8   Var;
extern u16	CallSystemAddr;

// 功能描述  : 邮电部协议发送数据帧头部分
// 输入参数  : RTN 返回状态.
extern void SendHead(void);
// 功能描述  : 邮电部协议回发的校验和函数（要先HEX转成ASC再求校验和）
// 输入参数  : len 校验数据长度.
extern void SendPostChksum(void);
// 功能描述  : 修正发送缓冲固定位置参数
// 输入参数  : FixupNo 位置 valus 参数.
extern void SendFixupByte(u8 FixupNo,u8 valus);
// 功能描述  : 邮电部协议发送数据长度
extern void SendDataLen(void);
// 功能描述  : 邮电部协议发送帧尾
extern void SendTail(void);
// 功能描述  : 邮电部协议数据长度校验
extern u8 ChkPostDataSum(void);
// 功能描述  : 邮电部协议应答的命令，无数据体
extern void SendDefault(void);
// 功能描述  : 邮电部协议应答的结尾处理 修正长度 加入校验和数据尾 并发送数据
extern void SendTaiDeal(void);
// 功能描述  : 邮电部协议接收数据的校验和算法
// 输入参数  : p1 要校验的数据 ReDaLen 数据长度.
static u8 chk_post_chksum(u8  *p1,u16  ReDaLen);
// 功能描述  : 邮电部协议解析
extern void DealPostProtocol(void);
// 功能描述  : 邮电部协议处理
extern void DealPostPro(void);

#ifdef __cplusplus
}
#endif

#endif
