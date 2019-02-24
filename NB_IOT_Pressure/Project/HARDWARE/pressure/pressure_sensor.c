#include "pressure_sensor.h"
#include "usart.h"
#include "led.h"
#include "stm32l1xx.h"
#include "bus.h"


#define N 1 //每通道采50次
#define M 2 //为12个通道

//u16 AD_Value[N][M]; //用来存放ADC转换结果，也是DMA的目标地址
u16 After_filter[M]; //用来存放求平均值之后的结果
u16 AD_Value[M]; 







void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE); //使能DMA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	//PB14/15 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void ADC1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE); //使能DMA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_Configuration();
	ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值

	// Enable the HSI oscillator 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready 
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode =ENABLE; //模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在连续转换模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; //外部触发转换关闭
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = M; //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	//ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
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

	// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}
}


void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟
	
	DMA_DeInit(DMA1_Channel1); //将DMA的通道1寄存器重设为缺省值
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = N*M; //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //工作在NO循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道

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
//	};//等待转换结束
//	UART1_send_byte('A');
	
//	AD_Value[0][0]=ADC_GetConversionValue(ADC1);	
//	start_ad_handle();
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
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
//			pp[0]=t/16+0x37;//转成A-F的字符
//		else
//			pp[0]=t/16+0x30; 
//		if(t%16>=10)
//			pp[1]=t%16+0x37;//转成A-F的字符
//		else
//			pp[1]=t%16+0x30;
//		UART1_send_byte('\n');
//		UART1_send_byte('p');
//		UART1_send_byte(pp[0]);
//		UART1_send_byte(pp[1]);
//		
//		t = (t>>8);
//		if(t/16>=10)
//			pp[0]=t/16+0x37;//转成A-F的字符
//		else
//			pp[0]=t/16+0x30; 
//		if(t%16>=10)
//			pp[1]=t%16+0x37;//转成A-F的字符
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

	// Enable the HSI oscillator 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready 
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

	 // Wait until the ADC1 is ready 
//	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
//	{}
	

}				  
//获得ADC值
//ch:通道值 0~3
u16 get_press_adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	

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

