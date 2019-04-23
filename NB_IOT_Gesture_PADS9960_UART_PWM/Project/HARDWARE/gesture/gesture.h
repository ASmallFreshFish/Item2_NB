#ifndef __GESTURE_H
#define __GESTURE_H

#define AT_COMMAND "AT\r\n"
#define OK_STRING 	"OK"

//手势传感器的顺序也是上传的数据
typedef enum
{
	NO_GESTURE = 0,
	UP_GESTURE,		
	DOWN_GESTURE,
	LEFT_GESTURE,	
	RIGHT_GESTURE,
	FORWARD_GESTURE,	
	BACKWARD_GESTURE,
	CLKWISE_GESTURE,   
	ANTICLKWISE_GESTURE,
	WAVE_GESTURE
}gesture_type;

typedef struct
{
	gesture_type sta;
}gesture_struct_type;

void gesture_handle(void);
void gesture_pads9960_pwm_handle(void);
u8 check_buf_contain_str(char *buf,const char *str,u8 len);
void clear_buf(char *buf,u8 len);
u8 count_buf(char *buf);
void gesture_pwm_init(void); 


#endif


