#include "led.h"

void LED_Init(void)
{		
    GPIO_InitTypeDef   GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOA, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	
	LEDTEST_CLOSE;
	LEDPOWER_CLOSE;
 } 

void LED_handle(void)
{
	LEDMCU_TOGGLE;
}


