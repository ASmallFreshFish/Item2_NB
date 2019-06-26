#ifndef __GESTURE_H
#define __GESTURE_H

typedef enum
{
	NO_GESTURE = 0,
	UP_GESTURE,		DOWN_GESTURE,
	LEFT_GESTURE,	RIGHT_GESTURE,
	FAR_GESTURE,	NEAR_GESTURE
}gesture_type;

typedef struct
{
	gesture_type sta;
}gesture_struct_type;

void gesture_init(void);
void gesture_handle(void);
u8 check_buf_contain_str(char *buf,const char *str,u8 len);
void clear_buf(char *buf,u8 len);
u8 count_buf(char *buf);

#endif


