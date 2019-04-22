/*
	����COAPЭ�����򿪷��������ݷ��͵�����ƽ̨��������ʾ�����������Ϊ���򿪷�
	ע����ŵĿ�ֻ�ܽ�����ŵ�IOTƽ̨����Ӧ�ƶ��Ŀ������������Ż����ǻ�Ϊ��ƽ̨��
	�����Ҫ����oceanconncet�Ļ�����Ҫ�����ƶ���B8ϵ�п�����
*/

#include "head_include.h"

#define VERSION_Y_M_D		"VERSION_Y_M_D:190422\r\n"

extern volatile char RxBuffer2[USART2_BUF_LEN]; 
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

int main(void)
{
	main_init();

	int sta;
	
	while (1)
	{	
//		gesture_handle();
//		delay_ms(500);
//		upload_buf_gesture_frame();
//		upload_gesture_handle();

		gesture_handle();
		upload_buf_gesture_frame();
		upload_gesture_handle();


		//test code
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15))	
//		{
//			GS_SDA_OUT();
//			PAout(4) = 0;
//			PAout(5) = 0;
//			delay_ms(1500);
//			PAout(4) = 1;
//			PAout(5) = 1;
//			delay_ms(1500);
//		}
//		else
//		{
//			GS_i2c_set_input();
//			GS_SDA_IN();
//			sta = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
//			if(sta)
//				Uart1_SendStr("PA5=1!\r\n");
//			else
//				Uart1_SendStr("PA5=0!\r\n");
//			delay_ms(1000);
//		}
	}
}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//Ӳ��ģ���ʼ��
    LED_Init();
//	KEY_init();
//	press_sensor_adc_init();
//	ID �¶� ʪ��
//  while(DHT11_Init());//��ʼ��DHT11

	//���ڡ���ʱ��
    uart_init(9600);  
//	uart2_gesture_init(9600);   //���Ƽ��
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199);  // 500msһ���ж�
    
    //NBģ��ĳ�ʼ��
    CDP_Init();//CDP��������ʼ��    
    BC95_Init();
	upload_init();

	//��ӡ����汾��
	Uart1_SendStr(VERSION_Y_M_D);
	//���ƴ�������ʼ��
	gesture_paj7620u2_init();
//	GS_i2c_init();//IIC��ʼ��
	
	//ʹ�ܴ���2:Gesture���
//	uart2_enable();
	
//	KEY_scan_start();
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



