#ifndef __SYS_H
#define __SYS_H	
//#include "stm32f10x.h"
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO寄存器地址偏移量
#define GPIO_ODR_ADDR_OFFSET 0X14
#define GPIO_IDR_ADDR_OFFSET 0X10

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+GPIO_ODR_ADDR_OFFSET) //0x40020014 
#define GPIOB_ODR_Addr    (GPIOB_BASE+GPIO_ODR_ADDR_OFFSET) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+GPIO_ODR_ADDR_OFFSET) //0x40020814  
#define GPIOD_ODR_Addr    (GPIOD_BASE+GPIO_ODR_ADDR_OFFSET) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+GPIO_ODR_ADDR_OFFSET) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+GPIO_ODR_ADDR_OFFSET) //0x40021814    
#define GPIOG_ODR_Addr    (GPIOG_BASE+GPIO_ODR_ADDR_OFFSET) //0x40021C14    

#define GPIOA_IDR_Addr    (GPIOA_BASE+GPIO_IDR_ADDR_OFFSET) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+GPIO_IDR_ADDR_OFFSET) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+GPIO_IDR_ADDR_OFFSET) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+GPIO_IDR_ADDR_OFFSET) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+GPIO_IDR_ADDR_OFFSET) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+GPIO_IDR_ADDR_OFFSET) //0x40021810 
#define GPIOG_IDR_Addr    (GPIOG_BASE+GPIO_IDR_ADDR_OFFSET) //0x40021C10 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#endif


