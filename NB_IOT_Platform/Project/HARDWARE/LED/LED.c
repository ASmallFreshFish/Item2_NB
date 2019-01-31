#include "led.h"

void LED_Init(void)
{		
    GPIO_InitTypeDef   GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	 	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	LEDTEST_CLOSE;
	LEDPOWER_CLOSE;
 } 

void LED_handle(void)
{
	LEDMCU_TOGGLE;
}


