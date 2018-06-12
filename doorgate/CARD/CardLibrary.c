#include "CardLibrary.h"	
#include "debug.h"	

UserCardInfo_T	user_info[5];


void CardLib_Init(void)	
{	
	u8 i;
	
	for(i=0;i<sizeof(user_info);i++)	
	{		
		user_info[i].card_ID[4] = {i,i,i,i};						
		user_info[i].user_ID[4] = {i,i,i,i};		
		user_info[i].user_pw[2] = {i,i};
		user_info[i].user_life[4] = {i,i,i,i};					
		user_info[i].user_auth = i;					
	}
}

int CardLib_Clear()
{
		
}

int CardLib_Check()
{
	
}

u16 CardLib_UserNum_Read()		
{	
	return sizeof(user_info);	
}

UserCardInfo_T *CardLib_UserInfo_Read(void)
{


	return 
}


u16 CardLib_UserNum_Write(u16 )			
{
	
}

int CardLib_UserInfo_Write()	
{
	
	
	
}










int CardLib_CardAdd()
{
	
}


int CardLib_CardDel()
{
	
}

	
int CardLib_CardSearch()	
{

}





















