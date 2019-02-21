/*
	����COAPЭ�����򿪷��������ݷ��͵�����ƽ̨��������ʾ�����������Ϊ���򿪷�
	ע����ŵĿ�ֻ�ܽ�����ŵ�IOTƽ̨����Ӧ�ƶ��Ŀ������������Ż����ǻ�Ϊ��ƽ̨��
	�����Ҫ����oceanconncet�Ļ�����Ҫ�����ƶ���B8ϵ�п�����
*/

#include "head_include.h"

//u8 version[3]={0x19,0x02,0x21};
//u8 temp,humi;
//u8 sendata[7]="001234";	
u8 sendata_press[SEND_DATA_PRESS_LEN]={'0'};
u8 sendata[SEND_DATA_LEN]={'0'};
u8 send_buf[SEND_BUF_LEN]={'0'};
u8 upload_buf_sequence = 1;
u8 volatile write_location = 0;
u8 volatile read_location = 0;
u8 read_data_success_flag = 0;
u8 write_data_success_flag = 0;
u8 upload_main_loop_count = 0;

#ifdef DEBUG_MACRO
extern u8 debug_char;
#endif

int main(void)
{
//	main_init();
//	
//	while (1)
//	{	
//		press_handle();
//		upload_buf_press_frame();
//		upload_press_handle();
//	}





	all_init();
	UART1_send_byte('Q');
	while(1)
	{
		start_ad_sample();
		start_ad_handle();
		delay_ms(1000);
	}





}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
    LED_Init();
	KEY_init();
	press_sensor_adc_init();
//	ID �¶� ʪ��
//  while(DHT11_Init());//��ʼ��DHT11
    uart_init(9600);          
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199); // 500msһ���ж�
    CDP_Init();//CDP��������ʼ��    
    BC95_Init();
	upload_init();
	
}

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
    LED_Init();

	GPIO_Configuration();
	ADC1_Configuration();
	DMA_Configuration();
	
    uart_init(9600);          
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199); // 500msһ���ж�
//    CDP_Init();//CDP��������ʼ��    
//    BC95_Init();
	upload_init();
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




