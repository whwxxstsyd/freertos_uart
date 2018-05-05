#include "platform.h"	
#include "delay.h"

//#include "beep.h"
//#include "voice.h" 	
//#include "sd2068.h"	
//#include "led.h"		


int platformInit(void)   	   
{ 		
	//Low level init: Clock and Interrupt controller
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	

	//DWT≥ı ºªØ
	//DWT_INIT(SystemCoreClock);		
		
	//Systick Init
	delay_init();


#if BEEP_OPEN 
	//Beep_Init();
#endif

#if EEPROM_OPEN 
	
#endif

#if SST_OPEN 
	
#endif

#if SD2068_OPEN 
	//SD2068A_Initial();
#endif

#if VOICE_OPEN 
	//Voice_Init();
#endif

#if OUTSWITCH_OPEN 
	
#endif
	 
	 
#if DateManager_OPEN 
	 
#endif	
			
	//Led_Init();			
	
  return 1;	
}

