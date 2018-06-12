#ifndef HASH_H
#define HASH_H	
#include "sys.h"	


#define TYPE_NORMAL_CARD    2	//ÃÅ½û¿¨
#define TYPE_PATROL_CARD	3	//Ñ²¸ü¿¨
#define TYPE_LIFT_CARD		4	//ÌÝ¿Ø¿¨


typedef enum list_status
{
	LIST_STATUS_ONLY_ONE = 0,
	LIST_STATUS_FIRST,
	LIST_STATUS_MIDDLE,
	LIST_STATUS_LAST
}LIST_STATUS;



typedef struct
{
	u16 Func2List;
	u8  UsedFlag;
	u8  Reserved;	
}HASH_Func;


typedef struct
{
	u16 HashList_Next;
	u16 HashDataAddr;
	u8	HashUsedFlag;
	u8	CardType;	
	u8	Reserved[2];
}HASH_List;




extern HANDLE *OpenCardDataBase(void);
extern bool CloseCardDataBase( );
extern bool WriteCardInfo( CDINFO *Card,u8 CardType);
extern bool ReadCard( u8 *CSN,u8 CardType,CDINFO *Card);
extern bool DeleteCard( u8 *CSN,u8 CardType);
extern int GetCardNumber(  int nCardType);
extern bool GetCardBuffInfo( int nBufType, u8 **pCardBuff,int *pCardFileSize);
extern bool SetCardBuffInfo( int nBufType, u8 *pCardBuff,int pCardFileSize,int pCardFlieOffset);


#endif /*MYHASH_H*/		
