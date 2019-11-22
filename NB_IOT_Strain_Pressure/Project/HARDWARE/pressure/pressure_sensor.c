//#include "pressure_sensor.h"
//#include "usart.h"
//#include "led.h"
//#include "bus.h"
#include "head_include.h"

press_ad_type g_press;

u16 RegularConvData_Tab[2];//存储2个电压值
u16 VREFINT_DATA;
float Vbat_value;

void press_ad_debug_print(u16 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex8_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	
	t = (data>>8);
	hex8_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	UART1_send_byte('\t');
}

void press_ad_debug_print8(u8 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex8_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
}

//获得ADC值
//ch:通道值  4/5/6/7/  18/19/20/21
//对应引脚 PA4/5/6/7/PB12/13/14/15
u16 get_adc(u8 ch)   
{
//	adc_init();
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles );	//ADC1,ADC通道,采样时间为4周期	  			    

	// Start ADC1 Software Conversion
	ADC_SoftwareStartConv(ADC1);

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 get_press_adc_average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=get_adc(ch);
//		delay_ms(1);
//		delay_ms(5);
		delay_us(200);
	}
	return temp_val/times;
} 	 


//我们默认将开启6个通道																	   
void  old_press_sensor_adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	//bat检测口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//PA4 5 6 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB12 13 14 15作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	memset(&g_press,0,sizeof(g_press));

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}

}	

void old_press_ad_sample(void)
{
#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
	UART1_send_byte('Q');
	UART1_send_byte('\t');
#endif

//PA4
	g_press.press_ad_value[0] = 0;
	g_press.press_ad_value[0] = get_press_adc_average(ADC_Channel_4,5);
	g_press.press_ad_value[0] =(g_press.press_ad_value[0] >> 8); 
#ifdef DEBUG_MACRO
	press_ad_debug_print(g_press.press_ad_value[0]);
#endif

//PA5
	g_press.press_ad_value[1] = 0;
	g_press.press_ad_value[1] = get_press_adc_average(ADC_Channel_5,5);
	g_press.press_ad_value[1] =(g_press.press_ad_value[1] >> 8); 
#ifdef DEBUG_MACRO
	press_ad_debug_print(g_press.press_ad_value[1]);
#endif

//PA6
	g_press.press_ad_value[2] = 0;
	g_press.press_ad_value[2] = get_press_adc_average(ADC_Channel_6,5);
	g_press.press_ad_value[2] =(g_press.press_ad_value[2] >> 8); 
#ifdef DEBUG_MACRO
	press_ad_debug_print(g_press.press_ad_value[2]);
#endif

//PB12
//	g_press.press_ad_value[3] = 0;
//	g_press.press_ad_value[3] = get_press_adc_average(ADC_Channel_18,5);
//	g_press.press_ad_value[3] =(g_press.press_ad_value[3] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(g_press.press_ad_value[3]);
#endif

//PB13
//	g_press.press_ad_value[4] = 0;
//	g_press.press_ad_value[4] = get_press_adc_average(ADC_Channel_19,5);
//	g_press.press_ad_value[4] =(g_press.press_ad_value[4] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(g_press.press_ad_value[4]);
#endif

//PB14
//	g_press.press_ad_value[5] = 0;
//	g_press.press_ad_value[5] = get_press_adc_average(ADC_Channel_20,5);
//	g_press.press_ad_value[5] =(g_press.press_ad_value[5] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(g_press.press_ad_value[5]);
#endif

//PB15
//	g_press.press_ad_value[6] = 0;
//	g_press.press_ad_value[6] = get_press_adc_average(ADC_Channel_21,5);
//	g_press.press_ad_value[6] =(g_press.press_ad_value[6] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(g_press.press_ad_value[6]);
#endif

}

void old_press_ad_judge(void)
{
	u8 i;
	u8 agg_count=0;
	u8 light_count=0;
	u8 stable_count =0;
	
	for(i=0;i<3;i++)
	{
		if(! g_press.press_ad_value_last[i])
		{
			g_press.change_detail_flag[i] = NO_DETAIL_CHANGE;
			g_press.press_ad_value_last[i] = g_press.press_ad_value[i];
		}else if( g_press.press_ad_value[i] > g_press.press_ad_value_last[i]+PRESSURE_CHANGE_LIMIT)
		{
			//变重了
			g_press.change_detail_flag[i] = GO_AGGRAVATE;
			g_press.press_ad_value_last[i] = g_press.press_ad_value[i];
			
		}else if( g_press.press_ad_value_last[i] > g_press.press_ad_value[i]+PRESSURE_CHANGE_LIMIT )
		{
			//变轻了
			g_press.change_detail_flag[i] = GO_LIGHTEN;
			g_press.press_ad_value_last[i] = g_press.press_ad_value[i];
			
		}else
		{
			g_press.change_detail_flag[i] = NO_DETAIL_CHANGE;
			g_press.press_ad_value_last[i] = g_press.press_ad_value[i];
		}
	}

	for(i=0;i<3;i++)
	{
		if(g_press.change_detail_flag[i] == GO_AGGRAVATE)
			agg_count++;
		else if(g_press.change_detail_flag[i] == GO_LIGHTEN)
			light_count++;
		else if(g_press.change_detail_flag[i] == NO_DETAIL_CHANGE)
			stable_count++;
	}

	if((agg_count>=2)||((agg_count == 1)&&(light_count == 0)))
	{
		g_press.change_flag = NO_TO_HAVE_FLAG;
	}
	else if(((agg_count == 1)&&(light_count >= 1))
			||((agg_count == 0)&&(light_count>=1)))
	{
		g_press.change_flag = HAVE_TO_NO_FLAG;
	}
	else
	{
		g_press.change_flag = NO_CHAGE;
	}
	
#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
	UART1_send_byte('S');
	UART1_send_byte('\t');
	press_ad_debug_print8(agg_count);
	UART1_send_byte('\t');
	press_ad_debug_print8(light_count);
	UART1_send_byte('\t');
	press_ad_debug_print8(stable_count);
	UART1_send_byte('\t');
	press_ad_debug_print8(g_press.change_flag);
#endif	
	
}


/**********************************************************
adc资源：
PA4（ADC_IN4 、PA5（ADC_IN5） 、PA6（ADC_IN6） 、PA7（ADC_IN7）（PA7不好用）、
PB12（ADC_IN18）、PB13（ADC_IN19）、PB14（ADC_IN20）、PB15 （ADC_IN21）
其中：		PA4、PA5用作重量读取；
		PB14 PB15 用作薄膜压力传感器
***********************************************************/
void  adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	//bat检测口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//PB14 15作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

//	ADC_DeInit(ADC1);  //复位ADC1 

	// Enable the HSI oscillator
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;	//8位分辨率
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//12位分辨率
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

//	memset(&g_press,0,sizeof(g_press));

	// Wait until the ADC1 is ready 
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}
}

void press_sensor_init()
{
	memset(&g_press,0,sizeof(g_press));
}

void press_ad_sample(void)
{

//PB14
//	g_press.press_ad_value[5] = 0;
//	g_press.press_ad_value[5] = get_press_adc_average(ADC_Channel_20,5);
//	g_press.press_ad_value[5] =(g_press.press_ad_value[5] >> 8); 
//#ifdef DEBUG_MACRO
//	press_ad_debug_print(g_press.press_ad_value[5]);
//	printf_u16_hexStr(g_press.press_ad_value[5]);
//#endif

//PB15
	g_press.press_ad_value[6] = 0;
	g_press.press_ad_value[6] = get_press_adc_average(ADC_Channel_21,3);
	g_press.press_ad_value[6] &=(0x00FF);	//8位分辨率
//	g_press.press_ad_value[6] &=(0x0FFF);	//12位分辨率

#ifdef DEBUG_MACRO
	printf_string("\npress_sample:");
	printf_u16_decStr(g_press.press_ad_value[6]);
#endif

}

void press_ad_judge(void)
{
	if(g_press.press_ad_value[6]>10)
	{
		g_press.press_result = STA_BOX_CLOSED;
	}
	else
	{
		g_press.press_result = STA_BOX_OPENED;
		g_sta = STRAIN_HANDLE_STA;
	}
}

void press_sensor_handle(void)
{
	if(g_press.sample_flag == 1 )
	{
		g_press.sample_flag = 0; 
		press_ad_sample();
		press_ad_judge();
	}
}


/*********************************************************
* 电池电量相关处理
**********************************************************/
bat_type g_bat;

void bat_init(void)
{
	memset(&g_bat,0,sizeof(bat_type));
}

void bat_sample(void)
{
	//PA0
	g_bat.bat_ad_value = 0;
	g_bat.bat_ad_value = get_press_adc_average(ADC_Channel_0,3);
	g_bat.bat_ad_value &=(0x00FF);	//8位分辨率
//	g_bat.bat_ad_value &=(0x0FFF);	//12位分辨率
	//error algorithm
//	g_bat.bat_ad_value =(g_bat.bat_ad_value >> 8); 	//8位分辨率
//	g_bat.bat_ad_value =(g_bat.bat_ad_value >> 4); 	//12位分辨率

#ifdef DEBUG_MACRO
		printf_string("\nbat_sample:");
		printf_u16_hexStr(g_bat.bat_ad_value);
#endif
}

void bat_get_value(void)
{
	float bat_v=g_bat.bat_ad_value;

	bat_v =((3.3*g_bat.bat_ad_value)/255.0)*2;		//8位分辨率，等待返回 电阻分压的
//	bat_v =((3.3*g_bat.bat_ad_value)/4096.0)*2;		//12位分辨率，等待返回 电阻分压的

	g_bat.bat_value =bat_v*100;
	
#ifdef DEBUG_MACRO
	printf_char('\t');
	printf_bat_value(g_bat.bat_value);
	printf_char('\t');
	printf_u16_decStr(g_bat.bat_value);
#endif
}

void bat_judge(void)
{
	if(g_bat.bat_value > BAT_HIGH_POWER_LIMIT)
	{
		return;
	}
	
	if(g_bat.bat_value<=BAT_LOW_POWER_LIMIT)
	{
		g_bat.normal_power_count = 0;
		if(g_bat.bat_value<=BAT_OFF_POWER_LIMIT)
		{
			g_bat.off_power_count++;
			if(g_bat.off_power_count>=3)
			{
				g_bat.sta =BAT_STA_OFF_POWER;
				LED_all_off();
			}
		}
		else
		{
			g_bat.off_power_count=0;
			g_bat.low_power_count++;
			if(g_bat.low_power_count>=3)
			{
				g_bat.sta =BAT_STA_LOW_POWER;
			}
		}
	}
	else
	{
		g_bat.off_power_count=0;
		g_bat.low_power_count=0;
		g_bat.normal_power_count++;
		if(g_bat.normal_power_count>=3)
		{
			g_bat.sta =BAT_STA_NORMAL_POWER;
		}
	}

	switch(g_bat.sta)
	{
		case BAT_STA_NORMAL_POWER:
			g_bus.report_flag &= ~BAT_OFF_POWER_FLAG;
			g_bus.have_reported_flag &= ~BAT_OFF_POWER_FLAG;
			g_bus.report_flag &= ~BAT_LOW_POWER_FLAG;
			g_bus.have_reported_flag &= ~BAT_LOW_POWER_FLAG;
			break;
		case BAT_STA_OFF_POWER:
			if(!(g_bus.have_reported_flag&BAT_OFF_POWER_FLAG))
			{
				g_bus.report_flag = BAT_OFF_POWER_FLAG;
			}
			break;
		case BAT_STA_LOW_POWER:
			if(!(g_bus.have_reported_flag&BAT_LOW_POWER_FLAG))
			{
				g_bus.report_flag = BAT_LOW_POWER_FLAG;
			}
			break;
		default:
			break;
	}
}

void bat_hangdle()
{
	bat_sample();
	bat_get_value();
	bat_judge();
}

