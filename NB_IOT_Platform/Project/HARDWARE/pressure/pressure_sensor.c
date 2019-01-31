#include "pressure_sensor.h"
#include "usart.h"
#include "led.h"

press_ad_type press_ad;

//我们默认将开启通道0~3																	   
void  press_sensor_adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	//PB15 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

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
//	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
//	{}
	
//	ADC_ResetCalibration(ADC1);	//使能复位校准  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
//	
//	ADC_StartCalibration(ADC1);	 //开启AD校准
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 get_press_adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	

	/* Start ADC1 Software Conversion */ 
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
		delay_ms(1);
//		delay_ms(5);
	}
	return temp_val/times;
} 	 

void press_ad_sample(void)
{
//#ifdef DEBUG_MACRO
	u8 t;
	u8 pp[2];
//#endif

	press_ad.press_ad_value = 0;
	press_ad.press_ad_value = get_press_adc_average(ADC_Channel_21,10);
	press_ad.press_ad_value =(press_ad.press_ad_value >> 8); 
//#ifdef DEBUG_MACRO

	t = press_ad.press_ad_value;

	if(t/16>=10)
		pp[0]=t/16+0x37;//转成A-F的字符
	else
		pp[0]=t/16+0x30; 
	if(t%16>=10)
		pp[1]=t%16+0x37;//转成A-F的字符
	else
		pp[1]=t%16+0x30;
	UART1_send_byte('\n');
	UART1_send_byte('p');
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	
	t = (press_ad.press_ad_value>>8);
	if(t/16>=10)
		pp[0]=t/16+0x37;//转成A-F的字符
	else
		pp[0]=t/16+0x30; 
	if(t%16>=10)
		pp[1]=t%16+0x37;//转成A-F的字符
	else
		pp[1]=t%16+0x30; 
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);	
	UART1_send_byte('\n');

//#endif

}
void press_ad_judge(void)
{
	if(press_ad.press_ad_value > PRESSURE20_LIMIT )
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


