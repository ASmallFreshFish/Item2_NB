#include "head_include.h"

bus_type g_bus ={0};
bus_receive_type g_receive={0};

#define UPLOAD_BUF_NUM 10
//#define UPLOAD_BUF_NUM 5
#define UPLOAD_BUF_LEN 200
//#define UPLOAD_BUF_LEN 100


u8 upload_buf_head =0;
u8 upload_buf_tail =0;
u8 upload_buf_count =0;
u8 upload_buf_number =0;
u8 upload_buf_round =0;
u8 upload_buf[UPLOAD_BUF_NUM][UPLOAD_BUF_LEN]={0};

u8 upload_send_data[UPLOAD_SEND_DATA_LEN]={0};
u8 upload_buf_sequence = 1;

extern press_ad_type g_press;
extern gesture_struct_type g_gesture;
extern char imei_str[16];

/**************************************************************

* ��ӡ��־��غ���

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
	
	hex8_to_hexchar(t,pp);
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
	
	hex16_to_hexstr(t,pp);
	for(i=0;i<4;i++)
	{
		printf_char(pp[i]);
	}
}

//������8λ��ʮ���Ʊ�ʾ�����3λ
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

//������16λ��ʮ���Ʊ�ʾ�����5λ
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

//������32λ��ʮ���Ʊ�ʾ�����10λ
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

//����Ӧ��
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
����ת�ַ���غ���
**************************************************************/

void hex8_to_hexchar(u8      data_hex,u8  data_ch[])
{
	if(data_hex/16>=10)
		data_ch[0]=data_hex/16+0x37;//ת��A-F���ַ�
	else
		data_ch[0]=data_hex/16+0x30; 
	
	if(data_hex%16>=10)
		data_ch[1]=data_hex%16+0x37;//ת��A-F���ַ�
	else
		data_ch[1]=data_hex%16+0x30;
}

void hex16_to_hexstr(u16 data_hex,u8 *outtxt)
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

void hex8_to_hexstr(u8 *inchar,u8 *outtxt,u32 len)
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

void dec8_to_str(u8 *inchar,u8 *outtxt,u32 len)
{
	u8 hbit,lbit;
	u32 i;
	for(i=0;i<len;i++)
	{
		hbit = (*(inchar+i)/10);
		lbit = *(inchar+i)%10;
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

void bytesToHexString (const char * inBuf, char *outBuf, u32 len)
{
    static  char hexTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int x = 0;
	
    for (x = 0; x < len; x++)
    {
        outBuf[x] = hexTable[(inBuf[x] >> 4) & 0x0f]; // upper nibble
        outBuf[x+1] = hexTable[inBuf[x] & 0x0f]; // lower nibble
    }
}



/**************************************************************

ͨ�������ϱ���غ���

**************************************************************/
void upload_init(void)
{
	memset(&g_bus,0,sizeof(g_bus));
	g_bus.report_times = UPLOAD_STRAIN_REPORT_TIMES;
	g_bus.report_interval = UPLOAD_STRAIN_REPORT_INTERVAL;
}


void upload_change_sequence(void)
{
	upload_buf_sequence++;
	if(upload_buf_sequence>200)
		upload_buf_sequence = 1;
}

//u8 upload_buf[UPLOAD_BUF_NUM][UPLOAD_BUF_LEN]={0};
//���buf�����Ƿ���д��
u8 upload_buf_is_full_check(void)
{
	for(u8 i=0;i<UPLOAD_BUF_NUM;i++)
	{
		if(upload_buf[i][0] == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

u8 old_upload_buf_is_full_check(void)
{
	if((upload_buf_head == upload_buf_tail)&&(upload_buf[upload_buf_head][0] != 0))
	{
		return TRUE;
	}
	return FALSE;
}

//���buf�����Ƿ�û������
u8 upload_buf_is_empty_check(void)
{
	for(u8 i=0;i<UPLOAD_BUF_NUM;i++)
	{
		if(upload_buf[i][0] != 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

u8 upload_buf_bumber_check(void)
{
	u8 count = 0;
	for(u8 i=0;i<UPLOAD_BUF_NUM;i++)
	{
		if(upload_buf[i][0] != 0)
		{
			count++;
		}
	}
	return count;
}


void upload_buf_write_in(void)
{
	if(upload_buf_is_full_check())
	{
		printf_string("\n write_in_full!");
		return;
	}
	
	upload_buf[upload_buf_head][0] = g_bus.report_times;
	copy_buf(&upload_buf[upload_buf_head][1],upload_send_data,UPLOAD_BUF_LEN);

#ifdef DEBUG_MACRO
		printf_string("\n !!!!!!!!write_in:head:");
		printf_u8_decStr(upload_buf_head);
#endif

	upload_buf_head ++;
	upload_buf_number ++;
	if(upload_buf_head >= UPLOAD_BUF_NUM) 
	{
		upload_buf_head = 0;
	}

}
u8 upload_buf_read_out(void)
{
	u8 result = TRUE;
	u8 point =0;
	
	if(upload_buf_is_empty_check())
	{
		printf_string("\n read_out_empty!");
		return FALSE;
	}
	
	point = upload_buf_tail + upload_buf_count;
	if(point >= UPLOAD_BUF_NUM)
	{
		point -= UPLOAD_BUF_NUM;
	}

#ifdef DEBUG_MACRO
		printf_string("\n readout:tail_head_number_count:");
		printf_u8_decStr(upload_buf_tail);
		printf_string("\t");
		printf_u8_decStr(upload_buf_head);
		printf_string("\t");
		printf_u8_decStr(upload_buf_number);
		printf_string("\t");
		printf_u8_decStr(upload_buf_count);

		for(int j=0;j<UPLOAD_BUF_NUM;j++)
		{
			if(upload_buf[j][0] != 0)
			{
				printf_string("\n");
				printf_u8_decStr(j);
				printf_string("\t");
				printf_u8_decStr(upload_buf[j][0]);
				printf_string("\t");
				printf_string(upload_buf[j]);
			}
		}	
#endif
	
	copy_buf(upload_send_data,&upload_buf[point][1],UPLOAD_BUF_LEN);
	upload_buf[point][0]--;

	return TRUE;
}

//����ÿһ�η������ݺ��״̬�жϺͷ���buf
u8 upload_buf_handle(void)
{
#ifdef DEBUG_MACRO
	printf_string("\n ?????????buf_handle:count:");
	printf_u8_decStr(upload_buf_count);
#endif

	int i,head,tail;
	if(upload_buf_count >= upload_buf_number)
	{
			
		head=upload_buf_head;
		tail=upload_buf_tail;

		if(tail < head)
		{
			for(i=tail;i<head;i++)
			{
				if(upload_buf[i][0] != 0)
				{
					break;
				}
			}
		}
		else
		{
			for(i=tail;i<UPLOAD_BUF_NUM;i++)
			{
				if(upload_buf[i][0] != 0)
				{
					break;
				}
			}
			if( i == UPLOAD_BUF_NUM)
			{
				for(i=0;i<head;i++)
				{
					if(upload_buf[i][0] != 0)
					{
						break;
					}
				}
			}
		}
		
		
//		i = (i>head)?(head):(i);
		upload_buf_tail = i;
		upload_buf_count = 0;
		upload_buf_round = 0;

//		upload_buf_number = (upload_buf_head >= upload_buf_tail)?(upload_buf_head - upload_buf_tail)
//			:(upload_buf_head+UPLOAD_BUF_NUM-upload_buf_tail);

		upload_buf_number = upload_buf_bumber_check();
		if(upload_buf_is_empty_check())
		{
			g_bus.report_it_enable_flag = FALSE;
			upload_buf_number =0;
		}

//		g_sta = BUS_GET_HANDLE_STA;
#ifdef DEBUG_MACRO
					
			printf_string("\t tail_head_number_count_round:");
			printf_u8_decStr(upload_buf_tail);
			printf_string("\t");
			printf_u8_decStr(upload_buf_head);
			printf_string("\t");
			printf_u8_decStr(upload_buf_number);
			printf_string("\t");
			printf_u8_decStr(upload_buf_count);
			printf_string("\t");
			printf_u8_decStr(upload_buf_round);
#endif
	}
	else
	{
	
#ifdef DEBUG_MACRO
			printf_string("\t  a round!");
#endif
		
		g_sta =  BUS_UPLOAD_HANDLE_STA;
		upload_buf_round =1;
	}
}

void upload_send_data_frame(u8* command_type,u8 event,u16 data1,u16 data2,u16 data3)
{
	u8 loc = 0;
	u8 t;
	u8 pp[2];
	s8 imei_hex_str[31];
	u8 change_data_str[5];
	u8 time_dec[7];
	u8 time_str[13];
	u8 time_hex_str[25];
	u8 version_hex_str[7];
	
	memset(upload_send_data,0,sizeof(upload_send_data));
	
	//BUS1:MESSAGE_ID
	strncat(upload_send_data,BUS1_MESSAGE_ID_GESTURE,2);
	loc += 2;
	
	//BUS2:HEAD
	strncat(upload_send_data,BUS2_HEAD,2);
	loc += 2;
	
	//BUS3:IMEI  (15hex,30hexstr)
	hex8_to_hexstr(imei_str,imei_hex_str,15);
	strncat(upload_send_data,imei_hex_str,30);
	loc += 30;
	
	//BUS4:COMMAND_TYPE
	strncat(upload_send_data,command_type,2);
	loc += 2;
	
	//BUS5:SEQUENCE
	t=upload_buf_sequence;
	hex8_to_hexchar(t,pp);
	strncat(upload_send_data,pp,2);
	loc += 2;
	
	//BUS6:EVENT
	t=event;
	hex8_to_hexchar(t,pp);
	strncat(upload_send_data,pp,2);
	loc += 2;

	//BUS7:data1  (2hex,4hexstr)
	hex16_to_hexstr(data1,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;
	
	//BUS8:data2  (2hex,4hexstr)
	hex16_to_hexstr(data2,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;
	
	//BUS9:data3  (2hex,4hexstr)
	hex16_to_hexstr(data3,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;

	//BUS10:time  (6hex,12hexstr)
	clock_copy_time_to_buf(time_dec);
	dec8_to_str(time_dec,time_str,6);
	hex8_to_hexstr(time_str, time_hex_str, 12);
	strncat(upload_send_data,&time_hex_str[0],24);
	loc += 24;

	//BUS11:version  (3hex,6hexstr)
	hex8_to_hexstr((u8*)version_number, version_hex_str, 6);
	strncat(upload_send_data,&version_hex_str[0],6);
	loc += 6;
	
	upload_send_data[loc] = '\0';
	
	upload_change_sequence();

#ifdef DEBUG_MACRO
//		 UART1_send_byte('\n');
//		 Uart1_SendStr(upload_send_data);
//		 UART1_send_byte('\n');
#endif
}


void old_upload_send_data_frame(u8* command_type,u8 event,u16 data1,u16 data2,u16 data3)
{
	u8 loc = 0;
	u8 t;
	u8 pp[2];
	s8 imei_hex_str[31];
	u8 change_data_str[5];
	u8 time_dec[7];
	u8 time_str[13];
	u8 time_hex_str[25];
	u8 version_hex_str[7];
	
	memset(upload_send_data,0,sizeof(upload_send_data));
	
	//BUS1:MESSAGE_ID
	strncat(upload_send_data,BUS1_MESSAGE_ID_GESTURE,2);
	loc += 2;
	
	//BUS2:HEAD
	strncat(upload_send_data,BUS2_HEAD,2);
	loc += 2;
	
	//BUS3:IMEI  (15hex,30hexstr)
	hex8_to_hexstr(imei_str,imei_hex_str,15);
	strncat(upload_send_data,imei_hex_str,30);
	loc += 30;
	
	//BUS4:COMMAND_TYPE
	strncat(upload_send_data,command_type,2);
	loc += 2;
	
	//BUS5:SEQUENCE
	t=upload_buf_sequence;
	hex8_to_hexchar(t,pp);
	strncat(upload_send_data,pp,2);
	loc += 2;
	
	//BUS6:EVENT
	t=event;
	hex8_to_hexchar(t,pp);
	strncat(upload_send_data,pp,2);
	loc += 2;

	//BUS7:data1  (2hex,4hexstr)
	hex16_to_hexstr(data1,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;
	
	//BUS8:data2  (2hex,4hexstr)
	hex16_to_hexstr(data2,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;
	
	//BUS9:data3  (2hex,4hexstr)
	hex16_to_hexstr(data3,change_data_str);
	strncat(upload_send_data,change_data_str,4);
	loc += 4;

	//BUS10:time  (6hex,12hexstr)
	clock_copy_time_to_buf(time_dec);
	dec8_to_str(time_dec,time_str,6);
	hex8_to_hexstr(time_str, time_hex_str, 12);
	strncat(upload_send_data,&time_hex_str[0],24);
	loc += 24;

	//BUS11:version  (3hex,6hexstr)
	hex8_to_hexstr((u8*)version_number, version_hex_str, 6);
	strncat(upload_send_data,&version_hex_str[0],6);
	loc += 6;
	
	upload_send_data[loc] = '\0';
	upload_change_sequence();

#ifdef DEBUG_MACRO
//		 UART1_send_byte('\n');
//		 Uart1_SendStr(upload_send_data);
//		 UART1_send_byte('\n');
#endif
}

void upload_send_data_handle(void)
{		
#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
	Uart1_SendStr(upload_send_data);
	UART1_send_byte('\n');
#endif

	BC95_SendCOAPdata("41",upload_send_data);
	delay_ms(1000);
	BC95_RECCOAPData();
}

void old_upload_send_data_handle(void)
{		
	BC95_SendCOAPdata("22",upload_send_data);
	delay_ms(1000);
	BC95_RECCOAPData();
}

// ���������ϱ��¼�
// �¼����࣬�����¼����ȼ���ʱ�ϱ���
void  upload_handle(void)
{
	
#ifdef DEBUG_MACRO
	printf_string("\nreport_flag:");
	printf_u8_hexStr(g_bus.report_flag);
	printf_string("\t interrupt_flag:");
	printf_u8_hexStr(g_time.interrupt_flag);
#endif

	if((g_bus.report_flag & STRAIN_FLAG)
		||(g_time.interrupt_flag & TIME_IR_FLAG_REPORT_INTERVAL)
		||(upload_buf_round))				
	{
		//�����ϱ�
//		g_time.interrupt_flag &= ~TIME_IR_FLAG_REPORT_INTERVAL;
		upload_strain_handle();
	}
	else if(g_bus.report_flag&BAT_LOW_POWER_FLAG)
	{
		//�͵����ϱ�����
		upload_bat_low_handle();
//		g_sta = BUS_GET_HANDLE_STA;
	}
	else if(g_bus.report_flag&BAT_POWER_FLAG)
	{
		//�����ϱ�����
		upload_bat_power_handle();
//		g_sta = BUS_GET_HANDLE_STA;
	}
	else if(g_bus.report_flag & HEART_FLAG)		
	{
		//�����ϱ�
		upload_heart_handle();
//		g_sta = BUS_GET_HANDLE_STA;
	}
}


// ���������ϱ��¼�
// �¼����࣬�����¼����ȼ���ʱ�ϱ���
void  old_upload_handle(void)
{
	if((g_bus.have_reported_flag & BAT_OFF_POWER_FLAG) == BAT_OFF_POWER_FLAG )
	{
		return;
	}
	
#ifdef DEBUG_MACRO
	printf_string("\nreport_flag:");
	printf_u8_hexStr(g_bus.report_flag);
#endif

	if(g_bus.report_flag & STRAIN_FLAG)				//�����ϱ�
	{
		upload_strain_handle();
	}
	else if(g_bus.report_flag & PRESS_SENSOR_FLAG)
	{
		g_bus.report_flag &= ~PRESS_SENSOR_FLAG;
		upload_send_data_frame(BUS4_COMMAND_TYPE_PRESS_SENSOR,BUS6_PRESS_SENSOR_OPEN_DATA,g_press.press_result,0,0);
		upload_send_data_handle();
	}
	else if(g_bus.report_flag&BAT_OFF_POWER_FLAG)	//��ش���
	{
		//�ػ��ϱ�����
		g_bus.report_flag &= ~BAT_OFF_POWER_FLAG;
		g_bus.have_reported_flag|=BAT_OFF_POWER_FLAG;
		upload_send_data_frame(BUS4_COMMAND_TYPE_BAT,BUS6_BAT_OFF_POWER_DATA,0,0,0);
		upload_send_data_handle();
	}
	else if(g_bus.report_flag&BAT_LOW_POWER_FLAG)
	{
		//�͵����ϱ�����
		upload_bat_low_handle();
	}
	else if(g_bus.report_flag&BAT_POWER_FLAG)
	{
		//�����ϱ�����
		upload_bat_power_handle();
	}
	else if(g_bus.report_flag & HEART_FLAG)		//�����ϱ�
	{
		//�����ϱ�
		upload_heart_handle();
	}
}


void upload_strain_handle(void)
{
	if(g_bus.report_flag & STRAIN_FLAG)
	{
		g_bus.report_flag &= ~STRAIN_FLAG;
		upload_send_data_frame(BUS4_COMMAND_TYPE_STRAIN,g_weight.sta,g_weight.changed_data,g_press.press_ad_value[5],g_press.press_ad_value[6]);
		upload_buf_write_in();
	}

	if(((g_bus.report_it_enable_flag)&&(g_time.interrupt_flag & TIME_IR_FLAG_REPORT_INTERVAL))
		||(!g_bus.report_it_enable_flag)||(upload_buf_round))
	{
		g_time.interrupt_flag &= ~TIME_IR_FLAG_REPORT_INTERVAL;
		if(upload_buf_read_out())
		{
			g_bus.report_it_enable_flag = TRUE;
			upload_buf_count++;
			
			upload_send_data_handle();

			upload_buf_handle();
		}
	}	
}


//�����ϱ�����
void old_upload_strain_handle(void)
{
#ifdef DEBUG_MACRO
	printf_string("\nreport_count:");
	printf_u8_decStr(g_bus.report_count);
#endif

	if(g_bus.report_count == BUS_FRAME_STA)
	{
		upload_send_data_frame(BUS4_COMMAND_TYPE_STRAIN,g_weight.sta,g_weight.changed_data,g_press.press_ad_value[5],g_press.press_ad_value[6]);
	}
	
	upload_send_data_handle();
	g_bus.report_count++;

	if(g_bus.report_count == g_bus.report_times)
	{
		g_bus.report_flag &= ~STRAIN_FLAG;
		g_bus.report_count=BUS_FRAME_STA;
	}
}

//�͵����ϱ�һ��
void upload_bat_low_handle(void)
{
	upload_send_data_frame(BUS4_COMMAND_TYPE_BAT,BUS6_BAT_LOW_POWER_DATA,0,0,0);
	upload_send_data_handle();
	g_bus.report_flag &= ~BAT_LOW_POWER_FLAG;
	g_bus.have_reported_flag|=BAT_LOW_POWER_FLAG;
}

//��ѹ�ϱ�һ��
void upload_bat_power_handle(void)
{
	upload_send_data_frame(BUS4_COMMAND_TYPE_BAT,BUS6_BAT_POWER_DATA,g_bat.bat_value,0,0);
	upload_send_data_handle();
	g_bus.report_flag &= ~BAT_POWER_FLAG;
}

//�����ϱ�һ��
void upload_heart_handle(void)
{
	upload_send_data_frame(BUS4_COMMAND_TYPE_HEART,BUS6_HEART_TICK_EVENT_DATA,0,0,0);
	upload_send_data_handle();
	g_bus.report_flag &= ~HEART_FLAG;
}



/**************************************************************

ͨ�����������غ���

**************************************************************/

// �������������¼�
void  bus_get_handle(void)
{
	//����ͬ��ʱ��
	if(my_g_time.m_clock_syn_flag)
	{
		my_g_time.m_clock_syn_flag = 0;
		get_syn_clock_handle();
	}

	//������յ���ָ��
	get_receive_data_handle();
	
}

void get_syn_clock_handle(void)
{
	//����ʱ��
	my_time_type now_time;

	clock_get_time(&now_time);

	//0��ͬ����վʱ��
//	if(now_time.hour == 0)
	if(now_time.hour == 16)
	{
		if(!my_g_time.m_clock_syn_result)
		{
			my_g_time.m_clock_syn_result = clock_syn_time();
#ifdef DEBUG_MACRO
			printf_string("\nsyn:");
			printf_u8_decStr(my_g_time.m_clock_syn_result);
#endif
		}
	}
	else
	{
		my_g_time.m_clock_syn_result =0;
	}
}

void get_receive_data_handle(void)
{
	//���������Ϣ��ע��״̬�����л�
	if(g_bus.receive_flag)
	{
		printf_string("\nreceive: ");
		
		printf("AT+NMGR\r\n");
		delay_ms(1000);

		strx=strstr((const char*)RxBuffer,(const char*)",");
		if(strx)
		{
			//��������
			get_parse_data();
			//������ȡ����
			g_sta = BUS_GET_HANDLE_STA;
		}
		else
		{
			//������ȡ����
			g_bus.receive_flag =0;
			g_sta = PRESS_HANDLE_STA;
		}
		
		Clear_Buffer();	
	}
	else
	{
		g_sta = PRESS_HANDLE_STA;
	}
}
void get_parse_data(void)
{
	u32 len;
	u32 r_message_id,r_head,r_command_type,r_sequence,r_data1,r_data2,r_data3;

	sscanf(strx-2,"%02d,%02x%02x%02x%02x%04x%04x%04x",&len,&r_message_id,
		&r_head,&r_command_type,&r_sequence,
		&r_data1,&r_data2,&r_data3);
#ifdef DEBUG_MACRO
printf_string("\nparse:");
	printf_u8_hexStr(len);printf_string(",\t");
	printf_u8_hexStr(r_message_id);printf_string("\t");
	printf_u8_hexStr(r_head);printf_string("\t");
	printf_u8_hexStr(r_command_type);printf_string("\t");
	printf_u8_hexStr(r_sequence);printf_string("\t");
	printf_u16_hexStr(r_data1);printf_string("\t");
	printf_u16_hexStr(r_data2);printf_string("\t");
	printf_u16_hexStr(r_data3);printf_string("\t");
#endif

	if((r_message_id == BUS1_R_MESSAGE_ID )&&(r_head == BUS2_R_HEAD))
	{
		if(r_sequence == g_receive.r_sequence )
		{
			return;
		}

		g_receive.r_sequence = r_sequence;
		if(r_command_type == BUS4_R_C_TYPE_WEIGHT_THRESHOLD)
		{
			//���ı�����д��eeprom
			

			if((g_weight.change_threshold != r_data1)&&(r_data1 != 0))
			{
				g_weight.change_threshold = r_data1;
				eeprom_write((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,&g_weight.change_threshold,
					(u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].length);
			}
			
#ifdef DEBUG_MACRO
			printf_string("\nc_weight_threshold:");
			printf_u16_decStr(g_weight.change_threshold);
#endif
		}
		else if(r_command_type == BUS4_R_C_TYPE_WEIGHT_UPLOAD_TIMES)
		{
			//���ı�����д��eeprom
			if((g_bus.report_times != r_data1)&&(r_data1 != 0))
			{
				g_bus.report_times =r_data1;
				eeprom_write((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,(u16 *)(&g_bus.report_times),
					(u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].length);
			}

#ifdef DEBUG_MACRO
			printf_string("\nc_upload_times:");
			printf_u16_decStr(g_bus.report_times);
#endif
		}
		else if(r_command_type == BUS4_R_C_TYPE_WEIGHT_UPLOAD_INTERVAL)
		{
			//���ı�����д��eeprom
			if((g_bus.report_interval != r_data1)&&(r_data1 != 0))
			{
				g_bus.report_interval =r_data1;
				eeprom_write((u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].offset_addr,(u16 *)(&g_bus.report_interval),
					(u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].length);
			}

#ifdef DEBUG_MACRO
			printf_string("\nc_upload_interval:");
			printf_u16_decStr(g_bus.report_interval);
#endif
		}
		
	}
	
}

/*************************************************
test 
**************************************************/
test_type g_test ={0};


