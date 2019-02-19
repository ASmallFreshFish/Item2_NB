#include "pressure_sensor.h"
#include "usart.h"
#include "led.h"

press_ad_type press_ad;

void press_ad_debug_print(u16 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex_to_char(t,pp);
	UART1_send_byte('\n');
	UART1_send_byte('p');
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	
	t = (data>>8);
	hex_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);	
	UART1_send_byte('\n');
}

//我们默认将开启6个通道																	   
void  press_sensor_adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	//PA4 5 6 7 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB12 13 14 15作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	memset(&press_ad,0,sizeof(press_ad));

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}

}	

//获得ADC值
//ch:通道值  4/5/6/7/  18/19
//对应引脚 PA4/5/6/7/PB12/13
u16 get_press_adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_384Cycles );	//ADC1,ADC通道,采样时间为239.5周期	  			    

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
		temp_val+=get_press_adc(ch);
//		delay_ms(1);
//		delay_ms(5);
		delay_us(200);

	}
	return temp_val/times;
} 	 

void press_ad_sample(void)
{
//#ifdef DEBUG_MACRO
//	u8 t;
//	u8 pp[2];
	
	UART1_send_byte('Q');
	UART1_send_byte('\n');
//#endif
	
	press_ad.press_ad_value[0] = 0;
	press_ad.press_ad_value[0] = get_press_adc_average(ADC_Channel_4,5);
	press_ad.press_ad_value[0] =(press_ad.press_ad_value[0] >> 8); 
//#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[0]);
//#endif

	press_ad.press_ad_value[1] = 0;
	press_ad.press_ad_value[1] = get_press_adc_average(ADC_Channel_5,5);
	press_ad.press_ad_value[1] =(press_ad.press_ad_value[1] >> 8); 
//#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[1]);
//#endif

	press_ad.press_ad_value[2] = 0;
	press_ad.press_ad_value[2] = get_press_adc_average(ADC_Channel_6,5);
	press_ad.press_ad_value[2] =(press_ad.press_ad_value[2] >> 8); 
//#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[2]);
//#endif

	press_ad.press_ad_value[3] = 0;
	press_ad.press_ad_value[3] = get_press_adc_average(ADC_Channel_18,5);
	press_ad.press_ad_value[3] =(press_ad.press_ad_value[3] >> 8); 
//#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[3]);
//#endif

	press_ad.press_ad_value[4] = 0;
	press_ad.press_ad_value[4] = get_press_adc_average(ADC_Channel_19,5);
	press_ad.press_ad_value[4] =(press_ad.press_ad_value[4] >> 8); 
//#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[4]);
//#endif

	press_ad.press_ad_value[5] = 0;
	press_ad.press_ad_value[5] = get_press_adc_average(ADC_Channel_20,5);
	press_ad.press_ad_value[5] =(press_ad.press_ad_value[5] >> 8); 
//#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[5]);
//#endif

	press_ad.press_ad_value[6] = 0;
	press_ad.press_ad_value[6] = get_press_adc_average(ADC_Channel_21,5);
	press_ad.press_ad_value[6] =(press_ad.press_ad_value[6] >> 8); 
//#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[6]);
//#endif



}
void press_ad_judge(void)
{
	if((press_ad.press_ad_value[0] > PRESSURE20_LIMIT )
		||(press_ad.press_ad_value[1] > PRESSURE20_LIMIT )
		||(press_ad.press_ad_value[2] > PRESSURE20_LIMIT ))
		
	{
		press_ad.have_press_count++;
		press_ad.no_press_count = 0;
		if(press_ad.have_press_count >= PRESSURE_CONFIRM_NUM)
		{
			press_ad.sta = HAVE_PRESSURE_SENEOR;
		}
	}
	else
	{
		press_ad.have_press_count = 0;
		press_ad.no_press_count++;
		if(press_ad.no_press_count >= PRESSURE_CONFIRM_NUM)
		{
			press_ad.sta = NO_PRESSURE_SENSOR;
		}
		
	}

	if(press_ad.last_sta != press_ad.sta)
	{
		press_ad.last_sta = press_ad.sta;
		if(press_ad.sta == HAVE_PRESSURE_SENEOR)
		{
			press_ad.change_flag = NO_TO_HAVE_FLAG;
			LEDTEST_OPEN;
		}
		else
		{
			press_ad.change_flag = HAVE_TO_NO_FLAG;
			LEDTEST_CLOSE;
		}
	}
	else
	{
		press_ad.change_flag = NO_CHAGE;
//		LEDTEST_CLOSE;
	}
}

void press_handle(void)
{
	if(press_ad.sample_flag)
	{
		press_ad.sample_flag = 0; 
		press_ad_sample();
		press_ad_judge();
	}
}


