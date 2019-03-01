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
//printf���򵽴���3
PUTCHAR_PROTOTYPE
{
  	while((USART3->SR&0X40)==0)
		{}//ѭ������,ֱ���������   
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

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char RxCounter,RxBuffer[100];     //���ջ���,���USART_REC_LEN���ֽ�.
char RxCounter1,RxBuffer1[100];     //���ջ���,���USART_REC_LEN���ֽ�.
volatile char RxBuffer2[USART2_BUF_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
volatile u8 usart2_read_loc = 0;
volatile u8 usart2_write_loc = 0;

//��ʼ��IO ����1 log��ӡ
//bound:������
void uart_init(u32 bound)
{
    //USART1 GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//ʹ��ʱ��
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE);		//GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1

	//��ӳ�� PA9 -> USART1_TX,PA10 -> USART1_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
   	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

//��ʼ��IO ����2 Gesture���
//bound:������
void uart2_gesture_init(u32 bound)
{
    //USART2 GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʹ��ʱ��
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE);       //GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //USART2
	
	//��ӳ�� PA2 -> USART2_TX,PA3 -> USART2_RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
   	//Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   	//USART2 ���ڳ�ʼ������
	USART_InitStructure.USART_BaudRate = bound;		//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure);     //��ʼ������
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//�����ж�
//    USART_Cmd(USART1, ENABLE);                    	//ʹ�ܴ��� 

	//soft_init
	memset(RxBuffer2,0,USART2_BUF_LEN-1);
}

//��ʼ��IO ����3 NBģ��ͨѶ
//bound:������
void uart3_init(u32 bound)
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
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
   //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

void Uart1_SendStr(char*SendBuf)//����1��ӡ���ݴ���
{
	while(*SendBuf)
	{
		while((USART1->SR&0X40)==0){} //�ȴ��������
		USART1->DR = (u8) *SendBuf; 
		SendBuf++;
	}
}

void Uart1_SendStr_Len(char*SendBuf,u8 len)//����1��ӡ���ݴ���
{
	int i;
	for(i=0;i<len;i++)
	{
		while((USART1->SR&0X40)==0){} //�ȴ��������
//		if(*SendBuf == 0) *SendBuf ='*';
		USART1->DR = (u8) *SendBuf; 
		SendBuf++;
	}
}


void uart2_enable(void)
{
	USART_Cmd(USART2, ENABLE); 
}

//�жϺ���
//log ��ӡ(1,3)
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����жϣ�������չ������
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		RxBuffer1[RxCounter1++] =USART_ReceiveData(USART1);//����ģ�������
	} 
} 

// gesture ������(1,2)
void USART2_IRQHandler(void)                	//����2�жϷ������
{
	char temp; 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����жϣ�������չ������
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if(check_buf_empty(RxBuffer2,USART2_BUF_LEN,usart2_write_loc))
		{
			temp =USART_ReceiveData(USART2);
//			if(((temp>='a')&&(temp<='z'))||((temp>='A')&&(temp<='Z')))
			if((temp>='A')&&(temp<='Z'))
				RxBuffer2[usart2_write_loc++] =USART_ReceiveData(USART2);//����ģ�������
		}
		if(usart2_write_loc > USART2_BUF_LEN-1)
			usart2_write_loc =0; 
	} 
} 

// NBģ��(1,1)
void USART3_IRQHandler(void)                			//����3�жϷ������
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  	//����ģ�鷵�ص�����
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		RxBuffer[RxCounter++]=USART_ReceiveData(USART3);		//����ģ�������;
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




