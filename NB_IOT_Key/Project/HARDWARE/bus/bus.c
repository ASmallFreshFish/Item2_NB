#include "head_include.h"

#ifdef DEBUG_MACRO
u8 debug_char;
#endif

extern u8 sendata[SEND_DATA_LEN];
extern u8 send_buf[SEND_BUF_LEN];
extern u8 upload_buf_sequence;
extern u8 volatile write_location;
extern u8 volatile read_location;
extern u8 read_data_success_flag;
extern u8 write_data_success_flag;
extern u8 upload_main_loop_count;

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


