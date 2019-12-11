#ifndef __KEY_H
#define __KEY_H

#include <stm32l1xx.h>

#define KEY_PA4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define KEY_PA5 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define KEY_PA6 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define KEY_PB12 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define KEY_PB13 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)

typedef enum 
{
	NO_PRESS = 0,
	PA4_PRESS = 0x11,
	PA5_PRESS = 0x22,
	PA6_PRESS = 0x33,
	PA7_PRESS = 0x44,
	PB12_PRESS = 0x55,
	PB13_PRESS = 0x66
}key_press_type;

typedef struct
{
	key_press_type sta;
	unsigned char key_up_flag; 
}key_type;

extern key_type g_key;

void KEY_init(void);
void KEY_scan(void);
void KEY_scan_start(void);
void KEY_scan_stop(void);



/*******************************************************************

使用线路板集成的按键PB12，用于校准重量检测

********************************************************************/

void key_cali_init();
u32 key_Get_Weight(void);









#endif


