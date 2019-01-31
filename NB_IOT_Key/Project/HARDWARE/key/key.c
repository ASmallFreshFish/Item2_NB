#include "head_include.h"

key_type g_key;
extern u8 volatile write_location;

void KEY_init(void)
{
	//hardware_init
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//software_init
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
		
		LEDTEST_TOGGLE;
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


