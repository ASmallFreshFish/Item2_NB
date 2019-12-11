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
	hex8_to_hexchar(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	
	t = (data>>8);
	hex8_to_hexchar(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	UART1_send_byte('\t');
}

void press_ad_debug_print8(u8 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex8_to_hexchar(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
}

//获得ADC值
//ch:通道值  4/5/6/7/  18/19/20/21
//对应引脚 PA4/5/6/7/PB12/13/14/15
u16 get_adc(u8 ch)   
{
	adc_init();
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_384Cycles );	//ADC1,ADC通道,采样时间为4周期	  			    
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_9Cycles );	
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
		delay_ms(5);
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

	//PB14 15作为薄膜压力模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

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

void adc_enable(void)
{
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	// Wait until the ADC1 is ready 
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}
}

void adc_disable(void)
{
	ADC_Cmd(ADC1, DISABLE);	//失能指定的ADC1

}


void press_sensor_init()
{
	memset(&g_press,0,sizeof(g_press));
}

void press_ad_sample(void)
{

//PB14
	g_press.press_ad_value[5] = 0;
//	g_press.press_ad_value[5] = get_press_adc_average(ADC_Channel_20,3);
	g_press.press_ad_value[5] = get_adc(ADC_Channel_20);
	g_press.press_ad_value[5] &=(0x00FF);	//8位分辨率

//PB15
	g_press.press_ad_value[6] = 0;
//	g_press.press_ad_value[6] = get_press_adc_average(ADC_Channel_21,3);
	g_press.press_ad_value[6] = get_adc(ADC_Channel_21);
	g_press.press_ad_value[6] &=(0x00FF);	//8位分辨率
//	g_press.press_ad_value[6] &=(0x0FFF);	//12位分辨率

#ifdef DEBUG_MACRO
	printf_string("\npress_sample:");
	printf_u16_decStr(g_press.press_ad_value[5]);
	printf_string("\t");
	printf_u16_decStr(g_press.press_ad_value[6]);
#endif

}

void press_ad_judge(void)
{
	if(g_press.press_ad_value[5]<PRESS_CLOSE_THRESHOLD)
	{
		if(g_press.press_ad_value[6]<PRESS_CLOSE_THRESHOLD)
		{
			g_press.press_result = STA_BOX_OPENED_TWO;
		}
		else
		{
			g_press.press_result = STA_BOX_OPENED_ONE;
		}
	}
	else
	{
		if(g_press.press_ad_value[6]<PRESS_CLOSE_THRESHOLD)
		{
			g_press.press_result = STA_BOX_OPENED_ONE;
		}
		else
		{
			g_press.press_result = STA_BOX_CLOSED;
		}
	}

}

void press_sensor_handle(void)
{
	if(g_press.sample_flag == 1 )
	{
		g_press.sample_flag = 0; 
//		adc_enable();
		press_ad_sample();
		adc_disable();
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
	g_bat.sample_flag =1;
}

void bat_sample(void)
{
//	adc_enable();
		
	//PA0
	g_bat.bat_ad_value = 0;
//	g_bat.bat_ad_value = get_press_adc_average(ADC_Channel_0,3);
	g_bat.bat_ad_value = get_adc(ADC_Channel_0);

	g_bat.bat_ad_value &=(0x00FF);	//8位分辨率
//	g_bat.bat_ad_value &=(0x0FFF);	//12位分辨率
	//error algorithm
//	g_bat.bat_ad_value =(g_bat.bat_ad_value >> 8); 	//8位分辨率
//	g_bat.bat_ad_value =(g_bat.bat_ad_value >> 4); 	//12位分辨率

	adc_disable();
#ifdef DEBUG_MACRO_INIT
		printf_string("\nbat_sample:");
		printf_u16_hexStr(g_bat.bat_ad_value);
#endif
}

void bat_get_value(void)
{
	float bat_v=0.0;

	bat_v =((3.3*g_bat.bat_ad_value)/255.0)*2;		//8位分辨率，等待返回 电阻分压的
//	bat_v =((3.3*g_bat.bat_ad_value)/4096.0)*2;		//12位分辨率，等待返回 电阻分压的

	g_bat.bat_value =bat_v*100;

	if(g_bat.bat_value == 0)
	{
		g_bat.bat_zero_count ++;
		if(g_bat.bat_zero_count >= BAT_ZERO_NUMBER_LIMIT)
		{
			g_bat.last_bat_value = g_bat.bat_value;;
		}
		else
		{
			g_bat.bat_value = g_bat.last_bat_value;
		}
	}
	else
	{
		g_bat.bat_zero_count =0;
		g_bat.last_bat_value =g_bat.bat_value;
	}

#ifdef DEBUG_MACRO_INIT
	printf_string("\t");
	printf_bat_value(g_bat.bat_value);
	printf_string("\t");
	printf_u16_decStr(g_bat.bat_value);
	printf_string("\tcount:");
	printf_u8_decStr(g_bat.bat_zero_count);
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
//			g_bat.off_power_count++;
//			if(g_bat.off_power_count>=3)
//			{
//				g_bat.sta =BAT_STA_OFF_POWER;
//				g_bat.sample_flag =0;
//				LED_all_off();
//				
//			}
		}
		else
		{
			g_bat.off_power_count=0;
			g_bat.low_power_count++;
			if(g_bat.low_power_count>=3)
			{
				g_bat.sta = BAT_STA_LOW_POWER;
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
//		case BAT_STA_OFF_POWER:
//			if(!(g_bus.have_reported_flag&BAT_OFF_POWER_FLAG))
//			{
//				g_bus.report_flag |= BAT_OFF_POWER_FLAG;
//			}
//			break;
		case BAT_STA_LOW_POWER:
			g_bus.have_reported_flag &= ~BAT_OFF_POWER_FLAG;
			if(!(g_bus.have_reported_flag&BAT_LOW_POWER_FLAG))
			{
				g_bus.report_flag |= BAT_LOW_POWER_FLAG;
			}
			break;
		default:
			break;
	}
}

void bat_hangdle()
{	
	if(g_bat.sample_flag)
	{	
		g_bat.sample_flag =0;
		
		bat_sample();
		bat_get_value();
		bat_judge();
	}
}

void test_bat()
{
	g_test.count++;
	if(g_test.count<=5)
	{
		g_bat.bat_value =380;
	}
	else if(g_test.count<=10)
	{
		g_bat.bat_value =360;
	}
	else if(g_test.count<=15)
	{
		g_bat.bat_value =340;
	}
	else if(g_test.count<=20)
	{
		g_bat.bat_value =360;
	}
	else if(g_test.count<=25)
	{
		g_bat.bat_value =380;
	}
	else if(g_test.count<=30)
	{
		g_bat.bat_value =360;
	}
	else if(g_test.count<=35)
	{
		g_bat.bat_value =340;
	}
	else if(g_test.count<=40)
	{
		g_bat.bat_value =360;
	}
	else if(g_test.count<=45)
	{
		g_bat.bat_value =380;
	}
}


/*********************************************************
* 设备时间相关处理
**********************************************************/
my_time_val_type my_g_time;

void clock_init_time(void)
{
	memset(&my_g_time,0,sizeof(my_g_time));
}

void clock_timer(void)
{
    ++my_g_time.m_clock_utc;   
}

u8 clock_time_get_week(my_time_type *time)
{
	u8 week;
	u16 year = time->year;
	u8 month = time->month;
	if(month == 1 || month == 2)
	{
		month += 12;
		year --;
	}
	week = (time->day+2*month+3*(month+1)/5+year+year/4-year/100+year/400) % 7;
	time->week = (week+1)%7;
	
	return time->week;
}

int clock_time_to_utc(my_time_type *time)
{
    int utc = 0;
    struct tm  timeinfo = {0};
    timeinfo.tm_year =  time->year - 1900;
    timeinfo.tm_mon = time->month - 1;
    timeinfo.tm_mday = time->day;
    timeinfo.tm_hour = time->hour;
    timeinfo.tm_min = time->minutes;
    timeinfo.tm_sec = time->seconds;
    utc = mktime(&timeinfo);
    return utc;
}

void clock_set_time(my_time_type *time) 
{
    time->year += 2000;
    my_g_time.m_clock_utc = clock_time_to_utc(time);
	
#ifdef DEBUG_MACRO
	printf_string("\nutc:");
	printf_u32_decStr(my_g_time.m_clock_utc);
#endif
}

 void clock_get_time(my_time_type* time)
{
    struct tm *ltime;

   ltime = localtime(&my_g_time.m_clock_utc);
   
   time->seconds = ltime->tm_sec;
   time->minutes = ltime->tm_min;
   time->hour    = ltime->tm_hour + 8;
   time->day     = ltime->tm_mday;
   time->month   = ltime->tm_mon + 1;
   time->year    = ltime->tm_year + 1900;
   time->hour %= 24;
   clock_time_get_week(time);
}

 void clock_cclk_handle(char *p) 
 {
 	my_time_type now_time={0};
	my_time_type next_time={0};
	u32 year,month,day,hour,minute,second;

	p += strlen("+CCLK:");

	sscanf(p,"%02d/%02d/%02d,%02d:%02d:%02d",&year,&month,&day,&hour,&minute,&second);
	
	now_time.year = year;
	now_time.month = month;
	now_time.day= day;
	now_time.hour= hour;
	now_time.minutes= minute;
	now_time.seconds= second;
	clock_set_time(&now_time);

#ifdef DEBUG_MACRO_INIT
if(0)
{
	printf_string("\ntime:");
	printf_u16_decStr(now_time.year);	printf_string("/");
	printf_u8_decStr(now_time.month);	printf_string("/");
	printf_u8_decStr(now_time.day);		printf_string(":");
	printf_u8_decStr(now_time.hour);	printf_string(":");
	printf_u8_decStr(now_time.minutes);	printf_string(":");
	printf_u8_decStr(now_time.seconds);
}
#endif

#ifdef DEBUG_MACRO_INIT
	if(0)
	{
		clock_timer();
		clock_get_time(&next_time);

		printf_string("\ntime:");
		printf_u16_decStr(next_time.year);	printf_string("/");
		printf_u8_decStr(next_time.month);	printf_string("/");
		printf_u8_decStr(next_time.day);	printf_string(":");
		printf_u8_decStr(next_time.hour);	printf_string(":");
		printf_u8_decStr(next_time.minutes);printf_string(":");
		printf_u8_decStr(next_time.seconds);
	}
#endif

 } 

 u8 clock_syn_time(void)
 {
	 u8 result =FALSE;
	 
	 printf("AT+CCLK?\r\n");//同步基站时间
		 Delay(300);
		 strx=strstr((const char*)RxBuffer,(const char*)"+CCLK:");
 
		 if(strx !=NULL)
		 {
			 clock_cclk_handle(strx);
			 result=TRUE;
		 }
		 Clear_Buffer();
 
		 return result;
 }

void clock_copy_time_to_buf(u8 *buf)
{
	my_time_type now_time={0};

	clock_get_time(&now_time);

	buf[0] = (now_time.year-2000);
	buf[1] = now_time.month;
	buf[2] = now_time.day;
	buf[3] = now_time.hour;
	buf[4] = now_time.minutes;
	buf[5] = now_time.seconds;
	buf[6] = 0;
}



