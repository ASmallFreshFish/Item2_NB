#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

//#include "stm32l1xx.h"
#include "sys.h"

//#define GS_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
//#define GS_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

#define GS_SDA_IN()  {GPIOA->MODER &=0XFFFF3FFF;GPIOA->PUPDR &=0XFFFF3FFF;}
#define GS_SDA_OUT() {GPIOA->MODER &=0XFFFF3FFF;GPIOA->MODER |= 4<<8;GPIOA->OTYPER &= 0XFFFFFFDF;}

//IO²Ù×÷º¯Êý	 
#define GS_IIC_SCL    PAout(4) 		//SCL
#define GS_IIC_SDA    PAout(5) 		//SDA	 
#define GS_READ_SDA   PAin(5) 			//ÊäÈëSDA 



u8 GS_Write_Byte(u8 REG_Address,u8 REG_data);
u8 GS_Read_Byte(u8 REG_Address);
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);


#endif


