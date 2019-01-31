#ifndef __BUS_H
#define __BUS_H

//#define DEBUG_MACRO
#define SEND_DATA_LEN 11
#define SEND_BUF_LEN  (6*SEND_DATA_LEN)


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

#endif

