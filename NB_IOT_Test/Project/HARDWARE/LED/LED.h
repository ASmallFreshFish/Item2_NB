#ifndef __LED_H
#define __LED_H
#include <stm32l1xx.h>

#define LEDMCU_CLOSE   GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LEDMCU_OPEN    GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define LEDMCU_TOGGLE  GPIO_ToggleBits(GPIOB,GPIO_Pin_9)

#define LEDNET_CLOSE   GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LEDNET_OPEN    GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LEDNET_TOGGLE  GPIO_ToggleBits(GPIOB,GPIO_Pin_3)

#define LEDTEST_CLOSE   GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LEDTEST_OPEN    GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LEDTEST_TOGGLE	 GPIO_ToggleBits(GPIOB,GPIO_Pin_0)

#define LEDPOWER_CLOSE  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LEDPOWER_OPEN   GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LEDPOWER_TOGGLE	 GPIO_ToggleBits(GPIOB,GPIO_Pin_1)

void LED_Init(void);
void LED_handle(void);


#define  PA4_INPUT_VALUE GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define  PA5_INPUT_VALUE GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

void GPIO_Input_Detection_Init(void);


#endif


