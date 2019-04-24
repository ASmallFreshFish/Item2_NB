/*
	����COAPЭ�����򿪷��������ݷ��͵�����ƽ̨��������ʾ�����������Ϊ���򿪷�
	ע����ŵĿ�ֻ�ܽ�����ŵ�IOTƽ̨����Ӧ�ƶ��Ŀ������������Ż����ǻ�Ϊ��ƽ̨��
	�����Ҫ����oceanconncet�Ļ�����Ҫ�����ƶ���B8ϵ�п�����
*/

#include "head_include.h"

#define VERSION_Y_M_D		"VERSION_Y_M_D:190424\r\n"

extern volatile char RxBuffer2[USART2_BUF_LEN]; 
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

int main(void)
{
	main_init();
	
	while (1)
	{	
		IWDG_Feed();	 //ι��
		gesture_pads9960_pwm_handle();
		upload_buf_gesture_frame();
		upload_gesture_handle();

//		gesture_handle();
//		upload_buf_gesture_frame();
//		upload_gesture_handle();
	}
}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	

	///*****************Ӳ��ģ���ʼ��***********************/
	//����
    uart1_init(9600);  			//LOG��ӡ
	uart2_gesture_init(9600); //���Ƽ��
    uart3_init(9600);			//NBģ��ͨѶ
    //��ʱ��
	TIM3_Int_Init(199,3199);   // 20msһ���ж�
    TIM4_Int_Init(4999,3199);  // 500msһ���ж�
    //NBģ��ĳ�ʼ��
    CDP_Init();//CDP��������ʼ��    
    BC95_Init();
	upload_init();

	LED_Init();
//	KEY_init();
//	press_sensor_adc_init();

	//���ƴ�������ʼ��
	//PAJ7620u2
//	gesture_paj7620u2_init();

	//��ӡ����汾��
	Uart1_SendStr(VERSION_Y_M_D);

	///*****************Ӳ��ģ��ʹ��***********************/
	//ʹ�ܴ���2:Gesture���
	//PADS9960
	uart2_enable();
	gesture_pwm_init();
	
//	KEY_scan_start();

	///*****************���Ź���ʼ��***********************/
	IWDG_Init(4,0x0FFF);   //��λʱ��6.5s(4.3s-8.7s)

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



