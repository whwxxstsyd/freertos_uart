#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "param.h"	
#include "mp_protocol_hostif.h"

#define Makepower_Protocol_Open 	CFG_OFF	
#define Post_Protocol_Open			CFG_ON				

#define POST_VER	0x20 //协议版本	  

#define PROTOCOL_HEAD_LEN	13//协议头部固定长度
#define PROTOCOL_TAIL_LEN	5 //协议尾部固定长度
#define	PROTOCOL_HEAD	0x7E
#define PROTOCOL_TAIL	0x0D	



//0x4D命令的type	
#define CMD_0x4D_GET_DATE 		0xE0			


//0x48命令的type
#define CMD_0x48_GET_AUTHOR 	0xE0	
#define CMD_0x48_DEL_AUTHOR 	0xE1	
#define CMD_0x48_CHG_PASSWD 	0xE2	


//0x49命令的type
#define CMD_0x49_SET_WORK_DAY_IN 		0xE0
#define CMD_0x49_SET_REST_DAY_IN 		0xE1
#define CMD_0x49_SET_WEEK_IN 			0xE2
	


//0x4A命令的type	
#define CMD_0x4A_GET_WORK_DAY_IN 		0xE3		
#define CMD_0x4A_GET_REST_DAY_IN 		0xE4
#define CMD_0x4A_GET_WEEK_IN 			0xEB		


//0x4B命令的type
#define CMD_0x4B_SET_VOICE		0xE7	
#define CMD_0x4B_PLY_VOICE		0xEA

	
//0x4C命令的type		
#define CMD_0x4C_GET_HANDLE_POS		 	0xE0//读取左右把手位置
#define CMD_0x4C_GET_CARD_BIT		 		0xE1//读取卡有效位
#define CMD_0x4C_GET_ARMY_OFF_STA	 		0xE2//读取门禁撤布防参数
#define CMD_0x4C_GET_ALARM_PARAM		 	0xE3//读取报警参数
#define CMD_0x4C_GET_ARMY_ON_STA		 	0xE4//读取设布防的状态
#define CMD_0x4C_GET_DIY_PARAM		 		0xE5//获取自定义数据
#define CMD_0x4C_GET_VOICE_STA		 		0xE7//读取屏蔽语音状态
#define CMD_0x4C_GET_SWITCH_PARAM		 	0xE8//读取输入输出开关量配置
#define CMD_0x4C_ARMY_PARAM		 		0xE9//读取门禁撤布防配置参数
#define CMD_0x4C_GET_USER_PWD				0xEB//读取用户密码
#define CMD_0x4C_GET_FLASH_ID 				0xED//读取flash id
#define CMD_0x4C_GET_DOOR_LOG 				0xEF//读取门禁对应日志




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



typedef struct ProtocolResource_T
{	
	//端口说明
	u8  Port_Type;		    	//通信端口 =0为串口1,=1为串口2, =10.为网口
	u8  Port_BufferNum;		//使用的缓冲区号 有时候不同通信端口缓冲区需要独立
	//协议
	u8 	ResState;			    //资源使用状态 =0.空闲 =1.正在结收数据 =2.正在发送数据 =3.准备多次发送数据 =4.一条协议接收完或主发准备完毕
	u8  Pro_Status;            //资源接收状态
	u8 	Pro_Type;        	    //协议类型    公司协议 邮电部协议 门禁协议
	u16 Pro_Lenth;	        	//统计协议的字节长度
	u16 Pro_RealityLenth;      //数据体长度，无尾协议使用
	//指针
	u8 	*ptr_sending;	        //指向发送缓存区

	//分包发送
	u8	SendMode;			//发送20字节延时200ms
	u8	SendCount;			//发送字节计数
	u8	PacketCount;		//包数
	//定时器
	u16 Delay_ms;	

}ProtocolResource;


	
typedef int (* protocmd_proc)(struct tls_protocmd_token_t *tok, char *res_resp, u32 *res_len);

typedef struct tls_protocmd_t {	
    char name;
    u8   flag;	
    int (* proc_func)(struct tls_protocmd_token_t *tok, char *res_resp, u32 *res_len);
}tls_protocmd;


typedef struct tls_respon_head_t {	
	u8 	 SOI;		
	u8   VER;
	u8 	 ADR;
	u8 	 CID1;		
	u8 	 RTN;		
	u16  RESPON_DATA;
	u8 	 HEAD_LEN;
	u8 	 BODY_LEN;	
	u8   TAIL_LEN;
}tls_respon_head;		

		
	
int tls_protocol_rebuild(struct tls_protocmd_token_t *tok,u8 *buff,u32 *res_len,u8 err);	

int tls_protocmd_parse(struct tls_protocmd_token_t *tok, char *buf,u32 *res_len);	

int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp, u32 *res_len);	



#endif

