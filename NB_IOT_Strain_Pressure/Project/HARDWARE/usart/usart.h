#ifndef __USART_H
#define __USART_H

#include "delay.h"

#define USART2_BUF_LEN 100

extern volatile char RxBuffer2[USART2_BUF_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


void uart_init(u32 bound);
void uart3_init(u32 bound);
void UART1_send_byte(char data);
void UART3_send_byte(char data);
void Uart1_SendStr(char*SendBuf);
void Uart1_SendStr_Len(char*SendBuf,u8 len);

void uart2_gesture_init(u32 bound);
void uart2_enable(void);
u8 check_buf_empty(char ch[],u8 len,u8 loc);
u8 check_buf_valid_data(char ch[],u8 len,u8 loc);

#endif


