#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"

#define PortIntervalTime     2000

///////////////////////////////////////////////////////////////////////////////

typedef struct	
{	
	u8 RHead;					//头字符串
	u8 RSLenth;					//协议最短长度 无尾协议使用
	u8 RTail;					//尾字符串
}ProTypeStruct;
/////////////////////////////////////////////////////
//枚举量
//不同端口使用的通道资源  PortRes
enum
{
	ProtocolRes_USART = 0,	//串口1
  ProtocolRes_USART2 = 1, //串口2 stm32f030 蓝牙模块
#if CPU_Type == CPU_STM32F103
  ProtocolRes_USART3 = 1, //串口3	stm32f103 蓝牙模块
  ProtocolRes_USART5 = 1, //串口3
#endif
//	ProtocolRes_TCP   = 2,	//网口
};
//通信状态枚举
enum 
{
	ConnectState_IDLE = 0, 					//空闲状态
	ConnectState_RECEIVE= 1,				//正在接收
	ConnectState_RECEIVE_FINISH= 2,	//接收完毕
	ConnectState_DATA_DEAL= 3,			//数据处理状态
	ConnectState_SEND= 4						//发送状态
};

#define     Port_Buffer_Count      2     //协议缓冲个数
#define    	MAXPCReLen     	 	    1200 	//PC串口接收的最大长度4000  to 2000

/*************数据结构*************************/
/****************************************************************************************
// 功能块  : 时段公用结构体  
****************************************************************************************/
#define	WEEK_TABLE_LIST_NUM		10					//按星期 准进时段，总共的时段表  本来是16张的,由于容量不够
//※※※※※时间段※※※※※※※※※※※※※※※※※※※
typedef struct
{
  //起始时段
  u8 StartTimeHH;
  u8 StartTimeMM;
  //结束时段
  u8 EndTimeHH;
  u8 EndTimeMM;
}Ariber_SegTime;
/*************邮电部协议**********************************************************************/
/****************************************************************************************
// 功能块  : E3命令 设置设备地址  
****************************************************************************************/
typedef struct
{	
  // 世纪、年、月、日、时、分秒， 日志的日期与时间  7字节
  u8	century;	//世纪
  u8	year;	  	//年
  u8 	month;		//月
  u8 	day;			//日
  u8	week;
  u8 	hour;			//时
  u8 	minute;		//分
  u8 	second;		//秒
}ReceCMD4EStruct;
/****************************************************************************************
// 功能块  : E3命令 设置设备地址  
****************************************************************************************/
typedef struct
{	
	u16 SetAddr;
}ReceCMDE3Struct;
/*******************************************************************************************
// 功能块  : 48 命令  
******************************************************************************************/
typedef struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
  u8 Password[5];   
}ReceCMD48Struct;
/****************************************************************************************
// 功能块  : 49 命令  
****************************************************************************************/
//※※※※※※※※※※※※※※※※※※※※※※※※
typedef  struct
{ // 世纪、年、月、日、时、分秒， 日志的日期与时间  7字节
	u8	century;	//世纪
	u8	year;	  	//年
	u8 	month;		//月
	u8 	day;			//日
	u8	week;
	u8 	hour;			//时
	u8 	minute;		//分
	u8 	second;		//秒
}Ariber_SetDateAndTimeStruct;
typedef struct
{
	u16  WritePoint;			
	u16  ReadPoint;
	u8 	 LoadpMF;     //备注
	u8   ChkSum;			//为校验和配置1 byte的内存空间
}Record_Point;

//※※※※※增加卡号※※※※※※※※※※※※※※※※※※※
//D7	D6	D5，D4	D3，D2，D1，D0
//=0	=0	X，X	按星期一到星期日准进列表管理方式管理的序号0—15
//1	0	X，X	工作日/非工作日准进列表管理方式管理的序号（张）0—3
//	1	X，X	不受准进时段限制 
typedef struct
{
  u8     CardNO[5];             //IC卡号
  u8     UserID[4];		        	//用户编号
  u8     UserPassword[2];       //用户密码
  //有效期限
  u8     Century;                //世纪
  u8     Year;                   //年 0~99
  u8     Month;                  //月
  u8     Day;                    //日
  //用户权限
  u8     UserPermission  ;       //用户权限
}Ariber_AddCardListStruct;

enum
{
  DELMODE_BYUSERCARDNO        = 0x00,	//删除对应卡号
  DELMODE_BYUSERID            = 0x01,	//根据用户ID号码进行删除
  DELMODE_ALL                 = 0x02  //删除全部卡库   
};
typedef struct
{
  u8     DelMode;            //删除模式
  u8     CardNO[5];          //待删除的卡号      
}Ariber_DelCardListStruct;

//配卡状态，有上位机协议进行下发
enum
{
  CONFIG_STATE_START          = 0x00,
  CONFIG_STATE_INPROCES       = 0x01,
  CONFIG_STATE_END            = 0x02
};
typedef struct
{
  u8     ConfigState;          //配卡状态 
  u8     ConfigCount;          //待配数量
  Ariber_AddCardListStruct     NewAddCardList[3];     //最多可以配三张卡    
}Ariber_BatchAddCardStu;
//※※※※※工作星期准进时间※※※※※※※※※※※※※※※※※※※
typedef   struct
{
  u8 				TableNO;            //标号
  u8 				WeekDay;            //星期    
  Ariber_SegTime  SegTime[6];   //待设定的指定星期时段
}Ariber_SetWeekSegStu;
//工作日或休息日时段
#define		WORK_REST_SEG_NUM		4
typedef  struct
{
  u8              GroupNO ;      //表号
  Ariber_SegTime  SegTime[4];    //支持4个时段        
}Ariber_WorkDayTimeSegStu;	 
//※※※※※周休息日※※※※※※※※※※※※※※※※※※※
typedef   struct
{
  u8 RestDay[2];     //休息日：其采用BCD格式，如无，则用0xFF代替。，如：7，0XFF，表示仅星期日休息。
}Ariber_SetWeekRestDay;
//※※※※※节假休息日※※※※※※※※※※※※※※※※※※※
typedef   struct
{
  u8   Month;            //月
  u8   Day;              //日    
}Ariber_HolidayStu;  	
//※※※※※设置门禁参数※※※※※※※※※※※※※※※※※※※ 
typedef   struct
{
	u8 Channel;			//设置的通道号
	u8 SetValue[2];	//设置的值	
}Ariber_SetDoorParaStu;

typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8             				      Data[20];           //暂时代替
    Ariber_SetDateAndTimeStruct	SetDateAndTime;			//设置日期时间E0
    Ariber_WorkDayTimeSegStu   	SetTimeSeg;         //设置时段E1，E2

    Ariber_AddCardListStruct  	NewAddCardList;     //新配置的用户资料E3
    Ariber_DelCardListStruct  	NewDelCardList;     //带删除的用户E4
    //***************时段设置 
    Ariber_SetWeekRestDay       SetWeekRestDay;     //设置星期休息日EA   
    Ariber_HolidayStu           Holiday;            //增加或删除的节假日EB,EC
    Ariber_SetWeekSegStu        SetWeekListSeg;     //设置星期表F1
    //************设置门禁参数** 
    Ariber_SetDoorParaStu				SetDoorPara;  
  }Info; 
}ReceCMD49Struct;

/*******************************************************************************************
// 功能块  : 4A 命令  
******************************************************************************************/
typedef   struct
{
  u8 TableNO;            //标号
  u8 WeekDay;            //星期    
}Ariber_ReadWeekStu;
typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8            	Data[20];           //暂时代替
    //***************时段设置 
    Ariber_ReadWeekStu  ReadWeekListSeg;       //读取星期表
  }Info;      
}ReceCMD4AStruct;
/*******************************************************************************************
// 功能块  : 4B 命令  
******************************************************************************************/
//设置布防联动的参数
typedef   struct
{
	u8 Type;          	//联动类型  默认04
	u8 ShiDuanNumber; 	//数值默认48  16进制 0x30
	u8 NBytes;        	//默认 01
	u8 ShiDuan[6];   	  //默认 0xFFFFFFFFFFFF
	u8 HaveMan;      	  //00 不勾选有人设备 01勾选有人设备
	u8 Bftj;						//布防条件 	
//第0位 =1勾选远程布防 	=0不勾选  
//第1位	=1按钮布防 			=0不勾选 
//第3位	=1时段点布防 		=0不勾选 
//第4位	=1时段内自动布防 =0不勾选 其它位0
	u8 LianDong[8];  	//LianDong[0];  第0位 非法刷卡 第1位 非法入侵 第2位 红外告警 第3位 震动告警 第4位 钥匙告警
                    //LianDong[1];  默认0
                    //LianDong[2];  =1 门开关门关灭 =2门开关门关延时 =3 出门刷卡或按钮开灯
                    //LianDong[3];  当LianDong[2]=2时 为延时的时间  
                    //LianDong[4];  =0x00高输出 =0x01低输出
	u8 Chk;    
}Ariber_BFLD;//布防联动
typedef   struct 
{
	u8			      ID_Flag;		//标识数据必须为0x01	
 	Ariber_BFLD		BFLD_Para;
}Ariber_SetBFLDStruct;
typedef   struct 
{
	u8 		Reserved;		//
	u8		BF_Type;		//布防类型（0：撤防自动；1：撤防手动；2：布防自动；3：布防手动）
 	u16		BF_TimeM;		//布防时间
}Ariber_SetBFEnableStruct;
typedef   struct 
{
	u16		BF_TimeS;	      //布防时间
	u8 		ShieldAlarm;    //报警屏蔽0：否 1：是
	u8		CFLD_DoorKey;   //钥匙开门联动撤防0：否 1：是
}Ariber_SetBFLDAlarmStruct;
typedef   struct 
{
	u8		groupNo;			//密码组号 此版本只支持1组密码 固定为0x01
	u8 		useEnabled;		//对应密码使能 1：开启 0：关闭
	u8		password[3];	//6位密码 ASCII
	u8		backup[3];		//备用
}Ariber_SetPasswordStruct;
typedef   struct 
{
	u8		data[64];			
}Ariber_BluetoothPasswordStruct;
typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8 			        			        Data[20];       	//暂时代替
    Ariber_SetBFEnableStruct 		  SetBFEnable;     	//设置布防使能
    Ariber_SetBFLDAlarmStruct 	  SetBLDFAlarm;     //设置报警参数
    Ariber_SetBFLDStruct 				  SetBFLDPara;     	//设置布防联动的参数
    Ariber_SetPasswordStruct		    SetPassword; 		//设置密码
    Ariber_BluetoothPasswordStruct  BluetoothPassword;
  }Info; 
}ReceCMD4BStruct;
/*******************************************************************************************
// 功能块  : 4C 命令  
******************************************************************************************/
typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8 		Data[20];       	//暂时代替
  }Info;
}ReceCMD4CStruct;
typedef   struct
{	
	u32 USART_BaudRate;
}ReceCMDA1Struct;
/*******************************************************************************************
// 功能块  : 邮电部协议命令的共用体结构(参数)
******************************************************************************************/ 
typedef   union    
{	
	u8                      OtherDaBuf[MAXPCReLen-7];
  ReceCMD4EStruct         ReceCMD4E;
	ReceCMDE3Struct         ReceCMDE3;
	ReceCMD48Struct         ReceCMD48;
	ReceCMD49Struct         ReceCMD49;
	ReceCMD4AStruct         ReceCMD4A;
	ReceCMD4BStruct         ReceCMD4B;
	ReceCMD4CStruct         ReceCMD4C;
	ReceCMDA1Struct         ReceCMDA1;
}ProParaStruct;
// 邮电部协议结构(~200160420000FDB1)	
typedef   struct
{	
	u8   Head;   				
	u8   Ver;  
	u8   Addr;
	u8   Cid1;
	u8   Cid2;
	u16  Length; 				
	ProParaStruct		ProPara;  	
}PostProProStruct; 
/*************公司门禁协议*************************/
//存储器测试
typedef   struct
{	
  u8 	Storage_Type;		//0：EEP，1：Flash
  u8 	Cmd_Type;				//0：读，1：写，2：擦除
	u8 	Storage_Add[4];	//存储器地址
	u8 	Info_Len;	 			//信息长度
  u8 	Data[48];       //信息体
}MPDoorCMD37Struct;
typedef   struct
{	
  u16 SetAddr;	//地址
}MPDoorCMDA1Struct;
typedef   struct
{	
	u8 YearH; 				//
	u8 YearL; 				//
	u8 Month;				  //月数
	u8 Day;				    //天数
	//u8 Week;				//星期               
	u8 Hour;	   			//24h制
	u8 Minute;				//分钟
	u8 Second;				//秒	

}MPDoorCMDA3Struct;
typedef   struct
{	
  u8 RunState;
}MPDoorCMDA4Struct;
typedef   struct
{	
  u8  ResetSystem;
}MPDoorCMDA5Struct; /*系统复位*/
typedef   union
{	
	u8 OthDaBuf[MAXPCReLen-9];
	MPDoorCMD37Struct       MPDoorCMD37;//存储器测试	
	MPDoorCMDA1Struct       MPDoorCMDA1;//设置地址
	MPDoorCMDA3Struct       MPDoorCMDA3;//设置时间
	MPDoorCMDA4Struct       MPDoorCMDA4;//设置运行状态
	MPDoorCMDA5Struct       MPDoorCMDA5;//系统复位
	    
}MPDoorProParaStruct;
// 公司门禁协议结构(#80FFFF02111A00008A)
typedef   struct
{	u8		Head;		  //协议头
	u8		VerInfor;	//基本信息 
	u16	  Addr;		  //地址
	u8		Style;		//设备类型
	u8		CMD;		  //命令
	u8		CmdID;		//命令标识码
	u16	Lenth;		  //信息长度单位 				
	MPDoorProParaStruct   Para;					
}MakepowerDoorProStruct; 
/*************通信端口协议资源*************************/
//收发缓冲区结构
typedef    union
{	
	u8  	                  DaBuf[MAXPCReLen]; 	
	PostProProStruct     	  PostProDa;   	//邮电部协议
	MakepowerDoorProStruct  MPDoorProDa;  //创力门禁协议
}ProtocolStruct;

typedef   struct
{	
	ProtocolStruct 	ProBuffer; 		//缓冲区
	//端口说明
	u8 	    Port_Type;		    		//通信端口 =0为串口1,=1为串口2, =10.为网口
	u8 	    Port_BufferNum;			//使用的缓冲区号 有时候不同通信端口缓冲区需要独立
	//协议
	u8 	    ResState;			   //资源使用状态 =0.空闲 =1.正在结收数据 =2.正在发送数据 =3.准备多次发送数据 =4.一条协议接收完或主发准备完毕
	u8      Pro_Status;           //资源接收状态
	u8 	    Pro_Type;        	   //协议类型    公司协议 邮电部协议 门禁协议
	u16 		Pro_Lenth;	       //统计协议的字节长度
	u16     Pro_RealityLenth;     //数据体长度，无尾协议使用
	//指针	
	u8 	    *ptr_sending;	        //指向发送缓存区

	//分包发送	
	u8			SendMode;						//发送20字节延时200ms
	u8			SendCount;						//发送字节计数
	u8			PacketCount;					//包数
	//定时器
	u16     Delay_ms;
	//
	u8      ReceiveStatus;        //AT命令模式使用
}ProtocolResource;
//接收数据结构体  
extern ProtocolResource PortRes[Port_Buffer_Count];
//外部使用的指针
extern ProtocolResource *ptr_PortResP;

/////////////////////////////////////////
void Protocol_timer_open(u8 PortBufferNum,u16 time);
void Protocol_timer_close(u8 PortBufferNum);
void Protocol_TimerDeal(void);
/////////////////////////////////////////
// 功能描述  : 初始化协议资源状态、长度、缓冲区
void Init_protocol(u8 PortBufferNum);
// 功能描述  : 数据处理状态的通道初始化
void Init_protocol_DATA_DEAL(void);
/////////////////////////////////////////
// 功能描述  : 协议接收数据接口
// 输入参数1  : PortNum  使用的端口类型下的端口编号 例如串口1和串口2接收使用同一个缓冲区
// 输入参数2  : chr		接收的数据 
void BackComm0_RX(u8 PortNum,u8 PortBufferNum,u8 data);
///////////////////////////////////////////
// 功能描述  : 通道0的协议发送数据接口
// 输入参数  : ComNum.//端口号
void BackComm0_TX(u8 ComNum,u8 PortBufferNum);
//////////////////////////////////////////////
// 功能描述  : 复位发送缓冲区
void ResetBuffer(void);
// 功能描述  : 将要发送的字节送入发送缓冲区
// 输入参数  : TempChar 发送数据.
void SendByte(u8 TempChar);
// 功能描述  : 发送n个字节
// 输入参数  : pchar 发送数据 count 长度.
void SendHexBytes(u8 *pchar,u8 count);
// 功能描述  : 发送一个字节，一个字节需分解成两个ASCII码发送，先发送High 4位，后发送Low 4位
// 输入参数  : c1 发送数据.
void SendOneHexToTwoAsciiByte(u8 c1);
// 功能描述  : 送两个字符数
void Send_Word(u16 TemInt);
// 功能描述  : 发送n个字节，即2*n个ASCII码
// 输入参数  : pchar 发送数据 count 长度.
void SendAsciiBytes(u8 *pchar,u8 count);
// 功能描述  : 发送一个Float型数据
void SendOCEDataFloat(float ocedt);
// 功能描述  : 开始发送缓冲区数据
void StartSend(void);
// 功能描述  : 开始发送缓冲区数据
void TimerDelayStartSend(u8 ComNum,u8 PortBufferNum);
// 功能描述  : 开始发送缓冲区数据
void TimerDelaySendReply(u8 ComNum,u8 PortBufferNum);

// 功能描述  : 协议处理
void DealProtocol(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
