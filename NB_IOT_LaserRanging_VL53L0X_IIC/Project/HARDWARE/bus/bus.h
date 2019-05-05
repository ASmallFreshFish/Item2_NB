#ifndef __BUS_H
#define __BUS_H

#define DEBUG_MACRO
#define SEND_DATA_LEN 11
#define SEND_BUF_LEN  (6*SEND_DATA_LEN)
#define SEND_DATA_PRESS_LEN 17
#define SEND_DATA_GESTURE_LEN 50

//BUS1:MESSAGE_ID
#define BUS1_MESSAGE_ID_KEY "00"
#define BUS1_MESSAGE_ID_PRESS "02"
#define BUS1_MESSAGE_ID_GESTURE "04"
//BUS2:HEAD
#define BUS2_HEAD "DD"
//BUS3:IMEI
//BUS4:COMMAND_TYPE
#define BUS4_COMMAND_TYPE_KEY "FF"
#define BUS4_COMMAND_TYPE_PRESS "FE"
#define BUS4_COMMAND_TYPE_GESTURE "FD"
//BUS5:SEQUENCE
//BUS6:DATA

//zhou
//typedef struct
//{
//	u8 message_id;
//	u8 head_data;
//	u8 command;
//	u8 sequence;
//	u8 value;
//	u8 end_data;
//}bus_type;

//bus_type g_bus;

void upload_init(void);
void upload_data(void);
void upload_handle(void);
void upload_buf_write(void);
void upload_buf_read(void);
void upload_buf_check_read(void);
void upload_buf_check_write(void);
void upload_change_sequence(void);
void hex_to_char(u8 data_hex,u8 data_ch[2]);
void hex_to_str(u8 *inchar,u8 *outtxt,u32 len);

void upload_buf_press_frame(void);
void upload_press_handle(void);
void upload_buf_gesture_frame(void);
void upload_gesture_handle(void);
void upload_buf_ranging_frame(void);
void upload_ranging_handle(void);

#endif

