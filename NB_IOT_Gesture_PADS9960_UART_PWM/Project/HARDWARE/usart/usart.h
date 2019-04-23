#ifndef __USART_H
#define __USART_H

#include "delay.h"

#define USART2_BUF_LEN 100

//´®¿Ú1
void uart1_init(u32 bound);
void UART1_send_byte(char data);
void Uart1_SendStr(char*SendBuf);
void Uart1_SendStr_Len(char*SendBuf,u8 len);

//´®¿Ú2
void uart2_gesture_init(u32 bound);
void uart2_enable(void);
void UART2_send_byte(char data);
void Uart2_SendStr(char*SendBuf);

//´®¿Ú3
void uart3_init(u32 bound);
void UART3_send_byte(char data);
void Uart3_SendStr(char*SendBuf);

u8 check_buf_empty(char ch[],u8 len,u8 loc);
u8 check_buf_valid_data(char ch[],u8 len,u8 loc);

#endif


