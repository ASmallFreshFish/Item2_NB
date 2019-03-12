/*
	����COAPЭ�����򿪷��������ݷ��͵�����ƽ̨��������ʾ�����������Ϊ���򿪷�
	ע����ŵĿ�ֻ�ܽ�����ŵ�IOTƽ̨����Ӧ�ƶ��Ŀ������������Ż����ǻ�Ϊ��ƽ̨��
	�����Ҫ����oceanconncet�Ļ�����Ҫ�����ƶ���B8ϵ�п�����
*/

#include "head_include.h"

extern volatile char RxBuffer2[USART2_BUF_LEN]; 
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

extern pressure_strain_type g_weight;

//u8 version[3]={0x19,0x03,0x0121};
//u8 temp,humi;
//u8 sendata[7]="001234";	


int main(void)
{		

	main_init();

	Get_Maopi();				//��ëƤ����
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//���»�ȡëƤ����
	
	while(1)
	{
		UART1_send_byte('\n');
		
		press_strain_handle();
		upload_buf_press_stra_frame();
		upload_press_stra_handle();
	}
}






//int main(void)
//{
//	main_init();
//	
//	while (1)
//	{	
//		gesture_handle();
//		delay_ms(500);
//		upload_buf_gesture_frame();
//		upload_gesture_handle();

//		press_handle();
//		upload_buf_press_frame();
//		upload_press_handle();

//		bat_sample();
//		bat_value_print();
//			delay_ms(1000);




//	}
//}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//Ӳ��ģ���ʼ��
    LED_Init();
	KEY_init();
	press_sensor_adc_init();
	Init_HX711pin();

	//���ڡ���ʱ��
    uart_init(9600);  
	uart2_gesture_init(9600); 
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199);  // 500msһ���ж�
    
    //NBģ��ĳ�ʼ��
    CDP_Init();//CDP��������ʼ��    
    BC95_Init();
	upload_init();
	
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



