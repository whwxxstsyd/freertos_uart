#ifndef __CARDLibrary_H
#define __CARDLibrary_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
#include "globe.h"
	
#if ReadCard_Open

typedef __packed struct
{
	u8 	 TidyEnable;
}CardLibraryStruct;
extern CardLibraryStruct CardLibrary;

#define CardLibraryParaHashInit 29

//※※※※※※※※※※※※※※※※变量※※※※※※※※※※※※※※※※※※※※※※※
typedef __packed struct  _CardListPos_
{
	u8	 Hash;						//Hash索引
  u8   SectorIndex;     //扇区索引
  u8   NextSector;      //下个扇区索引
	u8	 CardCount;			  //此扇区卡个数
  u8   Pos;             //扇区位置
}CardList_Pos;
//※※※※※※※※※※※※※※※※卡库的分配※※※※※※※※※※※※※※※※※※※※※※※
#define FLASH_CardRecord_Size   		32     	//定义FLASH中卡信息结构体大小
#define	FLASH_CardSector_Size				4096   	//FLASH扇区大小
#define FLASH_CardRecord_StartAddr 	0 			//卡库的起始地址
//扇区存储卡信息条数	   4096/32=128
#define Card_SectorRecordCount			(FLASH_CardSector_Size/FLASH_CardRecord_Size) 

#define EEP_CardRecord_Size 				2      			//定义EEP中卡索引记录的大小
#define EEP_CardRecord_StartAddr		(2*1024) 		//卡库的起始地址 0x0860
#define EEP_CardRecord_EndAddr  		(32*1024) 	//卡库的结束地址 

#define EEP_CardRecord2_StartAddr		((32+2)*1024) 		//卡库的起始地址 0x0860
#define EEP_CardRecord2_EndAddr  		((32+32)*1024) 	  //卡库的结束地址 

#define EEP_CardRecord3_StartAddr		((32*2+2)*1024) 	//卡库的起始地址 0x0860
#define EEP_CardRecord3_EndAddr  		((32*2+32)*1024) 	//卡库的结束地址 

#define EEP_CardRecord4_StartAddr		((32*3+2)*1024) 	//卡库的起始地址 0x0860
#define EEP_CardRecord4_EndAddr  		((32*3+32)*1024) 	//卡库的结束地址 
//EEP扇区大小 (人为划分)	 128*2+7+1=264
#define	EEP_CardSector_Size				(Card_SectorRecordCount*EEP_CardRecord_Size+8) 
//存储区的大小	32*1024- 2*1024=30720;
#define EEP_CardStorage_Size     	(EEP_CardRecord_EndAddr-EEP_CardRecord_StartAddr) 
//扇区个数	 30720/264=116
#define EEP_CardSector_Count    	(EEP_CardStorage_Size/EEP_CardSector_Size)
//真实大小  116*264=30624
#define EEP_CardStorageReal_Size  (EEP_CardSector_Count*EEP_CardSector_Size)
//#define CardLibraryParaHashInit 29 Hash=29
//最好情况 	128*116=14848
//最坏情况	 	128*(116-Hash+1)=11264
//128*(116-Hash+1)>=10000 -> Hash<=117-78.125=38.875
 
//flash卡库的结束地址 0+ 116*4K= 464K    116*4096
#define FLASH_CardRecord_EndAddr 	    (FLASH_CardRecord_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //卡库的结束地址

#define FLASH_CardRecord2_StartAddr 	(512*1024) 			//卡库的起始地址
#define FLASH_CardRecord2_EndAddr 		(FLASH_CardRecord2_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //卡库的结束地址

#define FLASH_CardRecord3_StartAddr 	(512*2*1024) 			//卡库的起始地址
#define FLASH_CardRecord3_EndAddr 		(FLASH_CardRecord2_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //卡库的结束地址

#define FLASH_CardRecord4_StartAddr 	(512*3*1024) 			//卡库的起始地址
#define FLASH_CardRecord4_EndAddr 		(FLASH_CardRecord2_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //卡库的结束地址


//卡数：最坏情况=（扇区个数-Hash索引+1）*扇区存储卡数=（116-29+1）*128=11264
//卡数：最好情况=扇区个数*扇区存储卡数=14848
/**********************************************EEP卡库扇区构体**************************************/ 
typedef __packed struct 
{//在空间中占用7个字节
	u8    UpSector;				//上个扇区
	u8    NextSector;			//下个扇区
	u8    CardCount;			//卡记录个数
	u8    FLASHCardCount;	//FLASH中卡的记录个数
	u8		EmptyByte[3];		//备用 空留3个字节
}EEP_CardSectorManageStruct;                                 
typedef __packed struct 
{  	
	u8     State;	      //卡状态
	u8     FlashPos;	  //卡flash位置			
}EEP_CardRecordStruct;   
typedef __packed struct 
{	 
	EEP_CardSectorManageStruct	Manage;//扇区管理信息
	EEP_CardRecordStruct      	CardRecord[Card_SectorRecordCount];//一个扇区存储的卡信息		
}EEP_CardSectorStruct;                                 
/**********************************************名单结构体**************************************/
#define Card_Size       4  	    //卡号的大小

typedef __packed struct 
{  		 	 
	u8     CardNo[Card_Size];			//0 4个字节的卡号
	u32    CardUsefulLife;       	//4 有效期为时间：4个字节
	u8     CardType;              //8 卡类型 =0特权卡 =1用户卡 =2巡更卡 =3饭卡
	u8     CardState;             //9 卡状态 =0有效卡 =1挂失卡 =2冻结卡
	u8     LimitEffective;		    /*10 权限有效性说明: 00000000
																			位0: 时段是否有效,
																			位1: 节假日是否有效,
																			位2: 有效期限是否有效,
																			位3: 锁常闭是否有效,
																			位4-6: 保留,
                            	        位7:作为权限类型判别==1为级别，则TimeSegSelect值为级别号(有10种)；
                             	                           ==0为自定义，则TimeSegSelect值为时段
                                 */
	u16    TimeSegSelect;		    	 //11 时段选择，通过位来选择相应的时段，共16位,每天最多能选择3个时段；4＊7
	u8     CardToDoorState;        //13 卡号对门状态，以位来表示，最多支持8个门
	u8     CardToPCDAntState;      //14 卡号对读卡头状态，以位来表示，最多支持8个读卡头
	u8     CardUseID[4];    			 //15 用户编号
	u8     CardUserKey[2];	       //19 密码
	u8     WhichProtocol;          //21 有哪个协议配置
	u8     UserVIP	;							 //22 权限
	u8     No_Use[8];							 //30 未用字节，备用 			
}CardList_Record;//31字节


//////////////////////初始化///////////////////////////////////
//功能描述: 上电 初始化卡库
//入口参数：	CardLibraryParaEepAddr	卡库参数Eep地址
//入口参数	: CardLibraryParaPointer 卡库参数
extern void  CardLibrary_Init(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer);
// 功能描述: 卡库清空
//入口参数：	CardLibraryParaEepAddr	卡库参数Eep地址
//入口参数	: CardLibraryParaPointer 卡库参数
extern void  CardLibrary_Clear(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer);
//////////////////////初始化end///////////////////////////////////

////////////////////////////卡库添加和删除//////////////////////////////////////
// 功能描述  : 卡库查询
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 : pCardNo 卡号
// 输入参数2 : 返回pEEP_Sector 卡的EEP扇区信息
// 输入参数3 : 返回pRecord 卡的记录
// 输入参数4 : 返回Index   扇区索引
// 输入参数5 : 返回pPos    在扇区中位置
// 返回参数  : 返回查询结果
extern u8 CardLibrary_CardSearch(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo,EEP_CardSectorStruct *pEEP_Sector,CardList_Record *pRecord,u8 *pIndex,u8 *pPos);
// 功能描述  : 卡删除
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数  : pCardNo 卡号
// 返回参数  : 返回删除结果
extern u8 CardLibrary_DelCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo );
// 功能描述  : 卡增加
// 输入参数  :	 CardLibraryParaEepAddr	卡库参数Eep地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 : pRecord 卡的记录
// 返回参数  : 返回查询结果
extern u8 CardLibrary_AddCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord);
////////////////////////////卡库添加和删除end////////////////////////////////////

///////////////////////////////读卡库//////////////////////////////
// 功能描述  : 复位获取卡块记录配置
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
extern u8 CardLibrary_ResetReadCardConfig(u32 EEPCardRecordStartAddr);
// 功能描述  : 根据指定的地址获取 卡信息
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数1 : pCardPos 卡记录位置 
// 输入参数2 : pRecord 卡记录 
extern u8 CardLibrary_GetPosCard(u32 EEPCardRecordStartAddr,CardList_Pos *pCardPos,CardList_Record *pRecord);
// 功能描述  : 获取靠近指定位置的卡信息
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数1 :  pRecord 卡记录 
extern u8 CardLibrary_GetNearPosCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord);
// 功能描述  : 当前读取卡号 根据curReadIndexPos提供的信息读取
//入口参数	: EEPCardRecordStartAddr	eeprom的卡库起始地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
// 输入参数  : pRecord 卡记录 
extern u8 CardLibrary_curReadCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord );
/////////////////////////////读卡库end////////////////////////////////
// 功能描述  : 整理一个哈希值的卡库，回收不使用的扇区，整理flash扇区荒废的空间
// 输入参数  :	 CardLibraryParaEepAddr	卡库参数Eep地址
// 输入参数  :	 CardLibraryParaPointer 卡库参数
extern u8 CardLibrary_Tidy(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer);

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
