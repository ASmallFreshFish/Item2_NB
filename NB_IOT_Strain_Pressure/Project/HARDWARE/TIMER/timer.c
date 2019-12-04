#include "head_include.h"

time_type g_time;
volatile u8 netstatus;//网络状态灯

extern BC95 BC95_Status;
extern press_ad_type g_press;


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, DISABLE);  //不使能TIMx外设
							 
}
/********************************************************
T=(arr+1)*(psc+1)/HCLK;
ex:HCLK=2Mhz
TIM4_Int_Init(4999,3199);
T=(5000)*(3200)/32=500000us=500ms
**********************************************************/


void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //开启外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 

			KEY_scan();
			
//			g_time.ms20++;
//			if( g_time.ms20 > 200) g_time.ms20 = 0;
//			g_time.ms20 =(g_time.ms20&TIME_500MS_BY_20MS);
//			switch(g_time.ms20)
//			{
//				case 0:
//					break;
//				case 1:
//					break;
//				case 24:
//					break;
//				default:
//					break;	
//			}
		}
}


//定时器4中断服务程序
// 500ms一次中断
void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		netstatus++;
		if(netstatus>=BC95_Status.netstatus)
		{
			netstatus=0;
			if(g_bat.sta != BAT_STA_OFF_POWER)
			{
				if(BC95_Status.netstatus>2)
				{
					LEDNET_BLUE_CLOSE;
					LEDMCU_RED_TOGGLE;
				}
				else
				{
					LEDNET_BLUE_TOGGLE;
				}
			}
		}

		g_weight.sample_count++;
		if(g_weight.sample_count >= PRESS_STRA_SAMPLE_TIME_COUNT )
		{
			g_weight.sample_count = 0;
			g_weight.sample_flag = 1;
		}
		
		//电池在关机情况下30s采样一次
		if(g_bat.sta == BAT_STA_OFF_POWER)
		{
			g_bat.sample_count++;
			if(g_bat.sample_count >= BAT_SAMPLE_INTERVAL_HALFMIN)
			{
				g_bat.sample_count = 0;
				g_bat.sample_flag = 1;
			}
		}
		
		//0.5s进行一次薄膜压力和重量检测
		g_press.sample_count++;
		if(g_press.sample_count >= PRESS_SAMPLE_TIME_COUNT)
		{
			g_press.sample_count = 0;
			g_press.sample_flag = 1;
		}
		//心跳5min一次
		g_bus.heart_count++;
		if(g_bus.heart_count >= HEART_UPLOAD_INTERVAL_5MIN )
		{
			g_bus.heart_count = 0;
			g_bus.report_flag |= HEART_FLAG;

			my_g_time.m_clock_syn_flag = 1;
		}

		my_g_time.m_clock_count++;
		if(my_g_time.m_clock_count>=2)
		{
			my_g_time.m_clock_count=0;
			my_g_time.m_clock_utc++;
		}
	}
}


