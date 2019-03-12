#include "head_include.h"

pressure_strain_type g_weight;

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
	g_weight.maopi_weight = g_weight.maopi_ad*10/GapValue; 		//����ëƤ��ʵ������

#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
	printf_press_strain_ad(g_weight.maopi_ad);
	UART1_send_byte('\t\t\t\t');
	printf_press_strain_weight(g_weight.maopi_weight);
#endif	

	g_weight.maopi_ad = MAOPI_AD;
	g_weight.maopi_weight = MAOPI_WEIGHT;
} 

//����
//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
void Get_Weight(void)
{
	u8 i;
	for(i=0;i<20;i++)
	{
		g_weight.shiwu_ad = HX711_Read();
		if(g_weight.shiwu_ad > g_weight.maopi_ad)			
		{
			g_weight.shiwu_ad = g_weight.shiwu_ad - g_weight.maopi_ad;		//��ȡʵ���AD������ֵ��
			g_weight.shiwu_weight[i] = g_weight.shiwu_ad*10/GapValue; 			//����ʵ���ʵ������
		}
		else
			g_weight.shiwu_weight[i] = 0; 		
	}
}

//void press_strain_judge(void)
//{ 
//	press_strain_sort_average(g_weight.shiwu_weight ,20);

//	#ifdef DEBUG_MACRO
//		printf_press_strain_weight(g_weight.shiwu_weight_ave);
//	#endif

//	g_weight.sta = NO_S_STA;
//	if(g_weight.shiwu_weight_ave >= g_weight.shiwu_weight_ave_last[0])
//	{
//		if(g_weight.shiwu_weight_ave-g_weight.shiwu_weight_ave_last[0] > PRESS_STRAIN_CHANGE_LIMIT)
//		{
//			g_weight.sta = GO_S_AGGRAVATE;
//		}
//	}
//	else
//	{
//		if(g_weight.shiwu_weight_ave_last[0]-g_weight.shiwu_weight_ave > PRESS_STRAIN_CHANGE_LIMIT)
//		{
//			g_weight.sta = GO_S_LIGHTEN;
//		}
//	}	

//	g_weight.shiwu_weight_ave_last[0] =g_weight.shiwu_weight_ave;
//}

void press_strain_judge(void)
{ 
	press_strain_sort_average(g_weight.shiwu_weight ,20);

	#ifdef DEBUG_MACRO
		printf_press_strain_weight(g_weight.shiwu_weight_ave);
	#endif

	g_weight.sta = NO_S_STA;
	
	if((g_weight.shiwu_weight_ave > g_weight.shiwu_weight_ave_last[0]+PRESS_STRAIN_STABLE_LIMIT)
		||(g_weight.shiwu_weight_ave_last[0] > g_weight.shiwu_weight_ave+PRESS_STRAIN_STABLE_LIMIT))
	{
		g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave_last[0];
		g_weight.shiwu_weight_ave_last[0] =	 g_weight.shiwu_weight_ave;
		return;
	}
	
	
	if(g_weight.shiwu_weight_ave >= g_weight.shiwu_weight_ave_last[1])
	{
		if(g_weight.shiwu_weight_ave-g_weight.shiwu_weight_ave_last[1] >= PRESS_STRAIN_CHANGE_LIMIT)
		{
			g_weight.sta = GO_S_AGGRAVATE;
			g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave;
		}
	}
	else
	{
		if(g_weight.shiwu_weight_ave_last[1]-g_weight.shiwu_weight_ave >= PRESS_STRAIN_CHANGE_LIMIT)
		{
			g_weight.sta = GO_S_LIGHTEN;
			g_weight.shiwu_weight_ave_last[1] = g_weight.shiwu_weight_ave;
		}
	}	

	g_weight.shiwu_weight_ave_last[0] =g_weight.shiwu_weight_ave;
}


void press_strain_handle(void)
{
	if(g_weight.sample_flag)
	{
		g_weight.sample_flag = 0;
		Get_Weight();
		press_strain_judge();
	}
}

//����
//20������������β��ȥ5�����ݣ�ʣ����ƽ��
void press_strain_sort_average(u32 ch[],u8 num)
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
				int temp=ch[j];
				ch[j]=ch[j+1];
				ch[j+1]=temp;
			}
		}
	}

	for(i=5;i<num-5;i++)
		total+=ch[i];
	g_weight.shiwu_weight_ave = total/10;
}


//��ӡ����
void printf_press_strain_weight(u32 num_f)
{
	u32 para_f = num_f;
	u8 bit1,bit2,bit3,bit4,point1;
	
	bit1 = para_f/10000;
		printf_press_strain_u8(bit1);
	bit2 = para_f%10000/1000;
		printf_press_strain_u8(bit2);
	bit3 = para_f%1000/100;
		printf_press_strain_u8(bit3);
	bit4 = para_f%100/10;
		printf_press_strain_u8(bit4);
	UART1_send_byte('.');
	point1 = para_f%10;
		printf_press_strain_u8(point1);
	UART1_send_byte('g');

//	UART1_send_byte('\n');
}

void printf_press_strain_ad(u32 num_d)
{
	u32 para_f = num_d;
	u8 i,bit[10];

	for(i=0;i<10;i++)
	{
		bit[i] = para_f%10;
		para_f = para_f/10;
	}
	for(i=0;i<10;i++)
	{
		printf_press_strain_u8(bit[9-i]);
	}
}

void printf_press_strain_u8(u8 data)
{
 	u8 t;
	u8 pp[2];
	
	t = data;
	hex_to_char(t,pp);
	if(pp[0]!= '0')
	{
		UART1_send_byte(pp[0]);
	}
	UART1_send_byte(pp[1]);
}


