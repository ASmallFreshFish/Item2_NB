#include "head_include.h"
#include "string.h"

extern volatile char RxBuffer2[USART2_BUF_LEN];     //接收缓冲,最大USART2_BUF_LEN个字节.
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

static const char *GESTURE_STA[6]={"UP","DOWN","LEFT","RIGHT","FAR","NEAR"};
static const u8 GESTURE_STA_LEN[6]={2,4,4,5,3,4};
//static const char *GESTURE_INIT="Gesture sensor initialization success  Gesture sensor is ready";

gesture_struct_type g_gesture;

// 判断字符串数据UP\DOWN\LEFT\RIGHT\FAR\NEAR
void gesture_handle(void)
{
	u8 num,len;
	g_gesture.sta = NO_GESTURE;
		
	if (check_buf_valid_data(RxBuffer2,USART2_BUF_LEN,usart2_read_loc))
    {		
#ifdef DEBUG_MACRO
UART1_send_byte('\n');
UART1_send_byte('Q');
Uart1_SendStr_Len(RxBuffer2,USART2_BUF_LEN);
#endif
		if(check_buf_contain_str(RxBuffer2,GESTURE_STA[0],GESTURE_STA_LEN[0]))  
		{
#ifdef DEBUG_MACRO
UART1_send_byte('u');
#endif
			g_gesture.sta = UP_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[0]);
			usart2_read_loc+=GESTURE_STA_LEN[0];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[1],GESTURE_STA_LEN[1]))
		{
#ifdef DEBUG_MACRO
UART1_send_byte('d');
#endif
			g_gesture.sta = DOWN_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[1]);
			usart2_read_loc+=GESTURE_STA_LEN[1];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[2],GESTURE_STA_LEN[2]))
		{
#ifdef DEBUG_MACRO
UART1_send_byte('l');
#endif
			g_gesture.sta = LEFT_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[2]);
			usart2_read_loc+=GESTURE_STA_LEN[2];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[3],GESTURE_STA_LEN[3]))
		{
#ifdef DEBUG_MACRO
UART1_send_byte('r');
#endif
			g_gesture.sta = RIGHT_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[3]);
			usart2_read_loc+=GESTURE_STA_LEN[3];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[4],GESTURE_STA_LEN[4]))
		{	
#ifdef DEBUG_MACRO
UART1_send_byte('f');
#endif
			g_gesture.sta = FAR_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[4]);
			usart2_read_loc+=GESTURE_STA_LEN[4];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[5],GESTURE_STA_LEN[5]))
		{
#ifdef DEBUG_MACRO
UART1_send_byte('n');
#endif
			g_gesture.sta = NEAR_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[5]);
			usart2_read_loc+=GESTURE_STA_LEN[5];
		}
		else
		{
#ifdef DEBUG_MACRO
UART1_send_byte('p');
#endif
			g_gesture.sta = NO_GESTURE;
			clear_buf(RxBuffer2,1);
			usart2_read_loc+=1;
		}

		if(usart2_read_loc > USART2_BUF_LEN-1)
		{
			usart2_read_loc = 0;
		}
#ifdef DEBUG_MACRO
UART1_send_byte('\t');
press_ad_debug_print8(usart2_read_loc);
UART1_send_byte('\t');
press_ad_debug_print8(usart2_write_loc);
#endif
	}
}


u8 check_buf_contain_str(char *buf,const char *str,u8 len)
{
	char buf_tmp[len+1];
	u8 i;

	for( i=0;i<len;i++)
	{
		if((usart2_read_loc+i)<(USART2_BUF_LEN))
			buf_tmp[i]=buf[usart2_read_loc+i];
		else
			buf_tmp[i]=buf[usart2_read_loc+i-USART2_BUF_LEN];
	}
	buf_tmp[len] = NULL;

	if(!strncmp(buf_tmp, str, len))
		return 1;
	return 0;
}

void clear_buf(char *buf,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		buf[usart2_read_loc+i]=0;
	}
}

u8 count_buf(char *buf)
{
	u8 count;
	while(*buf != 0)
	{
		count++;
		buf++;
	}
	
	return count;
}


