#include "usart.h"	  
#include "stdio.h"
#include "string.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
//printf定向到串口3
PUTCHAR_PROTOTYPE
{
  	while((USART3->SR&0X40)==0)
		{}//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}

void UART1_send_byte(char data)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, data);
}
void UART3_send_byte(char data)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, data);
}

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
char RxCounter,RxBuffer[100];     //接收缓冲,最大USART_REC_LEN个字节.
char RxCounter1,RxBuffer1[100];     //接收缓冲,最大USART_REC_LEN个字节.
volatile char RxBuffer2[USART2_BUF_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
volatile u8 usart2_read_loc = 0;
volatile u8 usart2_write_loc = 0;

//初始化IO 串口1 log打印
//bound:波特率
void uart_init(u32 bound)
{
    //USART1 GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//使能时钟
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE);		//GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1

	//重映射 PA9 -> USART1_TX,PA10 -> USART1_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
   	//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

//初始化IO 串口2 Gesture检测
//bound:波特率
void uart2_gesture_init(u32 bound)
{
    //USART2 GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能时钟
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE);       //GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //USART2
	
	//重映射 PA2 -> USART2_TX,PA3 -> USART2_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
   	//Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   	//USART2 串口初始化设置
	USART_InitStructure.USART_BaudRate = bound;		//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure);     //初始化串口
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//开启中断
//    USART_Cmd(USART1, ENABLE);                    	//使能串口 

	//soft_init
	memset(RxBuffer2,0,USART2_BUF_LEN-1);
}

//初始化IO 串口3 NB模块通讯
//bound:波特率
void uart3_init(u32 bound)
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
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
   //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 

}

void Uart1_SendStr(char*SendBuf)//串口1打印数据代码
{
	while(*SendBuf)
	{
		while((USART1->SR&0X40)==0){} //等待发送完成
		USART1->DR = (u8) *SendBuf; 
		SendBuf++;
	}
}

void Uart1_SendStr_Len(char*SendBuf,u8 len)//串口1打印数据代码
{
	int i;
	for(i=0;i<len;i++)
	{
		while((USART1->SR&0X40)==0){} //等待发送完成
//		if(*SendBuf == 0) *SendBuf ='*';
		USART1->DR = (u8) *SendBuf; 
		SendBuf++;
	}
}


void uart2_enable(void)
{
	USART_Cmd(USART2, ENABLE); 
}

//中断函数
//log 打印(1,3)
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断，可以扩展来控制
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		RxBuffer1[RxCounter1++] =USART_ReceiveData(USART1);//接收模块的数据
	} 
} 

// gesture 传感器(1,2)
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	char temp; 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断，可以扩展来控制
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if(check_buf_empty(RxBuffer2,USART2_BUF_LEN,usart2_write_loc))
		{
			temp =USART_ReceiveData(USART2);
//			if(((temp>='a')&&(temp<='z'))||((temp>='A')&&(temp<='Z')))
			if((temp>='A')&&(temp<='Z'))
				RxBuffer2[usart2_write_loc++] =USART_ReceiveData(USART2);//接收模块的数据
		}
		if(usart2_write_loc > USART2_BUF_LEN-1)
			usart2_write_loc =0; 
	} 
} 

// NB模块(1,1)
void USART3_IRQHandler(void)                			//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  	//接收模块返回的数据
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		RxBuffer[RxCounter++]=USART_ReceiveData(USART3);		//接收模块的数据;
	} 
} 	

u8 check_buf_empty(char ch[],u8 len,u8 loc)
{
	u8 location,result;
	for(location = loc;location<len;location++)
	{
//		if((ch[location] < 'A' )||((ch[location] > 'Z' )&&(ch[location] < 'a' ))||(ch[location] > 'z' ))
//		{
//			loc = location;
//			return 1;
//		}

		if((ch[location] < 'A' )||(ch[location] > 'Z' ))
		{
			loc = location;
			return 1;
		}
	}

	for(location = 0;location<loc;location++)
	{
//		if((ch[location] < 'A' )||((ch[location] > 'Z' )&&(ch[location] < 'a' ))||(ch[location] > 'z' ))
//		{
//			loc = location;
//			return 1;
//		}

		if((ch[location] < 'A' )||(ch[location] > 'Z' ))
		{
			loc = location;
			return 1;
		}
	}
	return 0;
}

u8 check_buf_valid_data(char ch[],u8 len,u8 loc)
{
	u8 location,result;
	for(location = loc;location<len;location++)
	{
		if(((ch[location]>='a')&&(ch[location]<='z'))||((ch[location]>='A')&&(ch[location]<='Z')))
		{
			loc = location;
			return 1;
		}
	}

	for(location = 0;location<loc;location++)
	{
		if(((ch[location]>='a')&&(ch[location]<='z'))||((ch[location]>='A')&&(ch[location]<='Z')))
		{
			loc = location;
			return 1;
		}
		
	}
	return 0;
}




