//#include "pressure_strain.h"
#include "head_include.h"
//#include "delay.h"

u32 HX711_Buffer;
u32 Weight_Maopi;
float Weight_Shiwu;
u8 Flag_Error = 0;

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 1530
#define GapValue 2322

void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);        
		
	//HX711_SCK		PA4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                      // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;                 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//HX711_DOUT	PA5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

//****************************************************
//��ȡHX711
//****************************************************
//u32 HX711_Read(void)        //����128
//{
//	unsigned long count; 
//	unsigned char i; 

//	HX711_SCK=0; 
//	count=0; 
//	delay_us(1);
//	while(HX711_DOUT); 
//	for(i=0;i<24;i++)
//	{ 
//		HX711_SCK=1; 
//		count=count<<1; 
//		delay_us(1);
//		HX711_SCK=0; 
//		if(HX711_DOUT)
//		count++; 
//		delay_us(1);
//	} 
//	HX711_SCK=1; 
//	count=count^0x800000;//��25�������½�����ʱ��ת������
//	delay_us(1);
//	HX711_SCK=0;  
//	return(count);
//}

u32 HX711_Read(void)        //����128
{
	unsigned long count; 
	unsigned char i; 

//	HX711_SCK=0; 
	HX711_SCK_SET_0;
	count=0; 
	delay_us(1);
	while(HX711_DOUT); 
	for(i=0;i<24;i++)
	{ 
//		HX711_SCK=1; 
		HX711_SCK_SET_1;
		count=count<<1; 
		delay_us(1);
//		HX711_SCK=0; 
		HX711_SCK_SET_0;
		if(HX711_DOUT)
		count++; 
		delay_us(1);
	} 
//	HX711_SCK=1; 
	HX711_SCK_SET_1;
	count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);
//	HX711_SCK=0;  
	HX711_SCK_SET_0;
	return(count);
}

//��ȡëƤ����
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//����
//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//void Get_Weight(void)
//{
//	HX711_Buffer = HX711_Read();
//	if(HX711_Buffer > Weight_Maopi)			
//	{
//		Weight_Shiwu = HX711_Buffer;
//		Weight_Shiwu = HX711_Buffer - Weight_Maopi;				//��ȡʵ���AD������ֵ��
//		Weight_Shiwu = (s32)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
//																		
//	}
//}

void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
		Weight_Shiwu = (float)(Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
	}
}

void printf_press_strain_float(float num_f)
{
	u32 para_f = num_f*10;
	u8 bit1,bit2,bit3,bit4,point1;
	u8 flag;
	bit1 = para_f/10000;
	if(bit1 > 0) 
	{
		press_ad_debug_print8(bit1);
		flag = 1;
	}
	
	bit2 = para_f%10000/1000;
	if((bit2 > 0) ||(flag))
	{
		press_ad_debug_print8(bit2);
		flag = 1;
	}

	bit3 = para_f%1000/100;
	if((bit3 > 0) ||(flag))
	{
		press_ad_debug_print8(bit3);
		flag = 1;
	}

	bit4 = para_f%100/10;
	if((bit4 > 0)  ||(flag))
	{
		press_ad_debug_print8(bit4);
		flag = 1;
	}
	UART1_send_byte('.');
	point1 = para_f%10;
	if((point1 > 0)  ||(flag))
	{
		press_ad_debug_print8(point1);
	}
	UART1_send_byte('g');

	UART1_send_byte('\n');
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


