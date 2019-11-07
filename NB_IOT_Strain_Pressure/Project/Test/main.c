/*
	����COAPЭ�����򿪷��������ݷ��͵�����ƽ̨��������ʾ�����������Ϊ���򿪷�
	ע����ŵĿ�ֻ�ܽ�����ŵ�IOTƽ̨����Ӧ�ƶ��Ŀ������������Ż����ǻ�Ϊ��ƽ̨��
	�����Ҫ����oceanconncet�Ļ�����Ҫ�����ƶ���B8ϵ�п�����
*/

#include "head_include.h"

#define VERSION_Y_M_D		"VERSION_Y_M_D:191107"

int main(void)
{		
	main_init();

	while(1)
	{
		IWDG_Feed();	 //ι��
		press_strain_handle();
		upload_handle();
	}
}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//Ӳ��ģ���ʼ��
    LED_Init();
	KEY_init();
	press_sensor_adc_init();
	press_strain_init();

	//���ڡ���ʱ��
    uart_init(9600);  
	uart2_gesture_init(9600); 
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199);  // 500msһ���ж�
    
    //NBģ��ĳ�ʼ��
    CDP_Init();     //CDP��������ʼ��    
    BC95_Init();
	upload_init();

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



