/**
******************************************************************************
* @file    sst26vf032b.h
* @author  LL
* @version V0.0.0
* @date    2017-06-07
* @brief   
******************************************************************************
******************************************************************************
***/


#ifndef __SST26VF032B_H__
#define	__SST26VF032B_H__

#include <stdint.h>

void SST26_ULBRP(void);//全局解锁
uint32_t SST26_ReadJedecID(void);//读取SST的ID
void SST26_ChipErase(void);//擦除芯片
void SST26_4KByteSectorErase(uint32_t addr);//擦除所指定的4KB扇区
void SST26_32KByteSectorErase(uint32_t addr);//擦除所指定的32KB扇区
void SST26_64KByteSectorErase(uint32_t addr);//擦除所指定的64KB扇区
uint8_t SST26_Write(uint32_t addr,uint8_t* p_data,uint32_t no);//写多个字节
uint8_t SST26_EraseWrite(uint32_t addr,const uint8_t* p_data,uint32_t no);
void SST26_HRead(uint32_t addr,uint8_t* p_data,uint32_t no);//高速读 对于后续带5的芯片
void SST26_LRead(uint32_t addr,uint8_t* p_data,uint32_t no);//低速读
uint8_t SST26_LReadByte(uint32_t addr);//低速读一个字节
void SST26_Init(void);
void SST26_Deinit(void);
void SST26_Test(void);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* __SST26VF032B_H__ */

