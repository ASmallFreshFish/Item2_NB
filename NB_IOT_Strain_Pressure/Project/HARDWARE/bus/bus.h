#ifndef __BUS_H
#define __BUS_H

//LOGºê¿ª¹Ø
//#define DENUG_MACRO_ALL_OPEN 

#ifdef DENUG_MACRO_ALL_OPEN
#define DEBUG_MACRO
#define DEBUG_MACRO_INIT
#else
#define DEBUG_MACRO_INIT
#endif

#define SEND_DATA_LEN 11
#define SEND_BUF_LEN  (6*SEND_DATA_LEN)
#define SEND_DATA_PRESS_LEN 17
#define SEND_DATA_PRESS_STRA_LEN 50
#define SEND_DATA_GESTURE_LEN 50

//BUS1:MESSAGE_ID
#define BUS1_MESSAGE_ID_KEY 		"00"
#define BUS1_MESSAGE_ID_PRESS 		"02"
#define BUS1_MESSAGE_ID_GESTURE 	"04"
//BUS2:HEAD
#define BUS2_HEAD 		"DD"

//BUS3:IMEI
//BUS4:COMMAND_TYPE
//#define BUS4_COMMAND_TYPE_KEY 			"FF"
//#define BUS4_COMMAND_TYPE_PRESS 		"FE"
//#define BUS4_COMMAND_TYPE_GESTURE		"FD"
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

//heart tick
//#define HEART_UPLOAD_INTERVAL_5MIN	60
#define HEART_UPLOAD_INTERVAL_5MIN	600


typedef enum
{
	HEART_FLAG = 0x01,
	KEY_FLAG =  0x02,
	PRESS_FLAG = 0x04,
	GESTURE_FLAG = 0x08,
	reserved_FLAG1 = 0x10,
	reserved_FLAG2 = 0x20,
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
}bus_type;

extern bus_type g_bus;

void upload_init(void);
void upload_handle(void);
void upload_change_sequence(void);
void upload_send_data_frame(u8* command_type,u8 data);
void upload_send_data_frame_heart_tick(void);
void upload_send_data_frame_press(void);
void upload_send_data_handle(void);

void hex_to_char(u8 data_hex,u8 data_ch[2]);
void hex_to_str(u8 *inchar,u8 *outtxt,u32 len);

#endif

