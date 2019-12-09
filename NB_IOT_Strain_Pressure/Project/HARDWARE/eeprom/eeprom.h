#ifndef __EEPROM_H
#define __EEPROM_H

#include "delay.h"

/************************************************
stm32L151C8T6A memory map
flash:	64K(0x10000),	0x0800_0000 ~ 0x0801_0000
eeprom:	4K(0x1000),		0x0808_0000 ~ 0x0808_1000
sram:	32K(0x8000),	0x2000_0000 ~ 0x2000_8000

*********************EEPROM ����*****************
ÿ100���ֽڴ洢һ��������Ϣ������3����
order	offset_addr    	variable
0		0x0				���ص���ֵ
1		0x64			ҵ���ϱ�����
2		0xC8			Ԥ��
3		0x12C			Ԥ��
4		0x190			Ԥ��
5		0x1F4			Ԥ��
6		0x258			Ԥ��
7		0x2BC			Ԥ��
8		0x320			Ԥ��
9		0x384			Ԥ��
************************************************/

//EEPROM��ʼ��ַ
#define EEPROM_BASE_ADDR 0x08080000 	//STM32 FLASH����ʼ��ַ
#define EEPROM_BYTE_SIZE 0x3FF 			//STM32 FLASH�ĳ���

#define PEKEY1	0x89ABCDEF		//FLASH_PEKEYR
#define PEKEY2	0x02030405		//FLASH_PEKEYR

#define EN_INT      	__enable_irq();		//ϵͳ��ȫ���ж�
#define DIS_INT     	__disable_irq();	//ϵͳ��ȫ���ж�

#define BYTES_EACH_VARIABLE 25		//ÿ������ռ��25*4bytes

typedef enum
{
	EEP_ID_W_CHANGE_THRESHOLD = 0,		//�����仯��ֵ���
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

//FLASH������ֵ
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����


void eeprom_clear(u16 Addr,u32 data,u16 Length);
void eeprom_write(u16 addr,u16 *buffer,u16 length);
u8 eeprom_read(u16 addr,u16 *buffer,u16 length);

void test(void);
#endif


