#include "platform.h"	
#include "delay.h"

#include "flash.h"	
#include "24CXXX.h"
#include "beep.h"
#include "calendar.h"		
#include "led.h"
	

int Platform_Init(void)   	   
{ 		
	//Low level init: Clock and Interrupt controller
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	

	//DWT≥ı ºªØ
	//DWT_INIT(SystemCoreClock);		
		
	//Systick Init
	delay_init();	

#if	LED_OPEN	
	Led_Init();			
#endif	

#if BEEP_OPEN 
	Beep_Init();	
#endif
		
#if EEPROM_OPEN 
	_24CXXX_init();	
#endif

#if FLASH_OPEN 		
	SPI_Flash_Init();
#endif

#if DATE_OPEN 		
	Calendar_Init();	
#endif

#if OUTSWITCH_OPEN 
		
#endif
	 
#if DateManager_OPEN 
	 
#endif		

#if	ReadCard_OPEN
	//Card_Init();		
	//CardLibrary_Init();
#endif		

  return 1;	
}



void Platform_Test(void)
{	
	Led_Test();		
	_24CXXX_Test();
	SPI_Flash_Test();	
	Beep_Test();
}

