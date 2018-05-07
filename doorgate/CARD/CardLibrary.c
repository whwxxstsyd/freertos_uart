//zhj
//20170712
//F2.1V1.0

#include "globe.h"
#include "cardlibrary.h"
#include "config.h"

#if ReadCard_Open

CardLibraryStruct CardLibrary;

CardList_Pos    curReadIndexPos = {0};    	//读卡库的时候使用:当前正读取的位置= {0}

//功能描述: 根据卡库参数Eep地址获得卡库起始地址
//入口参数：	CardLibraryParaEepAddr	卡库参数Eep地址
//返回参数：	eeprom的卡库起始地址
u32 CardLibrary_GetEEPCardRecordStartAddr(u32 CardLibraryParaEepAddr)
{
	u32 addr;
	if(CardLibraryParaEepAddr == CardLibraryPara_EepAddr)
		addr = EEP_CardRecord_StartAddr;
	if(CardLibraryParaEepAddr == CardLibraryPara2_EepAddr)
		addr = EEP_CardRecord2_StartAddr;
	if(CardLibraryParaEepAddr == CardLibraryPara3_EepAddr)
		addr = EEP_CardRecord3_StartAddr;
	if(CardLibraryParaEepAddr == CardLibraryPara4_EepAddr)
		addr = EEP_CardRecord4_StartAddr;
	return addr;
}
//功能描述: 根据卡库参数Eep地址获得flash的卡库起始地址
//入口参数：	CardLibraryParaEepAddr	卡库参数Eep地址
//返回参数：flash的卡库起始地址 
u32 CardLibrary_GetFlashCardRecordStartAddr(u32 CardLibraryParaEepAddr)
{
	u32 ADDR;
	if(CardLibraryParaEepAddr == CardLibraryPara_EepAddr) 
		ADDR = FLASH_CardRecord_StartAddr;
  if(CardLibraryParaEepAddr == CardLibraryPara2_EepAddr) 
		ADDR = FLASH_CardRecord2_StartAddr;
  if(CardLibraryParaEepAddr == CardLibraryPara3_EepAddr) 
		ADDR = FLASH_CardRecord3_StartAddr;
  if(CardLibraryParaEepAddr == CardLibraryPara4_EepAddr) 
		ADDR = FLASH_CardRecord4_StartAddr;
	return ADDR;
}
//由eeprom的卡库起始地址得到flash的卡库起始地址
//输入 EEPCardRecordStartAddr	eeprom的卡库起始地址
//返回 flash的卡库起始地址 
u32 CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(u32 EEPCardRecordStartAddr)
{
	u32 ADDR;
	if(EEPCardRecordStartAddr == EEP_CardRecord_StartAddr) 
		ADDR = FLASH_CardRecord_StartAddr;
	if(EEPCardRecordStartAddr == EEP_CardRecord2_StartAddr) 
		ADDR = FLASH_CardRecord2_StartAddr;
	if(EEPCardRecordStartAddr == EEP_CardRecord3_StartAddr) 
		ADDR = FLASH_CardRecord3_StartAddr;
	if(EEPCardRecordStartAddr == EEP_CardRecord4_StartAddr) 
		ADDR = FLASH_CardRecord4_StartAddr;
	return ADDR;
}

//////////////////////初始化///////////////////////////////////
//功能描述: 根据EEP扇区信息 初始化扇区
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
//入口参数	: SectorIndex 索引
//入口参数	: pEEP_Sector	扇区信息
//返回参数	: 返回初始化结果
u8 CardLibrary_SectorInit(u32 CardLibraryParaEepAddr,u8 SectorIndex,EEP_CardSectorStruct *pEEP_Sector)
{
   u32 ADDR;
   u32 EEPCardRecordStartAddr;
   EEPCardRecordStartAddr=CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);
   ADDR=EEPCardRecordStartAddr+(SectorIndex*EEP_CardSector_Size);//获取EEP扇区地址
   WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)pEEP_Sector);
  
	 ADDR=CardLibrary_GetFlashCardRecordStartAddr(CardLibraryParaEepAddr);
   ADDR=ADDR+SectorIndex*FLASH_CardSector_Size; //在flash中的扇区地址
   SST_Earse_Area(ADDR);//擦除flash扇区
   return Right;
}
//功能描述	: 初始化卡库 
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
//入口参数	: CardLibraryParaPointer 卡库参数
//返回参数	: 返回初始化结果
u8 CardLibrary_LibraryInit(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{
   u8 i,Ta;       
   EEP_CardSectorStruct EEP_Sector={0};

   EEP_Sector.Manage.UpSector=0xFF;
   EEP_Sector.Manage.NextSector=0xFF;
   EEP_Sector.Manage.CardCount=0;
   EEP_Sector.Manage.FLASHCardCount=0; 
   
   for(i=0;i<CardLibraryParaPointer->Hash;i++)
   {//hash扇区初始化
	   CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
   } 
   //备用开始扇区初始化    
   EEP_Sector.Manage.UpSector=0xFF;
   EEP_Sector.Manage.NextSector=i+1;//i=29
   EEP_Sector.Manage.CardCount=0;
   EEP_Sector.Manage.FLASHCardCount=0; 
   CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
	 i++;
	 //备用扇区初始化
   Ta=EEP_CardSector_Count-1;
   for(;i<Ta;i++)//i=30
   {
			EEP_Sector.Manage.UpSector=i-1;
      EEP_Sector.Manage.NextSector=i+1; 
			CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
   }
   //备用最后扇区初始化
   EEP_Sector.Manage.UpSector=i-1;
   EEP_Sector.Manage.NextSector=0xFF; 
   CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
   return Right;
}
//功能描述: 上电 初始化卡库
//入口参数：	CardLibraryParaEepAddr	卡库参数Eep地址
//入口参数	: CardLibraryParaPointer 卡库参数
void CardLibrary_Init(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{	
  if(ReadEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash)!=Right)
	{//卡库没有初始化
		CardLibrary_Clear(CardLibraryParaEepAddr,CardLibraryParaPointer);
	}
	else if(CardLibraryParaPointer->Hash != CardLibraryParaHashInit)//参数不对，初始化卡库
	{
		CardLibrary_Clear(CardLibraryParaEepAddr,CardLibraryParaPointer);
	}
}
//功能描述: 卡库清空
//入口参数：	CardLibraryParaEepAddr	卡库参数Eep地址
//入口参数	: CardLibraryParaPointer 卡库参数
void CardLibrary_Clear(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{	
	CardLibraryParaPointer->Hash= CardLibraryParaHashInit;
	CardLibraryParaPointer->CardCount=0;//卡个数
	CardLibraryParaPointer->SectorCount=(u8)(EEP_CardSector_Count); //扇区个数
	CardLibraryParaPointer->UsableListCount=(u8)(EEP_CardSector_Count)-CardLibraryParaPointer->Hash;//未用扇区个数
	CardLibraryParaPointer->UsableListNo=CardLibraryParaPointer->Hash;//备用扇区指针
	WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash);

	CardLibrary_LibraryInit(CardLibraryParaEepAddr,CardLibraryParaPointer);//初始化卡库
}
//////////////////////初始化end///////////////////////////////////

////////////////////////////卡库添加和删除///////////////////////////////////////
// 功能描述  : 卡号比较
// 输入参数1 : pCardNo1 第一张卡号
// 输入参数2 : pCardNo2 第二张卡号
// 返回参数  : 返回比较结果 1 小于 2 大于 0 等于
u8 CardLibrary_CompareCardNo(u8 *pCardNo1, u8 *pCardNo2)
{
	u8 i = 0;
	
	for(i= 0;i<Card_Size;i++)
	{
		if(pCardNo1[i]<pCardNo2[i])
		{
			return 1;		//小于	
		}
		else if(pCardNo1[i]>pCardNo2[i])
		{
			return 2;		//大于
		}
	} 	
	return 0;			    //等于	
}
// 功能描述  : 卡扇区内查询
// 输入参数1 : FLASHCardRecordStartAddr 卡库起始地址
// 输入参数1 : Index 扇区索引
// 输入参数2 : pCardNo 卡号
// 输入参数3 : SectorInfo 扇区卡索引信息
// 输入参数4 : pRecord 卡的记录
// 输入参数5 : pPos    在扇区中的地址
// 返回参数  : 返回查询结果
u8 CardLibrary_CardSectorSearch(u32 FLASHCardRecordStartAddr,u8 Index,u8 *pCardNo,EEP_CardSectorStruct *pEEP_Sector,CardList_Record *pRecord,u8 *pPos )
{	 
	int16_t Result,Low,High,Mid;  	
	u8 i = 0;
	u32 ADDR;
	CardList_Record Record;

	if((*pEEP_Sector).Manage.CardCount < 1)		//参数检查
	{		
		*pPos = 0;
		return Error;
	}
	if((*pEEP_Sector).Manage.CardCount>Card_SectorRecordCount)
	{
		(*pEEP_Sector).Manage.CardCount = Card_SectorRecordCount;
	}
	
	Low = 0;
	High = (*pEEP_Sector).Manage.CardCount-1;
  //采用二分法进行查找
	while(Low<=High)		//折半查找
	{  
 	 	Mid = (Low+High)/2; 
		ADDR=FLASHCardRecordStartAddr+Index*FLASH_CardSector_Size; //在flash中的扇区地址
		ADDR+= (*pEEP_Sector).CardRecord[Mid].FlashPos*FLASH_CardRecord_Size;//在flash中的绝对地址
		Read_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&Record.CardNo);
		Result=CardLibrary_CompareCardNo(pCardNo, Record.CardNo);	   		
		if(Result==1)		//如果是小于
		{
			High = Mid-1;
		}
		else if(Result==2)	//如果是大于
		{
			Low = Mid+1;
		}
		else if(Result==0 )
		{
		 	for(i= 0; i< sizeof(CardList_Record);i++)
				pRecord->CardNo[i] = Record.CardNo[i]; 		 	
		 	*pPos = Mid;
		 	return Right;;
		}
	}
	
	if(Result==1)		//主要用于插入功能
	{
		*pPos = Mid;
	}
	else		//如果大于
	{
		*pPos = Mid+1;	
	}
	return ERROR;  	
}
// 功能描述  : 获取卡号的Hash索引
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数  : pCardNo 卡号指针
// 返回参数  : Index	Hash索引
u8 CardLibrary_GetHashIndex(CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo)
{	 	
	uni_float ufloat; 
	u8 Index;
	ufloat.byte[3]= pCardNo[0];
	ufloat.byte[2]= pCardNo[1];
	ufloat.byte[1]= pCardNo[2];
	ufloat.byte[0]= pCardNo[3];
	Index=ufloat.uzlong % CardLibraryParaPointer->Hash; 
	return Index;
}
// 功能描述  : 卡库查询
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 : pCardNo 卡号
// 输入参数2 : 返回pEEP_Sector 卡的EEP扇区信息
// 输入参数3 : 返回pRecord 卡的记录
// 输入参数4 : 返回Index   扇区索引
// 输入参数5 : 返回pPos    在扇区中位置
// 返回参数  : 返回查询结果
u8 CardLibrary_CardSearch(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo,EEP_CardSectorStruct *pEEP_Sector,CardList_Record *pRecord,u8 *pIndex,u8 *pPos)
{ 			
	u32 ADDR; 	
	
	*pIndex=CardLibrary_GetHashIndex(CardLibraryParaPointer,pCardNo);  	
	while(1)
	{
		ADDR = EEPCardRecordStartAddr+ ((*pIndex)*EEP_CardSector_Size);//获取EEP扇区地址
		if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&(*pEEP_Sector).Manage.UpSector)==Error)
		{//获取校验错误
			return Error;
		} 
		if(((*pEEP_Sector).Manage.CardCount == 0xff)||((*pEEP_Sector).Manage.FLASHCardCount == 0xff))
		{//未格式化
			return Error;	
		}
		ADDR = CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr);
		if(CardLibrary_CardSectorSearch(ADDR,*pIndex,pCardNo,pEEP_Sector,pRecord,pPos)==Right)
		{//查到卡 
			return Right;
		} 		
		(*pIndex)=(*pEEP_Sector).Manage.NextSector;//下一个扇区
		if( (*pIndex)== 0xff)
		{//没有下个扇区
		  return Error;
		}  		
	}	
} 
// 功能描述  : 卡删除
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数  : pCardNo 卡号
// 返回参数  : 返回删除结果
u8 CardLibrary_DelCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo )
{	
	u8  i,Index,Pos;	
	u32 ADDR;
	CardList_Record Record;//卡记录
	EEP_CardSectorStruct EEP_Sector;//EEP中卡扇区

	if(CardLibrary_CardSearch(EEPCardRecordStartAddr,CardLibraryParaPointer,pCardNo,&EEP_Sector,&Record,&Index,&Pos)==Right)
	{  
		for(i=Pos+1;i<EEP_Sector.Manage.CardCount;i++)
		{
      EEP_Sector.CardRecord[i-1]=EEP_Sector.CardRecord[i];
		}
		EEP_Sector.Manage.CardCount--;//卡扇区总数减一
		ADDR=EEPCardRecordStartAddr+(Index*EEP_CardSector_Size);//获取EEP扇区地址
		WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);

		CardLibraryParaPointer->CardCount--;//卡库总卡数减一
    switch(EEPCardRecordStartAddr)
    {
      case EEP_CardRecord_StartAddr:
        SysData.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
        SetTimer(Door_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
        break;
      case EEP_CardRecord2_StartAddr:
        SysData2.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
        SetTimer(Door2_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
        break;
      case EEP_CardRecord3_StartAddr:
        SysData3.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
        SetTimer(Door3_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
        break;
      case EEP_CardRecord4_StartAddr:
        SysData4.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
        SetTimer(Door4_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
        break;
    }
	}
	else 
  {
    return Error;
  }
	return Right;		
}
// 功能描述  : 申请扇区
// 输入参数  :	 CardLibraryParaEepAddr	卡库参数Eep地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数  : UpIndex 上个扇区索引
// 返回参数  : SectorIndex	 申请的扇区索引
u8 CardLibrary_ApplySector(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 UpIndex)
{
	u8 SectorIndex;
	u32 ADDR,EEPCardRecordStartAddr;
	EEP_CardSectorStruct EEP_Sector;

	SectorIndex=CardLibraryParaPointer->UsableListNo; //备用扇区
	if(SectorIndex!=0xFF)
	{//有备用扇区
		EEPCardRecordStartAddr = CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);
	  ADDR=EEPCardRecordStartAddr+(SectorIndex*EEP_CardSector_Size);//获取备用EEP扇区地址
	  ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);
		CardLibraryParaPointer->UsableListCount--;
		CardLibraryParaPointer->UsableListNo=EEP_Sector.Manage.NextSector;//下个备用扇区写入备用扇区指针
		WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash); 
		EEP_Sector.Manage.UpSector=UpIndex;//上个卡区号
		EEP_Sector.Manage.NextSector=0xFF;//下个卡区号
		EEP_Sector.Manage.CardCount=0;//  卡个数
		EEP_Sector.Manage.FLASHCardCount=0;//空间使用的个数	
		WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 		
	}	
	return SectorIndex;		
}
// 功能描述  : 卡增加
// 输入参数  :	 CardLibraryParaEepAddr	卡库参数Eep地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 : pRecord 卡的记录
// 返回参数  : 返回查询结果
u8 CardLibrary_AddCard(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord)
{	
	u8 i,j,Pos,Index,AddCard=0;
	u32 ADDR,fADDR,EEPCardRecordStartAddr,FLASHCardRecordStartAddr;
	CardList_Record Record;
	EEP_CardSectorStruct EEP_Sector;//EEP中卡扇区
	
	EEPCardRecordStartAddr = CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);
	FLASHCardRecordStartAddr = CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr);
	if(CardLibrary_CardSearch(EEPCardRecordStartAddr,CardLibraryParaPointer,(*pRecord).CardNo,&EEP_Sector,&Record,&Index,&Pos )==Right)//查询卡
	{//卡库已经有这个卡
		for(i=0;i<sizeof(CardList_Record);i++)
		{
			if(Record.CardNo[i]!= (*pRecord).CardNo[i])
			{//卡信息不同，则需要删除此卡
				for(j=Pos+1;j<EEP_Sector.Manage.CardCount;j++)
				{
				   EEP_Sector.CardRecord[j-1]=EEP_Sector.CardRecord[j];
				}
				EEP_Sector.Manage.CardCount--;
				CardLibraryParaPointer->CardCount--;
				ADDR=EEPCardRecordStartAddr+(Index*EEP_CardSector_Size);//获取EEP扇区地址
				WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);
				AddCard=1;//需要增加卡					
				break;
			}
		}
	}
	else
	{
		AddCard=1;//需要增加卡
	}

	if(AddCard==0)
	{//卡信息相同不用增加 直接返回结果
		 return Right;
	}
	else
	{		  		
		Index=CardLibrary_GetHashIndex(CardLibraryParaPointer,(*pRecord).CardNo);			
		while(1)
		{
			ADDR=EEPCardRecordStartAddr+(Index*EEP_CardSector_Size);//获取EEP扇区地址
			if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector)==Error)
			{//获取校验错误
				return Error;
			}
			if(EEP_Sector.Manage.FLASHCardCount<Card_SectorRecordCount)
			{//此卡扇区未满
				CardLibrary_CardSectorSearch(FLASHCardRecordStartAddr,Index,(*pRecord).CardNo,&EEP_Sector,&Record,&Pos);//查找插入的位置 
				for(i=EEP_Sector.Manage.CardCount;i>Pos;i--)
				{//比此卡号大的 往下移动
				   EEP_Sector.CardRecord[i]=EEP_Sector.CardRecord[i-1];
				}
				EEP_Sector.Manage.CardCount++;//扇区有效个个数
				EEP_Sector.CardRecord[Pos].FlashPos=EEP_Sector.Manage.FLASHCardCount;//存放在flash扇区的位置
				EEP_Sector.Manage.FLASHCardCount++;//flash扇区中卡空间使用个数  				
				WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);//写入EEP
				
				fADDR=FLASHCardRecordStartAddr+Index*FLASH_CardSector_Size; //在flash中的扇区地址
				fADDR+= (EEP_Sector.Manage.FLASHCardCount-1)*FLASH_CardRecord_Size;//在flash中的绝对地址
				Write_SST_Cfg(fADDR,sizeof(CardList_Record),(u8 *)pRecord);//卡信息存储在flash中
				CardLibraryParaPointer->CardCount++;
        switch(EEPCardRecordStartAddr)
        {
          case EEP_CardRecord_StartAddr:
            SysData.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
            SetTimer(Door_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
            break;
          case EEP_CardRecord2_StartAddr:
            SysData2.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
            SetTimer(Door2_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
            break;
          case EEP_CardRecord3_StartAddr:
            SysData3.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
            SetTimer(Door3_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
            break;
          case EEP_CardRecord4_StartAddr:
            SysData4.SysParamChange|=SysParam_CardLibrary;  //系统卡库数据有变化
            SetTimer(Door4_MemoryTimer,Door_MemoryTime); //两秒钟后 保存记录变化
            break;
        }
				return Right;
			}			
			if(EEP_Sector.Manage.NextSector!= 0xff)
			{//还有下个链接扇区
			   Index=EEP_Sector.Manage.NextSector;
			}
			else
			{//没有下个扇区 需要申请下个扇区				 
				 Index=CardLibrary_ApplySector(CardLibraryParaEepAddr,CardLibraryParaPointer,Index); 
				 if(Index==0xFF)
				 {//申请扇区失败 卡扇区已经使用完
					 return Error;
				 }
				 EEP_Sector.Manage.NextSector=Index;//申请的下个扇区
				 WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);//写入EEP					
			}  		
		}		
	} 	
} 
////////////////////////////卡库添加和删除end////////////////////////////////////

///////////////////////////////读卡库//////////////////////////////
// 功能描述  : 获取扇区的管理信息
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  : SectorIndex扇区索引
// 输入参数  : SectorManage扇区的管理信息
// 返回参数  : 获取的结果
u8 CardLibrary_GetSectorManage(u32 EEPCardRecordStartAddr,u8 SectorIndex,EEP_CardSectorManageStruct *SectorManage)
{	 
	u32 ADDR=0;	  	
	EEP_CardSectorStruct EEP_Sector;//EEP中卡扇区 
	
	ADDR  =EEPCardRecordStartAddr+ (SectorIndex*EEP_CardSector_Size);//获取EEP扇区地址
	if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector)==Right)
	{ 	
		if(EEP_Sector.Manage.CardCount>Card_SectorRecordCount)
		{//超过最大卡数
		  return Error;
		}
		else
		{
			SectorManage->UpSector=EEP_Sector.Manage.UpSector;
			SectorManage->NextSector=EEP_Sector.Manage.NextSector;
			SectorManage->CardCount=EEP_Sector.Manage.CardCount;
			SectorManage->FLASHCardCount=EEP_Sector.Manage.FLASHCardCount;
			return Right;
		} 		
	} 	
	return	Error;		
}
// 功能描述  : 复位获取卡块记录配置
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
u8 CardLibrary_ResetReadCardConfig(u32 EEPCardRecordStartAddr)
{
  EEP_CardSectorManageStruct SectorManage;
	 
	curReadIndexPos.Hash=0;
	curReadIndexPos.SectorIndex = 0;
	curReadIndexPos.Pos = 0; 
	curReadIndexPos.CardCount=0	;
	curReadIndexPos.NextSector=0xFF;     
	if(CardLibrary_GetSectorManage(EEPCardRecordStartAddr,0,&SectorManage)==Right)
	{
	   curReadIndexPos.CardCount= SectorManage.CardCount; //扇区卡个数
	   curReadIndexPos.NextSector= SectorManage.NextSector;		
	}    
  return Right; 
}
// 功能描述  : 根据指定的地址获取 卡信息
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 : pCardPos 卡记录位置 
// 输入参数2 : pRecord 卡记录 
// 返回参数  : 获取的状态
u8 CardLibrary_GetPosCard(u32 EEPCardRecordStartAddr,CardList_Pos *pCardPos,CardList_Record *pRecord)
{		
	u32 ADDR=0;
	EEP_CardSectorStruct EEP_Sector;

	ADDR = EEPCardRecordStartAddr + ((*pCardPos).SectorIndex*EEP_CardSector_Size);//获取EEP扇区地址
	if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector)==Right)
	{ 	
		ADDR=CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr)+((*pCardPos).SectorIndex*FLASH_CardSector_Size);
		ADDR+=EEP_Sector.CardRecord[(*pCardPos).Pos].FlashPos *FLASH_CardRecord_Size;
		Read_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&(*pRecord).CardNo);//获取flash的卡信息
		return	Right;
	} 	
	return Error;	
}
// 功能描述  : 获取靠近指定位置的卡信息
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 :  pRecord 卡记录
u8 CardLibrary_GetNearPosCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord)
{     
	EEP_CardSectorManageStruct SectorManage;
	while(curReadIndexPos.Hash<CardLibraryParaPointer->Hash)
	{
		if(curReadIndexPos.Pos<curReadIndexPos.CardCount)//当前扇区取卡情况
		{
		   CardLibrary_GetPosCard(EEPCardRecordStartAddr,&curReadIndexPos,pRecord);//获取卡信息
		   return Right;	 		  
		}
		else
		{	   
		   curReadIndexPos.Pos=0; 
		   while(curReadIndexPos.NextSector !=0xFF)
		   {//获取下个链接卡区的信息					   			
					if(CardLibrary_GetSectorManage(EEPCardRecordStartAddr,curReadIndexPos.NextSector,&SectorManage)==Right)
					{
						 curReadIndexPos.SectorIndex=curReadIndexPos.NextSector;//当前扇区
						 curReadIndexPos.CardCount= SectorManage.CardCount; 	  //扇区卡个数
						 curReadIndexPos.NextSector= SectorManage.NextSector;	  //下个扇区
						 if(curReadIndexPos.CardCount>0)
						 {
								CardLibrary_GetPosCard(EEPCardRecordStartAddr,&curReadIndexPos,pRecord);//获取卡信息
								return Right;
						 }
					}
					else
					{
						 curReadIndexPos.NextSector= 0xFF;
					}    
		   }  		   	
		}
		curReadIndexPos.Hash++;//下个Hash值
		curReadIndexPos.SectorIndex=curReadIndexPos.Hash;//当前扇区
		if(CardLibrary_GetSectorManage(EEPCardRecordStartAddr,curReadIndexPos.SectorIndex,&SectorManage)==Right)
		{		   
		   curReadIndexPos.CardCount= SectorManage.CardCount; 	  //扇区卡个数
		   curReadIndexPos.NextSector= SectorManage.NextSector;	  //下个扇区
		}
		else
		{
		   curReadIndexPos.CardCount= 0; 	  //扇区卡个数
		   curReadIndexPos.NextSector= 0xFF;	  //下个扇区
		}
	}   
    return Error; 
}
// 功能描述  : 当前读取卡号 根据curReadIndexPos提供的信息读取
// 入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数  : pRecord 卡记录 
u8 CardLibrary_curReadCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord )
{	      
	if(CardLibraryParaPointer->CardCount > 0)			//内有名单记录
	{			 
		if(CardLibrary_GetNearPosCard(EEPCardRecordStartAddr,CardLibraryParaPointer,pRecord)==Right)//获取卡信息
		{
		 	curReadIndexPos.Pos++;
			return Right;	
		}		
	} 	        		
	return Error;		
}
///////////////////////////读卡库end/////////////////////

///////////////////////////卡库整理////////////////////////////////////////////////
// 功能描述  : 整理一个哈希值的卡库，回收不使用的扇区，整理flash扇区荒废的空间。
// 输入参数  :	 CardLibraryParaEepAddr	卡库参数Eep地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
u8 CardLibrary_Tidy(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{	 
  u8 static HashIndex=0;
  u16 static CardCount=0;
  u8  status=0;
  
	u32 ADDREEp=0,ADDRFlash=0,ADDR=0,EEPCardRecordStartAddr=0;
	u8 i,UpSector,NextSector,Index;	
	EEP_CardSectorStruct 		EEP_Sector;//EEP中卡扇区
	//CardList_Record RecordG[128];   //一个扇区的卡信息 31*128=3968=4096-128=4k-128 要求堆栈大于4k，所以使用全局变量 
	CardList_Record RecordG[64];   //一个扇区的卡信息 31*64
	
	EEPCardRecordStartAddr = CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);	
  Index=HashIndex;
  while(Index<EEP_CardSector_Count)
  {
    ADDREEp = EEPCardRecordStartAddr + (Index*EEP_CardSector_Size);//获取EEP扇区地址	
    ReadEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //获取EEP的卡扇区
    UpSector = EEP_Sector.Manage.UpSector;      //此卡区的上个卡区号
    NextSector = EEP_Sector.Manage.NextSector;  //此卡区的下个卡区号
    CardCount += EEP_Sector.Manage.CardCount;   //统计卡库的卡个数
    if(EEP_Sector.Manage.CardCount==0)//此扇区已经没有卡 需要清空
    { 				
      if(Index>=CardLibraryParaPointer->Hash) //此扇区需要回收
      {	
        ADDRFlash = CardLibrary_GetFlashCardRecordStartAddr(CardLibraryParaEepAddr)+(Index*FLASH_CardSector_Size);
        SST_Earse_Area(ADDRFlash);//擦除flash扇区
        //回收此扇区  由于都是往下找备用扇区，因此备用扇区的UpSector没有实用意义，所以不修正链表中的UpSector
        //EEP扇区信息
        EEP_Sector.Manage.UpSector=0xFF;
        EEP_Sector.Manage.NextSector=CardLibraryParaPointer->UsableListNo;//备用扇区链表					
        EEP_Sector.Manage.FLASHCardCount=0;
        WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 
        //管理信息
        CardLibraryParaPointer->UsableListNo=Index;
        CardLibraryParaPointer->UsableListCount++;
        WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash);
        //修正此链表前个链表链接链接信息								
        ADDREEp=EEPCardRecordStartAddr+ (UpSector*EEP_CardSector_Size);//获取上个EEP扇区地址	
        ReadEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //
        EEP_Sector.Manage.NextSector=NextSector;
        WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //
        //修正此链表后个链表链接链接信息
        if(NextSector<EEP_CardSector_Count)
        {
          ADDREEp=EEPCardRecordStartAddr+ (NextSector*EEP_CardSector_Size);//获取上个EEP扇区地址	
          ReadEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);
          EEP_Sector.Manage.UpSector=UpSector;
          WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 
        }				
      }
      else //清空flash
      {
        if(EEP_Sector.Manage.FLASHCardCount != 0)
        {
          EEP_Sector.Manage.FLASHCardCount=0;
          WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 
          ADDRFlash=CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr)+(Index*FLASH_CardSector_Size);
          SST_Earse_Area(ADDRFlash);//擦除flash扇区
        }
      }
    }
    else if((EEP_Sector.Manage.FLASHCardCount-EEP_Sector.Manage.CardCount)>(Card_SectorRecordCount/2)
      && EEP_Sector.Manage.CardCount<=64)
    {//此扇区需要整理
      ADDRFlash = CardLibrary_GetFlashCardRecordStartAddr(CardLibraryParaEepAddr)+(Index*FLASH_CardSector_Size);
      for(i=0;i<EEP_Sector.Manage.CardCount;i++)
      {			   	
        ADDR=ADDRFlash+EEP_Sector.CardRecord[i].FlashPos*FLASH_CardRecord_Size;
        EEP_Sector.CardRecord[i].FlashPos=i;
        Read_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&RecordG[i]);//获取flash的卡信息
      }
      SST_Earse_Area(ADDRFlash);//擦除flash扇区
      for(i=0;i<EEP_Sector.Manage.CardCount;i++)
      {//写回flash			   	
        ADDR=ADDRFlash+i*FLASH_CardRecord_Size;					
        Write_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&RecordG[i]);//写flash的卡信息
      }
      EEP_Sector.Manage.FLASHCardCount=i;
      WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //写EEP的卡扇区
    }
    if(NextSector==0)
    {	
      NextSector=0xff;
    }
    Index=NextSector;
  }
  HashIndex++;
  if(HashIndex>=CardLibraryParaPointer->Hash)
  {
    if(CardLibraryParaPointer->CardCount!=CardCount) //卡库卡个数统计错误
    {
      CardLibraryParaPointer->CardCount=CardCount;	
      WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash); 
    } 
    CardCount = 0;  
    HashIndex = 0;
    status = 1;
  }
	return status;	
}

#endif
