/*
	利用COAP协议南向开发，将数据发送到电信平台并进行显示。这里术语称为南向开发
	注意电信的卡只能接入电信的IOT平台，对应移动的可以任意接入电信或者是华为的平台。
	如果需要开发oceanconncet的话，需要采用移动的B8系列开发。
*/

#include "head_include.h"

extern volatile char RxBuffer2[USART2_BUF_LEN]; 
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

#define VERSION_Y_M_D		"VERSION_Y_M_D:190313"

int main(void)
{		
	main_init();
	
	while(1)
	{
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

//		bat_sample();
//		bat_value_print();
//			delay_ms(1000);
//	}
//}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//硬件模块初始化
    LED_Init();
	KEY_init();
	press_sensor_adc_init();
	press_strain_init();
//	Init_HX711pin();

	//串口、定时器
    uart_init(9600);  
	uart2_gesture_init(9600); 
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20ms一次中断
    TIM4_Int_Init(4999,3199);  // 500ms一次中断
    
    //NB模块的初始化
    CDP_Init();//CDP服务器初始化    
    BC95_Init();
	upload_init();

	//打印程序版本号
	Uart1_SendStr(VERSION_Y_M_D);
	//去皮操作
	press_strain_init_remove();
	
	//使能串口2:Gesture检测
//	uart2_enable();

	//启动按键扫描
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



