#include "head_include.h"

void main_init(void)
{
	init_hardware();
	init_software();
	init_start();
}
void init_hardware(void)
{
	init_Cortex_M3();
	init_clock();
	init_port();
	init_ad();
	init_usart1(9600);
//	uart_init(9600);
	init_usart2(9600);
	init_usart3(9600);
	init_timer3(199,3199);  	//20msһ���жϣ����ڰ������
	init_timer4(4999,3199);  	// 500msһ���жϣ����ڵ���˸			
//	init_NB_IOT();

//	UART1_send_byte('1');

	CDP_Init();		//CDP��������ʼ��    
    BC95_Init();
	
}
void init_software(void)
{
	LED_Init();
	KEY_init();
	press_sensor_adc_init();
	gesture_init();
	upload_init();
}

void init_start(void)
{
	//ʹ�ܴ���2:Gesture���
	uart2_enable();
	//ʹ�ܰ������
	//	KEY_scan_start();
}


//void main_init(void)
//{
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
//    delay_init();	
//	
//	//Ӳ��ģ���ʼ��
//    LED_Init();
//	KEY_init();
//	press_sensor_adc_init();

//	//���ڡ���ʱ��
//    uart_init(9600);  
//	uart2_gesture_init(9600); 
//    uart3_init(9600);
//	TIM3_Int_Init(199,3199);   // 20msһ���ж�
//    TIM4_Int_Init(4999,3199);  // 500msһ���ж�
//    
//    //NBģ��ĳ�ʼ��
//    CDP_Init();//CDP��������ʼ��    
//    BC95_Init();
//	upload_init();
//	
//	//ʹ�ܴ���2:Gesture���
//	uart2_enable();
//	
//}






void init_Cortex_M3(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
}

void init_clock(void)
{
	delay_init();	
}
void init_port(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	
	//for LED 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	//for KEY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void init_ad(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	//for BAT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//for PRESSURE
	//PA4 5 6 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB12 13 14 15��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1 

	// Enable the HSI oscillator 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready 
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//ģ��ת�������ڵ���ת��ģʽ
	//ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//�ֱ���Ϊ12λ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);			//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
  
	ADC_Cmd(ADC1, ENABLE);		//ʹ��ָ����ADC1

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}
	
}

//��ʼ��IO ����1 log��ӡ
//bound:������
void init_usart1(u32 bound)
{
	//USART1 GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//ʹ��ʱ��
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE); 		//GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1

	//��ӳ�� PA9 -> USART1_TX,PA10 -> USART1_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 	//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;					//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�����ж�
	USART_Cmd(USART1, ENABLE);					  	//ʹ�ܴ��� 

}

//��ʼ��IO ����2 Gesture���
//bound:������
void init_usart2(u32 bound)
{
	//USART2 GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʹ��ʱ��
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE); 	  	//GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART2
	
	//��ӳ�� PA2 -> USART2_TX,PA3 -> USART2_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
  
	//USART2 ���ڳ�ʼ������
	USART_InitStructure.USART_BaudRate = bound; 				//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);	  
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//�����ж�
	USART_Cmd(USART1, DISABLE);							//��ʹ�ܴ��� 

}

//��ʼ��IO ����3 NBģ��ͨѶ
//bound:������
void init_usart3(u32 bound)
{
    //USART3 GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//ʹ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//��ӳ�� PB10 -> USART3_TX, PB11 -> USART3_RX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 		//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
   //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
  
   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;					//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); 
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

void init_timer3(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 	//ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM3, DISABLE);  	//��ʹ��TIMx����
}

void init_timer4(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM4, ENABLE);  	//��������
}

void init_NB_IOT(void)
{
	CDP_Init();		//CDP��������ʼ��    
    BC95_Init();
	upload_init();
}

void init_watchdog(void);
void init_reset(void);


