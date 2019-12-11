#include "head_include.h"

pressure_strain_type g_weight;

void press_strain_init(void)
{
	Init_HX711pin();
	memset(&g_weight,0,sizeof(pressure_strain_type));
	g_weight.change_threshold = 25;   //����2.5g
	g_weight.factor100 = 1000;
}

void press_strain_init_remove(void)
{
	Get_Maopi();				//��ëƤ����
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();	
}

void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);        
		
	//HX711_SCK		PA4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			// �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//HX711_DOUT	PA5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

////��ȡHX711
u32 HX711_Read(void)        //����128
{
	unsigned long count; 
	unsigned char i; 

	HX711_SCK_SET_0;
	count=0; 
	delay_us(1);
	while(HX711_DOUT); 
	for(i=0;i<24;i++)
	{ 
		HX711_SCK_SET_1;
		count=count<<1; 
		delay_us(1);
		HX711_SCK_SET_0;
		if(HX711_DOUT)
		count++; 
		delay_us(1);
	} 
	HX711_SCK_SET_1;
	count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);
	HX711_SCK_SET_0;
	return(count);
}

//��ȡëƤ����
void Get_Maopi(void)
{	
	g_weight.maopi_ad = HX711_Read();	
//	g_weight.maopi_weight = g_weight.maopi_ad*10/GapValue;//����ëƤ��ʵ������(�˴�Ϊ����10������ʾ��0.1g)		

	g_weight.maopi_weight = ((g_weight.maopi_ad*10.0/GapValue/g_weight.factor100)*1000);
	
	UART1_send_byte('\n');
	printf_u32_decStr(g_weight.maopi_ad);
	UART1_send_byte('\t');UART1_send_byte('\t');
	printf_press_strain_weight(g_weight.maopi_weight); //����ëƤ��ʵ������(�˴�Ϊ����������ʾ��0.1g)	

	g_weight.maopi_ad = MAOPI_AD;
	g_weight.maopi_weight = MAOPI_WEIGHT;

	UART1_send_byte('\t');UART1_send_byte('\t');
	printf_u32_decStr(g_weight.maopi_ad);
	UART1_send_byte('\t');UART1_send_byte('\t');
	printf_press_strain_weight(g_weight.maopi_weight);
} 

//����
//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
void Get_Weight(void)
{
	u8 i;
	for(i=0;i<WEIGHT_SAMPLE_NUMBER;i++)
	{
		g_weight.shiwu_ad = HX711_Read();
		
#ifdef DEBUG_MACRO
	if(0)
	{
		printf_string("\nweight sample:\t");
		printf_u32_decStr(g_weight.shiwu_ad);
	}
#endif
		if(g_weight.shiwu_ad > g_weight.maopi_ad)			
		{
			g_weight.shiwu_ad = g_weight.shiwu_ad - g_weight.maopi_ad;		//��ȡʵ���AD������ֵ��
//			g_weight.shiwu_weight[i] = g_weight.shiwu_ad*10/GapValue; 			//����ʵ���ʵ������

			g_weight.shiwu_weight[i] = ((g_weight.shiwu_ad*10.0/GapValue/g_weight.factor100)*1000);
		}
		else
			g_weight.shiwu_weight[i] = 0; 		

		delay_ms(100);
	}
}


/***************************************************************************/
//��ֵ�ж�
//�ȶ�������(���С��0.3g)������10g����Ϊû��ҩ�ˣ���������ϱ�10�Σ������ϱ�
//������10g��������Ӻͼ���ҩ��(һ����ֵ2g)
//last[0]��ʵʱֵ�������ж��Ƿ��ȶ���last[1]����ֵ̬�������ж��Ƿ��Ǳ仯������ֵ��
/***************************************************************************/

void press_strain_judge(void)
{ 
	g_weight.shiwu_weight_ave = press_strain_sort_average(g_weight.shiwu_weight ,WEIGHT_SAMPLE_NUMBER);

		//test
//		g_weight.shiwu_weight_ave=0;
	
	#ifdef DEBUG_MACRO
		printf_string("\nweight_ave:");
		printf_press_strain_weight(g_weight.shiwu_weight_ave);
	#endif

	g_weight.sta = NO_S_STA;
	g_weight.changed_data =0;

	//������ݲ��ȶ�,�����ж�
	if((g_weight.shiwu_weight_ave > g_weight.shiwu_weight_ave_last[0]+PRESS_STRAIN_STABLE_LIMIT)
		||(g_weight.shiwu_weight_ave_last[0] > g_weight.shiwu_weight_ave+PRESS_STRAIN_STABLE_LIMIT))
	{
		g_weight.shiwu_weight_ave_last[0] =	 g_weight.shiwu_weight_ave;
		return;
	}

	//�����ȶ�֮��
	//�߼��ж�:С��10g,���ϱ�ȱ��ҩ��;������������֮ǰ������,����;��֮,����;
	
	if(g_weight.shiwu_weight_ave < PRESS_STRAIN_LITTLE_LIMIT)
	{
		if(g_weight.little_count < PRESS_STRA_LIEELE_COUNT)
		{
			g_weight.sta = GO_S_LITTLE;
			if(++g_weight.little_count >= PRESS_STRA_LIEELE_COUNT) 
				g_weight.little_count = PRESS_STRA_LIEELE_COUNT;
//			g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave;
		}
	}
	else if(g_weight.shiwu_weight_ave >= g_weight.shiwu_weight_ave_last[1])
	{
		g_weight.little_count = 0;
		if(g_weight.shiwu_weight_ave >= g_weight.shiwu_weight_ave_last[1] + g_weight.change_threshold)
		{
			if(g_weight.shiwu_weight_ave_last[1] ==0)
				g_weight.sta = NO_S_STA;
			else
				g_weight.sta = GO_S_AGGRAVATE;
			
			g_weight.changed_data=g_weight.shiwu_weight_ave-g_weight.shiwu_weight_ave_last[1];
//			g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave;
//			g_weight.sta = NO_S_STA;	//���ز����ϱ�
		}
	}
	else
	{
		g_weight.little_count = 0;
		if(g_weight.shiwu_weight_ave_last[1]>= g_weight.shiwu_weight_ave + g_weight.change_threshold)
		{
			g_weight.sta = GO_S_LIGHTEN;
			g_weight.changed_data=g_weight.shiwu_weight_ave_last[1]-g_weight.shiwu_weight_ave;
//			g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave;
		}
	}	
	
	g_weight.shiwu_weight_ave_last[0] =g_weight.shiwu_weight_ave;
	g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave;

	if(g_weight.sta)
	{
		g_bus.report_flag |= STRAIN_FLAG;
//		g_bus.report_flag |= PRESS_SENSOR_FLAG;
		g_sta =BUS_UPLOAD_HANDLE_STA;
	}
	else
	{
		g_sta =PRESS_HANDLE_STA;
	}

#ifdef DEBUG_MACRO
	printf_string("\tsta change last:\t ");
	printf_u8_hexStr(g_weight.sta);
	printf_char('\t');
	printf_u16_decStr(g_weight.changed_data);
	printf_char('\t');
	printf_press_strain_weight((u32)g_weight.changed_data);
	printf_char('\t');
	printf_press_strain_weight((u32)g_weight.shiwu_weight_ave_last[0]);
	printf_char('\t');
	printf_press_strain_weight((u32)g_weight.shiwu_weight_ave_last[1]);
#endif

}

//Ӧ��ʽѹ����������������
void press_strain_handle(void)
{
//	if(g_weight.sample_flag)
//	{
//		g_weight.sample_flag = 0;
		Get_Weight();
		press_strain_judge();
//	}
}

void old_press_strain_handle(void)
{
	if(g_weight.sample_flag)
	{
		g_weight.sample_flag = 0;
		Get_Weight();
		press_strain_judge();
	}
}


//����
//10������������β��ȥ2�����ݣ�ʣ����ƽ��
u32 press_strain_sort_average(u32 ch[],u8 num)
{
	u8 i,j;
	u32 temp;
	u32 total = 0; 
	 
	for(i=0;i<num;i++)//���ѭ����Ҫ�ȽϵĴ�����
	{
	 
		for(j=0;j<num-1-i;j++)
		{
			if(ch[j]>ch[j+1])
			{
				temp=ch[j];
				ch[j]=ch[j+1];
				ch[j+1]=temp;
			}
		}
	}

#ifdef DEBUG_MACRO
	if(0)
	{
		UART1_send_byte('\n');
		for(i=0;i<num;i++)
		{
			printf_press_strain_weight(ch[i]);
			UART1_send_byte('\t');
			UART1_send_byte('\t');
			if(i%10 == 9)
				UART1_send_byte('\n');
		}
	}
#endif
	
	for(i=2;i<num-2;i++)
		total+=ch[i];
	return total/6;

#ifdef DEBUG_MACRO
	if(0)
	{
		UART1_send_byte('\n');
		printf_press_strain_weight(total/6);
		UART1_send_byte('\t');
	}
#endif

}



