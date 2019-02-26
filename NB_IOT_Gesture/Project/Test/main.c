/*
	利用COAP协议南向开发，将数据发送到电信平台并进行显示。这里术语称为南向开发
	注意电信的卡只能接入电信的IOT平台，对应移动的可以任意接入电信或者是华为的平台。
	如果需要开发oceanconncet的话，需要采用移动的B8系列开发。
*/

#include "head_include.h"

extern volatile char RxBuffer2[USART2_BUF_LEN]; 
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;


//u8 version[3]={0x19,0x02,0x26};
//u8 temp,humi;
//u8 sendata[7]="001234";	

int main(void)
{
	main_init();
	
	while (1)
	{	
//		int i=0;
//		UART1_send_byte('\n');
//		Uart1_SendStr(RxBuffer2);
//		for(i=0;i<200;i++)
//			RxBuffer2[i]=0;
//		usart2_write_loc =0;
//			
//		delay_ms(1000);


//		press_handle();
//		upload_buf_press_frame();
//		upload_press_handle();

		gesture_handle();
		delay_ms(500);
		upload_buf_gesture_frame();
		upload_gesture_handle();
		
	}
}

void main_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // interrupt priority level set
    delay_init();	
	
	//硬件模块初始化
    LED_Init();
	KEY_init();
	press_sensor_adc_init();
//	ID 温度 湿度
//  while(DHT11_Init());//初始化DHT11

	//串口、定时器
    uart_init(9600);  
	uart2_gesture_init(9600); 
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20ms一次中断
    TIM4_Int_Init(4999,3199);  // 500ms一次中断
    
    //NB模块的初始化
//    CDP_Init();//CDP服务器初始化    
//    BC95_Init();
	upload_init();
	
	//使能串口2:Gesture检测
	uart2_enable();
	
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




