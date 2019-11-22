#include "head_include.h"

bus_type g_bus ={0};
u8 upload_send_data[UPLOAD_SEND_DATA_LEN]={0};
u8 upload_buf_sequence = 1;

extern press_ad_type g_press;
extern gesture_struct_type g_gesture;
extern char imei_str[16];

/**************************************************************
打印日志相关函数
**************************************************************/
void printf_char(char ch)
{
	UART1_send_byte(ch);
}

void printf_string(char ch[])
{
	Uart1_SendStr(ch);
}

void printf_u8_hexStr(u8 data)
{
 	u8 t = data;
	u8 pp[2];
	
	hex8_to_char(t,pp);
	if(pp[0]!= '0')
	{
		printf_char(pp[0]);
	}
	printf_char(pp[1]);
}

void printf_u16_hexStr(u16 data)
{
	u8 i;
	u16 t=data;
	u8 pp[4];
	
	hex16_to_str(t,pp);
	for(i=0;i<4;i++)
	{
		printf_char(pp[i]);
	}
}

//二级制8位以十进制表示最大有3位
void printf_u8_decStr(u8 data)
{
	u8 para_f = data;
	u8 i,bit[3];

	for(i=0;i<3;i++)
	{
		bit[i] = para_f%10;
		para_f = para_f/10;
	}
	for(i=0;i<3;i++)
	{
		printf_u8_hexStr(bit[2-i]);
	}
}

//二级制16位以十进制表示最大有5位
void printf_u16_decStr(u16 data)
{
	u16 para_f = data;
	u8 i,bit[5];

	for(i=0;i<5;i++)
	{
		bit[i] = para_f%10;
		para_f = para_f/10;
	}
	for(i=0;i<5;i++)
	{
		printf_u8_hexStr(bit[4-i]);
	}
}

//二级制32位以十进制表示最大有10位
void printf_u32_decStr(u32 data)
{
	u32 para_f = data;
	u8 i,bit[10];

	for(i=0;i<10;i++)
	{
		bit[i] = para_f%10;
		para_f = para_f/10;
	}
	for(i=0;i<10;i++)
	{
		printf_u8_hexStr(bit[9-i]);
	}
}

//特殊应用
void printf_press_strain_weight(u32 num_f)
{
	u32 para_f = num_f;
	u8 i,j,bit[10];

	for(i=0;i<10;i++)
	{
		bit[i] = para_f%10;
		para_f = para_f/10;
	}
	for(i=0;i<8;i++)
	{
		if(bit[9-i]!=0)
			break;
	}
	for(j=i;j<9;j++)
	{
		printf_u8_hexStr(bit[9-j]);
	}
	
	printf_char('.');
		printf_u8_hexStr(bit[0]);
	printf_char('g');

}

void printf_bat_value(u16 num_f)
{
	u8 num=5;
	u16 para_f = num_f;
	u8 i,j,bit[num];

	for(i=0;i<num;i++)
	{
		bit[i] = para_f%10;
		para_f = para_f/10;
	}
	for(i=0;i<num-3;i++)
	{
		if(bit[num-1-i]!=0)
			break;
	}
	for(j=i;j<num-2;j++)
	{
		printf_u8_hexStr(bit[num-1-j]);
	}
	
	printf_char('.');
		printf_u8_hexStr(bit[1]);
		printf_u8_hexStr(bit[0]);
	printf_char('v');

}


/**************************************************************
数字转字符相关函数
**************************************************************/

void hex8_to_char(u8      data_hex,u8  data_ch[])
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

void hex16_to_str(u16 data_hex,u8 *outtxt)
{
	u8 hbit,lbit,i;
	u8 t =(u8)(data_hex>>8);
	
	for(i=0;i<2;i++)
	{
		hbit = (t&0xf0)>>4;
		lbit = t&0x0f;
		
		if (hbit>9) 
			outtxt[2*i]='A'+hbit-10;
		else 
			outtxt[2*i]='0'+hbit;

		if (lbit>9) 
			outtxt[2*i+1]='A'+lbit-10;
		else    
			outtxt[2*i+1]='0'+lbit;

		t=(u8)(data_hex);
	}
}

void hex8_to_str(u8 *inchar,u8 *outtxt,u32 len)
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


/**************************************************************
通信上报相关函数
**************************************************************/
void upload_init(void)
{
	memset(&g_bus,0,sizeof(g_bus));
}


void upload_change_sequence(void)
{
	upload_buf_sequence++;
	if(upload_buf_sequence>200)
		upload_buf_sequence = 1;
}

void upload_send_data_frame(u8* command_type,u8 event,u16 data)
{
	u8 loc = 0;
	u8 t;
	u8 pp[2];
	u8 imei_hex_str[31];
	u8 change_data_str[5];
	memset(upload_send_data,0,sizeof(upload_send_data));
	
	//BUS1:MESSAGE_ID
	strncat(upload_send_data,BUS1_MESSAGE_ID_GESTURE,2);
	loc += 2;
	
	//BUS2:HEAD
	strncat(upload_send_data,BUS2_HEAD,2);
	loc += 2;
	
	//BUS3:IMEI  (15hex,30hexstr)
	hex8_to_str(imei_str,imei_hex_str,15);
	strncat(upload_send_data,imei_hex_str,30);
	loc += 30;
	
	//BUS4:COMMAND_TYPE
	strncat(upload_send_data,command_type,2);
	loc += 2;
	
	//BUS5:SEQUENCE
	t=upload_buf_sequence;
	hex8_to_char(t,pp);
	strncat(upload_send_data,pp,2);
	loc += 2;
	
	//BUS6:DATA
	t=event;
	hex8_to_char(t,pp);
	strncat(upload_send_data,pp,2);
	loc += 2;

	//BUS7:changed_data  (2hex,4hexstr)
	hex16_to_str(data,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;
	
	upload_send_data[loc] = 0;
	upload_change_sequence();

#ifdef DEBUG_MACRO
		 UART1_send_byte('\n');
		 Uart1_SendStr(upload_send_data);
		 UART1_send_byte('\n');
#endif
}



void upload_send_data_handle(void)
{		
	BC95_SendCOAPdata("22",upload_send_data);
	delay_ms(1000);
	BC95_RECCOAPData();
}

// 处理所有上报事件
void  upload_handle(void)
{
	if(g_bus.report_flag&BAT_OFF_POWER_FLAG)
	{
		//关机上报处理
		g_bus.have_reported_flag|=BAT_OFF_POWER_FLAG;
	}
	else if(g_bus.report_flag&BAT_LOW_POWER_FLAG)
	{
		//低电量上报处理
		g_bus.have_reported_flag|=g_bus.have_reported_flag;
	}

	if(g_bat.sta == BAT_STA_OFF_POWER)
	{
		return;
	}

	if(g_bus.report_flag & HEART_FLAG)
	{
		g_bus.report_flag &= ~HEART_FLAG;
		//处理
		upload_send_data_frame(BUS4_COMMAND_TYPE_HEART,BUS6_HEART_TICK_EVENT_DATA,0);
		upload_send_data_handle();
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
		upload_send_data_frame(BUS4_COMMAND_TYPE_PRESS,g_weight.sta,g_weight.changed_data);
		upload_send_data_handle();
	}

	if(g_bus.report_flag & GESTURE_FLAG)
	{	
		g_bus.report_flag &= ~GESTURE_FLAG;
		//处理
	}

	
}

void old_ad_handle(void)
{

	if(g_bus.report_flag & HEART_FLAG)
	{
		g_bus.report_flag &= ~HEART_FLAG;
		//处理
		upload_send_data_frame(BUS4_COMMAND_TYPE_HEART,BUS6_HEART_TICK_EVENT_DATA,0);
		upload_send_data_handle();
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
		upload_send_data_frame(BUS4_COMMAND_TYPE_PRESS,g_weight.sta,g_weight.changed_data);
		upload_send_data_handle();
	}

	if(g_bus.report_flag & GESTURE_FLAG)
	{	
		g_bus.report_flag &= ~GESTURE_FLAG;
		//处理
	}

	
}



