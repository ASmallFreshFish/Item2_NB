#include "head_include.h"
#include "string.h"

extern volatile char RxBuffer2[USART2_BUF_LEN];     //接收缓冲,最大USART2_BUF_LEN个字节.
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

static const char *GESTURE_STA[6]={"UP","DOWN","LEFT","RIGHT","FAR","NEAR"};
static const char *GESTURE_INIT="Gesture";

gesture_struct_type g_gesture;

// 判断字符串数据UP\DOWN\LEFT\RIGHT\FAR\NEAR
void gesture_handle(void)
{
	u8 num;
	g_gesture.sta = NO_GESTURE;
	if (check_buf_valid_data(RxBuffer2,USART2_BUF_LEN,usart2_read_loc))
    {
	    	UART1_send_byte('\n');
			UART1_send_byte('Q');
			Uart1_SendStr_Len(RxBuffer2,USART2_BUF_LEN);
//			UART1_send_byte('\n');
//			UART1_send_byte('Q');
			
//			num= count_buf(RxBuffer2);
//			UART1_send_byte(num);
//			UART1_send_byte('\t');
//			press_ad_debug_print8(num);
//			UART1_send_byte('\t');
			
		if(check_buf_contain_str(RxBuffer2,GESTURE_STA[0],2))  
		{
			UART1_send_byte('u');
			
//			press_ad_debug_print8(RxBuffer2[usart2_read_loc]);
//			UART1_send_byte('\t');
//			press_ad_debug_print8(RxBuffer2[usart2_read_loc+1]);
//			UART1_send_byte('\t');
//			press_ad_debug_print8(RxBuffer2[usart2_read_loc+2]);
//			UART1_send_byte('\t');
//			press_ad_debug_print8(RxBuffer2[usart2_read_loc+3]);
//			UART1_send_byte('\t');
			
			g_gesture.sta = UP_GESTURE;
			clear_buf(RxBuffer2,4);
			usart2_read_loc+=4;
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[1],4))
		{
			UART1_send_byte('d');
			g_gesture.sta = DOWN_GESTURE;
			clear_buf(RxBuffer2,6);
			usart2_read_loc+=6;
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[2],4))
		{
			UART1_send_byte('l');
			g_gesture.sta = LEFT_GESTURE;
			clear_buf(RxBuffer2,6);
			usart2_read_loc+=6;
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[3],5))
		{
			UART1_send_byte('r');
			g_gesture.sta = RIGHT_GESTURE;
			clear_buf(RxBuffer2,7);
			usart2_read_loc+=7;
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[4],3))
		{	
			UART1_send_byte('f');
			g_gesture.sta = FAR_GESTURE;
			clear_buf(RxBuffer2,5);
			usart2_read_loc+=5;
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[5],4))
		{
			UART1_send_byte('n');
			g_gesture.sta = NEAR_GESTURE;
			clear_buf(RxBuffer2,6);
			usart2_read_loc+=6;
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_INIT,7))
		{
			UART1_send_byte('i');
			g_gesture.sta = NEAR_GESTURE;
			clear_buf(RxBuffer2,62);
			usart2_read_loc+=62;
		}
		else
		{
			UART1_send_byte('p');
			g_gesture.sta = NO_GESTURE;
			clear_buf(RxBuffer2,1);
			usart2_read_loc+=1;
		}

			UART1_send_byte('\t');
			press_ad_debug_print8(usart2_read_loc);
    }
}


u8 check_buf_contain_str(char *buf,const char *str,u8 len)
{
	char buf_tmp[len+1];
	u8 i;
	strncpy(buf_tmp,buf+usart2_read_loc,len);
	buf_tmp[len] = NULL;
//	for( i=0;i<len;i++)
//	{
//		buf_tmp[i]=buf[usart2_read_loc+i];
//	}
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


