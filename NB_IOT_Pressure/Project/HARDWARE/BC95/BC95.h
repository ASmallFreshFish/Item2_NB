#ifndef __BC95_H
#define __BC95_H

#include "delay.h"
//#include "usart.h"
//#include <stm32l1xx.h>

typedef struct
{
    u8 CSQ;    
    u8 Socketnum;   //编号
    u8 reclen;   //获取到数据的长度
    u8 res;      
    u8 recdatalen[10];
    u8 recdata[100];
    u8 netstatus;//网络指示灯
} BC95;

void Clear_Buffer(void);//清空缓存	
void CDP_Init(void);//COAP服务器配置初始化
void BC95_Init(void);
void BC95_RECCOAPData(void);
void BC95_SendCOAPdata(u8 *len,u8 *data);
void BC95_SendCOAPdata_try(u8 *len,u8 *data);


#endif

