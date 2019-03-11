#ifndef __PRESS_STRAIN_H
#define __PRESS_STRAIN_H

//#include "sys.h"

//#define HX711_SCK PBout(0)// PB0
//#define HX711_DOUT PBin(1)// PB1

#define HX711_SCK_SET_1 	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET)
#define HX711_SCK_SET_0 	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET)
#define HX711_DOUT			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

void Init_HX711pin(void);
u32 HX711_Read(void);
void Get_Maopi(void);
void Get_Weight(void);

void printf_press_strain_float(float num_f);
void printf_press_strain_u8(u8 data);


#endif

