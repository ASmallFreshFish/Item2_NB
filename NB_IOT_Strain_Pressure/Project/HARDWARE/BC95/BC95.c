#include "head_include.h"

char *strx,*extstrx;
//ע��char*��char[]������
//char *imei_str = 0;
char imei_str[16] = {0};
//	char imei_str[16] = {"868246048366603"};


extern char  RxBuffer[100],RxCounter;
BC95 BC95_Status;

void Clear_Buffer(void)//��ջ���
{
#ifdef DEBUG_MACRO
//	printf_string("\nclear1\n!");
	Uart1_SendStr(RxBuffer);
#endif

	u8 i;
	for(i=0;i<100;i++)
		RxBuffer[i]=0;
	RxCounter=0;

#ifdef DEBUG_MACRO
	printf_string("\nclear2!\n");
#endif
}

void old_Clear_Buffer(void)//��ջ���
{
#ifdef DEBUG_MACRO
	for(u8 i=0;i<100;i++)
	{
		printf_char(RxBuffer[i]);	
	}
#endif

	u8 i;
	for(i=0;i<100;i++)
		RxBuffer[i]=0;
	RxCounter=0;
}


void copy_buf(char *buf_dest,char *buf_source,u8 len)
{
	u8 i;
	for(i = 0;i<len;i++)
	{
		buf_dest[i] = buf_source[i];
	}
	buf_dest[len] = NULL;
		//LOG��ӡ
	#ifdef DEBUG_MACRO_INIT
		Uart1_SendStr("\r\n");
		Uart1_SendStr(buf_dest);
	#endif
}

void CDP_Init(void)//COAP���������ó�ʼ��
{
	printf("AT\r\n"); 
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		Clear_Buffer();	
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("AT\r\n"); 
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
    	BC95_Status.netstatus=1;//��˸ûע������
    	
    printf("AT+NBAND?\r\n");//��ȡƵ�κ�
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����5  ����  ����8���ƶ� �ƶ��Ŀͻ���ط�ע��һ���޸�
		Clear_Buffer();	
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("AT+NBAND?\r\n");//��ȡƵ�κ�
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����OK
		}
		
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//��46011�ǵ��ţ��ƶ���Ҫ����
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//��46011�ǵ��ţ��ƶ���Ҫ����
		}
		
    printf("AT+CFUN=0\r\n");//�ر���Ƶ
		Delay(300);	
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		while(strx==NULL)
		{
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		Clear_Buffer();	
		
    printf("AT+NCDP=180.101.147.115,5683\r\n");//����CDP��������ַ����ƽ̨�ģ����COAPЭ���IP��ַ�ǹ̶���
		Delay(300);	
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		while(strx==NULL)
		{
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		Clear_Buffer();
		
    printf("AT+NRB\r\n");//��ģ���������
    	Delay(2000);	//��ʱ2s	
    	
    printf("AT+CIMI\r\n");//��ȡ���ţ�ȷ��ģ����������
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//��46011
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//��ȡ���ţ�ȷ��ģ����������
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//����OK,˵�����Ǵ��ڵ�
		}	
		///����NCDP�ľ�ȷ�Ϻ���,�������������ñ�ȷ�Ϻ�.
}
void BC95_Init(void)
{
    unsigned char i;
    printf("AT\r\n"); 
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT\r\n"); 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		BC95_Status.netstatus=1;//��˸ûע������
		
    printf("AT+NBAND?\r\n");//��ȡƵ�κ�
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����5  ����,����8���ƶ���
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+NBAND?\r\n");//��ȡƵ�κ�
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����OK
		}
		
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//��46011�����ţ��ƶ�����������ֵ
		//LOG��ӡ
		#ifdef DEBUG_MACRO_INIT
			Uart1_SendStr("\r\n");
			Uart1_SendStr(strx);
		#endif
		
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//����OK,˵�����Ǵ��ڵĵ��ţ��ƶ�����������ֵ
		    //LOG��ӡ
		    #ifdef DEBUG_MACRO_INIT
				Uart1_SendStr("\r\n");
				Uart1_SendStr(strx);
			#endif
			
		}
		
	printf("AT+CGSN=1\r\n");//��ȡIMEI�ţ������Ƿ����NB�豸����˼���Ƚ���Ҫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");

		//����IMEI����
		copy_buf(imei_str,strx+6,16);
		
		Clear_Buffer();
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGSN=1\r\n");//��ȡIMEI�ţ������Ƿ����NB�豸����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");
			copy_buf(imei_str,strx+6,16);
		}
		
    printf("AT+CGATT=1\r\n");//�������磬���總�� PDP
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//��46011
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT=1\r\n");//�������� 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		
    printf("AT+CGATT?\r\n");//��ѯ����״̬
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT?\r\n");//��ȡ����״̬
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
		}
		
    printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//����CSQ
		if(strx)
		{
		    BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
		    if(BC95_Status.CSQ==99)//˵��ɨ��ʧ��
		    {
		        while(1)
		        {
		            Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
		            Delay(300);
		        }
		    }
		        BC95_Status.netstatus=4;//ע���ɹ�
		 }
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
		    if(strx)
		    {
		        BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
		        if(BC95_Status.CSQ==99)//˵��ɨ��ʧ��
		        {
		            while(1)
		            {
		                Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
		                Delay(300);
		            }
		        }
		     }
		}
    	Clear_Buffer();	
		
    printf("AT+CEREG?\r\n");	//�鿴����ע��״̬
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG?\r\n");//�ж���Ӫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
		    extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
		}
		
    printf("AT+CEREG=1\r\n");	//ע������
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG=1\r\n");//
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		
    printf("AT+NMSTATUS?\r\n");//�жϵ�ǰģ����ƽ̨֮������ӹ�ϵ��һ��δ������֮ǰ����INIITIALISED������������͵����ݵ�ƽ̨֮����MO_DATA_ENABLED
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NMSTATUS:INIITIALISED");//�ж��Ƿ񷵻س�ʼ��״̬���û������ڴ���һЩ��ص��ж�
//		if(strx)//����ǵģ��͵���LED��
//			LEDMCU_RED_OPEN;

//	printf("AT+NNMI=0\r\n");  //����ģ�飬����������
//	printf("AT+NNMI=1\r\n");//����ģ�飬����������ɺ��Զ��յ���������ֱ�Ӵ�ӡ������
	printf("AT+NNMI=2\r\n");  //����ģ�飬����������ɺ�ͨ��AT+NMGR��ȡ����
    
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"OK");//
	    for(i=0;i<100;i++)
	    	RxBuffer[i]=0;	
	    Clear_Buffer();    

	//ͬ��ʱ��
	printf("AT+CCLK?\r\n");//ͬ����վʱ��
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CCLK?\r\n");//��ȡIMEI�ţ������Ƿ����NB�豸����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		}
		my_g_time.m_clock_syn_result = 1;
		clock_cclk_handle(strx);
		Clear_Buffer();

}

void old_BC95_Init(void)
{
    unsigned char i;
    printf("AT\r\n"); 
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT\r\n"); 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		BC95_Status.netstatus=1;//��˸ûע������
		
    printf("AT+NBAND?\r\n");//��ȡƵ�κ�
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����5  ����,����8���ƶ���
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+NBAND?\r\n");//��ȡƵ�κ�
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����OK
		}
		
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//��46011�����ţ��ƶ�����������ֵ
		//LOG��ӡ
		#ifdef DEBUG_MACRO_INIT
			Uart1_SendStr("\r\n");
			Uart1_SendStr(strx);
		#endif
		
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//����OK,˵�����Ǵ��ڵĵ��ţ��ƶ�����������ֵ
		    //LOG��ӡ
		    #ifdef DEBUG_MACRO_INIT
				Uart1_SendStr("\r\n");
				Uart1_SendStr(strx);
			#endif
			
		}
		
	printf("AT+CGSN=1\r\n");//��ȡIMEI�ţ������Ƿ����NB�豸����˼���Ƚ���Ҫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");

		//����IMEI����
		copy_buf(imei_str,strx+6,16);
		
		Clear_Buffer();
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGSN=1\r\n");//��ȡIMEI�ţ������Ƿ����NB�豸����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");
			copy_buf(imei_str,strx+6,16);
		}
		
    printf("AT+CGATT=1\r\n");//�������磬PDP
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//��46011
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT=1\r\n");//�������� 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		
    printf("AT+CGATT?\r\n");//��ѯ����״̬
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT?\r\n");//��ȡ����״̬
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
		}
		
    printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//����CSQ
		if(strx)
		{
		    BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
		    if(BC95_Status.CSQ==99)//˵��ɨ��ʧ��
		    {
		        while(1)
		        {
		            Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
		            Delay(300);
		        }
		    }
		        BC95_Status.netstatus=4;//ע���ɹ�
		 }
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
		    if(strx)
		    {
		        BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
		        if(BC95_Status.CSQ==99)//˵��ɨ��ʧ��
		        {
		            while(1)
		            {
		                Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
		                Delay(300);
		            }
		        }
		     }
		}
    	Clear_Buffer();	
		
    printf("AT+CEREG?\r\n");
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG?\r\n");//�ж���Ӫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
		    extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
		}
		
    printf("AT+CEREG=1\r\n");
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG=1\r\n");//
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		
    printf("AT+NMSTATUS?\r\n");//�жϵ�ǰģ����ƽ̨֮������ӹ�ϵ��һ��δ������֮ǰ����INIITIALISED������������͵����ݵ�ƽ̨֮����MO_DATA_ENABLED
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NMSTATUS:INIITIALISED");//�ж��Ƿ񷵻س�ʼ��״̬���û������ڴ���һЩ��ص��ж�
//		if(strx)//����ǵģ��͵���LED��
//			LEDMCU_RED_OPEN;
		
    printf("AT+NNMI=1\r\n");//����ģ���յ���������ֱ�Ӵ�ӡ������
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"OK");//
	    for(i=0;i<100;i++)
	    	RxBuffer[i]=0;	
	    Clear_Buffer();    

	//ͬ��ʱ��
	printf("AT+CCLK?\r\n");//ͬ����վʱ��
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CCLK?\r\n");//��ȡIMEI�ţ������Ƿ����NB�豸����˼���Ƚ���Ҫ��
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		}
		my_g_time.m_clock_syn_result = 1;
		clock_cclk_handle(strx);
		Clear_Buffer();

}


void BC95_SendCOAPdata(u8 *len,u8 *data)
{
	printf("AT+NMGS=%s,%s\r\n",len,data);//����COAP����
	Delay(1000);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	if(strx)//����������ȷ��ƽ̨�յ�����
	{
		#ifdef DEBUG_MACRO
			Uart1_SendStr("SEND DATA OK!\r\n");//���ô���1��ӡ���ͳɹ���ʾ
		#endif
	}
	else//�������error һ���ǵ�һ����ƽ̨�������������CDP�������������⡣�û����Բ�ѯ�� AT+NMSTATUS? ��ӡ����ʾ�˽��в鿴
	{
		if(BC95_SendCOAPdata_try(len,data) == FALSE)
		{
			printf("AT+NMSTATUS?\r\n");//�жϵ�ǰģ����ƽ̨֮������ӹ�ϵ��һ��δ������֮ǰ����INIITIALISED������������͵����ݵ�ƽ̨֮����MO_DATA_ENABLED
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"+NMSTATUS:MO_DATA_ENABLED");//�鿴�Ƿ񷵻���������ݣ�
			if(strx)
			{
				#ifdef DEBUG_MACRO
					Uart1_SendStr("\nConnect OK!");  //���������������ģ����´��ٷ����ݾͿ�����
				#endif
			}
		}
	}    

	//�ж��Ƿ����·�����
	strx=strstr((const char*)RxBuffer,(const char*)"+NNMI");//����+NSONMI:���������յ�UDP���������ص�����
	if(strx)
	{
		g_bus.receive_flag = 1;
//		printf_string("\nreceive_flag1");
	}
	
	Clear_Buffer();	
	
}

//���·���
u8 BC95_SendCOAPdata_try(u8 *len,u8 *data)
{
 	u8 count=0;
	u8 result = FALSE;
	for(count = 0;count <2;count++)
	{
		#ifdef DEBUG_MACRO
			Uart1_SendStr("retry\t");//���ô���1��ӡ���ͳɹ���ʾ
			UART1_send_byte((char)(count+1+'0'));
		#endif
		
		printf("AT+NMGS=%s,%s\r\n",len,data);//����COAP����
		Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		if(strx)//����������ȷ��ƽ̨�յ�����
		{
			#ifdef DEBUG_MACRO
				printf_string("\nSEND DATA OK!");//���ô���1��ӡ���ͳɹ���ʾ
			#endif
			
//			Clear_Buffer();
			result = TRUE;
			break;
		}

		//�ж��Ƿ����·�����+
		strx=strstr((const char*)RxBuffer,(const char*)"+NNMI");//����+NSONMI:���������յ�UDP���������ص�����
		if(strx)
		{
			g_bus.receive_flag = 1;
//			printf_string("\nreceive_flag2");
		}
		
		Clear_Buffer();
	}
	return result;
}


//����ƽ̨�˶��豸���ݵ��·�
void BC95_RECCOAPData(void)
{
	char i;
    strx=strstr((const char*)RxBuffer,(const char*)"+NNMI");//����+NSONMI:���������յ�UDP���������ص�����
	if(strx != NULL)
	{
		g_bus.receive_flag = 1;
		
		#ifdef DEBUG_MACRO
//			printf_string("\nreceive_flag3");
//			Uart1_SendStr("RECEIVE +NNMI OK!\r\n");
		#endif
	}
//	else
//	{
//		Uart1_SendStr("RECEIVE NO +NNMI!\r\n");
//	}

	Clear_Buffer();	
}





