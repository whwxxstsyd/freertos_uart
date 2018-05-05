//zhj
//v1.0
//20151110

#include "protocol.h"
#include "globe.h"
#include "config.h"


//通信协议类型		
ProTypeStruct const ProType[2]=	
{
	{'~',1,0x0D},	  	//邮电部协议	‘~’ 0x7E
	{'#',1,0x0D},	  	//门禁协议		‘#’	0x23
}; 

//接收数据结构体  
ProtocolResource  PortRes[Port_Buffer_Count];        //接收缓冲区资源 串口使用0区 网口使用1	

//外部使用的指针
ProtocolResource  *ptr_PortResP;	//指向使用接收缓冲区资源 

////////////////////////////////////////////////////////////////////////

void Protocol_TimerDeal(void)
{
  u16 i;
  for(i=0;i<Port_Buffer_Count;i++)
  {
    switch(PortRes[i].Delay_ms)
    {
      case 0xffff:
        break;
      case 0:
        PortRes[i].Delay_ms = 0xffff;
        PortRes[i].ResState = ConnectState_IDLE;		//表资源为空闲状态
        break;
      default:
        if(PortRes[i].Delay_ms) PortRes[i].Delay_ms--;
        break;
    }
  }
}
/////////////////////////////////////////////////////////////////////////
// 功能描述  : 初始化协议资源状态、长度、缓冲区
void Init_protocol(u8 PortBufferNum)
{ 	  	
	PortRes[PortBufferNum].ResState=ConnectState_IDLE;		//表资源为空闲状态
	PortRes[PortBufferNum].SendMode=0;
	Protocol_timer_close(PortBufferNum);  									  //关闭定时器1
}
// 功能描述  : 数据处理状态的通道初始化
void Init_protocol_DATA_DEAL(void)
{ 
  u8 i;  	
	for(i=0;i<Port_Buffer_Count;i++)
	{
		if(PortRes[i].ResState==ConnectState_DATA_DEAL)//此通道数据处理	
		{
			Init_protocol(i); 			
		}
	}
}
////////////////////接收函数处理//////////////////////////////////////////
// 功能描述  : 协议数据接收解析处理
// 输入参数  : data 接收的一个字节.
void ReceMainDeviceFun0(u8 PortBufferNum,u8 data)
{	
	int i;
	// 未收到帧头
	if(PortRes[PortBufferNum].Pro_Lenth >= MAXPCReLen)  //协议超长
	{
		Init_protocol(PortBufferNum);
	}
	
  	switch(PortRes[PortBufferNum].Pro_Status)
	{
	case 0://没有确定协议类型
	  for(i=0;i<ProTypeCount;i++)
		{	  			
			if(ProType[i].RHead==data)
			{	
				PortRes[PortBufferNum].Pro_Type=i; 				//协议类型
				PortRes[PortBufferNum].Pro_Lenth=0;	   		//协议的实际长度，实时长度					
				PortRes[PortBufferNum].ProBuffer.DaBuf[PortRes[PortBufferNum].Pro_Lenth++]=data;	
				switch(PortRes[PortBufferNum].Pro_Type)
				{
					case 0:

					case 1:
					PortRes[PortBufferNum].Pro_Status=1;//有尾协议
					break;
				}
				break;					
			}  			
		}		
	  break;
	  
	case 1:// 已收到帧头 有尾协议
			PortRes[PortBufferNum].ProBuffer.DaBuf[PortRes[PortBufferNum].Pro_Lenth++]=data;
			if(data==ProType[PortRes[PortBufferNum].Pro_Type].RTail)
			{					
				Protocol_timer_close(PortBufferNum);  // 关接收定时器	 						
				PortRes[PortBufferNum].ResState=ConnectState_RECEIVE_FINISH;  //表示已接收完一条完整的协议   														
			} 				
	  break;

		default:
			Init_protocol(PortBufferNum);
			break;
	}
}
// 功能描述  : 协议接收数据接口 串口中断中使用
// 输入参数1  : PortNum  使用的端口类型下的端口编号 例如串口1和串口2接收使用同一个缓冲区
// 输入参数2  : chr		接收的数据 
void BackComm0_RX(u8 PortNum,u8 PortBufferNum,u8 data)
{ 
	if(PortRes[PortBufferNum].ResState==ConnectState_IDLE)    //资源空闲
	{
    Protocol_timer_open(PortBufferNum,PortIntervalTime);    //收PC命令的定时，如时间到未收全则清本次已收数据.仿真是20000L 
	  PortRes[PortBufferNum].Port_Type = PortNum;             //记录使用资源的端口			
		PortRes[PortBufferNum].Port_BufferNum = PortBufferNum;
		PortRes[PortBufferNum].ResState=ConnectState_RECEIVE;		//表资源为接收状态
    PortRes[PortBufferNum].Pro_Status = 0; 
    PortRes[PortBufferNum].Pro_Lenth=0;	   		              //协议的实际长度，实时长度
		ReceMainDeviceFun0(PortBufferNum,data);
	}
	else if(PortRes[PortBufferNum].ResState==ConnectState_RECEIVE)
	{
		if(PortRes[PortBufferNum].Port_Type==PortNum) //是不是这个端口再接收数据
		{ 		    
      Protocol_timer_open(PortBufferNum,PortIntervalTime);
			ReceMainDeviceFun0(PortBufferNum,data);
    }
	} 	
}

/////////////////////发送函数处理/////////////////////////////////
// 功能描述  : 通道0的协议发送数据接口 串口中断中使用
// 输入参数  : ComNum.//端口号
void BackComm0_TX(u8 ComNum,u8 PortBufferNum)
{	 
	switch(PortRes[PortBufferNum].SendMode)
	{
		case 1://分包发送
			if(PortRes[PortBufferNum].Pro_Lenth==0)//发送结束
			{	
				Init_protocol(PortBufferNum);
				STM32_USARTx_TX_Finish(ComNum);
			}
			else
			{ 			
				if(PortRes[PortBufferNum].SendCount % 20 == 0)
				{
					if(PortRes[PortBufferNum].Pro_Lenth==0)//发送结束
					{	
						Init_protocol(PortBufferNum);
						STM32_USARTx_TX_Finish(ComNum);
					}
					else
					{
						STM32_USARTx_TX_Finish(ComNum);
						SetTimer(PortDelaySendTimer,PortDelaySendTime);
					}
				}
				else
				{
					STM32_USART_SendData(ComNum,*PortRes[PortBufferNum].ptr_sending++);
					PortRes[PortBufferNum].Pro_Lenth--;
					PortRes[PortBufferNum].SendCount++;
				}
			}  
			break;
    #if Bluetooth4_Open
		case 2:
			switch(PortRes[PortBufferNum].SendCount)
			{
				case 1:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0x24);
				break;
				case 2:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0x00);
				break;
				case 3:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0x01);
				break;
				case 4:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0xFF);
				break;
				case 5:
					PortRes[PortBufferNum].SendMode=0;
					STM32_USARTx_TX_Finish(ComNum);
				break;
			}
			break;
    #endif
		default:
			if(PortRes[PortBufferNum].Pro_Lenth==0)//发送结束
			{	
				Init_protocol(PortBufferNum);
				STM32_USARTx_TX_Finish(ComNum);
			}
			else
			{ 			
				STM32_USART_SendData(ComNum,*PortRes[PortBufferNum].ptr_sending++);
				PortRes[PortBufferNum].Pro_Lenth--;
			}  
			break;
	}
}
///////////////////////////////////////////
// 功能描述  : 复位发送缓冲区
void ResetBuffer(void)
{		
	(*ptr_PortResP).ptr_sending = &(*ptr_PortResP).ProBuffer.DaBuf[0];
	(*ptr_PortResP).Pro_Lenth = 0;
	(*ptr_PortResP).SendCount = 0;
}
// 功能描述  : 将要发送的字节送入发送缓冲区
// 输入参数  : TempChar 发送数据.
void SendByte(u8 TempChar)
{	
  if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
  {
    *(*ptr_PortResP).ptr_sending++=TempChar;
    (*ptr_PortResP).Pro_Lenth++;
  }
}
// 功能描述  : 发送n个字节
// 输入参数  : pchar 发送数据 count 长度.
void SendHexBytes(u8 *pchar,u8 count)
{
	if(count)
	{
		while(count--)	
			SendByte(*pchar++);
	}
}
/////////////
// 功能描述  : 发送一个字节，一个字节需分解成两个ASCII码发送，先发送High 4位，后发送Low 4位
// 输入参数  : c1 发送数据.
void SendOneHexToTwoAsciiByte(u8 c1)
{
	SendByte(Hi_HexToAsc(c1));
	SendByte(Low_HexToAsc(c1));
}
// 功能描述  : 送两个字符数
void Send_Word(u16 TemInt)  
{	
	u8  Te;
	Te = (u8 )(TemInt>>8);
	SendOneHexToTwoAsciiByte(Te);
	SendOneHexToTwoAsciiByte(TemInt);
}
// 功能描述  : 发送n个字节，即2*n个ASCII码
// 输入参数  : pchar 发送数据 count 长度.
void SendAsciiBytes(u8 *pchar,u8 count)
{
	if(count)
	{
		while(count--)	
			SendOneHexToTwoAsciiByte(*pchar++);
	}
}
// 功能描述  : 发送一个Float型数据
void SendOCEDataFloat(float ocedt)   
{
	uni_float ddd;
	uni_float *pidat=&ddd;
	pidat->zfloat=(float)ocedt;
	{	
		SendOneHexToTwoAsciiByte(pidat->byte[0]);
		SendOneHexToTwoAsciiByte(pidat->byte[1]);
		SendOneHexToTwoAsciiByte(pidat->byte[2]);
		SendOneHexToTwoAsciiByte(pidat->byte[3]);
	}
}
//////////////////////////////////////////////////////
// 功能描述  : 开始发送缓冲区数据
void StartSend(void)
{	 
  u8 i;
  for(i=0;i<Port_Buffer_Count;i++)
  {
    if(PortRes[i].ResState==ConnectState_DATA_DEAL)//第一通道 此通道数据已经处理	
    {	   		
      PortRes[i].ResState=ConnectState_SEND;  /*置正在发送状态*/ 	   			
      switch(PortRes[i].Port_Type)
      {
        case COM0://串口1
          if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
          {
            PortRes[i].ptr_sending = &PortRes[i].ProBuffer.DaBuf[0];				    
            GPIO485Direction_Send(); 		
            STM32_USART_SendData(COM0,*PortRes[i].ptr_sending++);
            STM32_USART_enable(COM0);
            (*ptr_PortResP).Pro_Lenth--;
						(*ptr_PortResP).SendCount++;
          }
          else
          {
            Init_protocol(i);
          }
          break;
        case COM1://串口2
          if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
          {
            PortRes[i].ptr_sending = &PortRes[i].ProBuffer.DaBuf[0];				     		
            STM32_USART_SendData(COM1,*PortRes[i].ptr_sending++);
            STM32_USART_enable(COM1);
            (*ptr_PortResP).Pro_Lenth--;
						(*ptr_PortResP).SendCount++;
          }
          else
          {
            Init_protocol(i);
          }
          break;
				#if CPU_Type == CPU_STM32F103	
				case COM2://串口3
          if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
          {
            PortRes[i].ptr_sending = &PortRes[i].ProBuffer.DaBuf[0];				     		
            STM32_USART_SendData(COM2,*PortRes[i].ptr_sending++);
            STM32_USART_enable(COM2);
            (*ptr_PortResP).Pro_Lenth--;
						(*ptr_PortResP).SendCount++;
          }
          else
          {
            Init_protocol(i);
          }
          break;
				#endif
        default:
          PortRes[i].ResState=ConnectState_IDLE;  /*空闲状态*/
          break;
      }
    } 
  }
}
// 功能描述  : 开始发送缓冲区数据
void TimerDelayStartSend(u8 ComNum,u8 PortBufferNum)
{	 
	if(PortRes[PortBufferNum].ResState==ConnectState_SEND)//第一通道 此通道数据已经处理	
	{	   			   	
		if(ComNum == COM0)
		{
			GPIO485Direction_Send();
		}
		STM32_USART_SendData(ComNum,*PortRes[PortBufferNum].ptr_sending++);
		STM32_USART_enable(ComNum);
		PortRes[PortBufferNum].Pro_Lenth--;
		PortRes[PortBufferNum].SendCount++;
  }
}
// 功能描述  : 开始发送缓冲区数据
void TimerDelaySendReply(u8 ComNum,u8 PortBufferNum)
{	 
	if(PortRes[PortBufferNum].ResState==ConnectState_RECEIVE)//第一通道 此通道数据已经处理	
	{	 
		if(ComNum == COM0)
		{
			GPIO485Direction_Send();
		}
		PortRes[PortBufferNum].SendCount = 1;
		STM32_USART_SendData(ComNum,0xAA);
		STM32_USART_enable(ComNum);
	}
	else
	{
		SetTimer(PortDelaySendTimer,PortDelaySendTime);
	}
}
/////////////////////////////////////////////////////////////////////
// 功能描述  : 协议处理
void DealProtocol(void)
{	
  if(PortRes[0].ResState==ConnectState_RECEIVE_FINISH)//表一条协议已接收完或者为主发时已准备完毕
  {	
    PortRes[0].ResState=ConnectState_DATA_DEAL;//此通道数据处理
    ptr_PortResP = &PortRes[0];		
	
    switch(PortRes[0].Pro_Type)
    {
      case 0:				//邮电部协议 					
        DealPostProtocol();	 
        break;
		
      case 1:				//门禁协议
        DealCLDoorProtocol();  
        break;
     
      default:
        PortRes[0].ResState=ConnectState_IDLE;
        break;
    }	
	
    if(PortRes[0].ResState==ConnectState_DATA_DEAL)
    {
      PortRes[0].ResState=ConnectState_IDLE;
    } 			
  }
}  



