#ifndef __EEPROM_H
#define __EEPROM_H

#include "delay.h"

/************************************************
stm32L151C8T6A memory map
flash:	64K(0x10000),	0x0800_0000 ~ 0x0801_0000
eeprom:	4K(0x1000),		0x0808_0000 ~ 0x0808_1000
sram:	32K(0x8000),	0x2000_0000 ~ 0x2000_8000

*********************EEPROM 分配*****************
每100个字节存储一个变量信息，并且3备份
order	offset_addr    	variable
0		0x0				称重的阈值
1		0x64			业务上报次数
2		0xC8			预留
3		0x12C			预留
4		0x190			预留
5		0x1F4			预留
6		0x258			预留
7		0x2BC			预留
8		0x320			预留
9		0x384			预留
************************************************/

//EEPROM起始地址
#define EEPROM_BASE_ADDR 0x08080000 	//STM32 FLASH的起始地址
#define EEPROM_BYTE_SIZE 0x3FF 			//STM32 FLASH的长度

#define PEKEY1	0x89ABCDEF		//FLASH_PEKEYR
#define PEKEY2	0x02030405		//FLASH_PEKEYR

#define EN_INT      	__enable_irq();		//系统开全局中断
#define DIS_INT     	__disable_irq();	//系统关全局中断

#define BYTES_EACH_VARIABLE 25		//每个变量占用25*4bytes

typedef enum
{
	EEP_ID_W_CHANGE_THRESHOLD = 0,		//重量变化阈值标号
	EEP_ID_W_UPLOAD_TIMES = 1,				
	EEPROM_ID_RESERVED2,
	EEPROM_ID_RESERVED3,
	EEPROM_ID_RESERVED4
}eeprom_id_type;

typedef struct
{
	u16 offset_addr;
	u16 length;
}eeprom_data_type;

extern eeprom_data_type g_eeprom[];

//FLASH解锁键值
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据


void eeprom_clear(u16 Addr,u32 data,u16 Length);
void eeprom_write(u16 addr,u16 *buffer,u16 length);
u8 eeprom_read(u16 addr,u16 *buffer,u16 length);

void test(void);
#endif


