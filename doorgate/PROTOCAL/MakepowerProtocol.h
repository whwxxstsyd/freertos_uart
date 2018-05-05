#ifndef __MakepowerProtocol_H
#define __MakepowerProtocol_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "sys.h"

enum
{
	LockTtype_SingleFaceMagnetic 		= 0,	//单面电磁锁
	LockTtype_DoubleFaceMagnetic 		= 1,	//双面电磁锁
	LockTtype_SingleFaceMachine	 	= 2,	//单面电机锁 
	LockTtype_DoubleFaceMachine	 	= 3,	//双面电机锁
	LockTtype_Password	 			    = 4,	//密码锁
	LockTtype_Hotel	 				= 5,	//宾馆锁
	LockTtype_Security	 			    = 6,	//安防锁
	LockTtype_StrongSecurity	 	  	= 7,	//加强安防锁
};

extern void MPDoorSendHead(void);

extern void MPDoorSendChksum(void);
extern void MPDoorSendDataLen(void);
extern void MPDoorSendTail(void);
extern void MPDoor_To_pc(void);

extern void MPDoorCMD11(void);
extern void MPDoorCMD13(void);
extern void MPDoorCMDA1(void);
extern void MPDoorCMDA3(void);
extern void MPDoorCMDA4(void);
extern void MPDoorCMDA5(void);
extern void MPDoorCMDB2(void);
extern void MPDoorCMD33(void);
extern void MPDoorCMDB3(void);
extern void MPDoorCMD34(void);
extern void MPDoorCMDB4(void);
extern void MPDoorCMD38(void);
extern void MPDoorCMDB8(void);
extern void MPDoorCMD39(void);
extern void MPDoorCMDB9(void);
extern void MPDoorCMD41(void);
extern void MPDoorCMD43(void);
extern void MPDoorCMD44(void);
extern void MPDoorCMD53(void);
extern void MPDoorCMD56(void);
extern void MPDoorCMDE3(void);
extern void MPDoorCMDE9(void);
extern void MPDoorCMD81(void);
extern void MPDoorCMD9F(void);
extern void MPDoorCMDF9(void);
extern void MPDoorCMD37(void);
extern void DealMPDoorPro(void);
extern void DealCLDoorProtocol(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

