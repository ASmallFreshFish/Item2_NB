#ifndef __BC95_H
#define __BC95_H

#include "delay.h"
//#include "usart.h"
//#include <stm32l1xx.h>

//typedef struct
//{
//    u8 CSQ;    
//    u8 Socketnum;   //编号
//    u8 reclen;   //获取到数据的长度
//    u8 res;      
//    u8 recdatalen[10];
//    u8 recdata[100];
//    u8 netstatus;//网络指示灯
//} BC95;

typedef struct
{
    u8 CSQ;    
    u8 Socketnum;   //编号
    u8 reclen;   //获取到数据的长度
    u8 res;      
    u8 recdatalen[10];
    u8 recdata[100];
    u8 netstatus;//网络指示灯
//		u8 CSQ;    
//		u8 Socketnum;   //±ào?
//		u8 reclen;   //??è?μ?êy?Yμ?3¤?è
//		u8 res;      
//		u8 recdatalen[10];
//		u8 recdata[100];
		u8 uart1len[10];
		u8 senddata[100];
} BC95;


extern char *strx,*extstrx;

void Clear_Buffer(void);//清空缓存	
void CDP_Init(void);//COAP服务器配置初始化
void BC95_reset_init(void);
void BC95_Init(void);
void BC95_RECCOAPData(void);
void BC95_SendCOAPdata(u8 *len,u8 *data);
u8 BC95_SendCOAPdata_try(u8 *len,u8 *data);

void BC95_PDPACT(void);
void BC95_ConUDP(void);



void copy_buf(char *buf_dest,char *buf_sour,u8 len);


#endif

