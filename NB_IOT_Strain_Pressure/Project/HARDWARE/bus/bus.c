#include "head_include.h"

u8 sendata_press[SEND_DATA_PRESS_LEN]={0};
u8 sendata_gesture[SEND_DATA_GESTURE_LEN]={0};
u8 sendata_press_stra[SEND_DATA_PRESS_LEN]={0};
u8 sendata[SEND_DATA_LEN]={'0'};
u8 send_buf[SEND_BUF_LEN]={'0'};
u8 upload_buf_sequence = 1;
u8 volatile write_location = 0;
u8 volatile read_location = 0;
u8 read_data_success_flag = 0;
u8 write_data_success_flag = 0;
u8 upload_main_loop_count = 0;
extern press_ad_type press_ad;
extern gesture_struct_type g_gesture;
extern char imei_str[16];

bus_type g_bus;


void upload_init(void)
{	
	memset(sendata,'0',SEND_DATA_LEN-1);
	memset(send_buf,'0',(SEND_BUF_LEN*sizeof(u8))-1);
}

void upload_change_sequence(void)
{
	upload_buf_sequence++;
	if(upload_buf_sequence>200)
		upload_buf_sequence = 1;
}
void hex_to_char(u8  data_hex,u8  data_ch[])
{

	if(data_hex/16>=10)
		data_ch[0]=data_hex/16+0x37;//转成A-F的字符
	else
		data_ch[0]=data_hex/16+0x30; 
	
	if(data_hex%16>=10)
		data_ch[1]=data_hex%16+0x37;//转成A-F的字符
	else
		data_ch[1]=data_hex%16+0x30;
}
void hex_to_str(u8 *inchar,u8 *outtxt,u32 len)
{
	u8 hbit,lbit;
	u32 i;
	for(i=0;i<len;i++)
	{
		hbit = (*(inchar+i)&0xf0)>>4;
		lbit = *(inchar+i)&0x0f;
		if (hbit>9) 
			outtxt[2*i]='A'+hbit-10;
		else 
			outtxt[2*i]='0'+hbit;

		if (lbit>9) 
			outtxt[2*i+1]='A'+lbit-10;
		else    
			outtxt[2*i+1]='0'+lbit;
	}
	outtxt[2*i] = 0;
}

//heart tick upload
// 0708
void upload_buf_heart_tick_frame(void)
{
	u8 loc = 0;
	u8 t;
	u8 pp[2];
	u8 imei_hex_str[31];
	memset(sendata_press_stra,0,sizeof(sendata_press_stra));
	
//	if(g_weight.sta)
	{
		//BUS1:MESSAGE_ID
		strncat(sendata_press_stra,BUS1_MESSAGE_ID_GESTURE,2);
		loc += 2;
		
		//BUS2:HEAD
		strncat(sendata_press_stra,BUS2_HEAD,2);
		loc += 2;
		
		//BUS3:IMEI(15hex,30hexstr)
		hex_to_str(imei_str,imei_hex_str,15);
		strncat(sendata_press_stra,imei_hex_str,30);
		loc += 30;
		
		//BUS4:COMMAND_TYPE
		strncat(sendata_press_stra,BUS4_COMMAND_TYPE_HEART,2);
		loc += 2;
		
		//BUS5:SEQUENCE
		t=upload_buf_sequence;
		hex_to_char(t,pp);
		strncat(sendata_press_stra,pp,2);
		loc += 2;
		
		//BUS6:DATA
		strncat(sendata_press_stra,BUS6_HEART_TICK_EVENT,2);
		loc += 2;
		
		sendata_press_stra[loc] = 0;
		upload_change_sequence();

#ifdef DEBUG_MACRO
		 UART1_send_byte('\n');
		 Uart1_SendStr(sendata_press_stra);
		 UART1_send_byte('\n');
#endif
	}
}




//press strain upload
//0312
void upload_buf_press_stra_frame(void)
{
	u8 loc = 0;
	u8 t;
	u8 pp[2];
	u8 imei_hex_str[31];
	memset(sendata_press_stra,0,sizeof(sendata_press_stra));
	
//	if(g_weight.sta)
	{
		//BUS1:MESSAGE_ID
		strncat(sendata_press_stra,BUS1_MESSAGE_ID_GESTURE,2);
		loc += 2;
		
		//BUS2:HEAD
		strncat(sendata_press_stra,BUS2_HEAD,2);
		loc += 2;
		
		//BUS3:IMEI(15hex,30hexstr)
		hex_to_str(imei_str,imei_hex_str,15);
		strncat(sendata_press_stra,imei_hex_str,30);
		loc += 30;
		
		//BUS4:COMMAND_TYPE
		strncat(sendata_press_stra,BUS4_COMMAND_TYPE_PRESS,2);
		loc += 2;
		
		//BUS5:SEQUENCE
		t=upload_buf_sequence;
		hex_to_char(t,pp);
		strncat(sendata_press_stra,pp,2);
		loc += 2;
		
		//BUS6:DATA
		switch(g_weight.sta)
		{
			case GO_S_AGGRAVATE:
				strncat(sendata_press_stra,BUS6_PRESS_EVENT_AGGRAVATE,2);
				break;
			case GO_S_LIGHTEN:
				strncat(sendata_press_stra,BUS6_PRESS_EVENT_LIGHTEN,2);
				break;
			case GO_S_LITTLE:
				strncat(sendata_press_stra,BUS6_PRESS_EVENT_LITTLE,2);
				break;
			default:
				strncat(sendata_press_stra,BUS6_PRESS_EVENT_NO_VALUE,2);
				break;
		}
		g_weight.sta = 0;
		
		loc += 2;
		
		sendata_press_stra[loc] = 0;
		upload_change_sequence();

#ifdef DEBUG_MACRO
		 UART1_send_byte('\n');
		 Uart1_SendStr(sendata_press_stra);
		 UART1_send_byte('\n');
#endif
	}
}


void upload_press_stra_handle(void)
{
//	if(g_weight.sta)
	{
		BC95_SendCOAPdata("20",sendata_press_stra);
		delay_ms(1000);
		BC95_RECCOAPData();
//		g_weight.sta = 0;
	}
}

// 处理所有上报事件
void upload_handle(void)
{
	if(g_bus.report_flag & HEART_FLAG)
	{
		g_bus.report_flag &= ~HEART_FLAG;
		upload_buf_heart_tick_frame();
		upload_press_stra_handle();
		//处理
	}

	if(g_bus.report_flag & KEY_FLAG)
	{
		g_bus.report_flag &= ~KEY_FLAG;
		//处理
	}

	if(g_bus.report_flag & PRESS_FLAG)
	{
		g_bus.report_flag &= ~PRESS_FLAG;
		//处理
		upload_buf_press_stra_frame();
		upload_press_stra_handle();
	}

	if(g_bus.report_flag & GESTURE_FLAG)
	{	
		g_bus.report_flag &= ~GESTURE_FLAG;
		//处理
	}

	
}


