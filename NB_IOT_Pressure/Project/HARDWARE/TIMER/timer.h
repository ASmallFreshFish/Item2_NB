#ifndef __TIMER_H
#define __TIMER_H

#include "delay.h"

#define TIME_500MS_BY_20MS 24

typedef struct
{
	unsigned char ms20;
}time_type;

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
#endif

