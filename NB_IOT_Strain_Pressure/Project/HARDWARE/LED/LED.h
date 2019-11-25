#ifndef __LED_H
#define __LED_H
#include <stm32l1xx.h>

#define LEDMCU_RED_CLOSE   GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LEDMCU_RED_OPEN    GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define LEDMCU_RED_TOGGLE  GPIO_ToggleBits(GPIOB,GPIO_Pin_9)

#define LEDNET_BLUE_CLOSE   GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LEDNET_BLUE_OPEN    GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LEDNET_BLUE_TOGGLE  GPIO_ToggleBits(GPIOB,GPIO_Pin_3)

#define LEDTEST_CLOSE   GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LEDTEST_OPEN    GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LEDTEST_TOGGLE	 GPIO_ToggleBits(GPIOB,GPIO_Pin_0)

#define LEDPOWER_CLOSE  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LEDPOWER_OPEN   GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LEDPOWER_TOGGLE	 GPIO_ToggleBits(GPIOB,GPIO_Pin_1)

void LED_Init(void);
void LED_all_off(void);


#endif


