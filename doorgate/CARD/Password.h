#ifndef __PASSWORD_H
#define __PASSWORD_H
/**********************************************名单结构体**************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

//门密码
enum
{
	receive_keyNumber = 1,
};
#define Password_buffer_len 20	//密码缓存大小
#define	Passwore_Digit			6		//密码位数
#define door_count					4 	//门数量
typedef __packed struct
{
	u8 state;
	u8 keyNumber;			//输入密码
	u8 right_flag;		//密码正确标志位
	u8 end;						//密码换结束位
	u8 start;					//密码换开始位
	u8 count;					//输入密码位数
	u8 buffer[Password_buffer_len];				//输入密码缓存
	u8 deal_buffer[Password_buffer_len];	//处理后密码缓存
}PasswordStruct;
extern PasswordStruct Password[door_count];

void Password_Deal(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
