#ifndef	__POSTPRODAPROFUNC_H
#define __POSTPRODAPROFUNC_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"	
	

typedef struct UserCardInfo
{
	u8 card_ID[5];					
	u8 user_ID[4];		
	u8 user_pw[2];			
	u8 user_life[4];					
	u8 user_auth;		
} UserCardInfo_T;	




#endif


