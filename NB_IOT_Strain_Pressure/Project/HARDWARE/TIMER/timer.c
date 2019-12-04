#include "head_include.h"

time_type g_time;
volatile u8 netstatus;//����״̬��

extern BC95 BC95_Status;
extern press_ad_type g_press;


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, DISABLE);  //��ʹ��TIMx����
							 
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //��������
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 

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


//��ʱ��4�жϷ������
// 500msһ���ж�
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
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
		
		//����ڹػ������30s����һ��
		if(g_bat.sta == BAT_STA_OFF_POWER)
		{
			g_bat.sample_count++;
			if(g_bat.sample_count >= BAT_SAMPLE_INTERVAL_HALFMIN)
			{
				g_bat.sample_count = 0;
				g_bat.sample_flag = 1;
			}
		}
		
		//0.5s����һ�α�Ĥѹ�����������
		g_press.sample_count++;
		if(g_press.sample_count >= PRESS_SAMPLE_TIME_COUNT)
		{
			g_press.sample_count = 0;
			g_press.sample_flag = 1;
		}
		//����5minһ��
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


