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

//����Ĭ�Ͻ�����6��ͨ��																	   
void  press_sensor_adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	//PA4 5 6 7 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB12 13 14 15��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1 

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

	memset(&press_ad,0,sizeof(press_ad));

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}

}	

//���ADCֵ
//ch:ͨ��ֵ  4/5/6/7/  18/19
//��Ӧ���� PA4/5/6/7/PB12/13
u16 get_press_adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_384Cycles );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    

	// Start ADC1 Software Conversion
	ADC_SoftwareStartConv(ADC1);

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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


