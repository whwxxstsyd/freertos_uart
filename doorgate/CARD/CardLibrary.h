#ifndef __CARDLIBRARY_H
#define __CARDLIBRARY_H	

#ifdef __cplusplus	
extern "C" {
#endif
	
#include "sys.h"	
#include "card.h"	

#define MAX_NAME_SIZE 50

#define MAX_CARDLIST_SIZE 51


#define	Flash_CardSector_Size			4096   	//FLASH扇区大小
#define Flash_CardRecord_Size   		16     	//定义FLASH中卡信息结构体大小
#define Card_SectorRecordCount			(Flash_CardSector_Size/Flash_CardRecord_Size)	//一个扇区中存储的数据个数 

#define FLASH_CardRecord1_StartAddr 	0						//卡库1的起始地址	
#define FLASH_CardRecord2_StartAddr 	(2*1024*1024) 			//卡库2的起始地址
		


typedef struct UserCardInfo{
	u8 card_ID[4];						
	u8 user_ID[4];			
	u8 user_pw[2];			
	u8 user_life[4];					
	u8 user_auth;	
} UserCardInfo_T;

//作为测试使用
typedef struct{
	char first_name[MAX_NAME_SIZE];
	char initial;	
	char last_name[MAX_NAME_SIZE];
	long long int cardnumber;
	int month;
	int year;
	int cvc;
	int ID;
}card_t;


void CardLib_Init();

void CardLib_Clear();	

u16 CardLib_UserNum_Read()	;	

void CardLib_UserInfo_Read();

u16 CardLib_UserNum_Write();			

void CardLib_UserInfo_Write();	

void CardLib_Test(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
