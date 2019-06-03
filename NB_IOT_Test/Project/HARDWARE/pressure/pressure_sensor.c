#include "pressure_sensor.h"
#include "usart.h"
#include "led.h"
#include "bus.h"

press_ad_type press_ad;

u16 RegularConvData_Tab[2];//�洢2����ѹֵ
u16 VREFINT_DATA;
float Vbat_value;

void press_ad_debug_print(u16 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	
	t = (data>>8);
	hex_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	UART1_send_byte('\t');
}

void press_ad_debug_print8(u8 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex_to_char(t,pp);
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
}


//����Ĭ�Ͻ�����6��ͨ��																	   
void  press_sensor_adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	//bat����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//PA4 5 6 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB12 13 14 15��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
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
//ch:ͨ��ֵ  4/5/6/7/  18/19/20/21
//��Ӧ���� PA4/5/6/7/PB12/13/14/15
u16 get_press_adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_384Cycles );	//ADC1,ADCͨ��,����ʱ��Ϊ4����	  			    

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
#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
	UART1_send_byte('Q');
	UART1_send_byte('\t');
#endif

//PA4
	press_ad.press_ad_value[0] = 0;
	press_ad.press_ad_value[0] = get_press_adc_average(ADC_Channel_4,5);
	press_ad.press_ad_value[0] =(press_ad.press_ad_value[0] >> 8); 
#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[0]);
#endif

//PA5
	press_ad.press_ad_value[1] = 0;
	press_ad.press_ad_value[1] = get_press_adc_average(ADC_Channel_5,5);
	press_ad.press_ad_value[1] =(press_ad.press_ad_value[1] >> 8); 
#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[1]);
#endif

//PA6
	press_ad.press_ad_value[2] = 0;
	press_ad.press_ad_value[2] = get_press_adc_average(ADC_Channel_6,5);
	press_ad.press_ad_value[2] =(press_ad.press_ad_value[2] >> 8); 
#ifdef DEBUG_MACRO
	press_ad_debug_print(press_ad.press_ad_value[2]);
#endif

//PB12
//	press_ad.press_ad_value[3] = 0;
//	press_ad.press_ad_value[3] = get_press_adc_average(ADC_Channel_18,5);
//	press_ad.press_ad_value[3] =(press_ad.press_ad_value[3] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[3]);
#endif

//PB13
//	press_ad.press_ad_value[4] = 0;
//	press_ad.press_ad_value[4] = get_press_adc_average(ADC_Channel_19,5);
//	press_ad.press_ad_value[4] =(press_ad.press_ad_value[4] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[4]);
#endif

//PB14
//	press_ad.press_ad_value[5] = 0;
//	press_ad.press_ad_value[5] = get_press_adc_average(ADC_Channel_20,5);
//	press_ad.press_ad_value[5] =(press_ad.press_ad_value[5] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[5]);
#endif

//PB15
//	press_ad.press_ad_value[6] = 0;
//	press_ad.press_ad_value[6] = get_press_adc_average(ADC_Channel_21,5);
//	press_ad.press_ad_value[6] =(press_ad.press_ad_value[6] >> 8); 
#ifdef DEBUG_MACRO
//	press_ad_debug_print(press_ad.press_ad_value[6]);
#endif

}

void press_ad_judge(void)
{
	u8 i;
	u8 agg_count=0;
	u8 light_count=0;
	u8 stable_count =0;
	
	for(i=0;i<3;i++)
	{
		if(! press_ad.press_ad_value_last[i])
		{
			press_ad.change_detail_flag[i] = NO_DETAIL_CHANGE;
			press_ad.press_ad_value_last[i] = press_ad.press_ad_value[i];
		}else if( press_ad.press_ad_value[i] > press_ad.press_ad_value_last[i]+PRESSURE_CHANGE_LIMIT)
		{
			//������
			press_ad.change_detail_flag[i] = GO_AGGRAVATE;
			press_ad.press_ad_value_last[i] = press_ad.press_ad_value[i];
			
		}else if( press_ad.press_ad_value_last[i] > press_ad.press_ad_value[i]+PRESSURE_CHANGE_LIMIT )
		{
			//������
			press_ad.change_detail_flag[i] = GO_LIGHTEN;
			press_ad.press_ad_value_last[i] = press_ad.press_ad_value[i];
			
		}else
		{
			press_ad.change_detail_flag[i] = NO_DETAIL_CHANGE;
			press_ad.press_ad_value_last[i] = press_ad.press_ad_value[i];
		}
	}

	for(i=0;i<3;i++)
	{
		if(press_ad.change_detail_flag[i] == GO_AGGRAVATE)
			agg_count++;
		else if(press_ad.change_detail_flag[i] == GO_LIGHTEN)
			light_count++;
		else if(press_ad.change_detail_flag[i] == NO_DETAIL_CHANGE)
			stable_count++;
	}

	if((agg_count>=2)||((agg_count == 1)&&(light_count == 0)))
	{
		press_ad.change_flag = NO_TO_HAVE_FLAG;
	}
	else if(((agg_count == 1)&&(light_count >= 1))
			||((agg_count == 0)&&(light_count>=1)))
	{
		press_ad.change_flag = HAVE_TO_NO_FLAG;
	}
	else
	{
		press_ad.change_flag = NO_CHAGE;
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
	press_ad_debug_print8(press_ad.change_flag);
#endif	
	
}

void press_handle(void)
{
	if(press_ad.sample_flag == 1 )
	{
		press_ad.sample_flag = 0; 
		press_ad_sample();
		press_ad_judge();
	}
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bat_sample(void)
{
		
#ifdef DEBUG_MACRO
		UART1_send_byte('\n');
		UART1_send_byte('Q');
		UART1_send_byte('\t');
#endif
	
		//PA4
		RegularConvData_Tab[0] = 0;
		RegularConvData_Tab[0] = get_press_adc_average(ADC_Channel_0,5);
//		RegularConvData_Tab[0] =(press_ad.press_ad_value[0] >> 4); 
#ifdef DEBUG_MACRO
		press_ad_debug_print(RegularConvData_Tab[0]);
		UART1_send_byte('\t');

#endif
}

float bat_get_value(void)
{
    VREFINT_DATA=RegularConvData_Tab[0];//��ȡ��ADCֵ
    Vbat_value =((3.3*VREFINT_DATA)/4096.0)*2;//�ȴ����� �����ѹ��
//     	Vbat_value =((3.3*VREFINT_DATA)/255.0)*2;//�ȴ����� �����ѹ��
    return Vbat_value;//���ص�ǰֵ  
}


void bat_value_print(void)
{
		float Vbat;//��ص�ѹ
    	int SendVbat;//���͵�ص�ѹ
    	u16 send2_vbat;
    	char str[5];
		
		Vbat=bat_get_value();//��ȡ��ص�ѹ
		SendVbat=Vbat*1000;//3.33*100=333����2λ��Ч����
		send2_vbat=Vbat*1000;
//		str[0]=(SendVbat/16)/10+0x30;
//		str[1]=(SendVbat/16)%10+0x30;
//		str[2]=(SendVbat%16)/10+0x30;
//		str[3]=(SendVbat%16)%10+0x30;

		str[0]=SendVbat/1000+0x30;
		str[1]=SendVbat%1000/100+0x30;
		str[2]=SendVbat%100/10+0x30;
		str[3]=SendVbat%10+0x30;
		str[4] ='\0';
//		BC95_SendCOAPdata("5",str);
		Uart1_SendStr(str);

		UART1_send_byte('\t');
		press_ad_debug_print(send2_vbat);
}


