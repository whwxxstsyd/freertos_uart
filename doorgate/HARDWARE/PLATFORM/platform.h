#ifndef __PLATFORM_H
#define __PLATFORM_H 
#include "sys.h"
#include "param.h"	

#define	DEVICE_ALL				CFG_OFF				

#define LED_OPEN        		CFG_ON	
#define	BEEP_OPEN 	        	CFG_ON
#define EEPROM_OPEN         	CFG_ON
#define FLASH_OPEN 		        CFG_ON	
#define DATE_OPEN         		CFG_ON
#define VOICE_OPEN				CFG_ON

#define OUTSWITCH_OPEN      	CFG_ON
#define DateManager_OPEN		CFG_ON

#define Memory_OPEN         	CFG_ON
#define ReadCard_OPEN       	CFG_ON	

#define TEST_OPEN				CFG_OFF		
	
int Platform_Init(void);  	
void Platform_Test(void);	



#endif	
