#include "head_include.h"

#ifdef DEBUG_MACRO
u8 debug_char;
#endif

u8 sendata_gesture[SEND_DATA_GESTURE_LEN]={0};
u8 sendata_press[SEND_DATA_PRESS_LEN]={'0'};
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

void upload_buf_check_write(void)
{
	u8 i,j;
	u8 part_location = write_location;
	
	for( i= part_location;i<SEND_BUF_LEN;i+=SEND_DATA_LEN)
	{
		if((send_buf[i+2] != '0')&&(send_buf[i+3] != '0'))
		{
			write_data_success_flag = 0; 
		}
		else
		{
			write_data_success_flag = 1; 
			write_location = i;
			break;
		}
	}

	if(!write_data_success_flag)
	{
		for( j= 0;j<part_location;j+=SEND_DATA_LEN)
		{
			if((send_buf[j+2] != '0')&&(send_buf[j+3] != '0'))
			{
				write_data_success_flag = 0; 
			}
			else
			{
				write_data_success_flag = 1; 
				write_location = j;
				break;
			}
		}
	}
}

void upload_buf_check_read(void)
{
	u8 i,j;
	u8 part_location = read_location;
	
	for( i= part_location;i<SEND_BUF_LEN;i+=SEND_DATA_LEN)
	{
		if((send_buf[i+2] == '0')&&(send_buf[i+3] == '0'))
		{
			read_data_success_flag = 0; 
		}
		else
		{
			read_data_success_flag = 1; 
			read_location = i;
			break;
		}
	}
	
	if(!read_data_success_flag)
	{
		for( j= 0;j<part_location;j+=SEND_DATA_LEN)
		{
			if((send_buf[j+2] == '0')&&(send_buf[j+3] == '0'))
			{
				read_data_success_flag = 0; 
			}
			else
			{
				read_data_success_flag = 1; 
				read_location = j;
				break;
			}
		}
	}
}

//write data to buf
void upload_buf_write(void)
{
	u8 part_loca = write_location;
	
	if(g_key.sta == NO_PRESS)
	{
#ifdef DEBUG_MACRO
#else
		return; 
#endif	
	}
	else 
	{
		upload_buf_check_write();
		
		if(write_data_success_flag)
		{
			part_loca =write_location;

			send_buf[part_loca] = '0';
			send_buf[part_loca+1] = '0';
			send_buf[part_loca+2] = 0x44;   //D
			send_buf[part_loca+3] = 0x44;    

			send_buf[part_loca+4] =  'F';   //F
			send_buf[part_loca+5] =  'F';

			if(upload_buf_sequence/16>=10)
				send_buf[part_loca+6]=upload_buf_sequence/16+0x37;//转成A-F的字符
			else
				send_buf[part_loca+6]=upload_buf_sequence/16+0x30; 
			if(upload_buf_sequence%16>=10)
				send_buf[part_loca+7]=upload_buf_sequence%16+0x37;//转成A-F的字符
			else
				send_buf[part_loca+7]=upload_buf_sequence%16+0x30; 

			if(g_key.sta/16>=10)
				send_buf[part_loca+8]=g_key.sta/16+0x37;//转成A-F的字符
			else
				send_buf[part_loca+8]=g_key.sta/16+0x30; 
			if(g_key.sta%16>=10)
				send_buf[part_loca+9]=g_key.sta%16+0x37;//转成A-F的字符
			else
				send_buf[part_loca+9]=g_key.sta%16+0x30; 

			send_buf[part_loca+10] = 0;

			g_key.sta = NO_PRESS;

//			upload_change_sequence();

#ifdef DEBUG_MACRO
			send_buf[part_loca+10] = '1';
#endif
		}
	}
	
#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
		debug_char =write_location/11;
		debug_char += 0x30;
		UART1_send_byte(debug_char);
	UART1_send_byte('\n');
		Uart1_SendStr(send_buf);
		delay_ms(2000);
#endif

}

//read data from buf
void upload_buf_read(void)
{
	u8 i;
	u8 part_loca = read_location;
	
	upload_buf_check_read();

	if(read_data_success_flag)
	{
		part_loca =read_location;

		for(i=0;i<SEND_DATA_LEN;i++)
		{
			sendata[i] = send_buf[part_loca+i];
		}
		if(upload_buf_sequence/16>=10)
			sendata[6]=upload_buf_sequence/16+0x37;//转成A-F的字符
		else
			sendata[6]=upload_buf_sequence/16+0x30; 
		if(upload_buf_sequence%16>=10)
			sendata[7]=upload_buf_sequence%16+0x37;//转成A-F的字符
		else
			sendata[7]=upload_buf_sequence%16+0x30; 
		sendata[SEND_DATA_LEN-1] = 0;

		for(i=0;i<SEND_DATA_LEN;i++)
		{
			send_buf[part_loca+i] = '0';
		}
	}
	else
	{
		sendata[0] = '0';
		sendata[1] = '0';
		sendata[2] = 0x44;   //D
		sendata[3] = 0x44;    

		sendata[4] =  'F';   //F
		sendata[5] =  'F';

		if(upload_buf_sequence/16>=10)
			sendata[6]=upload_buf_sequence/16+0x37;//转成A-F的字符
		else
			sendata[6]=upload_buf_sequence/16+0x30; 
		if(upload_buf_sequence%16>=10)
			sendata[7]=upload_buf_sequence%16+0x37;//转成A-F的字符
		else
			sendata[7]=upload_buf_sequence%16+0x30; 

		sendata[8] =send_buf[part_loca+8];
		sendata[9] =send_buf[part_loca+9];
			
		sendata[10] = 0;
	}

#ifdef DEBUG_MACRO
	UART1_send_byte('\n');
		debug_char =read_location/11;
		debug_char += 0x30; 	
		UART1_send_byte(debug_char);
	UART1_send_byte('\n');
		Uart1_SendStr(sendata);
		delay_ms(2000);
#endif

}

void upload_handle(void)
{
	upload_main_loop_count ++;
	switch(upload_main_loop_count)
	{
		case MAIN_LOOP_UPLOAD_NUM:
			upload_buf_read();
			BC95_SendCOAPdata("5",sendata);
			read_location +=SEND_DATA_LEN;
			if(read_location>=SEND_BUF_LEN)
				read_location = 0;
			upload_change_sequence();
			
			delay_ms(10);
			break;
			
		case MAIN_LOOP_RECEIVE_NUM:
			upload_main_loop_count = 0;
			BC95_RECCOAPData();//接收服务器下发的数据
			break;
			
		default:
			delay_ms(10);
			break;
	}

}


void hex_to_char(u8  data_hex,u8  data_ch[])
{

	if(data_hex/16>=10)
		data_ch[0]=data_hex/16+0x37;//转成A-F的字符
	else
		data_ch[0]=data_hex/16+0x30; 
//	UART1_send_byte(data_ch[0]);
	
	if(data_hex%16>=10)
		data_ch[1]=data_hex%16+0x37;//转成A-F的字符
	else
		data_ch[1]=data_hex%16+0x30;
//	UART1_send_byte(data_ch[1]);

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


void upload_buf_press_frame(void)
{
	if(press_ad.change_flag)
	{
		//message_id
		sendata_press[0] = '0';
		sendata_press[1] = '2';
		//head
		sendata_press[2] = 0x44;   //D
		sendata_press[3] = 0x44;   
		//id
		sendata_press[4] =  'F';   //F
		sendata_press[5] =  'F';
		sendata_press[6] =  'F';   //F
		sendata_press[7] =  'F';
		sendata_press[8] =  'F';   //F
		sendata_press[9] =  'F';
		//command_type
		sendata_press[10] =  'F';   //F
		sendata_press[11] =  'E';

		if(upload_buf_sequence/16>=10)
			sendata_press[12]=upload_buf_sequence/16+0x37;//转成A-F的字符
		else
			sendata_press[12]=upload_buf_sequence/16+0x30; 
		if(upload_buf_sequence%16>=10)
			sendata_press[13]=upload_buf_sequence%16+0x37;//转成A-F的字符
		else
			sendata_press[13]=upload_buf_sequence%16+0x30; 

		if( NO_TO_HAVE_FLAG == press_ad.change_flag)
		{
			sendata_press[14]= 'F';//转成A-F的字符
			sendata_press[15]= 'F';
	//		UART1_send_byte('P');
		}
		else
		{
			sendata_press[14]= '0';//转成A-F的字符
			sendata_press[15]= '0';
	//		UART1_send_byte('Q');
		}

		sendata_press[16] = 0;
		
		upload_change_sequence();


#ifdef DEBUG_MACRO
		 UART1_send_byte('\n');
		 Uart1_SendStr(sendata_press);
		 delay_ms(1000);
		 UART1_send_byte('\n');
		//			sendata_press[part_loca+10] = '1';
#endif
	}
}


void upload_press_handle(void)
{
	if(press_ad.change_flag)
	{
		BC95_SendCOAPdata("8",sendata_press);
		delay_ms(1000);
		BC95_RECCOAPData();
	}
}

//gesture upload
//0227
void upload_buf_gesture_frame(void)
{
	u8 loc = 0;
	u8 imei_hex_str[31];
	u8 temp_str[3];
	memset(sendata_gesture,0,sizeof(sendata_gesture));
	
	if(g_gesture.sta)
	{
		//BUS1:MESSAGE_ID
		strncat(sendata_gesture,BUS1_MESSAGE_ID_GESTURE,2);
		loc += 2;
		
		//BUS2:HEAD
		strncat(sendata_gesture,BUS2_HEAD,2);
		loc += 2;
		
		//BUS3:IMEI(15hex,30hexstr)
		hex_to_str(imei_str,imei_hex_str,15);
		strncat(sendata_gesture,imei_hex_str,30);
		loc += 30;
		
		//BUS4:COMMAND_TYPE
		strncat(sendata_gesture,BUS4_COMMAND_TYPE_GESTURE,2);
		loc += 2;
		
		//BUS5:SEQUENCE
		hex_to_str(&upload_buf_sequence,temp_str,1);
		strncat(sendata_gesture,temp_str,2);
		loc += 2;
		
		//BUS6:DATA
		hex_to_str(&g_gesture.sta,temp_str,1);
		strncat(sendata_gesture,temp_str,2);
		loc += 2;
		
		sendata_gesture[loc] = 0;
		upload_change_sequence();

#ifdef DEBUG_MACRO
		 Uart1_SendStr(sendata_gesture);
		 UART1_send_byte('\n');
#endif
	}
}


void upload_gesture_handle(void)
{
	if(g_gesture.sta)
	{
		BC95_SendCOAPdata("20",sendata_gesture);
		delay_ms(1000);
		BC95_RECCOAPData();
	}
}

