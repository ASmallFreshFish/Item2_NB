#include "head_include.h"

key_type g_key;
u8 volatile write_location = 0;


void KEY_init(void)
{
//	//hardware_init
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	//software_init
	memset(&g_key,0,sizeof(key_type));
	g_key.key_up_flag = 1;
 
}

void KEY_scan(void)
{
	if((g_key.key_up_flag)&&((KEY_PA4==1)||(KEY_PA5==1)||(KEY_PA6==1)||(KEY_PB12==1)||(KEY_PB13==1)))
	{
		delay_ms(10);// 消除抖动 
		g_key.key_up_flag = 0;
		if(KEY_PA4==1)
		{
			g_key.sta = PA4_PRESS;
			write_location+=SEND_DATA_LEN;
			UART1_send_byte('a');
		}
		else if(KEY_PA5==1)
		{
			g_key.sta = PA5_PRESS;
			write_location+=SEND_DATA_LEN;
			UART1_send_byte('b');
		}
		else if(KEY_PA6==1)
		{
			g_key.sta = PA6_PRESS;
			write_location+=SEND_DATA_LEN;
			UART1_send_byte('c');
		}
		else if(KEY_PB12==1)
		{
			g_key.sta = PB12_PRESS;
			write_location+=SEND_DATA_LEN;
			UART1_send_byte('d');
		}
		else if(KEY_PB13==1)
		{
			g_key.sta = PB13_PRESS;
			write_location+=SEND_DATA_LEN;
			UART1_send_byte('e');
		}

		if(write_location >= SEND_BUF_LEN)
		{
			write_location = 0;
		}
		
//		LEDTEST_TOGGLE;
	}
	else if((KEY_PA4==0)&&(KEY_PA5==0)&&(KEY_PA6==0)&&(KEY_PB12==0)&&(KEY_PB13==0))
	{
		g_key.key_up_flag = 1;
	}
}

void KEY_scan_start(void)
{
	TIM_Cmd(TIM3, ENABLE);
}
void KEY_scan_stop(void)
{
	TIM_Cmd(TIM3, DISABLE);
}


/*******************************************************************

ʹ����·�弯�ɵİ���PB12��ͨ��������⣬����У׼�������

********************************************************************/

void key_cali_init()
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void key_calibration()
{
	u32 t_weight_ave;
	u32 t_weight_ave_last[2]={0};
	u32 diff;
	u32 factor;


	if(KEY_PB12 == 0)
	{
		delay_ms(1000);

		if(KEY_PB12 == 1)
		{
			return;
		}
		
		LEDTEST_OPEN;	//��⵽����������
		
		t_weight_ave =t_weight_ave_last[0]=t_weight_ave_last[1]= key_Get_Weight();
		LEDTEST_CLOSE;	//�����ԭʼֵ���յ�
		
		for(int i=0;i<10;i++)
		{
			IWDG_Feed();
			t_weight_ave = key_Get_Weight();
			while((t_weight_ave > t_weight_ave_last[0]+4)||(t_weight_ave_last[0] > t_weight_ave+4))
			{
				t_weight_ave_last[0] = t_weight_ave;
				t_weight_ave = key_Get_Weight();
			}
			if(t_weight_ave > t_weight_ave_last[1]+50)
			{
				IWDG_Feed();
				t_weight_ave = key_Get_Weight();
				break;
			}
		}
		LEDTEST_OPEN;	//�������ֵ������

		if((t_weight_ave > t_weight_ave_last[1]))
		{
			diff = t_weight_ave - t_weight_ave_last[1];
		}
		else
		{
			diff = t_weight_ave_last[1] - t_weight_ave;
		}

		//factor100��ʵ��factor��1000��(100g��10����1000g)��
		if((g_weight.factor100 != diff)&&(diff != 0))
		{
			g_weight.factor100 =(u32)(diff);
			eeprom_write((u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].offset_addr,(u16 *)(&g_weight.factor100),
				(u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].length);
		}

		delay_ms(1000);
		LEDTEST_CLOSE;	//����꣬�յ�

#ifdef DEBUG_MACRO
		printf_string("\nfactor100:");
		printf_u32_decStr(g_weight.factor100);
#endif
	}
}
	

//����
//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
u32 key_Get_Weight(void)
{
	u8 i;
	u32 t_ad,t_weight[10],t_weight_ave;
	for(i=0;i<WEIGHT_SAMPLE_NUMBER;i++)
	{
		t_ad = HX711_Read();
		
#ifdef DEBUG_MACRO
		if(0)
		{
			printf_string("\nweight sample:\t");
			printf_u32_decStr(t_ad);
		}
#endif
			
		if(t_ad > g_weight.maopi_ad)			
		{
			t_ad = t_ad - g_weight.maopi_ad;		//��ȡʵ���AD������ֵ��
//			t_weight[i] = t_ad*10/GapValue;		//����ʵ���ʵ��������10��
			t_weight[i] = ((t_ad*10.0/GapValue/g_weight.factor100)*1000);		//����ʵ���ʵ��������10��
		}
		else
			t_weight[i] = 0;		

		delay_ms(100);
	}

	t_weight_ave = press_strain_sort_average(t_weight ,10);
	return t_weight_ave;
}

