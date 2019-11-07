#ifndef __EEPROM_H
#define __EEPROM_H

#include "delay.h"

/************************************************
stm32L151C8T6A memory map
flash:64K,0x0800_0000 ~ 0x0802_0000
eeprom:4K,0x0808_0000 ~ 0x0808_1000
sram:32K,  0x2000_0000 ~ 0x2000_8000

************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define EEPROM_BASE_ADDR 0x08080000 	//STM32 FLASH的起始地址
#define EEPROM_BYTE_SIZE 0x3FF 			//STM32 FLASH的长度




//FLASH解锁键值
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

void test(void);
#endif


