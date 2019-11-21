#ifndef __BUS_H
#define __BUS_H
#include "data_setting.h"

//LOG宏开关
#ifdef DENUG_MACRO_ALL_OPEN
#define DEBUG_MACRO
#define DEBUG_MACRO_INIT
#else
#define DEBUG_MACRO_INIT
#endif

#define SEND_DATA_LEN 11
#define SEND_BUF_LEN  (6*SEND_DATA_LEN)

#define UPLOAD_SEND_DATA_LEN 100

/************************************************************************
*****************************帧格式**************************************
2019117前面
01			|02	    |03		|04				|05			|06		
MESSAGE_ID	|head	|IMEI	|COMMAND_TYPE	|SEQUENCE	|EVENT		 
1bytes		|1bytes	|15bytes|1bytes			|1bytes		1bytes

2019117
01			|02	    |03		|04				|05			|06		|07		
MESSAGE_ID	|head	|IMEI	|COMMAND_TYPE	|SEQUENCE	|EVENT	|changed_data 
1byte		|1byte	|15bytes|1byte			|1byte		|1byte	|2byte

*************************************************************************/
//BUS1:MESSAGE_ID
#define BUS1_MESSAGE_ID_KEY 		"00"
#define BUS1_MESSAGE_ID_PRESS 		"02"
#define BUS1_MESSAGE_ID_GESTURE 	"04"
//BUS2:HEAD
#define BUS2_HEAD 		"DD"

//BUS3:IMEI
//BUS4:COMMAND_TYPE
#define BUS4_COMMAND_TYPE_HEART 			"01"
#define BUS4_COMMAND_TYPE_KEY 			"11"
#define BUS4_COMMAND_TYPE_PRESS 		"12"
#define BUS4_COMMAND_TYPE_GESTURE		"13"

//BUS5:SEQUENCE
//BUS6:PRESS EVENT
#define BUS6_PRESS_EVENT_AGGRAVATE 	"FF"
#define BUS6_PRESS_EVENT_LIGHTEN 		"FE"
#define BUS6_PRESS_EVENT_LITTLE 		"FD"
#define BUS6_PRESS_EVENT_NO_VALUE 	"00"
//BUS6:HEART EVENT
#define BUS6_HEART_TICK_EVENT			"01"
#define BUS6_HEART_TICK_EVENT_DATA	0x01

//BUS7:CHANGED DATA


//heart tick(定时器0.5s一次)
#define HEART_UPLOAD_INTERVAL_5MIN	600
//#define HEART_UPLOAD_INTERVAL_5MIN	120


typedef enum
{
	NO_REPORT=0,
	HEART_FLAG = 0x01,
	KEY_FLAG =  0x02,
	PRESS_FLAG = 0x04,
	GESTURE_FLAG = 0x08,
	BAT_LOW_POWER_FLAG = 0x10,
	BAT_OFF_POWER_FLAG = 0x20,
	reserved_FLAG3 = 0x40,
	reserved_FLAG4 = 0x80
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
}bus_type;

extern bus_type g_bus;

void upload_init(void);
void upload_handle(void);
void upload_change_sequence(void);
void upload_send_data_frame(u8* command_type,u8 event,u16 data);
void upload_send_data_handle(void);

void hex8_to_char(u8 data_hex,u8 data_ch[2]);
void hex8_to_str(u8 *inchar,u8 *outtxt,u32 len);
void hex16_to_str(u16 data_hex,u8 *outtxt);

void printf_char(char ch);
void printf_string(char ch[]);
void printf_u8(u8 data);
void printf_u16(u16 data);
void printf_u32(u32 data);

void printf_press_strain_weight(u32 num_f);


#endif

