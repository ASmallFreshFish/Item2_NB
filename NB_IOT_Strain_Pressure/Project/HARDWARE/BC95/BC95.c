#include "head_include.h"

char *strx,*extstrx;
//注意char*与char[]的区别
//char *imei_str = 0;
char imei_str[16] = {0};
//	char imei_str[16] = {"868246048366603"};


extern char  RxBuffer[100],RxCounter;
BC95 BC95_Status;

void Clear_Buffer(void)//清空缓存
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

void old_Clear_Buffer(void)//清空缓存
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
		//LOG打印
	#ifdef DEBUG_MACRO_INIT
		Uart1_SendStr("\r\n");
		Uart1_SendStr(buf_dest);
	#endif
}

void CDP_Init(void)//COAP服务器配置初始化
{
	printf("AT\r\n"); 
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		Clear_Buffer();	
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("AT\r\n"); 
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
    	BC95_Status.netstatus=1;//闪烁没注册网络
    	
    printf("AT+NBAND?\r\n");//获取频段号
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//返回5  电信  返回8是移动 移动的客户这地方注意一下修改
		Clear_Buffer();	
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("AT+NBAND?\r\n");//获取频段号
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//返回OK
		}
		
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//返46011是电信，移动的要更改
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//返46011是电信，移动的要更改
		}
		
    printf("AT+CFUN=0\r\n");//关闭射频
		Delay(300);	
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		while(strx==NULL)
		{
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		Clear_Buffer();	
		
    printf("AT+NCDP=180.101.147.115,5683\r\n");//配置CDP服务器地址电信平台的，这个COAP协议的IP地址是固定的
		Delay(300);	
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		while(strx==NULL)
		{
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		Clear_Buffer();
		
    printf("AT+NRB\r\n");//对模块进行重启
    	Delay(2000);	//延时2s	
    	
    printf("AT+CIMI\r\n");//获取卡号，确认模块重启正常
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//返46011
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//获取卡号，确认模块重启正常
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//返回OK,说明卡是存在的
		}	
		///这样NCDP的就确认好了,服务器连接设置被确认好.
}
void BC95_Init(void)
{
    unsigned char i;
    printf("AT\r\n"); 
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT\r\n"); 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		BC95_Status.netstatus=1;//闪烁没注册网络
		
    printf("AT+NBAND?\r\n");//获取频段号
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//返回5  电信,返回8是移动。
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+NBAND?\r\n");//获取频段号
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//返回OK
		}
		
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//返46011，电信，移动的是其他数值
		//LOG打印
		#ifdef DEBUG_MACRO_INIT
			Uart1_SendStr("\r\n");
			Uart1_SendStr(strx);
		#endif
		
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//返回OK,说明卡是存在的电信，移动的是其他数值
		    //LOG打印
		    #ifdef DEBUG_MACRO_INIT
				Uart1_SendStr("\r\n");
				Uart1_SendStr(strx);
			#endif
			
		}
		
	printf("AT+CGSN=1\r\n");//获取IMEI号，类似是否存在NB设备的意思，比较重要。
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");

		//保存IMEI号码
		copy_buf(imei_str,strx+6,16);
		
		Clear_Buffer();
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGSN=1\r\n");//获取IMEI号，类似是否存在NB设备的意思，比较重要。
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");
			copy_buf(imei_str,strx+6,16);
		}
		
    printf("AT+CGATT=1\r\n");//激活网络，网络附着 PDP
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返46011
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT=1\r\n");//激活网络 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		
    printf("AT+CGATT?\r\n");//查询激活状态
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT?\r\n");//获取激活状态
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
		}
		
    printf("AT+CSQ\r\n");//查看获取CSQ值
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//返回CSQ
		if(strx)
		{
		    BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//获取CSQ
		    if(BC95_Status.CSQ==99)//说明扫网失败
		    {
		        while(1)
		        {
		            Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
		            Delay(300);
		        }
		    }
		        BC95_Status.netstatus=4;//注网成功
		 }
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CSQ\r\n");//查看获取CSQ值
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
		    if(strx)
		    {
		        BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//获取CSQ
		        if(BC95_Status.CSQ==99)//说明扫网失败
		        {
		            while(1)
		            {
		                Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
		                Delay(300);
		            }
		        }
		     }
		}
    	Clear_Buffer();	
		
    printf("AT+CEREG?\r\n");	//查看网络注册状态
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//返回注册状态
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//返回注册状态
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG?\r\n");//判断运营商
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//返回注册状态
		    extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//返回注册状态
		}
		
    printf("AT+CEREG=1\r\n");	//注册网络
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG=1\r\n");//
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		
    printf("AT+NMSTATUS?\r\n");//判断当前模块与平台之间的连接关系，一般未发数据之前都是INIITIALISED，如果正常发送到数据到平台之后是MO_DATA_ENABLED
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NMSTATUS:INIITIALISED");//判断是否返回初始化状态。用户可以在此做一些相关的判断
//		if(strx)//如果是的，就点亮LED灯
//			LEDMCU_RED_OPEN;

//	printf("AT+NNMI=0\r\n");  //配置模块，不接收数据
//	printf("AT+NNMI=1\r\n");//配置模块，发送数据完成后，自动收到下行数据直接打印到串口
	printf("AT+NNMI=2\r\n");  //配置模块，发送数据完成后，通过AT+NMGR获取数据
    
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"OK");//
	    for(i=0;i<100;i++)
	    	RxBuffer[i]=0;	
	    Clear_Buffer();    

	//同步时间
	printf("AT+CCLK?\r\n");//同步基站时间
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CCLK?\r\n");//获取IMEI号，类似是否存在NB设备的意思，比较重要。
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
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT\r\n"); 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		BC95_Status.netstatus=1;//闪烁没注册网络
		
    printf("AT+NBAND?\r\n");//获取频段号
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//返回5  电信,返回8是移动。
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+NBAND?\r\n");//获取频段号
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//返回OK
		}
		
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"46011");//返46011，电信，移动的是其他数值
		//LOG打印
		#ifdef DEBUG_MACRO_INIT
			Uart1_SendStr("\r\n");
			Uart1_SendStr(strx);
		#endif
		
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"46011");//返回OK,说明卡是存在的电信，移动的是其他数值
		    //LOG打印
		    #ifdef DEBUG_MACRO_INIT
				Uart1_SendStr("\r\n");
				Uart1_SendStr(strx);
			#endif
			
		}
		
	printf("AT+CGSN=1\r\n");//获取IMEI号，类似是否存在NB设备的意思，比较重要。
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");

		//保存IMEI号码
		copy_buf(imei_str,strx+6,16);
		
		Clear_Buffer();
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGSN=1\r\n");//获取IMEI号，类似是否存在NB设备的意思，比较重要。
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN");
			copy_buf(imei_str,strx+6,16);
		}
		
    printf("AT+CGATT=1\r\n");//激活网络，PDP
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返46011
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT=1\r\n");//激活网络 
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		
    printf("AT+CGATT?\r\n");//查询激活状态
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
		Clear_Buffer();	
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CGATT?\r\n");//获取激活状态
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
		}
		
    printf("AT+CSQ\r\n");//查看获取CSQ值
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//返回CSQ
		if(strx)
		{
		    BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//获取CSQ
		    if(BC95_Status.CSQ==99)//说明扫网失败
		    {
		        while(1)
		        {
		            Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
		            Delay(300);
		        }
		    }
		        BC95_Status.netstatus=4;//注网成功
		 }
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CSQ\r\n");//查看获取CSQ值
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
		    if(strx)
		    {
		        BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//获取CSQ
		        if(BC95_Status.CSQ==99)//说明扫网失败
		        {
		            while(1)
		            {
		                Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
		                Delay(300);
		            }
		        }
		     }
		}
    	Clear_Buffer();	
		
    printf("AT+CEREG?\r\n");
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//返回注册状态
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//返回注册状态
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG?\r\n");//判断运营商
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//返回注册状态
		    extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//返回注册状态
		}
		
    printf("AT+CEREG=1\r\n");
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		Clear_Buffer();	
		while(strx==NULL&&extstrx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CEREG=1\r\n");//
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		
    printf("AT+NMSTATUS?\r\n");//判断当前模块与平台之间的连接关系，一般未发数据之前都是INIITIALISED，如果正常发送到数据到平台之后是MO_DATA_ENABLED
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+NMSTATUS:INIITIALISED");//判断是否返回初始化状态。用户可以在此做一些相关的判断
//		if(strx)//如果是的，就点亮LED灯
//			LEDMCU_RED_OPEN;
		
    printf("AT+NNMI=1\r\n");//配置模块收到下行数据直接打印到串口
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"OK");//
	    for(i=0;i<100;i++)
	    	RxBuffer[i]=0;	
	    Clear_Buffer();    

	//同步时间
	printf("AT+CCLK?\r\n");//同步基站时间
	    Delay(300);
	    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		while(strx==NULL)
		{
		    Clear_Buffer();	
		    printf("AT+CCLK?\r\n");//获取IMEI号，类似是否存在NB设备的意思，比较重要。
		    Delay(300);
		    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK");
		}
		my_g_time.m_clock_syn_result = 1;
		clock_cclk_handle(strx);
		Clear_Buffer();

}


void BC95_SendCOAPdata(u8 *len,u8 *data)
{
	printf("AT+NMGS=%s,%s\r\n",len,data);//发送COAP数据
	Delay(1000);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
	if(strx)//表明发送正确，平台收到数据
	{
		#ifdef DEBUG_MACRO
			Uart1_SendStr("SEND DATA OK!\r\n");//就让串口1打印发送成功提示
		#endif
	}
	else//如果返回error 一般是第一次与平台握手问题或者是CDP服务器配置问题。用户可以查询下 AT+NMSTATUS? 打印到显示端进行查看
	{
		if(BC95_SendCOAPdata_try(len,data) == FALSE)
		{
			printf("AT+NMSTATUS?\r\n");//判断当前模块与平台之间的连接关系，一般未发数据之前都是INIITIALISED，如果正常发送到数据到平台之后是MO_DATA_ENABLED
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"+NMSTATUS:MO_DATA_ENABLED");//查看是否返回是这个数据，
			if(strx)
			{
				#ifdef DEBUG_MACRO
					Uart1_SendStr("\nConnect OK!");  //表明连接上正常的，待下次再发数据就可以了
				#endif
			}
		}
	}    

	//判断是否有下发数据
	strx=strstr((const char*)RxBuffer,(const char*)"+NNMI");//返回+NSONMI:，表明接收到UDP服务器发回的数据
	if(strx)
	{
		g_bus.receive_flag = 1;
//		printf_string("\nreceive_flag1");
	}
	
	Clear_Buffer();	
	
}

//重新发送
u8 BC95_SendCOAPdata_try(u8 *len,u8 *data)
{
 	u8 count=0;
	u8 result = FALSE;
	for(count = 0;count <2;count++)
	{
		#ifdef DEBUG_MACRO
			Uart1_SendStr("retry\t");//就让串口1打印发送成功提示
			UART1_send_byte((char)(count+1+'0'));
		#endif
		
		printf("AT+NMGS=%s,%s\r\n",len,data);//发送COAP数据
		Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		if(strx)//表明发送正确，平台收到数据
		{
			#ifdef DEBUG_MACRO
				printf_string("\nSEND DATA OK!");//就让串口1打印发送成功提示
			#endif
			
//			Clear_Buffer();
			result = TRUE;
			break;
		}

		//判断是否有下发数据+
		strx=strstr((const char*)RxBuffer,(const char*)"+NNMI");//返回+NSONMI:，表明接收到UDP服务器发回的数据
		if(strx)
		{
			g_bus.receive_flag = 1;
//			printf_string("\nreceive_flag2");
		}
		
		Clear_Buffer();
	}
	return result;
}


//接收平台端对设备数据的下发
void BC95_RECCOAPData(void)
{
	char i;
    strx=strstr((const char*)RxBuffer,(const char*)"+NNMI");//返回+NSONMI:，表明接收到UDP服务器发回的数据
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





