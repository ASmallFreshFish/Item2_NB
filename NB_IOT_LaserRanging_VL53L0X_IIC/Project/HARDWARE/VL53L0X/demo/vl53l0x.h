#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_gen.h"
#include "vl53l0x_cali.h"
#include "vl53l0x_it.h"
#include "sys.h"
#include "delay.h"


//VL53L0X传感器上电默认IIC地址为0X52(不包含最低位)
#define VL53L0X_Addr 0x52
//控制Xshut电平,从而使能VL53L0X工作 1:使能 0:关闭
#define VL53L0X_Xshut PAout(6)	
//使能2.8V IO电平模式
#define USE_I2C_2V8  1
//设置检测距离下界值为35mm(实际约30mm)
#define RANGING_LIMIT_LOW 35
//测量结果的判断
typedef enum
{
	NO_DETECTION,
	HAVE_DETECTION
}ranging_result_type;
typedef struct
{
	ranging_result_type sta;
}g_ranging_type;
//测量模式
typedef enum
{
	DEFAULT_MODE,
	HIGH_ACCURACY,
	LONG_RANGE,
	HIGH_SPEED
}raning_mode_type;

//vl53l0x模式配置参数集
typedef __packed struct
{
	FixPoint1616_t signalLimit;    //Signal极限数值 
	FixPoint1616_t sigmaLimit;     //Sigmal极限数值
	uint32_t timingBudget;         //采样时间周期
	uint8_t preRangeVcselPeriod ;  //VCSEL脉冲周期
	uint8_t finalRangeVcselPeriod ;//VCSEL脉冲周期范围
}mode_data;

extern mode_data Mode_data[];
extern g_ranging_type g_ranging;

VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev);//初始化vl53l0x
void mode_string(u8 mode,char *buf);//模式字符串显示
void vl53l0x_reset(VL53L0X_Dev_t *dev);//vl53l0x复位
void print_pal_error(VL53L0X_Error Status);
VL53L0X_Error vl53l0x_Addr_set(VL53L0X_Dev_t *dev,uint8_t newaddr);

void vl53l0x_init_xsh_gpio(void);
void test_vl5310x_init(void);
void test_vl5310x_judge(void);
void test_vl5310x_handle(void);

#endif


