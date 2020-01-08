#ifndef __TIMER_H
#define __TIMER_H

#include "delay.h"

#define TIME_500MS_BY_20MS 24

typedef enum
{
	TIME_IR_NO_FLAG = 0,
	TIME_IR_FLAG_5MIN_HEART = 0X01,
	TIME_IR_FLAG_REPORT_INTERVAL = 0X10,
}interrupt_flag_type;

typedef struct
{
//	unsigned char ms20;
	interrupt_flag_type interrupt_flag;
}time_type;

extern time_type g_time;

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
#endif

