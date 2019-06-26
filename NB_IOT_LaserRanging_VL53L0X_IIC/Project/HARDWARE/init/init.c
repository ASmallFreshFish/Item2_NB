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
	init_timer3(199,3199);  	//20ms一次中断，用于按键检测
	init_timer4(4999,3199);  	// 500ms一次中断，用于等闪烁			
//	init_NB_IOT();

//	UART1_send_byte('1');

	CDP_Init();		//CDP服务器初始化    
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
	//使能串口2:Gesture检测
	uart2_enable();
	//使能按键检测
	//	KEY_scan_start();
}


//void main_init(void)
//{
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
//    delay_init();	
//	
//	//硬件模块初始化
//    LED_Init();
//	KEY_init();
//	press_sensor_adc_init();

//	//串口、定时器
//    uart_init(9600);  
//	uart2_gesture_init(9600); 
//    uart3_init(9600);
//	TIM3_Int_Init(199,3199);   // 20ms一次中断
//    TIM4_Int_Init(4999,3199);  // 500ms一次中断
//    
//    //NB模块的初始化
//    CDP_Init();//CDP服务器初始化    
//    BC95_Init();
//	upload_init();
//	
//	//使能串口2:Gesture检测
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	//for BAT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//for PRESSURE
	//PA4 5 6 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB12 13 14 15作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	// Enable the HSI oscillator 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready 
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//模数转换工作在单次转换模式
	//ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//分辨率为12位
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);			//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	ADC_Cmd(ADC1, ENABLE);		//使能指定的ADC1

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{}
	
}

//初始化IO 串口1 log打印
//bound:波特率
void init_usart1(u32 bound)
{
	//USART1 GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//使能时钟
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE); 		//GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1

	//重映射 PA9 -> USART1_TX,PA10 -> USART1_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 	//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;					//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启中断
	USART_Cmd(USART1, ENABLE);					  	//使能串口 

}

//初始化IO 串口2 Gesture检测
//bound:波特率
void init_usart2(u32 bound)
{
	//USART2 GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能时钟
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE); 	  	//GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART2
	
	//重映射 PA2 -> USART2_TX,PA3 -> USART2_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
  
	//USART2 串口初始化设置
	USART_InitStructure.USART_BaudRate = bound; 				//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART2, &USART_InitStructure);	  
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//开启中断
	USART_Cmd(USART1, DISABLE);							//不使能串口 

}

//初始化IO 串口3 NB模块通讯
//bound:波特率
void init_usart3(u32 bound)
{
    //USART3 GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//使能时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//重映射 PB10 -> USART3_TX, PB11 -> USART3_RX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 		//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
   //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
  
   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;					//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure); 
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 

}

void init_timer3(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 	//使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM3, DISABLE);  	//不使能TIMx外设
}

void init_timer4(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM4, ENABLE);  	//开启外设
}

void init_NB_IOT(void)
{
	CDP_Init();		//CDP服务器初始化    
    BC95_Init();
	upload_init();
}

void init_watchdog(void);
void init_reset(void);


