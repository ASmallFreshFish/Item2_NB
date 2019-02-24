#include "pressure_sensor.h"
#include "usart.h"
#include "led.h"
#include "stm32l1xx.h"
#include "bus.h"


#define N 1 //ÿͨ����50��
#define M 2 //Ϊ12��ͨ��

//u16 AD_Value[N][M]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
u16 After_filter[M]; //���������ƽ��ֵ֮��Ľ��
u16 AD_Value[M]; 







void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE); //ʹ��DMAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	//PB14/15 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void ADC1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE); //ʹ��DMAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_Configuration();
	ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	// Enable the HSI oscillator 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready 
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode =ENABLE; //ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; //�ⲿ����ת���ر�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = M; //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 8, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_384Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_384Cycles );

	// ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE); //ʹ��ָ����ADC1

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}
}


void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��
	
	DMA_DeInit(DMA1_Channel1); //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = N*M; //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //������NOѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

}

//void filter(void)
//{
//	int i;
//	int sum = 0;
//	u8 count;
//	for(i=0;i<M;i++)
//	{
//		for(count=0;count<N;count++)
//		{
//			sum += AD_Value[count][i];
//		}
//		After_filter[i]=sum/N;
//		sum=0;
//	}
//}


void start_ad_sample(void)
{
	ADC_SoftwareStartConv(ADC1);
//	delay_ms(5);
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
//	{
//		UART1_send_byte('A');
//	};//�ȴ�ת������
//	UART1_send_byte('A');
	
//	AD_Value[0][0]=ADC_GetConversionValue(ADC1);	
//	start_ad_handle();
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
//	while(!DMA_GetFlagStatus(DMA1_FLAG_TC1) )
//	{
//		UART1_send_byte('B');
//	};
}



void start_ad_handle(void)
{

//	press_ad.press_ad_value[1] = 0;
//	press_ad.press_ad_value[1] = get_press_adc_average(ADC_Channel_5,5);
//	press_ad.press_ad_value[1] =(press_ad.press_ad_value[1] >> 8); 
//	press_ad_debug_print(press_ad.press_ad_value[1]);

	UART1_send_byte('Q');
	UART1_send_byte('\n');
	press_ad_debug_print(AD_Value[1]);
	UART1_send_byte('\t');
	AD_Value[1] = (AD_Value[1] >>8);
	press_ad_debug_print(AD_Value[1]);
	UART1_send_byte('\t');

	press_ad_debug_print(AD_Value[2]);
	UART1_send_byte('\t');
	AD_Value[2] = (AD_Value[2] >>8);
	press_ad_debug_print(AD_Value[2]);
	UART1_send_byte('\t');
	
}




//void start_ad_handle(void)
//{
//	u8 t;
//	u8 pp[2];
//	int  i;

//	for(i=0;i<M;i++)
//	{
//		t = AD_Value[0][i];

//		if(t/16>=10)
//			pp[0]=t/16+0x37;//ת��A-F���ַ�
//		else
//			pp[0]=t/16+0x30; 
//		if(t%16>=10)
//			pp[1]=t%16+0x37;//ת��A-F���ַ�
//		else
//			pp[1]=t%16+0x30;
//		UART1_send_byte('\n');
//		UART1_send_byte('p');
//		UART1_send_byte(pp[0]);
//		UART1_send_byte(pp[1]);
//		
//		t = (t>>8);
//		if(t/16>=10)
//			pp[0]=t/16+0x37;//ת��A-F���ַ�
//		else
//			pp[0]=t/16+0x30; 
//		if(t%16>=10)
//			pp[1]=t%16+0x37;//ת��A-F���ַ�
//		else
//			pp[1]=t%16+0x30; 
//		UART1_send_byte(pp[0]);
//		UART1_send_byte(pp[1]);	
//		UART1_send_byte('\n');
//	}
//	
//}








//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
press_ad_type press_ad;

//����Ĭ�Ͻ�����ͨ��0~3																	   
void  press_sensor_adc_init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	//PB15 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1 

	// Enable the HSI oscillator 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready 
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

	 // Wait until the ADC1 is ready 
//	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
//	{}
	

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 get_press_adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	

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
		pp[0]=t/16+0x37;//ת��A-F���ַ�
	else
		pp[0]=t/16+0x30; 
	if(t%16>=10)
		pp[1]=t%16+0x37;//ת��A-F���ַ�
	else
		pp[1]=t%16+0x30;
	UART1_send_byte('\n');
	UART1_send_byte('p');
	UART1_send_byte(pp[0]);
	UART1_send_byte(pp[1]);
	
	t = (press_ad.press_ad_value>>8);
	if(t/16>=10)
		pp[0]=t/16+0x37;//ת��A-F���ַ�
	else
		pp[0]=t/16+0x30; 
	if(t%16>=10)
		pp[1]=t%16+0x37;//ת��A-F���ַ�
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

