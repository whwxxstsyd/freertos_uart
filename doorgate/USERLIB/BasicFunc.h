#ifndef	__BASICFUNC_H
#define __BASICFUNC_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
	

// 功能描述  : BCD转Hex
u8 BCDToHex(u8 TC_char);
// 功能描述  : BCD转Hex
u32 U32BCDToU32Hex(u32 TC_char);
// 功能描述  : Hex转BCD
// hex值<100(0x64)
u8 HexToBCD(u8 TC_char);
// 功能描述  : Hex转BCD
u16 HexToIntBCD(u8 HexVar);
// 功能描述  : 16位Hex转BCD
// hex值<10000(0x2710)
u16 IntHexToIntBCD(u16 HexVar);
////////////////////////////////////////////////////////////////////////////////////
// 功能描述  : 字节的大小端的转换
// 输入参数1 : Count  处理的数据个数
// 输入参数2 : P      处理的数据指针
void ByteEndian(u16 Count ,u8 *P);
// 功能描述  : 将两个char型合成一个int
u16 TwoCharToInt(u8 c1,u8 c2);
////////////////////////////////hex ascii 码转换/////////////////////////////////////
// 功能描述  : "9"的ASCII码与"A"的ASCII码值相差==7  注：字母都统一为大写字母的ASCII码值 
u8 Hi_HexToAsc(u8 c1);
// 功能描述  : "9"的ASCII码与"A"的ASCII码值相差==7  注：字母都统一为大写字母的ASCII码值 
u8 Low_HexToAsc(u8 c1);
//将一个字节的数据转化为两个ASCII 码
void CharToAsc(u8 c1,u8 *buf);
//将两个字节的数据转化为两个ASCII 码
void u16ToAsc(u16 c1,u8 *buf);
//将四个字节的数据转化为两个ASCII 码
void u32ToAsc(u32 c1,u8 *buf);		
// 功能描述  : 把ascii 的'0'~'f' 转换成 0~f 一个byte
u8 AscToHex(u8 c1);
// 功能描述  : 把两个ASC码合成一个HEX字符
u8 TwoAscTOHex(u8 TPA,u8 TPB);
// 功能描述  : 把ASC码缓冲转换成HEX缓冲区
void  BufAscToHex(u8 *PTemp,u16 Len);
/////////////////////////////////////////////////////////////////////////////////////
//将大写字母转换为小写
u8 CapitalTo(u8 cl);
//将大写字母转换为小写
void BufCapitalTo(u8 *p,u16 len);

//////////////////////////////////校验和计算/////////////////////////////////////////
// 计算校验和
u16 BasicFunc_check_sum(u8 *data,u16 len);
//通用的CRC16位校验和算法
//Quantic 多项式0xA001,
//intData 初始值：0xFFFF
//prt			缓存指针
//Lenth		数据长度
u16 GeneralCRCFun(u16 Quantic,u16 intData,u8 *ptr,int Lenth);
//检验和计算
u8 ModBusCRC16ChkSum(u8 Ta,u16 Quantic,u16 CRCInit,u8 *ptr,int Lenth);
//////////////////////////////////////////////////////////////////////////////////////
u16 BasicFunc_GetRadom(void);
//////////////////////////////////////////////////////////////////////////////////////
//u32 TimeTransform32bit(Date_Time *date);	

#ifdef __cplusplus
}
#endif

#endif
