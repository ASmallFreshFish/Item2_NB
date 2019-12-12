#ifndef __PRESSURE_SENDOR_H
#define __PRESSURE_SENDOR_H
#include "delay.h"

#define PRESSURE0_LIMIT 0x0          //0g,电阻变成100k
#define PRESSURE20_LIMIT 0xA0       //20g,电阻变成30k
#define PRESSURE_CHANGE_LIMIT 0x0A
#define PRESSURE_CONFIRM_NUM 3
#define PRESSURE_SAMPLE_NUM 2
#define PRESS_SAMPLE_TIME_COUNT 1


#define PRESS_CLOSE_THRESHOLD 100

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

//old
//typedef struct
//{
//	press_sta_type sta;
//	press_sta_type last_sta;
//	u16 press_ad_value[7];
//	u16 press_ad_value_last[7];
//	press_change_detail_type change_detail_flag[7];
//	u8 have_press_count;
//	u8 no_press_count;
//	press_change_type change_flag;
//	u8 sample_time_count;
//	u8 sample_flag;
//	
//}press_ad_type;
typedef enum
{
	STA_BOX_CLOSED=0,
	STA_BOX_OPENED_ONE,
	STA_BOX_OPENED_TWO
}press_result_type;

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
	u8 sample_count;
	u8 sample_flag;
	press_result_type press_result;
}press_ad_type;

extern press_ad_type g_press;

void press_ad_debug_print(u16 data);
void press_ad_debug_print8(u8 data);

void adc_init(void);
void adc_enable(void);

void press_sensor_init();
u16 get_adc(u8 ch); 
u16 get_press_adc_average(u8 ch,u8 times); 
void press_ad_sample(void);
void press_ad_judge(void);
void press_sensor_handle(void);


/*********************************************************
* 电池电量相关处理
**********************************************************/
//#define BAT_SAMPLE_INTERVAL_3MIN 360
//#define BAT_SAMPLE_INTERVAL_3MIN 60
#define BAT_SAMPLE_INTERVAL_HALFMIN 60


#define BAT_LOW_POWER_LIMIT 370		//电量低于3.70V，低电量
#define BAT_OFF_POWER_LIMIT 350		//电量低于3.50V，关机
#define BAT_HIGH_POWER_LIMIT 440	//电量高于4.40V，异常
#define BAT_ZERO_NUMBER_LIMIT 4		//连续4次检测到0V，才上报0V


typedef enum
{
	BAT_STA_NORMAL_POWER,
	BAT_STA_LOW_POWER,
	BAT_STA_OFF_POWER
}bat_sta_type;

typedef struct
{
	u16 bat_ad_value;
	u16 bat_value;		//方便处理，存储实际电压的100倍
	u16 last_bat_value;
	u8 normal_power_count;
	u8 low_power_count;
	u8 off_power_count;
	u8 sample_count;
	u8 sample_flag; 
	bat_sta_type sta;
	u8 bat_zero_count;
}bat_type;

extern bat_type g_bat;

void bat_init(void);
void bat_sample(void);
void bat_get_value(void);
void bat_judge(void);
void bat_hangdle(void);


/*********************************************************
* 系统时间测量
**********************************************************/

typedef  struct
{
    u16 year;     // 1970+
    u8  month;    // 1-12
    u8  day;      // 1-31
    u8  hour;     // 0-23
    u8  minutes;  // 0-59
    u8  seconds;  // 0-59
    u8  week;
}my_time_type;

typedef struct
{
	u32 m_clock_utc;
	u8 m_clock_count ;
	u8 m_clock_syn_flag ;
	u8 m_clock_syn_result ;
}my_time_val_type;

extern my_time_val_type my_g_time;

void clock_init_time(void);
void clock_get_time(my_time_type* time);
void clock_set_time(my_time_type *time);
void clock_cclk_handle(char *p);
u8 clock_syn_time(void);

/*********************************************************

* adc+dma配置

**********************************************************/
extern volatile u16 adc_dma_ad_value[4];

void dma_init_config(void);
void adc_init_config(void);
void adc_dma_get_start(void);
void adc_dma_get_result(void);

#endif


