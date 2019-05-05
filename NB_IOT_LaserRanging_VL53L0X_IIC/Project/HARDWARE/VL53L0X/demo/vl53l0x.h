#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_gen.h"
#include "vl53l0x_cali.h"
#include "vl53l0x_it.h"
#include "sys.h"
#include "delay.h"


//VL53L0X�������ϵ�Ĭ��IIC��ַΪ0X52(���������λ)
#define VL53L0X_Addr 0x52
//����Xshut��ƽ,�Ӷ�ʹ��VL53L0X���� 1:ʹ�� 0:�ر�
#define VL53L0X_Xshut PAout(6)	
//ʹ��2.8V IO��ƽģʽ
#define USE_I2C_2V8  1
//���ü������½�ֵΪ35mm(ʵ��Լ30mm)
#define RANGING_LIMIT_LOW 35
//����������ж�
typedef enum
{
	NO_DETECTION,
	HAVE_DETECTION
}ranging_result_type;
typedef struct
{
	ranging_result_type sta;
}g_ranging_type;
//����ģʽ
typedef enum
{
	DEFAULT_MODE,
	HIGH_ACCURACY,
	LONG_RANGE,
	HIGH_SPEED
}raning_mode_type;

//vl53l0xģʽ���ò�����
typedef __packed struct
{
	FixPoint1616_t signalLimit;    //Signal������ֵ 
	FixPoint1616_t sigmaLimit;     //Sigmal������ֵ
	uint32_t timingBudget;         //����ʱ������
	uint8_t preRangeVcselPeriod ;  //VCSEL��������
	uint8_t finalRangeVcselPeriod ;//VCSEL�������ڷ�Χ
}mode_data;

extern mode_data Mode_data[];
extern g_ranging_type g_ranging;

VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev);//��ʼ��vl53l0x
void mode_string(u8 mode,char *buf);//ģʽ�ַ�����ʾ
void vl53l0x_reset(VL53L0X_Dev_t *dev);//vl53l0x��λ
void print_pal_error(VL53L0X_Error Status);
VL53L0X_Error vl53l0x_Addr_set(VL53L0X_Dev_t *dev,uint8_t newaddr);

void vl53l0x_init_xsh_gpio(void);
void test_vl5310x_init(void);
void test_vl5310x_judge(void);
void test_vl5310x_handle(void);

#endif


