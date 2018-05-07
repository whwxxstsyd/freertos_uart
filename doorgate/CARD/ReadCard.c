//zhj
//20170712
//F2.1V1.0

#include "readcard.h"	 
#include "globe.h"
#include "config.h"

#if ReadCard_Open

ReadCardStruct  ReadCard[ReadCardNum];//伟根缓冲区


// 功能描述  : 有效期判断
// 输入参数  : pCardInfoTemp 卡信息	
// 返回参数  : 有效期结果
u8 ReadCard_IsDateOfExpired(CardList_Record	*pCardInfoTemp)
{
	uint32_t Date;
	uint16_t temp; 	
	temp = IntHexToIntBCD(SysData.Sys_Date.StuDate.Year+2000);  
	Date = temp<<16; 
	Date+=IntHexToIntBCD(SysData.Sys_Date.StuDate.Month)<<8;
	Date+=IntHexToIntBCD(SysData.Sys_Date.StuDate.Day);
  if(Date < pCardInfoTemp->CardUsefulLife)
	{
		return Right;
	}
	else
	{
	  return Error;
	} 	 		
}
// 功能描述  : 查询	当天是不是 节假日
// 返回参数  : None
u8 ReadCard_IsHoliday(u8 DoorNumber)
{
	u32 addr = 0;   
	u8 i = 0;
	HolidayListStruct tempHoli = {0};//一年内的节假日
	
	if(DoorNumber == FIRST_DOOR) addr  = HolidayList_EepAddr;
	if(DoorNumber == SECOND_DOOR) addr  = HolidayList2_EepAddr;
	if(DoorNumber == THIRD_DOOR) addr  = HolidayList3_EepAddr;
	if(DoorNumber == FOURTH_DOOR) addr  = HolidayList4_EepAddr;

	ReadEepromAndChecksum(addr,sizeof(HolidayListStruct), (u8 *)&tempHoli) ; 
	
	if(tempHoli.Count < MAX_Holiday_NUM)  
	{
		for(i=0; i<tempHoli.Count; i++)
		{
			if((tempHoli.Day[i].Month == SysData.Sys_Date.StuDate.Month)		//年月日相同
				&&(tempHoli.Day[i].Day == SysData.Sys_Date.StuDate.Day))
			{
				return Right;
			}
		}	
	}  
	return Error;	
}

// 功能描述  : 时间比较	   
// 输入参数1 : pSegTime 时段
// 返回参数  : 比较结果
u8 ReadCard_IsValidSegTime(TimeRangeStruct *pSegTime)
{
	uint16_t StartTime;
	uint16_t EndTime;
	uint16_t Time;

	Time= SysData.Sys_Date.StuDate.Hour*0x100+SysData.Sys_Date.StuDate.Minute;
	StartTime= pSegTime->StartTimeHH*0x100+pSegTime->StartTimeMM;
	EndTime= pSegTime->EndTimeHH*0x100+pSegTime->EndTimeMM;
	if( Time>=StartTime)
	{		
		if( Time<EndTime)
		{
			return Right	;	
		}	
	}  
	return Error;
}	 

// 功能描述  : 查询	卡是否在有效时段
// 输入参数1 : pCardInfoTemp 卡的记录
// 返回参数  : None
u8 ReadCard_IsSegTimeValid(u8 DoorNumber,CardList_Record *pCardInfoTemp)
{
	u32 addr = 0;	    
  WorkWeeKDayTimeStruct  WorkWeekDayTime={0};             //支持6个时段
	WorkDayTimeStruct		   WorkDayTime={0};                 //支持4个时段
	RestDayTimeStruct		   RestDayTime={0};                 //支持4个时段
	WeekRestDayStruct		   WeekRestDay={0};                 //2天
	u8 i = 0;

	if((pCardInfoTemp->UserVIP &0xC0) == 0x00)	  	//按星期管理方式
	{
		if(DoorNumber == FIRST_DOOR) addr = WorkWeekDayTimeBase_EepAddr;
		if(DoorNumber == SECOND_DOOR) addr = WorkWeekDayTimeBase2_EepAddr;
		if(DoorNumber == THIRD_DOOR) addr = WorkWeekDayTimeBase3_EepAddr;
		if(DoorNumber == FOURTH_DOOR) addr = WorkWeekDayTimeBase4_EepAddr;

		addr += (pCardInfoTemp->UserVIP&0x0F)*175; //表：包括：7（天数）*(6（时段）*4（占用字节)+1(chk))+=175 
		if(SysData.Sys_Date.StuDate.Week>7 || SysData.Sys_Date.StuDate.Week == 0)
		{
			SysData.Sys_Date.StuDate.Week=1;
		}
		addr += (SysData.Sys_Date.StuDate.Week-1)*25;   //每天的：6（段数）*4（占用字节）+1(chk)		
    ReadEepromAndChecksum(addr, sizeof(WorkWeeKDayTimeStruct),(u8 *)&WorkWeekDayTime.TimeRange) ;  

		for(i=0;i<WorkWeeKDayTimeSEG_NUM;i++)   	//6个时段
		{
			if(ReadCard_IsValidSegTime(&WorkWeekDayTime.TimeRange[i]) == Right)
			{
				return Right;
			}
			else
			{}	//当前时间，该用户无权进入
		}
	}
	else if((pCardInfoTemp->UserVIP &0xC0) == 0x80)	  //按工作日处理方式
	{	    
	    if(DoorNumber == FIRST_DOOR) addr = WeekRestDay_EepAddr;
			if(DoorNumber == SECOND_DOOR) addr = WeekRestDay2_EepAddr; 
			if(DoorNumber == THIRD_DOOR) addr = WeekRestDay3_EepAddr; 
			if(DoorNumber == FOURTH_DOOR) addr = WeekRestDay4_EepAddr; 

	    ReadEepromAndChecksum(addr, sizeof(WeekRestDayStruct),(u8 *)&WeekRestDay.Day) ;    
	    
	    if((WeekRestDay.Day[0]<1)||(WeekRestDay.Day[0]>7))
	    {
	        WeekRestDay.Day[0] = 0x00;              //上位机采用0xFF会报警  
	    }
	    if((WeekRestDay.Day[1]<1)||(WeekRestDay.Day[1]>7))
	    {
	        WeekRestDay.Day[1] = 0x00;    
	    }

		if((SysData.Sys_Date.StuDate.Week == WeekRestDay.Day[0] ) 		// 为休息日，则按照休息日的时段进行判读
		  ||(SysData.Sys_Date.StuDate.Week == WeekRestDay.Day[1] ))
		{  //休息日，获取对应的休息时段	 			
			if(DoorNumber == FIRST_DOOR) addr = RestDayTimeBase_EepAddr;
			if(DoorNumber == SECOND_DOOR) addr = RestDayTimeBase2_EepAddr;
			if(DoorNumber == THIRD_DOOR) addr = RestDayTimeBase3_EepAddr;
			if(DoorNumber == FOURTH_DOOR) addr = RestDayTimeBase4_EepAddr;
      ReadEepromAndChecksum(addr, sizeof(RestDayTimeStruct),(u8 *)RestDayTime.TimeRange); 
			for(i=0;i<RestDayTimeSEG_NUM;i++)	  	//4个时段
			{
				if(ReadCard_IsValidSegTime(&RestDayTime.TimeRange[i]) == Right)
				{ 					
					return Right;
				}
				else
				{}	//当前时间，该用户无权进入
			}										
		}
		else  // 为工作日，则按照工作日的时段进行判读
		{
			if(DoorNumber == FIRST_DOOR) addr = WorkDayTimeBase_EepAddr;
			if(DoorNumber == SECOND_DOOR) addr = WorkDayTimeBase2_EepAddr;
			if(DoorNumber == THIRD_DOOR) addr = WorkDayTimeBase3_EepAddr;
			if(DoorNumber == FOURTH_DOOR) addr = WorkDayTimeBase4_EepAddr;
			addr += ((pCardInfoTemp->UserVIP&0x07)-1)*17;	//
      ReadEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(u8 *)WorkDayTime.TimeRange) ; 
			for(i=0;i<WorkDayTimeSEG_NUM;i++)	  	//4个时段
			{
				if(ReadCard_IsValidSegTime(&WorkDayTime.TimeRange[i]) == Right)
				{ 					
					return Right;
				}
				else
				{}	//当前时间，该用户无权进入
			}		
		} 
	}
	else
	{
		//无效数据
	}  
	return Error;
}
// 功能描述  : 根据卡库信息处理卡号
// 输入参数  : pCardInfoTemp 卡信息
// 输出参数  : pPara  参数信息
void ReadCard_JudgeCardValid(u8 DoorNumber,CardList_Record	*pCardInfoTemp, u8 *pPara)
{
	if((pCardInfoTemp->UserVIP &0xC0) == 0xC0)	  	//特权卡
	{
		//正常刷卡开门
		*pPara = Ariber_CARD_IN_OPEN_EVENT;
	}
	else 			//按星期管理方式/按工作日处理方式
	{
		if(ReadCard_IsDateOfExpired(pCardInfoTemp) == Right)//判断有效期是否已到期,目前时间未定
		{
			if(ReadCard_IsHoliday(DoorNumber) != Right)		//不处于假期时间内
			{
				if(ReadCard_IsSegTimeValid(DoorNumber,pCardInfoTemp) == Right)
				{
					*pPara = Ariber_CARD_IN_OPEN_EVENT;	
				}
				else
				{
					//无效时段刷卡	
					*pPara = Ariber_WEEK_OUT_CARD_OPEN_EVENT;
				}	
			}
			else
			{
				//假期内刷卡
				*pPara = Ariber_WEEK_OUT_CARD_OPEN_EVENT;		
			}		
		}
		else
		{
			//过期卡
			*pPara 	= Ariber_DATAOUT_CARD_OPEN_EVENT;			
		}
	}
}
// 功能描述  : 数据全00判断
// 输入参数1 : pObject 数据.
// 输入参数2 : cnt 个数
// 返回参数  : 0:全00；1：不是全00
u8 CompareZero(u8 *pObject,u8 cnt)
{
	u8 i;
	for(i=0;i<cnt;i++)
	{
		if(*pObject++)
		{
			return 1;
		}
	}
	return 0;
}
// 功能描述  : 数据全FF判断
// 输入参数1 : pObject 数据.
// 输入参数2 : cnt 个数
// 返回参数  : 0:全FF；1：不是全FF
u8 CompareFF(u8 *pObject,u8 cnt)
{
	u8 i;
	for(i=0;i<cnt;i++)
	
	{
		if(*pObject++!=0xFF)
		{
			return 1;
		}
	}
	return 0;
}
// 功能描述  : 刷卡处理函数
// 输入参数  : IDCard 卡号.
void ReadCard_OpenDoorDeal(u8 DoorNumber,SysDataStruct *SysDataP,CardLibraryParaStruct *CardLibraryParaP,u32 EEPCardRecordStartAddr)
{
  u8 Pos,Index;
	CardList_Record Record;		   //卡信息
	EEP_CardSectorStruct EEP_Sector;//EEP中卡扇区 
	u8  eventPara	 = 0;	
	
	if(CardLibrary_CardSearch(EEPCardRecordStartAddr,CardLibraryParaP,&SysDataP->IDCard.IDCardNO[0],&EEP_Sector,&Record,&Index,&Pos )==Right)//查询卡  
	{
		ReadCard_JudgeCardValid(DoorNumber,&Record, &eventPara);  					
	}
	else
	{	    	
		eventPara = Ariber_UNLOW_CARD_OPEN_EVENT;		//未授权卡开门
	}
	//执行对应动作，需要使用的变量
	switch(eventPara)
	{
		case  Ariber_CARD_IN_OPEN_EVENT:	 //正常刷卡开门
      SysDataP->Door_CurAction &= 0xff00;
		  SysDataP->Door_CurAction |= CARD_NORMAL_ACT;	
      break;
		case  Ariber_WEEK_OUT_CARD_OPEN_EVENT:	 //无效时段刷卡
			BEEP_INVALIDCARD();
      Door_StoreRecord_CommonPara(DoorNumber,Ariber_WEEK_OUT_CARD_OPEN_EVENT);
			SetVoiceAction(Card_WeekOut_Voc,1,1);		//现在时刻您的卡不能开门
		break;
		case  Ariber_DATAOUT_CARD_OPEN_EVENT:	 	//过期卡
			BEEP_INVALIDCARD();
      Door_StoreRecord_CommonPara(DoorNumber,Ariber_DATAOUT_CARD_OPEN_EVENT);
			SetVoiceAction(Card_TimeOut_Voc,1,1);		//卡已过期
		break;
		case  Ariber_UNLOW_CARD_OPEN_EVENT:	 	//未授权卡开门
		default:						 									
			BEEP_INVALIDCARD();
      Door_StoreRecord_CommonPara(DoorNumber,Ariber_UNLOW_CARD_OPEN_EVENT);
			SetVoiceAction(Card_Unlow_Voc,1,1);			//卡未授权
		break;
	}
}
// 功能描述  : 将在读卡状态下 刷卡上报
// 输入参数  : pIDCard 卡号.
void ReadCard_CardNoReport(IDCardStruct *pIDCard,SysDataStruct *SysDataP)
{  	
	if(PortRes[SysDataP->SysState[1]].ResState==ConnectState_IDLE)//调试DEBUGFLAG	jin20140419
	{			
		ptr_PortResP=&PortRes[SysDataP->SysState[1]];
		(*ptr_PortResP).ResState=ConnectState_DATA_DEAL;
		RTN = 0xD0;//一定要 0xD0
		SendHead();
		SendOneHexToTwoAsciiByte(0xF4);
		SendOneHexToTwoAsciiByte(0xEC);
		SendOneHexToTwoAsciiByte(0x01);						/*装入门号*/
		SendAsciiBytes((uchar*)pIDCard,sizeof(IDCardStruct));	/*装入卡号*/
		SendTaiDeal();		
	}
}
//读卡功能处理
//i 读卡头号
//DoorNumber 门号 上传卡号判断地址，时段假日信息获取
//DoorParaP	 卡号处理配置
//SysDataP	 读卡状态判断 产生事件
//CardLibraryParaP 卡库参数
//EEPCardRecordStartAddr 卡库地址
void ReadCard_Deal(u8 i,u8 DoorNumber,DoorParaStruct *DoorParaP,SysDataStruct *SysDataP,CardLibraryParaStruct *CardLibraryParaP,u32 EEPCardRecordStartAddr)
{
	IDCardStruct 	IDCard;
	//根据配置获取卡号
	Wiegand_DealData(i,&IDCard.IDCardNO[0],DoorParaP);
	//干扰过滤
	if((CompareZero(&IDCard.IDCardNO[0],sizeof(IDCardStruct))==0)||(CompareFF(&IDCard.IDCardNO[0],sizeof(IDCardStruct))==0))
	{
		return;
	}
	//同张卡刷卡过滤
	if(memcmp(&ReadCard[i].OldIDCard.IDCardNO[0],&IDCard.IDCardNO[0],sizeof(IDCard.IDCardNO))==0)
	{
		return;
	}
	ReadCard[i].OldIDCard=IDCard;
	ReadCard_TimerOpen(i,DoorParaP->BrushSameCardSecondTime*1000);//同张卡过滤时间
	//SysDataP->ReadCardNumber = i;
  SysData.ReadCardNumber = i;
	//状态判断
	if(SysDataP->SysState[0]==SysState_READCARD)
	{
		switch(DoorNumber)
		{
			case FIRST_DOOR:
				CallSystemAddr = SysConfig.ModAddress;
				SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime1);    //启动30秒定时器,时间到自动返回到正常状态
				break;
			case SECOND_DOOR:
				CallSystemAddr = SysConfig.ModAddress+1;
				SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime1);    //启动30秒定时器,时间到自动返回到正常状态
				break;
			case THIRD_DOOR:
				CallSystemAddr = SysConfig.ModAddress+2;
				SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime1);    //启动30秒定时器,时间到自动返回到正常状态
				break;
			case FOURTH_DOOR:
				CallSystemAddr = SysConfig.ModAddress+3;
				SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime1);    //启动30秒定时器,时间到自动返回到正常状态
				break;
		}
		ReadCard_CardNoReport(&IDCard,SysDataP);       //在读卡状态下；将读取的ID卡信息主发到上位机
		BEEP_UploadCard();
	}
	else
	{
		SysDataP->IDCard=IDCard;
		ReadCard_OpenDoorDeal(DoorNumber,SysDataP,CardLibraryParaP,EEPCardRecordStartAddr); 		//处理开门ID
	}
}
//功能描述  :系统处在正常状态 刷卡处理;在读卡状态下 刷卡上报
void ReadCard_Work(void)
{
	
}

#endif
