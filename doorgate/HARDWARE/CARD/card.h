#ifndef __CARD_H
#define __CARD_H	 
#include "sys.h"
 

#define CARD_DEV_NUM	2	

#define CARD_READ_DEV1		0
#define CARD_READ_DEV2		1		


#define CardRead1_PowerOn      	GPIO_SetBits(GPIOD,GPIO_Pin_1)						
#define CardRead1_PowerOff 		GPIO_ResetBits(GPIOD,GPIO_Pin_1)
 	
#define CardRead2_PowerOn      	GPIO_SetBits(GPIOD,GPIO_Pin_12)						
#define CardRead2_PowerOff 		GPIO_ResetBits(GPIOD,GPIO_Pin_12)
 

//////////////////////////////////////////////////////////////
/****************¶Á¿¨Í·1***************************************/
#define WGReader_H1D0_Reader		  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define WGReader_H1D1_Reader		  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)


/****************¶Á¿¨Í·2***************************************/
#define WGReader_H2D0_Reader		  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)	
#define WGReader_H2D1_Reader		  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)




void Card_Init(void);

			    
#endif


	
