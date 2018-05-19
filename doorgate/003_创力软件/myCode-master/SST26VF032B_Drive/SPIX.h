
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPIX_H
#define	SPIX_H


#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdbool.h> 
#include <stdint.h>

//#define SST26_CE              LATFbits.LATF4
#define SST26_CE_SET          LATFbits.LATF4=1//高电平
#define SST26_CE_CLR          LATFbits.LATF4=0//低电平



void SST26SPI_Initialization(void);//SPI初始化函数
void SST26SPI_DeInitialization(void);//关闭SPI
uint8_t SST26SPI_ReadWriteByte(uint8_t data);//SPI收发送单字节数据

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

