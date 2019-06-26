#ifndef __INIT_H
#define __INIT_H
#include <stm32l1xx.h>

void main_init(void);
void init_hardware(void);
void init_software(void);

void init_Cortex_M3(void);
void init_clock(void);
void init_port(void);
void init_ad(void);
void init_usart1(u32 bound);
void init_usart2(u32 bound);
void init_usart3(u32 bound);
void init_timer3(u16 arr,u16 psc);
void init_timer4(u16 arr,u16 psc);
void init_NB_IOT(void);

void init_watchdog(void);
void init_reset(void);

void init_start(void);


#endif



