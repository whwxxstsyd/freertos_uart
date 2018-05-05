#ifndef __PROTOCOLDEAL_H
#define __PROTOCOLDEAL_H		
#include "sys.h"	


typedef union
{
	u8  	                  DaBuf[MAXPCReLen]; 	
	PostProProStruct     	  PostProDa;   	//邮电部协议
	MakepowerDoorProStruct  MPDoorProDa;   //创力门禁协议
}ProtocolStruct;


typedef struct ProtocolResource_t	
{	
	ProtocolStruct 	ProBuffer; 	   //缓冲区
	//端口说明
	u8 	    Port_Type;		       //通信端口 =0为串口1,=1为串口2, =10.为网口
	u8 	    Port_BufferNum;	   //使用的缓冲区号 有时候不同通信端口缓冲区需要独立
	//协议
	u8 	    ResState;			   //资源使用状态 =0.空闲 =1.正在结收数据 =2.正在发送数据 =3.准备多次发送数据 =4.一条协议接收完或主发准备完毕
	u8      Pro_Status;           //资源接收状态
	u8 	    Pro_Type;         	   //协议类型    公司协议 邮电部协议 门禁协议
	u16 	Pro_Lenth;	           //统计协议的字节长度
	u16     Pro_RealityLenth;     //数据体长度，无尾协议使用
	//指针
	u8 	    *ptr_sending;	        //指向发送缓存区

	//分包发送
	u8			SendMode;			//发送20字节延时200ms
	u8			SendCount;			//发送字节计数
	u8			PacketCount;		//包数
	//定时器
	u16     Delay_ms;
	//
	u8      ReceiveStatus;        //AT命令模式使用
  
}ProtocolResource	


u8 CID2_Action(u8 CID2)	;


#endif

