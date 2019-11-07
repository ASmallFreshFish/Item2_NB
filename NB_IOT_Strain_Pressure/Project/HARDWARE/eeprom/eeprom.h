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
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define EEPROM_BASE_ADDR 0x08080000 	//STM32 FLASH����ʼ��ַ
#define EEPROM_BYTE_SIZE 0x3FF 			//STM32 FLASH�ĳ���




//FLASH������ֵ
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

void test(void);
#endif


