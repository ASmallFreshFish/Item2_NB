#include "head_include.h"
#include "string.h"

extern char RxBuffer2[USART2_BUF_LEN];     //接收缓冲,最大USART2_BUF_LEN个字节.
extern volatile u8 usart2_read_loc;
extern volatile u8 usart2_write_loc;

static const char *GESTURE_STA[7]={"UP","DOWN","LEFT","RIGHT","NEAR","FAR","NONE"};
//static const char *GESTURE_STA_PAJ[9]={"UP","DOWN","LEFT","RIGHT","FORWARD","BACKWARD","CLKW","ACLKW","WAVE"};
static const u8 GESTURE_STA_LEN[7]={2,4,4,5,4,3,4};
//static const u8 GESTURE_STA_PAJ_LEN[9]={2,4,4,5,3,4,4,5,4};

//static const char *GESTURE_INIT="Gesture sensor initialization success  Gesture sensor is ready";
gesture_struct_type g_gesture;

void gesture_pwm_init(void)
{
	Uart2_SendStr(AT_COMMAND);
	delay_ms(300);
	while(strstr((const char*)RxBuffer2,(const char*)OK_STRING) == NULL)
	{
		delay_ms(500);
		Uart2_SendStr(AT_COMMAND);
	}

	Uart1_SendStr("9960 init OK!\r\n");
}

// 判断字符串数据UP\DOWN\LEFT\RIGHT\FAR\NEAR
void gesture_handle(void)
{
	u8 sta,ges_sta;
	u8 data[2]={0x00};
	g_gesture.sta = NO_GESTURE;

    sta = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态			
	if(!sta)
	{   
		ges_sta =(u16)data[1]<<8 | data[0];
		if(ges_sta) 
		{
			switch(ges_sta)
			{
				case GES_UP:               
					g_gesture.sta = UP_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("up\n");
#endif
					break; //向上
				case GES_DOWM:             
					g_gesture.sta = DOWN_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("down\n");
#endif
					break; //向上
				case GES_LEFT:            
					g_gesture.sta = LEFT_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("left\n");
#endif
					break; //向上
				case GES_RIGHT:            
					g_gesture.sta = RIGHT_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("right\n");
#endif
					break; //向上
				case GES_FORWARD:          
					g_gesture.sta = FORWARD_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("forward\n");
#endif
					break; //向上
				case GES_BACKWARD:        
					g_gesture.sta = BACKWARD_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("backward\n");
#endif
					break; //向上
				case GES_CLOCKWISE:        
					g_gesture.sta = CLKWISE_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("clockwise\n");
#endif
					break; //向上
				case GES_COUNT_CLOCKWISE:  
					g_gesture.sta = ANTICLKWISE_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("anticlockwise\n");
#endif
					break; //向上
				case GES_WAVE:             
					g_gesture.sta = WAVE_GESTURE;
#ifdef DEBUG_MACRO
	Uart1_SendStr("wave\n");
#endif
					break; //向上
				default:  
					break;
				
			}	
		}
		
	}
	delay_ms(50);
}


void gesture_pads9960_pwm_handle(void)
{
	u8 num,len;
	g_gesture.sta = NO_GESTURE;
		
	if(check_buf_valid_data(RxBuffer2,USART2_BUF_LEN,usart2_read_loc))
    {	
    
#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
	Uart1_SendStr_Len(RxBuffer2,USART2_BUF_LEN);
	UART1_send_byte('\n');
#endif
		if(check_buf_contain_str(RxBuffer2,GESTURE_STA[0],GESTURE_STA_LEN[0]))  
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("up");
#endif
			g_gesture.sta = UP_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[0]);
			usart2_read_loc+=GESTURE_STA_LEN[0];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[1],GESTURE_STA_LEN[1]))
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("down");
#endif
			g_gesture.sta = DOWN_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[1]);
			usart2_read_loc+=GESTURE_STA_LEN[1];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[2],GESTURE_STA_LEN[2]))
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("left");
#endif
			g_gesture.sta = LEFT_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[2]);
			usart2_read_loc+=GESTURE_STA_LEN[2];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[3],GESTURE_STA_LEN[3]))
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("right");
#endif
			g_gesture.sta = RIGHT_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[3]);
			usart2_read_loc+=GESTURE_STA_LEN[3];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[4],GESTURE_STA_LEN[4]))
		{	
#ifdef DEBUG_MACRO
	Uart1_SendStr("near");
#endif
			g_gesture.sta = FORWARD_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[4]);
			usart2_read_loc+=GESTURE_STA_LEN[4];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[5],GESTURE_STA_LEN[5]))
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("far");
#endif
			g_gesture.sta = BACKWARD_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[5]);
			usart2_read_loc+=GESTURE_STA_LEN[5];
		}
		else if(check_buf_contain_str(RxBuffer2,GESTURE_STA[6],GESTURE_STA_LEN[6]))
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("none");
#endif
			g_gesture.sta = NO_GESTURE;
			clear_buf(RxBuffer2,GESTURE_STA_LEN[6]);
			usart2_read_loc+=GESTURE_STA_LEN[6];
		}
		else
		{
#ifdef DEBUG_MACRO
	Uart1_SendStr("no_gesture");
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
	delay_ms(200);
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


