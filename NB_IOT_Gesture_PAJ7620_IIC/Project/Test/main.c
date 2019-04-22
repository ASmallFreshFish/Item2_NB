/*
	利用COAP协议南向开发，将数据发送到电信平台并进行显示。这里术语称为南向开发
	注意电信的卡只能接入电信的IOT平台，对应移动的可以任意接入电信或者是华为的平台。
	如果需要开发oceanconncet的话，需要采用移动的B8系列开发。
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
	
	//硬件模块初始化
    LED_Init();
//	KEY_init();
//	press_sensor_adc_init();
//	ID 温度 湿度
//  while(DHT11_Init());//初始化DHT11

	//串口、定时器
    uart_init(9600);  
//	uart2_gesture_init(9600);   //手势检测
    uart3_init(9600);
	TIM3_Int_Init(199,3199);   // 20ms一次中断
    TIM4_Int_Init(4999,3199);  // 500ms一次中断
    
    //NB模块的初始化
    CDP_Init();//CDP服务器初始化    
    BC95_Init();
	upload_init();

	//打印程序版本号
	Uart1_SendStr(VERSION_Y_M_D);
	//手势传感器初始化
	gesture_paj7620u2_init();
//	GS_i2c_init();//IIC初始化
	
	//使能串口2:Gesture检测
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



