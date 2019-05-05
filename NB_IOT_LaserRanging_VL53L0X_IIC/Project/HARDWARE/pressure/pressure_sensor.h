#ifndef __PRESSURE_SENDOR_H
#define __PRESSURE_SENDOR_H
#include "delay.h"

#define PRESSURE0_LIMIT 0x0          //0g,电阻变成100k
#define PRESSURE20_LIMIT 0xA0       //20g,电阻变成30k
#define PRESSURE_CHANGE_LIMIT 0x0A
#define PRESSURE_CONFIRM_NUM 3
#define PRESSURE_SAMPLE_NUM 2

typedef enum
{
	NO_PRESSURE_SENSOR,
	HAVE_PRESSURE_SENEOR
}press_sta_type;

typedef enum
{
	NO_CHAGE,
	NO_TO_HAVE_FLAG,
	HAVE_TO_NO_FLAG
}press_change_type;

typedef enum
{
	NO_STA,
	GO_AGGRAVATE,
	GO_LIGHTEN,
	NO_DETAIL_CHANGE
}press_change_detail_type;

typedef enum
{
	STA_IDLE,
	STA_SAMPLE,
	STA_UPLOAD
}press_ad_current_sta_type;

typedef struct
{
	press_sta_type sta;
	press_sta_type last_sta;
	u16 press_ad_value[7];
	u16 press_ad_value_last[7];
	press_change_detail_type change_detail_flag[7];
	u8 have_press_count;
	u8 no_press_count;
	press_change_type change_flag;
	u8 sample_time_count;
	u8 sample_flag;
	
}press_ad_type;

void press_ad_debug_print(u16 data);
void press_ad_debug_print8(u8 data);
void press_sensor_adc_init(void);
u16 get_press_adc(u8 ch); 
u16 get_press_adc_average(u8 ch,u8 times); 
void press_ad_sample(void);
void press_ad_judge(void);
void press_handle(void);

void bat_sample(void);
float bat_get_value(void);
void bat_value_print(void);


#endif


