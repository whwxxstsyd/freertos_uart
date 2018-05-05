//zhj
//v1.0
//20151219

#include "PostProtocolDeal.h"
#include "globe.h"
#include "config.h"

#define   POST_VER		   	0x10 //协议版本	  
u8    RTN=0;
u8    Var;
u16	  CallSystemAddr;

// 功能描述  : 邮电部协议发送数据帧头部分
// 输入参数  : RTN 返回状态.
void SendHead(void)
{	
	u8 CID1;
	ResetBuffer();
	SendByte('~');
//  if(Var == 0x20)
//  {
//    SendOneHexToTwoAsciiByte(Var);
//  }
//  else
//  {
//    SendOneHexToTwoAsciiByte(POST_VER);
//  }
	SendOneHexToTwoAsciiByte(Var);
	if( (CallSystemAddr & 0x00ff) == 0x00ff ) CallSystemAddr = SysConfig.ModAddress;
	SendOneHexToTwoAsciiByte(CallSystemAddr);
  if(Var == 0x20)
  {
    CID1=0xD0;
  }
  else
  {
    CID1=0x80+((CallSystemAddr>>8)&0x0F);
  }
	SendOneHexToTwoAsciiByte(CID1);
	SendOneHexToTwoAsciiByte(RTN);  //RTN
	SendOneHexToTwoAsciiByte(0);	  //虚拟发送数据体长度
	SendOneHexToTwoAsciiByte(0);
}
// 功能描述  : 邮电部协议回发的校验和函数（要先HEX转成ASC再求校验和）
// 输入参数  : len 校验数据长度.
void SendPostChksum(void)  
{
	u8 *p;
	u16 i,ChksumData=0;
	u8 tempSum;	

	p=&(*ptr_PortResP).ProBuffer.DaBuf[1];   
	for(i=0;i<(*ptr_PortResP).Pro_Lenth-1;i++)
	{	
		ChksumData+=*p++;
	}
	ChksumData=~ChksumData+1;

	tempSum=(u8)(ChksumData>>8);
	SendOneHexToTwoAsciiByte(tempSum);
	SendOneHexToTwoAsciiByte((u8)ChksumData);	 
}
// 功能描述  : 修正发送缓冲固定位置参数
// 输入参数  : FixupNo 位置 valus 参数.
void SendFixupByte(u8 FixupNo,u8 valus)
{
	(*ptr_PortResP).ProBuffer.DaBuf[FixupNo]=Hi_HexToAsc(valus);
	(*ptr_PortResP).ProBuffer.DaBuf[FixupNo+1]=Low_HexToAsc(valus);
}
// 功能描述  : 邮电部协议发送数据长度
void SendDataLen(void)
{
	Taglchksum  tmplen;
	u16 word;
	u8 temp;

	tmplen.word = (*ptr_PortResP).Pro_Lenth-13;
	tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
	tmplen.div.lChksum=~tmplen.div.lChksum+1;
	word=tmplen.word;

	temp=word>>8;
	SendFixupByte(9,(u8)temp);
	SendFixupByte(11,(u8)word);
}
// 功能描述  : 邮电部协议发送帧尾
void SendTail(void)
{	
	SendByte(0x0d);
}
// 功能描述  : 邮电部协议数据长度校验
u8 ChkPostDataSum(void)
{
	Taglchksum  tmplen;
	u16  word;

	word=TwoCharToInt((*ptr_PortResP).ProBuffer.DaBuf[5],(*ptr_PortResP).ProBuffer.DaBuf[6]);	//收到的信息体长度
	tmplen.word = (*ptr_PortResP).Pro_Lenth-18;					//实际收到的信息体累加的长度,再对些计算校验 
	tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
	tmplen.div.lChksum=~tmplen.div.lChksum+1;
	
	if(word==tmplen.word)
		return 1;
	else 
		return 0;
}
// 功能描述  : 邮电部协议应答的命令，无数据体
void SendDefault(void)
{	
	SendHead();
	SendDataLen();
	SendPostChksum();
	SendTail();
	StartSend();
}
// 功能描述  : 邮电部协议应答的结尾处理 修正长度 加入校验和数据尾 并发送数据
void SendTaiDeal(void)
{	
	SendDataLen();
	SendPostChksum();
	SendTail();
	StartSend();
}
// 功能描述  : 邮电部协议接收数据的校验和算法
// 输入参数  : p1 要校验的数据 ReDaLen 数据长度.
u8 chk_post_chksum(u8  *p1,u16  ReDaLen)  
{	
  u16 i,csA,csB,Sum=0;
	u8 TA,TB,TeC;

	for(i=0;i<ReDaLen;i++)
	{	
		Sum += *p1++;
	}
	Sum = ~Sum+1;
	TA = *p1++;
	TB = *p1++;
	TeC=TwoAscTOHex(TA,TB);
	csA = (u16)TeC;
	csA = csA<<8;
	TA = *p1++;
	TB = *p1++;
	TeC=TwoAscTOHex(TA,TB);
	csB =(u16)TeC;
	if((u16)Sum==(u16)(csA+csB))
	{	
	  return 1;
	}
	else
	{	
	  return 0;
	}
}
// 功能描述  : 邮电部协议解析
void DealPostProtocol(void)
{ 		  
	u8 utemp;	
	//u8 Var; 	
	u8 Cid1;	 			
	Var = TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).ProBuffer.DaBuf[2]);//协议版本	
	CallSystemAddr=TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[3],(*ptr_PortResP).ProBuffer.DaBuf[4]);//上位机呼叫地址 
	Cid1=	TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[5],(*ptr_PortResP).ProBuffer.DaBuf[6]);
	CallSystemAddr+=(Cid1&0x0F)*0x100;
	Cid1&=0xF0;	
	//四门地址
	switch(SysConfig.DeviceType)
	{
		case OneDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		case TwoDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+1)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		case ThreeDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+1
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+2)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		case FourDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+1
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+2
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+3)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		default:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
	}	 
	if((Cid1==0x80)||(Cid1==0x90)||(Cid1==0xD0))
	{	
    utemp=chk_post_chksum((u8 *)&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-6);		
    //判校验和是否正确
    if(utemp==1) 		
    {  
      // 把ASCII转换成HEX
      BufAscToHex(&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-2);
      if((*ptr_PortResP).ProBuffer.PostProDa.Cid2==0xF8)
      {// ISP下载程序
        iap_data.Address = SysConfig.ModAddress;
        goto_iap();	
      }        
      else if((*ptr_PortResP).ProBuffer.PostProDa.Cid2==0x50 || (*ptr_PortResP).ProBuffer.PostProDa.Cid2 == 0x48)	//获取地址 广播直接能通过 不需要获得权限
      {	
        RTN=0;		
        DealPostPro();	
      }  
      else
      {		 				    
        //if((Var==0x10)||(Var==0x20)||(Var==0x21)) //不判断协议版本号
        {	
          if(ChkPostDataSum())
          { 
            if(CallSystemAddr==(SysConfig.ModAddress&0x0fff) || (CallSystemAddr&0x00ff)==0x00ff)
            {
              if(SysData.SysState[0]!=SysState_REALTIME || DoorPara.ProtocolPasswordEnableFlag == 0)
              {
                RTN=0;      
                if(SysData.SysState[0] == SysState_CONFIG)
                  SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime);
                DealPostPro();
              }
              else
              {
                RTN = RTN_NOACCESS;
              }
            }
						else if(CallSystemAddr==(SysConfig.ModAddress&0x0fff)+1)
						{
							if(SysData2.SysState[0]!=SysState_REALTIME || DoorPara2.ProtocolPasswordEnableFlag == 0)
							{
								RTN=0;       
								if(SysData2.SysState[0] == SysState_CONFIG)
									SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime);
								DealPostPro();
							}
							else
							{
								RTN = RTN_NOACCESS;
							}
						}
						else if(CallSystemAddr==(SysConfig.ModAddress&0x0fff)+2)
						{
							if(SysData3.SysState[0]!=SysState_REALTIME || DoorPara3.ProtocolPasswordEnableFlag == 0)
							{
								RTN=0;  
								if(SysData3.SysState[0] == SysState_CONFIG)
									SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime);
								DealPostPro();
							}
							else
							{
								RTN = RTN_NOACCESS;
							}
						}
						else if(CallSystemAddr==(SysConfig.ModAddress&0x0fff)+3)
						{
							if(SysData4.SysState[0]!=SysState_REALTIME || DoorPara4.ProtocolPasswordEnableFlag == 0)
							{
								RTN=0;
								if(SysData4.SysState[0] == SysState_CONFIG)
									SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime);
								DealPostPro();
							}
							else
							{
								RTN = RTN_NOACCESS;
							}
						}
          }
          else
          {	
            RTN = RTN_PARACHAK_ERR;	//参数部分累加和检查不对
          }									
        }
//        else
//        {
//          RTN=RTN_VER_ERR;//版本不符合
//        }
      }
    }
    else
    {
      RTN=RTN_CMDCHK_ERR;//累加和检查不对
    }

		if(RTN)
		{	//信息体错误
			SendDefault();
		}
	}
	else
	{
	  Init_protocol_DATA_DEAL(); 
	}
}
// 功能描述  : 邮电部协议处理
void  DealPostPro(void)
{	
  switch((*ptr_PortResP).Port_Type)
  {
    case COM0:
			Common_PostProtocol();
      #if DOORACTION_OPEN
      if(RTN != RTN_UNKNOW_CMD) return; 
      Door_PostProtocol();
      #endif
			#if ReadCard_Open
			if(RTN != RTN_UNKNOW_CMD) return; 
			Card_PostProtocol();
			#endif
      break;
    case COM1:
		#if CPU_Type == CPU_STM32F103
		case COM3:
		#endif
			Common_PostProtocol();
			break;
    default:
      break;
  }
}


