#ifndef __PRESS_STRAIN_H
#define __PRESS_STRAIN_H
#include "data_setting.h"

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//对应的系数和常量值
#ifdef WEIGHT_SENSOR_1KG

//#define GapValue 2322
//#define GapValue 2458.3
//(70671,1kg)
#define GapValue 2114.1


#define MAOPI_AD_REAL 9395837
#define MAOPI_AD_EVERY_100G (100*GapValue)
#define MAOPI_AD_EVERY_400G (400*GapValue) //该重量相当于传感器清零
#define MAOPI_AD_EVERY_500G (500*GapValue) //该重量相当于传感器称重最小100g
#define MAOPI_AD_EVERY_1000G (1000*GapValue) //该重量相当于传感器称重最小600g
#define MAOPI_AD (MAOPI_AD_REAL-MAOPI_AD_EVERY_1000G)
#define MAOPI_WEIGHT (10.0*MAOPI_AD/GapValue)

#else
//#define GapValue 715 //计算原始
//#define GapValue 703
//#define GapValue 689
//#define GapValue 784
#define GapValue 798.6

#define MAOPI_AD_REAL 8463254
#define MAOPI_AD_EVERY_100G (100*GapValue)
#define MAOPI_AD_EVERY_400G (400*GapValue) //该重量相当于传感器清零
#define MAOPI_AD_EVERY_500G (500*GapValue) //该重量相当于传感器称重最小100g
#define MAOPI_AD_EVERY_1000G (1000*GapValue) //该重量相当于传感器称重最小600g
#define MAOPI_AD (MAOPI_AD_REAL-MAOPI_AD_EVERY_1000G)
#define MAOPI_WEIGHT (10.0*MAOPI_AD/GapValue)
#endif

#define PRESS_STRAIN_LITTLE_LIMIT 100    // 10g
//#define g_weight.change_threshold 14    // 1.4g
//#define g_weight.change_threshold 25    // 2.5g
#define PRESS_STRAIN_STABLE_LIMIT 4	  // 0.4g

#define PRESS_STRA_SAMPLE_TIME_COUNT 1
#define PRESS_STRA_LIEELE_COUNT 5

typedef enum
{
	NO_S_STA,
	GO_S_AGGRAVATE = 0xFF,
	GO_S_LIGHTEN = 0xFE,
	GO_S_LITTLE =0xFD

}press_stra_ch_type;


#define WEIGHT_SAMPLE_NUMBER 10
typedef struct
{
	press_stra_ch_type sta;
	u8 sample_flag;
	u8 sample_count;
	u8 little_count;
	u16 changed_data;
	u16 change_threshold;
	u32 maopi_ad;
	u32 maopi_weight;
	u32 shiwu_ad;
	u32 shiwu_weight[WEIGHT_SAMPLE_NUMBER];		//存储的是实际重量的10倍
	u32 shiwu_weight_ave;						//存储的是实际重量的10倍
	u32 shiwu_weight_ave_last[2];				//存储的是实际重量的10倍
    u32 factor100;	
}pressure_strain_type;

extern pressure_strain_type g_weight;


#define HX711_SCK_SET_1 	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET)
#define HX711_SCK_SET_0 	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET)
#define HX711_DOUT			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

void Init_HX711pin(void);
u32 HX711_Read(void);
void Get_Maopi(void);
void Get_Weight(void);

void press_strain_init(void);
void press_strain_init_remove(void);
void press_strain_judge(void);
u32 press_strain_sort_average(u32 ch[],u8 num);
void press_strain_handle(void);


#endif

