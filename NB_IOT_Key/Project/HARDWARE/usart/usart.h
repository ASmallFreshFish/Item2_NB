#ifndef __USART_H
#define __USART_H

#include "delay.h"

void uart_init(u32 bound);
void uart3_init(u32 bound);
void UART1_send_byte(char data);
void UART3_send_byte(char data);
void Uart1_SendStr(char*SendBuf);

#endif


