#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "mp_protocol_hostif.h"

#define Makepower_Protocol_Open 	1
#define Post_Protocol_Open			0		

#define   POST_VER		   	0x10 //协议版本	  


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
	ProtocolStruct 	ProBuffer; 		//缓冲区
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

struct tls_protocmd_t {	
    char name;
    u8   flag;
    int (* proc_func)(struct tls_protocmd_token_t *tok, char *res_resp, u32 *res_len);
};





void tls_protocol_rebuild();

int tls_protomd_parse(struct tls_atcmd_token_t *tok, char *buf, u32 len);

int tls_protocmd_exec(struct tls_atcmd_token_t *tok,char *res_rsp, u32 *res_len);



#endif

