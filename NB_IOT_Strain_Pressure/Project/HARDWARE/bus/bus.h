#ifndef __BUS_H
#define __BUS_H
#include "data_setting.h"

//LOG宏开关
#ifdef DENUG_MACRO_ALL_OPEN
#define DEBUG_MACRO
#define DEBUG_MACRO_INIT
#define DEBUG_TEST_MACRO
#else
#define DEBUG_MACRO_INIT
#endif

#define SEND_DATA_LEN 11
#define SEND_BUF_LEN  (6*SEND_DATA_LEN)

#define UPLOAD_SEND_DATA_LEN 200

/************************************************************************
**********************************帧格式*********************************

******************************1 数据上传格式(发送)*************************
2019117前
01			|02	    |03		|04				|05			|06		
MESSAGE_ID	|head	|IMEI	|COMMAND_TYPE	|SEQUENCE	|EVENT		 
1bytes		|1bytes	|15bytes|1bytes			|1bytes		1bytes

2019117
01			|02	    |03		|04				|05			|06		|07		
MESSAGE_ID	|head	|IMEI	|COMMAND_TYPE	|SEQUENCE	|EVENT	|changed_data 
1byte		|1byte	|15bytes|1byte			|1byte		|1byte	|2byte

20191202
01			|02	    |03		|04				|05			|06		|07		|08		|09		|10		|11
MESSAGE_ID	|head	|IMEI	|COMMAND_TYPE	|SEQUENCE	|EVENT	|data1  |data2	|data3	|time	|version
1byte		|1byte	|15bytes|1byte			|1byte		|1byte	|2byte	|2bytes	|2bytes |12bytes|6bytes


******************************2 命令下发格式(接受)*************************
20191205
01			|02	    |03				|04			|06		
MESSAGE_ID	|head	|COMMAND_TYPE	|SEQUENCE	|data		 
1bytes		|1bytes	|1bytes			|1bytes		|2bytes

*************************************************************************/

/*****************************数据上传常量*******************************/

//BUS1:MESSAGE_ID
#define BUS1_MESSAGE_ID_KEY 		"00"
#define BUS1_MESSAGE_ID_PRESS 		"02"
#define BUS1_MESSAGE_ID_GESTURE 	"04"
//BUS2:HEAD
#define BUS2_HEAD 		"DD"

//BUS3:IMEI,different from each other
//BUS4:COMMAND_TYPE
#define BUS4_COMMAND_TYPE_HEART 		"01"
#define BUS4_COMMAND_TYPE_BAT 			"02"
#define BUS4_COMMAND_TYPE_KEY 			"11"
#define BUS4_COMMAND_TYPE_STRAIN 		"12"
#define BUS4_COMMAND_TYPE_GESTURE		"13"
#define BUS4_COMMAND_TYPE_PRESS_SENSOR 	"14"

//BUS5:SEQUENCE,changed with upload times
//BUS6:HEART EVENT
#define BUS6_BAT_POWER  "00"
#define BUS6_BAT_POWER_DATA  0x0
#define BUS6_BAT_LOW_POWER  "01"
#define BUS6_BAT_LOW_POWER_DATA  0x01
#define BUS6_BAT_OFF_POWER  "02"
#define BUS6_BAT_OFF_POWER_DATA  0x02

//BUS6:PRESS EVENT
#define BUS6_PRESS_EVENT_AGGRAVATE 	"FF"
#define BUS6_PRESS_EVENT_LIGHTEN 		"FE"
#define BUS6_PRESS_EVENT_LITTLE 		"FD"
#define BUS6_PRESS_EVENT_NO_VALUE 	"00"
//BUS6:HEART EVENT
#define BUS6_HEART_TICK_EVENT			"01"
#define BUS6_HEART_TICK_EVENT_DATA	0x01
//BUS6:PRESS_SENSOR EVENT
#define BUS6_PRESS_SENSOR_OPEN  "01"
#define BUS6_PRESS_SENSOR_OPEN_DATA  0x01
//BUS7:DATA1,	real time value
//BUS8:DATA2,	real time value
//BUS9:DATA3,	real time value
//BUS10:TIME,	real time value
//BUS11:VERSION

//heart tick(定时器0.5s一次)
#define HEART_UPLOAD_INTERVAL_5MIN	600
//#define HEART_UPLOAD_INTERVAL_5MIN	120


#define BUS_REPORT_NUMBER 3

typedef enum
{
	BUS_FRAME_STA=0,
	BUS_UPLOAD_STA=1,
	BUS_FINAL_UPLOAD_STA = BUS_REPORT_NUMBER,
}report_count_type;

typedef enum
{
	NO_REPORT=0,
	HEART_FLAG = 0x01,
	KEY_FLAG =  0x02,
	STRAIN_FLAG = 0x04,
	GESTURE_FLAG = 0x08,
	PRESS_SENSOR_FLAG= 0x10,
	BAT_POWER_FLAG = 0x20,
	BAT_LOW_POWER_FLAG = 0x40,
	BAT_OFF_POWER_FLAG = 0x80
}report_flag_type;

//zhou
typedef struct
{
//	u8 message_id;
//	u8 head_data;
//	u8 command;
//	u8 sequence;
//	u8 value;
//	u8 end_data;
	u16 heart_count;
	u8 report_flag;
	u8 have_reported_flag;
	report_count_type report_count;
	
}bus_type;

extern bus_type g_bus;


void upload_init(void);
void upload_handle(void);
void upload_strain_handle(void);
void upload_bat_low_handle(void);
void upload_bat_power_handle(void);
void upload_heart_handle(void);


void upload_change_sequence(void);
void upload_send_data_frame(u8* command_type,u8 event,u16 data1,u16 data2,u16 data3);
void old_upload_send_data_frame(u8* command_type,u8 event,u16 data);
void upload_send_data_handle(void);

void hex8_to_hexchar(u8 data_hex,u8 data_ch[2]);
void hex8_to_hexstr(u8 *inchar,u8 *outtxt,u32 len);
void hex16_to_hexstr(u16 data_hex,u8 *outtxt);

void printf_char(char ch);
void printf_string(char ch[]);
void printf_u8_hexStr(u8 data);
void printf_u16_hexStr(u16 data);
void printf_u8_decStr(u8 data);
void printf_u16_decStr(u16 data);
void printf_u32_decStr(u32 data);

//特殊应用
void printf_press_strain_weight(u32 num_f);
void printf_bat_value(u16 num_f);

/*************************************************
test 
**************************************************/
#define TEST_BAT_TIME1 120
#define TEST_BAT_TIME2 240
#define TEST_BAT_TIME3 360
#define TEST_BAT_TIME4 480
#define TEST_BAT_TIME5 600
#define TEST_BAT_TIME6 720
#define TEST_BAT_TIME7 960
#define TEST_BAT_TIME8 1040


typedef enum 
{
	TEST_BAT_NORMAL,
		TEST_BAT_LOW,
		TEST_BAT_OFF,
}test_bat_sta_type;

typedef struct
{
	test_bat_sta_type sta;
	u16 count;
}test_type;

extern test_type g_test;
#endif

