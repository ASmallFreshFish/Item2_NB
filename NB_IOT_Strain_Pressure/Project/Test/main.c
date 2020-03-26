/*
	����COAPЭ�����򿪷��������ݷ��͵�����ƽ̨��������ʾ�����������Ϊ���򿪷�
	ע����ŵĿ�ֻ�ܽ�����ŵ�IOTƽ̨����Ӧ�ƶ��Ŀ������������Ż����ǻ�Ϊ��ƽ̨��
	�����Ҫ����oceanconncet�Ļ�����Ҫ�����ƶ���B8ϵ�п�����
*/

#include "head_include.h"

#define VERSION_Y_M_D	"VERSION_Y_M_D:20200108"
#define VERSION_NUMBER 	"1.2"
const char version_number[]=VERSION_NUMBER;	

main_data_type g_sta =PRESS_HANDLE_STA;

u32 send_udp_count = 0;

//int main(void)
//{		
//	main_init();

//	
//	while(1)
//	{
//		IWDG_Feed();	 //ι��
//		main_sta_judge();
//		main_handle();
//	}
//}

int main(void)
{		
	main_init();

	
	while(1)
	{
//		IWDG_Feed();	 //ι��
//		main_sta_judge();
//		main_handle();
		
		for(int i=0;i<5;i++)
		{
			IWDG_Feed();	 //ι��
			delay_ms(1000);
		}
		BC95_Senddata("8","727394ACB8221234");

	}
}


void main_sta_judge(void)
{
	if((g_bus.report_flag)||(g_time.interrupt_flag & TIME_IR_FLAG_REPORT_INTERVAL)||(upload_buf_round))
	{
		g_sta = BUS_UPLOAD_HANDLE_STA;
	}
}
void main_handle(void)
{
	switch(g_sta)
	{
		case PRESS_HANDLE_STA:
			key_calibration();
			press_sensor_handle();
			g_sta = STRAIN_HANDLE_STA;
			break;
		
		case STRAIN_HANDLE_STA:
			press_strain_handle();
			break;
			
		case BUS_UPLOAD_HANDLE_STA:
			//�����ϴ�
			bat_hangdle();
			upload_handle();
			g_sta = BUS_GET_HANDLE_STA;
			break;

		case BUS_GET_HANDLE_STA:
			//��������
			bus_get_handle();
			break;
			
		default:
			g_sta = PRESS_HANDLE_STA;
			break;
	}
}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//Ӳ��ģ���ʼ��
    LED_Init();
	KEY_init();
	key_cali_init();
	//	adc_init();
//		press_strain_init();

	//���ڡ���ʱ��
    uart_init(9600);  
	uart2_gesture_init(9600); 
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199);  // 500msһ���ж�

	//��վʱ��
	clock_init_time();
    
    //NBģ��ĳ�ʼ��
    BC95_reset_init();
//    CDP_Init();     //CDP��������ʼ��    
//    BC95_Init();
//	upload_init();
	BC95_Init();
		BC95_PDPACT();
		BC95_ConUDP();//ע���ɹ����ȴ�����


	//adc������ʽ
	adc_init();
	//adc+dma��ʽ
//	DMA_config();
//	ADC_Config();

	bat_init();
	press_sensor_init();
	press_strain_init();
	eeprom_init();

	//��ӡ����汾��
	Uart1_SendStr("\r\n");
	Uart1_SendStr(VERSION_Y_M_D);
	//ȥƤ����
	press_strain_init_remove();
	
	//ʹ�ܴ���2:Gesture���
//	uart2_enable();

	//��������ɨ��
//	KEY_scan_start();

	///*****************���Ź���ʼ��***********************/
	IWDG_Init(5,0x0C4E);   //��λʱ��10.0s(6.7s-13.4s)

}


void old_main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//Ӳ��ģ���ʼ��
    LED_Init();
	KEY_init();
	key_cali_init();
	//	adc_init();
//		press_strain_init();

	//���ڡ���ʱ��
    uart_init(9600);  
	uart2_gesture_init(9600); 
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199);  // 500msһ���ж�

	//��վʱ��
	clock_init_time();
    
    //NBģ��ĳ�ʼ��
//    CDP_Init();     //CDP��������ʼ��    
//    BC95_Init();
//	upload_init();

	adc_init();
	bat_init();
	press_sensor_init();
	press_strain_init();
	eeprom_init();
	

	//��ӡ����汾��
	Uart1_SendStr("\r\n");
	Uart1_SendStr(VERSION_Y_M_D);
	//ȥƤ����
	press_strain_init_remove();
	
	//ʹ�ܴ���2:Gesture���
//	uart2_enable();

	//��������ɨ��
//	KEY_scan_start();

	///*****************���Ź���ʼ��***********************/
	IWDG_Init(5,0x0C4E);   //��λʱ��10.0s(6.7s-13.4s)

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



