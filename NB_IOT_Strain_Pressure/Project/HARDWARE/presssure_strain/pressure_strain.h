#ifndef __PRESS_STRAIN_H
#define __PRESS_STRAIN_H

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//对应的系数和常量值
#define GapValue 2322
//#define MAOPI_AD 8618982
//#define MAOPI_WEIGHT (3711.8)
#define MAOPI_AD_EVERY_100G 232200
#define MAOPI_AD (9852392-MAOPI_AD_EVERY_100G)
#define MAOPI_WEIGHT (1.0*MAOPI_AD/GapValue)

#define PRESS_STRAIN_LITTLE_LIMIT 100    // 10g
//#define PRESS_STRAIN_CHANGE_LIMIT 20    // 2g
#define PRESS_STRAIN_CHANGE_LIMIT 15    // 1.5g
#define PRESS_STRAIN_STABLE_LIMIT 3	  // 0.3g

#define PRESS_STRA_SAMPLE_TIME_COUNT 1
#define PRESS_STRA_LIEELE_COUNT 5

typedef enum
{
	NO_S_STA,
	GO_S_AGGRAVATE,
	GO_S_LIGHTEN,
	GO_S_LITTLE
}press_stra_ch_type;


typedef struct
{
	press_stra_ch_type sta;
	u8 sample_flag;
	u8 sample_count;
	u8 little_count;
	u32 maopi_ad;
	u32 maopi_weight;
	u32 shiwu_ad;
	u32 shiwu_weight[20];
	u32 shiwu_weight_ave;
	u32 shiwu_weight_ave_last[2];
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

void printf_press_strain_weight(u32 num_f);
void printf_press_strain_u8(u8 data);
void printf_press_strain_ad(u32 num_d);



#endif

