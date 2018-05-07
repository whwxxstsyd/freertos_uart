//zhj
//v1.0
//20160112

#include "CardPostProtocol.h"
#include "globe.h"
#include "config.h"

#if ReadCard_Open
////////////////////////////////////////////////////////////////
//每周休息日 数据初始化
void Init_CardRestDayMark(u32 EepAddr)
{   
  WeekRestDayStruct WeekRestDay = {0};
  WriteEepromAndChecksum(EepAddr, sizeof(WeekRestDayStruct),(u8 *)&WeekRestDay);
}
//一年内节假休息日 数据初始化
void Init_CardHoliday(u32 EepAddr)	
{	          
  HolidayListStruct tempHoli = {0};                  
  WriteEepromAndChecksum(EepAddr, sizeof(HolidayListStruct), (u8 *)&tempHoli);                      
}
//星期时段-准进时段列表
void Init_SetWeekListSeg(u32 EepAddr)
{		    
  u32 addr = 0;
	u8 i,j;	     
	WorkWeeKDayTimeStruct WorkWeeKDayTime;
	for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//将6个时段BCD转成hex
	{
		WorkWeeKDayTime.TimeRange[i].StartTimeHH = BCDToHex(0x00);
		WorkWeeKDayTime.TimeRange[i].StartTimeMM = BCDToHex(0x00);

		WorkWeeKDayTime.TimeRange[i].EndTimeHH = BCDToHex(0x23);
		WorkWeeKDayTime.TimeRange[i].EndTimeMM = BCDToHex(0x59);
	}
	for(i=0;i<MAX_WorkWeekTimeTable_NUM;i++)     
	{
		for(j=0;j<7;j++)  //每天的：6（段数）*4（占用字节）+1(chk) = 25          
		{  		
			addr = EepAddr+i*175+j*25;  //包括：7（天数）*25=175 	
			WriteEepromAndChecksum(addr,sizeof(WorkWeeKDayTimeStruct),(u8 *)&WorkWeeKDayTime); 	
		}
	}       
}
//工作日时段-准进时段列表
void Init_SetWorkDaySeg(u32 EepAddr)
{       
  u32 addr = 0;
	u8 i  = 0;
	WorkDayTimeStruct WorkDayTime;   
	
	for(i=0; i<WorkDayTimeSEG_NUM;i++)		//将4个时段BCD转成hex
	{
		WorkDayTime.TimeRange[i].StartTimeHH = BCDToHex(0x00)	;
		WorkDayTime.TimeRange[i].StartTimeMM = BCDToHex(0x00)	;
		WorkDayTime.TimeRange[i].EndTimeHH = BCDToHex(0x23)	;
		WorkDayTime.TimeRange[i].EndTimeMM = BCDToHex(0x59)	;
	}
	for(i=0;i<MAX_WorkDayTimeTable_NUM;i++)
	{
		addr = EepAddr + i*(16+1); 
		WriteEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(u8 *)&WorkDayTime); 
	}
}
// 功能描述  : 初始化星期内休息日的准进时段
void Init_SetRestDaySeg(u32 EepAddr)
{	      
	u8 i= 0;
	RestDayTimeStruct RestDayTime;
	
	//将4个时段BCD转成hex			
	for(i=0; i<RestDayTimeSEG_NUM; i++)		//将4个时段BCD转成hex
	{
		RestDayTime.TimeRange[i].StartTimeHH = BCDToHex(0x00);
		RestDayTime.TimeRange[i].StartTimeMM = BCDToHex(0x00);

		RestDayTime.TimeRange[i].EndTimeHH = BCDToHex(0x23);
		RestDayTime.TimeRange[i].EndTimeMM = BCDToHex(0x59);
	} 		
	WriteEepromAndChecksum(EepAddr, sizeof(RestDayTimeStruct),(u8*)&RestDayTime); 
}
//时段表 数据初始化
void Init_TimeListSeg(u8 DoorNumber)
{
  switch(DoorNumber)
  {
    case FIRST_DOOR:
			Init_CardRestDayMark(WeekRestDay_EepAddr);
			Init_CardHoliday(HolidayList_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase_EepAddr);
    break;
		case SECOND_DOOR:
      Init_CardRestDayMark(WeekRestDay2_EepAddr);
			Init_CardHoliday(HolidayList2_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase2_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase2_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase2_EepAddr);
    break;
    case THIRD_DOOR:
      Init_CardRestDayMark(WeekRestDay3_EepAddr);
			Init_CardHoliday(HolidayList3_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase3_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase3_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase3_EepAddr);
    break;
    case FOURTH_DOOR:
      Init_CardRestDayMark(WeekRestDay4_EepAddr);
			Init_CardHoliday(HolidayList4_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase4_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase4_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase4_EepAddr);
    break;
  }
}
//星期时段-准进时段列表
void SetWeekListSeg(u32 EepAddr,u8 *BCtimeP)
{		    
  u32 addr = 0;
	u8 i,j;	     
	WorkWeeKDayTimeStruct WorkWeeKDayTime;
	for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//将6个时段BCD转成hex
	{
		WorkWeeKDayTime.TimeRange[i].StartTimeHH = BCDToHex(*BCtimeP);
		WorkWeeKDayTime.TimeRange[i].StartTimeMM = BCDToHex(*(BCtimeP+1));

		WorkWeeKDayTime.TimeRange[i].EndTimeHH = BCDToHex(*(BCtimeP+2));
		WorkWeeKDayTime.TimeRange[i].EndTimeMM = BCDToHex(*(BCtimeP+3));
	}
	for(i=0;i<MAX_WorkWeekTimeTable_NUM;i++)     
	{
		for(j=0;j<7;j++)  //每天的：6（段数）*4（占用字节）+1(chk) = 25          
		{  		
			addr = EepAddr+i*175+j*25;  //包括：7（天数）*25=175 	
			WriteEepromAndChecksum(addr,sizeof(WorkWeeKDayTimeStruct),(u8 *)&WorkWeeKDayTime); 	
		}
	}       
}
void Init_SetWeekListSeg1(u32 EepAddr)
{
	u8 Data[4] = {0};
	SetWeekListSeg(EepAddr,Data);
}
//////////////////////////////////////////////////////////////////////
//////////////////// 功能块 49 命令/////////////////////////////////////  
// 功能描述  : 设置星期内工作准进时段列表 E1
void Card_SetWorkDaySeg(u32 EepAddr)
{       
	u32 addr = 0;
	uint8_t groupId = 0;
	uint8_t i  = 0;
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 38)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}

	groupId =(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.GroupNO; 
	if(( groupId<=MAX_WorkDayTimeTable_NUM )&&(groupId >=1 ))
	{
		addr = EepAddr + (groupId-1)*(16+1); 
		for(i=0; i<WorkDayTimeSEG_NUM; i++)		//将4个时段BCD转成hex
		{
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM)	;

			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM)	;
		}
		if(WriteEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime))== Error) 
		{	
			RTN = RTN_MODPARA_ERR;
			return;
		}	
	}
	else
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	} 
	SendDefault();
}
// 功能描述  : 设置星期内休息日的准进时段 //E2
void Card_SetRestDaySeg(u32 EepAddr)
{	      
  u32 addr = 0;
	uint8_t groupId = 0;
	uint8_t i= 0;
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 38)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}

	groupId= (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.GroupNO;
	if( groupId== 1)        //休息日时段，只支持一张表
	{
	//将4个时段BCD转成hex			
		for(i=0; i<4; i++)		//将4个时段BCD转成hex
		{
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM)	;

			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM)	;
		} 		
		addr = EepAddr+ (groupId-1)*(16+1);
		if(WriteEepromAndChecksum(addr, sizeof(RestDayTimeStruct),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime))==Error)
		{	
			RTN = RTN_MODPARA_ERR;
			return;
		}
	}
	else
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据 
		return;
	}    
	SendDefault();      
}
// 功能描述  : 增加门禁卡 E3
void Card_AddCardList(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)//E3
{
	CardList_Record Record = {0};
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 36)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}

	Record.WhichProtocol = 0;		//协议经由艾贝尔进行配置
	//IC卡号赋值
	Record.CardNo[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[1] ;
	Record.CardNo[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[2] ;
	Record.CardNo[2] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[3] ;
	Record.CardNo[3] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[4] ;
	//用户编号
	Record.CardUseID[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[0] ;
	Record.CardUseID[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[1] ;
	Record.CardUseID[2] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[2] ;
	Record.CardUseID[3] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[3] ;
	//用户密码
	Record.CardUserKey[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserPassword[0] ;
  Record.CardUserKey[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserPassword[1] ;
	//用户有效期
	Record.CardUsefulLife = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Century<<24;
	Record.CardUsefulLife += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Year<<16;
	Record.CardUsefulLife += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Month<<8;
	Record.CardUsefulLife += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Day;
	//卡号权限
  Record.UserVIP = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserPermission;
	
	if(CardLibrary_AddCard(CardLibraryParaEepAddr,CardLibraryParaPointer,&Record)== Error)//增加卡
	{
			RTN = RTN_MODPARA_ERR;                 
			return;
	}

	SendDefault();  
}
// 功能描述  : 删除卡信息 E4
void Card_DelCardList(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer)//E4
{	    
    CardList_Record tempRecord = {0};
    uint8_t mode = 0;
		uint16_t  recvDataLen ;         
		//Length中高4位用于校验  
		recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
		ByteEndian(2,(uint8_t*)&recvDataLen);
		recvDataLen&=0x0fff;
		if( recvDataLen != 16)
		{
			RTN = RTN_UNVALID_DATA;                  //具有无效数据
			return;
		}

		mode = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.DelMode; 
		if(mode == DELMODE_BYUSERCARDNO)            //根据用户卡号进行删除
		{
				//删除对应卡号
				tempRecord.CardNo[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[1];
				tempRecord.CardNo[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[2];
				tempRecord.CardNo[2] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[3];
				tempRecord.CardNo[3] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[4];
				if(Right != CardLibrary_DelCard(EEPCardRecordStartAddr,CardLibraryParaPointer,tempRecord.CardNo))
				{
						RTN = RTN_NOHAVE_INFO;          //无该信息项
						return;
				}	
		}
		else if(mode == DELMODE_BYUSERID)       //根据用户ID号码进行删除
		{
				RTN = RTN_UNVALID_DATA;                  //具有无效数据
				return;
				//暂不支持
		}
		else if(mode == DELMODE_ALL) 
		{
				if(EEPCardRecordStartAddr == EEP_CardRecord_StartAddr) 
					CardLibrary_Clear(CardLibraryPara_EepAddr,CardLibraryParaPointer);                //删除全部卡库   
				else if(EEPCardRecordStartAddr == EEP_CardRecord2_StartAddr) 
					CardLibrary_Clear(CardLibraryPara2_EepAddr,CardLibraryParaPointer);                //删除全部卡库   
				else if(EEPCardRecordStartAddr == EEP_CardRecord3_StartAddr) 
					CardLibrary_Clear(CardLibraryPara3_EepAddr,CardLibraryParaPointer);                //删除全部卡库   
				else if(EEPCardRecordStartAddr == EEP_CardRecord4_StartAddr) 
					CardLibrary_Clear(CardLibraryPara4_EepAddr,CardLibraryParaPointer);                //删除全部卡库   		
		}
		else            
		{
				RTN = RTN_UNVALID_DATA;                  //具有无效数据
				return;
		}

		SendDefault();       
}
// 功能描述  : 设置每周休息日 //EA
void Card_SetRestDayMark(u32 EepAddr)
{   
    u32 addr = 0;
		uint16_t  recvDataLen ;         
		//Length中高4位用于校验  
		recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
		ByteEndian(2,(uint8_t*)&recvDataLen);
		recvDataLen&=0x0fff;
		if( recvDataLen != 8)
		{
			RTN = RTN_UNVALID_DATA;                  //具有无效数据
			return;
		}
    
    if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[0] >7)
        ||((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[0] <1))
    {
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[0] = 0xFF;     //为节省EEPROM写次数，采用0xff
    } 
    
    if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[1] >7)
        ||((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[1] <1))
    {
      (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[1] = 0xFF;
		}        
    addr = EepAddr;
    if(WriteEepromAndChecksum(addr, sizeof(Ariber_SetWeekRestDay),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay))==Error)
		{
			RTN = RTN_MODPARA_ERR;
			return;
		}
    SendDefault();       
}

// 功能描述  : 设置节假休息日 //EB
void Card_AddHoliday(u32 EepAddr)  
{     
  uint8_t chkFlag = ERROR;
	u32 addr = 0;
	uint8_t i,j;
	HolidayStruct	Holiday;
  HolidayListStruct tempHoli = {0};
	uint16_t  recvDataLen ;         
	//Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 8)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}
 
	addr = EepAddr;

	Holiday.Month = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Month);
	Holiday.Day = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Day);
	if(Holiday.Month>12||Holiday.Month<1||Holiday.Day>31||Holiday.Day<1)
	{
	  RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}
	else
	{  
		ReadEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli);	
    if(tempHoli.Count > MAX_Holiday_NUM)  //20150907   
		{
			tempHoli.Count = 0;            //写入节假日数目
			i=0;
			chkFlag=SUCCESS;        
		}
		if(tempHoli.Count < MAX_Holiday_NUM)    
		{
			for(i=0;i<tempHoli.Count;i++)
			{
				if(tempHoli.Day[i].Month>=Holiday.Month) //先比较月份
				{	
					for(;i<tempHoli.Count;i++)//月相同再比较日期
					{
						if(Holiday.Month == tempHoli.Day[i].Month)
						{
							if(tempHoli.Day[i].Day>=Holiday.Day)
							{
								break;
							}
						}
						else	break;
					}
					break;
				}
			}
			if(i==tempHoli.Count)
			{//在最后添加新日期
				chkFlag=SUCCESS;
			}
			else if((tempHoli.Day[i].Month!=Holiday.Month)||(tempHoli.Day[i].Day!=Holiday.Day))
			{//添加新的节假日
				for(j=tempHoli.Count;j>i;j--)
				{
				   tempHoli.Day[j].Month=tempHoli.Day[j-1].Month ;
				   tempHoli.Day[j].Day=tempHoli.Day[j-1].Day ;
				}
				chkFlag=SUCCESS;  				
			} 			
		}
		else
		{
			RTN = RTN_MEMORY_FULL;
			return;
		}

		if(chkFlag==SUCCESS)
		{//执行添加节日动作
			tempHoli.Day[i].Month=Holiday.Month ;
			tempHoli.Day[i].Day=Holiday.Day ;
			tempHoli.Count++;
			if(WriteEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli)==Error)
			{
				RTN = RTN_MODPARA_ERR;
				return;
			}
		}
	}  	    
  SendDefault();         
}
// 功能描述  : 删除节假休息日 //EC
void Card_DelHoliday(u32 EepAddr)	
{	    
  u32 addr = 0;	    
  uint8_t i = 0;  
  uint8_t chkFlag = ERROR;  
  HolidayStruct	Holiday;
  HolidayListStruct tempHoli = {0};
  uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
  recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
  ByteEndian(2,(uint8_t*)&recvDataLen);
  recvDataLen&=0x0fff;
  if( recvDataLen != 8)
  {
    RTN = RTN_UNVALID_DATA;                  //具有无效数据
    return;
  }

	addr = EepAddr;
      
  Holiday.Month = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Month);
  Holiday.Day = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Day);	           
  if((Holiday.Month == 0)&&(Holiday.Day ==0))           // 当月与日都为0，则表示需要删除全部              
  {	             
    tempHoli.Count=0;
    chkFlag=SUCCESS;     
  }  
  else            
  {
    if(( Holiday.Month <= 12)&&( Holiday.Month  >=1))                  
    {        
      if(( Holiday.Day<= 31)&&( Holiday.Day >=1))     //不做瑞月处理，按最大31天计算                      
      {
        if(ReadEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli)==Error)
        {
          RTN = RTN_MODPARA_ERR;
          return;
        }
        if(tempHoli.Count == 0)
        {
          RTN = RTN_MEMORY_EMPTY;
          return;
        }
        if(tempHoli.Count > MAX_Holiday_NUM)  tempHoli.Count =  MAX_Holiday_NUM;

        for(i=0; i<tempHoli.Count; i++)                     //比较是否之前已存储过，对应节假日
        {
          if((tempHoli.Day[i].Month==Holiday.Month)&&(tempHoli.Day[i].Day==Holiday.Day))
          {								
            for(;i<tempHoli.Count;i++)
            {
              tempHoli.Day[i].Month=tempHoli.Day[i+1].Month ;
              tempHoli.Day[i].Day=tempHoli.Day[i+1].Day ;
            }
            tempHoli.Count--;
            chkFlag = SUCCESS;
            break;
          }
        }  
      }
      else      
      {
        RTN = RTN_UNVALID_DATA ;
        return;
      }
    }
    else
    {
      RTN = RTN_UNVALID_DATA;                  //具有无效数据
      return;
    }       
  }         
  if(chkFlag == SUCCESS)
  {              
    if(WriteEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli)==Error)
    {
      RTN = RTN_MODPARA_ERR;
      return;
    }
  }
  else      
  {
    RTN = RTN_NOHAVE_INFO ;
		return;
  }       
          
  SendDefault();  
}

// 功能描述  : 设置星期准进时段列表//F1
void Card_SetWeekListSeg(u32 EepAddr) 
{		    
  u32 addr = 0;
	uint8_t i = 0;
	uint16_t  recvDataLen ;         
	//Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 56)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}

  addr = EepAddr;  
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.TableNO < MAX_WorkWeekTimeTable_NUM)     
	{
		if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.WeekDay <=7)
					&&((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.WeekDay>=1))
		{  			
			for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//将6个时段BCD转成hex
			{
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeHH)	;
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeMM)	;
	
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeHH)	;
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeMM)	;
			}
			
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.TableNO)*175; //包括：7（天数）*(6（时段）*4（占用字节)+1(chk))+=175 
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.WeekDay-1)*25;   //每天的：6（段数）*4（占用字节）+1(chk)	
			if(WriteEepromAndChecksum(addr, sizeof(WorkWeeKDayTimeStruct),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime))==Error)
			{
				RTN = RTN_MODPARA_ERR;
				return;
			}
		}
		else
		{
			RTN = RTN_UNVALID_DATA;                  //具有无效数据 
			return;
		}
	}
	else
	{
		RTN = RTN_MEMORY_EMPTY;                  //具有无效数据
		return;
	}      
  SendDefault();         
}
// 功能块 49 命令
void Card_SetSysParaAndRecord(void)
{          
  //组命令号,按照艾贝尔协议，所有设置命令皆需要已通过密码校验才可以工作，根据调试软件目前暂不支持，
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Cmd_Group==0xF1)
	{	//组内命令
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Cmd_Type)
		{	
      case 0xE1:				//设置工作日的准进时段（按工作日/休息日形式管理时才须设置）
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetWorkDaySeg(WorkDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetWorkDaySeg(WorkDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetWorkDaySeg(WorkDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetWorkDaySeg(WorkDayTimeBase4_EepAddr);
        break;
      case 0xE2:				//设置 非工作日（节假日或星期内休息日）的准进时段 
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetRestDaySeg(RestDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetRestDaySeg(RestDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetRestDaySeg(RestDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetRestDaySeg(RestDayTimeBase4_EepAddr);
        break;
      //卡库
      case 0xE3:				//增加用户（至门控制器）
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_AddCardList(CardLibraryPara_EepAddr,&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_AddCardList(CardLibraryPara2_EepAddr,&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_AddCardList(CardLibraryPara3_EepAddr,&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_AddCardList(CardLibraryPara4_EepAddr,&CardLibraryPara4);
        break;
      case 0xE4:				//删除用户（从门控制器）
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_DelCardList(EEP_CardRecord_StartAddr,&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_DelCardList(EEP_CardRecord2_StartAddr,&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_DelCardList(EEP_CardRecord3_StartAddr,&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_DelCardList(EEP_CardRecord4_StartAddr,&CardLibraryPara4);
        break;	
      case 0xEA:				//设定SM的星期内休息日
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetRestDayMark(WeekRestDay_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetRestDayMark(WeekRestDay2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetRestDayMark(WeekRestDay3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetRestDayMark(WeekRestDay4_EepAddr);
        break;	 
      case 0xEB:				//设定SM的节假日（不包括星期内休息日）
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_AddHoliday(HolidayList_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_AddHoliday(HolidayList2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_AddHoliday(HolidayList3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_AddHoliday(HolidayList4_EepAddr);
        break;
      case 0xEC:				//删除SM内的一组节假日
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_DelHoliday(HolidayList_EepAddr); 
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_DelHoliday(HolidayList2_EepAddr); 
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_DelHoliday(HolidayList3_EepAddr); 
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_DelHoliday(HolidayList4_EepAddr); 
        break;	    
      case 0xF1:				//SU设定SM内按星期准进的时间段列表
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetWeekListSeg(WorkWeekDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetWeekListSeg(WorkWeekDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetWeekListSeg(WorkWeekDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetWeekListSeg(WorkWeekDayTimeBase4_EepAddr);
        break;

      default:					//不能识别的命令格式
        RTN = RTN_UNKNOW_CMD;			
      break;
		}
	}
	else
	{	
		RTN=RTN_UNKNOW_CMD;	//不能识别的命令格式
	}	        
}
///////////////// 功能块  : 4A 命令//////////////////////////////////////////  
// 功能描述  : 获取工作时间段 E3
void Card_ReadWorDaykSeg(u32 EepAddr)
{      
	WorkDayTimeStruct      SegTime;             //支持4个时段
	u32    addr = 0;  
	uint8_t 		groupId = 0; 
	uint8_t 		i = 0;
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 6)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}
	
	groupId = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[0];
	if(( groupId >MAX_WorkDayTimeTable_NUM )||(groupId == 0 ))
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	} 
	
	addr = EepAddr;
	addr += (groupId-1)*17;
	ReadEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(uint8_t *)&SegTime.TimeRange) ; 
	for(i=0; i<WorkDayTimeSEG_NUM; i++)		//将4个时段BCD转成hex
	{
		SegTime.TimeRange[i].StartTimeHH = HexToBCD(SegTime.TimeRange[i].StartTimeHH)	;
		SegTime.TimeRange[i].StartTimeMM = HexToBCD(SegTime.TimeRange[i].StartTimeMM)	;

		SegTime.TimeRange[i].EndTimeHH = HexToBCD(SegTime.TimeRange[i].EndTimeHH)	;
		SegTime.TimeRange[i].EndTimeMM = HexToBCD(SegTime.TimeRange[i].EndTimeMM)	;
	}  
	
	SendHead();  
	SendAsciiBytes((uchar*)&SegTime.TimeRange,sizeof(WorkDayTimeStruct));
	SendTaiDeal();                   
}
// 功能描述  : 获取休息日时间段//E4
void Card_ReadRestDaySeg(u32 EepAddr)
{     
  RestDayTimeStruct      SegTime;             //支持4个时段
  u32    addr = 0;  
	uint8_t 		groupId = 0;
	uint8_t 		i = 0;
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 6)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}

	groupId = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[0];	     
	if( groupId !=1 )
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	} 
	
	addr = EepAddr ;
	addr += (groupId-1)*17;
	ReadEepromAndChecksum(addr, sizeof(RestDayTimeStruct),(uint8_t *)&SegTime.TimeRange) ; 

	for(i=0; i<RestDayTimeSEG_NUM; i++)		//将4个时段BCD转成hex
	{
		SegTime.TimeRange[i].StartTimeHH = HexToBCD(SegTime.TimeRange[i].StartTimeHH)	;
		SegTime.TimeRange[i].StartTimeMM = HexToBCD(SegTime.TimeRange[i].StartTimeMM)	;

		SegTime.TimeRange[i].EndTimeHH = HexToBCD(SegTime.TimeRange[i].EndTimeHH)	;
		SegTime.TimeRange[i].EndTimeMM = HexToBCD(SegTime.TimeRange[i].EndTimeMM)	;
	}  

	SendHead();  
	SendAsciiBytes((uchar*)&SegTime.TimeRange,sizeof(RestDayTimeStruct));
	SendTaiDeal();                   
}
// 功能描述  : 获取卡用户数//E5
void Card_ReadCardListSum(CardLibraryParaStruct *CardLibraryParaPointer)
{       
  uint16_t listSum = 0;
  
  listSum = CardLibraryParaPointer->CardCount; 
  SendHead(); 
  SendOneHexToTwoAsciiByte((uint8_t)listSum);        //小端模式  //用户卡数，edit by zl
  SendOneHexToTwoAsciiByte((uint8_t)(listSum>>8));     
  SendTaiDeal();    
}
///////////////////////////读卡库开始/////////////////////////////////////////
// 功能描述  : 返回卡记录 
void Card_MakeUserStruct(CardList_Record *pRecord)
{       
  SendHead();	      
  //IC卡号
  SendOneHexToTwoAsciiByte(0);
  SendOneHexToTwoAsciiByte(pRecord->CardNo[0]);         //名单卡号
  SendOneHexToTwoAsciiByte(pRecord->CardNo[1]); 
  SendOneHexToTwoAsciiByte(pRecord->CardNo[2]); 
  SendOneHexToTwoAsciiByte(pRecord->CardNo[3]);    
  //用户编号
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[0]);         //名单卡号
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[1]); 
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[2]); 
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[3]);
  //用户密码 
  SendOneHexToTwoAsciiByte(pRecord->CardUserKey[0]); 
  SendOneHexToTwoAsciiByte(pRecord->CardUserKey[1]); 
  //有效期限 @edit by zl ，还需修改
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife>>24); 
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife>>16);
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife>>8); 
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife); 
  //权限
  SendOneHexToTwoAsciiByte(pRecord->UserVIP); 

  SendTaiDeal();      
}
// 功能描述  : 读取SM内指定用户的资料 E6
void Card_ReadUserInfo(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{
  uint16_t  recvDataLen ;      
  CardList_Record tempRecord = {0};    
  
  recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			//Length中高4位用于校验
  ByteEndian(2,(uint8_t*)&recvDataLen);
  recvDataLen&=0x0fff;
  if( recvDataLen == 8)		//4个字节 其统计包括Group与Type，与指定位置记录2个字节
  { //如果为读第一条名单时，先进行恢复
    if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[0] == 0x00)
        &&((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[1] == 0x00))
    {  //清楚读取的中间变量,
      CardLibrary_ResetReadCardConfig(EEPCardRecordStartAddr);
    }
    if(Right == CardLibrary_curReadCard(EEPCardRecordStartAddr,CardLibraryParaPointer,&tempRecord) )
    {
      Card_MakeUserStruct(&tempRecord);
        //需要做一个保护机制，保证每次读都能读全
    }
    else
    {
      RTN = RTN_NOHAVE_INFO;
      return;
    }              
  } 
  else if ( recvDataLen == 12)//6个字节，其统计包括Group与Type，与用户编号4个字节
  { //目前调试软件未做，先不支持，为节省code 空间
    RTN = RTN_UNKNOW_CMD;
    return;
  }
  else if ( recvDataLen == 14)//7个字节，其统计包括Group与Type，与用户卡号5个字节   
  { //目前调试软件未做，先不支持，为节省code 空间 
    RTN = RTN_UNKNOW_CMD;
    return;
  }  
  else
  {
    RTN = RTN_UNVALID_DATA;
    return;
  }        
}
///////////////////////////////读卡库结束////////////////////////////////////////
// 功能描述  : 获取每周休息日 //E9
void Card_ReadRestDay(u32 EepAddr) 
{  
  u32 addr = 0;
  WeekRestDayStruct  WeekrestDay = {0};
    
  addr = EepAddr;	     
  ReadEepromAndChecksum(addr, 2,(uchar*)&WeekrestDay) ;   
  
  if((WeekrestDay.Day[0]<1)||(WeekrestDay.Day[0]>7))
  {
    WeekrestDay.Day[0] = 0x00;              //上位机采用0xFF会报警  
  }      
  if((WeekrestDay.Day[1]<1)||(WeekrestDay.Day[1]>7))
  {
    WeekrestDay.Day[1] = 0x00;    
  }

  SendHead(); 
  SendAsciiBytes((uchar*)&WeekrestDay,2);
  SendTaiDeal();
}

// 功能描述  : 获取节假休息日 //EA
void Card_ReadHoliday(u32 EepAddr)
{	   
	u32 addr = 0;	    
	uint8_t i = 0;     
	HolidayListStruct tempHoli = {0};  

	addr = EepAddr;	   		//用于存储所包含的总数
	ReadEepromAndChecksum(addr,sizeof(HolidayListStruct), (uint8_t *)&tempHoli) ; 
  
  if(tempHoli.Count == 0||tempHoli.Count>MAX_Holiday_NUM)//20150906
	{
		RTN = RTN_MEMORY_EMPTY;
		return;
	}

	SendHead();
	SendOneHexToTwoAsciiByte(tempHoli.Count);  //发送节假日总数               
	for(i=0; i<tempHoli.Count; i++)
	{            
		SendOneHexToTwoAsciiByte(HexToBCD(tempHoli.Day[i].Month)) ;  
		SendOneHexToTwoAsciiByte(HexToBCD(tempHoli.Day[i].Day)) ; 
	}       
	SendTaiDeal();         
}

// 功能描述  : 获取工作星期时间段 //EB
void Card_ReadWeekListSeg(u32 EepAddr)
{     
	WorkWeeKDayTimeStruct      WorkDayTime={0};             //支持6个时段
	u32    addr = 0;  
	uint8_t i = 0;
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 8)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}
    
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.TableNO < MAX_WorkWeekTimeTable_NUM)		//协议规定是0~15，而目前我们容量缘故只支持0~9   		
	{//周工作时间准进时间段表号
		if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.WeekDay <=7)
				&&((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.WeekDay>=1))
		{//星期				
			addr = EepAddr ; //周时段表基地址
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.TableNO)*175; //包括：7（天数）*(6（时段）*4（占用字节)+1(chk))=175 
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.WeekDay-1)*25;   //每天的：6（段数）*4（占用字节）+1(chk)
			ReadEepromAndChecksum(addr, sizeof(WorkWeeKDayTimeStruct),(uint8_t *)&WorkDayTime.TimeRange) ;  

			for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//将6个时段BCD转成hex
			{
				WorkDayTime.TimeRange[i].StartTimeHH = HexToBCD(WorkDayTime.TimeRange[i].StartTimeHH)	;
				WorkDayTime.TimeRange[i].StartTimeMM = HexToBCD(WorkDayTime.TimeRange[i].StartTimeMM)	;

				WorkDayTime.TimeRange[i].EndTimeHH = HexToBCD(WorkDayTime.TimeRange[i].EndTimeHH)	;
				WorkDayTime.TimeRange[i].EndTimeMM = HexToBCD(WorkDayTime.TimeRange[i].EndTimeMM)	;
			}
		}
		else
		{
			RTN = RTN_UNVALID_DATA;                  //具有无效数据 
			return;
		}
	}
	else
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据 
		return;
	} 
	
	SendHead();  
	SendAsciiBytes((uchar*)&WorkDayTime.TimeRange,sizeof(WorkWeeKDayTimeStruct));
	SendTaiDeal();                    
}

// 功能块  : 4A 命令
void Card_GetSysParaAndRecord(void)
{	    	    
  //组命令号	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Cmd_Group==0xF2)
	{	//组内命令
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Cmd_Type)
		{	
			//********时段**********
			case 0xE3:				//读取SM的一组工作日前准进时段
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadWorDaykSeg(WorkDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadWorDaykSeg(WorkDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadWorDaykSeg(WorkDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadWorDaykSeg(WorkDayTimeBase4_EepAddr);
				break;
			case 0xE4:				//读取SM的一组非工作日准进时段
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadRestDaySeg(RestDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadRestDaySeg(RestDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadRestDaySeg(RestDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadRestDaySeg(RestDayTimeBase4_EepAddr);
				break;
			//********卡库*******
			case 0xE5:				//读取SM 内已调入的用户数目
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadCardListSum(&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadCardListSum(&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadCardListSum(&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadCardListSum(&CardLibraryPara4);
				break;
			case 0xE6:				//读取SM内指定用户的资料
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadUserInfo(EEP_CardRecord_StartAddr,&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadUserInfo(EEP_CardRecord2_StartAddr,&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadUserInfo(EEP_CardRecord3_StartAddr,&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadUserInfo(EEP_CardRecord4_StartAddr,&CardLibraryPara4);
				break;
			
			case 0xE9:				//读取SM控制的门之设定每星期内固定的休息日
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadRestDay(WeekRestDay_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadRestDay(WeekRestDay2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadRestDay(WeekRestDay3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadRestDay(WeekRestDay4_EepAddr);
				break;
			case 0xEA:				//读取SM控制的门之设定休息日（星期内休息日除外）
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadHoliday(HolidayList_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadHoliday(HolidayList2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadHoliday(HolidayList3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadHoliday(HolidayList4_EepAddr);
				break;
			case 0xEB:				// 读取SM控制的门之按星期一至星期日的准进时段
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase4_EepAddr);
				break;	                    						 	
						 
			default:
				RTN = RTN_UNKNOW_CMD;	// 不能识别的命令板式
			break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;
	}	  	
}
/////////////////// 功能块  : 4B 命令/////////////////////////////////////    
// 功能描述  : 设置卡有效位 1字节卡有效位+1字节高低标志位 E1  
//1字节卡有效位 = （1~8）1字节高低标志位   0默认 1 互换
void Card_SetCardNumValid(u32 DoorParaEepAddr,DoorParaStruct *DoorParaPointer)
{
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x08)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return;
	}

	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0]>9||(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0]==0)
	{
		RTN = RTN_UNVALID_DATA;	
		return;
	}
	DoorParaPointer->Wiegand_CardBits=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0];
	DoorParaPointer->Wiegand_Sequence=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[1];
	WriteEepromAndChecksum(DoorParaEepAddr,sizeof(DoorParaStruct),(uchar *)DoorParaPointer);
	SendDefault();
}	
// 功能描述  : 设置系统状态 E5
u8 Card_SetGetCardID(SysDataStruct	*SysDataPointer)
{
  uint8_t i = 0;
	uint16_t  recvDataLen ;         
  //Length中高4位用于校验  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x06)
	{
		RTN = RTN_UNVALID_DATA;                  //具有无效数据
		return 0;
	}

	SysDataPointer->SysState[0]=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0];
	for(i=0;i<Port_Buffer_Count;i++)
	{  
		if(PortRes[i].ResState==ConnectState_DATA_DEAL)//此通道数据处理
		{	
			SysDataPointer->SysState[1]=i;
		}
	}

	SendDefault();
	return 1;
}
//4B命令组
void Card_SetArmingPara(void)
{      	    
  //组命令号	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Cmd_Group==0xF3)
	{	//组内命令
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Cmd_Type)
		{	
			case 0xE1://设置卡有效位
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_SetCardNumValid(DoorPara_EepAddr,&DoorPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_SetCardNumValid(DoorPara2_EepAddr,&DoorPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_SetCardNumValid(DoorPara3_EepAddr,&DoorPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_SetCardNumValid(DoorPara4_EepAddr,&DoorPara4);
				break;
			case 0xE5://设置系统状态  读取卡号上报
				if(CallSystemAddr==SysConfig.ModAddress)
        {
					if(Card_SetGetCardID(&SysData))
						SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime1);//30秒没有刷卡 则切换到正常状态
				}
        else if(CallSystemAddr==SysConfig.ModAddress+1)
        {
					if(Card_SetGetCardID(&SysData2))
						SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime1);//30秒没有刷卡 则切换到正常状态
				}
        else if(CallSystemAddr==SysConfig.ModAddress+2)
				{	
          if(Card_SetGetCardID(&SysData3))
						SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime1);//30秒没有刷卡 则切换到正常状态
				}
        else if(CallSystemAddr==SysConfig.ModAddress+3)
        {
          if(Card_SetGetCardID(&SysData4))
						SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime1);//30秒没有刷卡 则切换到正常状态
				}
        break;	
					 
			default:
				RTN = RTN_UNKNOW_CMD;	// 不能识别的命令板式
			break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;
	}		//不能识别的命令板式  	
}
//////////////////////////////////////////////////////////////////////////
///////////////// 功能块  : 4C 命令/////////////////////////////// 
//功能描述  : 读取卡有效位	1字节卡有效位+1字节高低标志位 E1
//1字节卡有效位 = （1~8） 1字节高低标志位   0默认  1 互换
void Card_ReadCardNumValid(DoorParaStruct *DoorParaPointer)
{
	SendHead();
	SendOneHexToTwoAsciiByte(DoorParaPointer->Wiegand_CardBits);        //
	SendOneHexToTwoAsciiByte(DoorParaPointer->Wiegand_Sequence);
  SendTaiDeal();  
}
void Card_GetArmingPara(void)
{      	    
    //组命令号	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4C.Cmd_Group==0xF4)
	{	//组内命令
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4C.Cmd_Type)
		{	   
			case 0xE1:				//读取卡有效位
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadCardNumValid(&DoorPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadCardNumValid(&DoorPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadCardNumValid(&DoorPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadCardNumValid(&DoorPara4);
				break;

			default:
				RTN = RTN_UNKNOW_CMD;	// 不能识别的命令板式
			break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;//不能识别的命令板式
	}		  	
}	 
//协议处理
void Card_PostProtocol(void)
{
  u8  Cmd = (*ptr_PortResP).ProBuffer.PostProDa.Cid2;
  RTN = RTN_NORMAL;
	switch(Cmd)
	{	
		case 0x49:	//SU设置SM内部工作参数命令					
		  Card_SetSysParaAndRecord();  	 				
			break;
	  case 0x4A:	//SU读取SM的信息						
	   	Card_GetSysParaAndRecord(); 
			break;
		///////////////NewAdd属于公司独有/////////////
		case 0x4B:	//SU设置SM内部工作参数命令
		  Card_SetArmingPara();	
			break;
		case 0x4C:	//SU读取SM的信息
		  Card_GetArmingPara();	
			break;	

		default:
			RTN= RTN_UNKNOW_CMD;     //无效命令
			break; 
	}
}
#endif


