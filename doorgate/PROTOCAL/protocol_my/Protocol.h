#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "mp_protocol_hostif.h"

#define Makepower_Protocol_Open 	1
#define Post_Protocol_Open			0		


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







int tls_protomd_parse(struct tls_atcmd_token_t *tok, char *buf, u32 len);

int tls_protocmd_exec(struct tls_atcmd_token_t *tok,char *res_rsp, u32 *res_len);



#endif

