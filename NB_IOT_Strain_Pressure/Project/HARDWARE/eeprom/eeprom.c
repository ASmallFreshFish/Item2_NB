#include "head_include.h"

eeprom_data_type g_eeprom[]={{0x0,1},{0x64,1},{0xC8,1},{0x12C,1},{0x190,1}};

 /*
 //////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif	

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址，（zjw）此处扇区应该就是页码
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<EEPROM_BASE_ADDR||(WriteAddr>=(EEPROM_BASE_ADDR+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-EEPROM_BASE_ADDR;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+EEPROM_BASE_ADDR,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+EEPROM_BASE_ADDR);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+EEPROM_BASE_ADDR,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain;	//写地址偏移	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}

*/



/********************************************************************************
可以按字节和字读取。
********************************************************************************/
//按字节读出，数据在EEPROM里按4字节对齐存储，
void EEPROM_ReadBytes(u16 Addr,u8 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	while(Length--){
		*Buffer++=*wAddr++;
	}	
}

//按字读出，数据在EEPROM里按4字节对齐存储，
void EEPROM_ReadWords(u16 Addr,u16 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	while(Length--){
		*Buffer++=*wAddr++;
	}	
}

//按字节写入，数据在EEPROM里按4字节对齐存储，
void EEPROM_WriteBytes(u16 Addr,u8 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	DIS_INT
	FLASH->PEKEYR=PEKEY1;				//unlock
	FLASH->PEKEYR=PEKEY2;
	while(FLASH->PECR&FLASH_PECR_PELOCK);
	FLASH->PECR|=FLASH_PECR_FTDW;		//not fast write
	while(Length--){
		*wAddr++=*Buffer++;
		while(FLASH->SR&FLASH_SR_BSY);
	}
	FLASH->PECR|=FLASH_PECR_PELOCK;
	EN_INT
}

//按字写入，数据在EEPROM里按4字节对齐存储，
void EEPROM_WriteWords(u16 Addr,u16 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	DIS_INT
	FLASH->PEKEYR=PEKEY1;				//unlock
	FLASH->PEKEYR=PEKEY2;
	while(FLASH->PECR&FLASH_PECR_PELOCK);
	FLASH->PECR|=FLASH_PECR_FTDW;		//not fast write
	while(Length--){
		*wAddr++=*Buffer++;
		while(FLASH->SR&FLASH_SR_BSY);
	}
	FLASH->PECR|=FLASH_PECR_PELOCK;
	EN_INT
}


//以ID为偏移，每次双字节清除本ID的100个字节
//eg：eeprom_clear(0x0,0,25);
void eeprom_clear(u16 Addr,u32 data,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	DIS_INT
	FLASH->PEKEYR=PEKEY1;				//unlock
	FLASH->PEKEYR=PEKEY2;
	while(FLASH->PECR&FLASH_PECR_PELOCK);
	FLASH->PECR|=FLASH_PECR_FTDW;		//not fast write
	while(Length--){
		*wAddr++=data;
		while(FLASH->SR&FLASH_SR_BSY);
	}
	FLASH->PECR|=FLASH_PECR_PELOCK;
	EN_INT
}

//按字写入，数据在EEPROM里按4字节对齐存储，
void eeprom_write(u16 addr,u16 *buffer,u16 length)
{
	u8 i;
	u16 *p_buf=buffer;
	u16 len=length;
	u32 *w_addr=(u32 *)(EEPROM_BASE_ADDR+addr);

	DIS_INT
	FLASH->PEKEYR=PEKEY1;				//unlock
	FLASH->PEKEYR=PEKEY2;
	while(FLASH->PECR&FLASH_PECR_PELOCK);
	FLASH->PECR|=FLASH_PECR_FTDW;		//not fast write
	for(i=0;i<3;i++)
	{
		p_buf=buffer;
		len=length;
		while(len--)
		{
			*w_addr++=*p_buf++;
			while(FLASH->SR&FLASH_SR_BSY);
		}
	}
	
	FLASH->PECR|=FLASH_PECR_PELOCK;
	EN_INT
}

u8 eeprom_read(u16 addr,u16 *buffer,u16 length)
{
	u8 result=TRUE;
	u8 i;
	u16 data1,data2,data3;
	u32 *w_addr=(u32 *)(EEPROM_BASE_ADDR+addr);
	
	for(i = 0; i < length; i ++)
	{
		data1  = *(u16 *)(w_addr);
		data2  = *(u16 *)(w_addr + length);
		data3  = *(u16 *)(w_addr + (u16)(length<<1));

		if(data1 == data2)
		{
			*buffer++ = data1;
		}
		else if(data1 == data3)
		{
			*buffer++ = data1;
		}
		else if(data2 == data3)
		{
			*buffer++ = data2;
		}
		else
		{
			*buffer++ = 0;
			result = FALSE;
		}

		w_addr++;
	}
	return result;
}


/******************************************
EEPROM应用函数
******************************************/
//上电读一下eeprom，值为0，把默认值写进去；值非零且和默认值不一样，以eeprom值修改默认值。
void eeprom_clear_variable()
{	
	eeprom_clear((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,0,BYTES_EACH_VARIABLE);
	eeprom_clear((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,0,BYTES_EACH_VARIABLE);
}

//上电读一下eeprom，值为0，把默认值写进去；值非零且和默认值不一样，以eeprom值修改默认值。
void eeprom_init()
{	
	u16 read_buf[5];

	//初始化重量变化量阈值eeprom，数据为0，则把初始化的默认值写入，非0，则改变默认值
	if(eeprom_read((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,read_buf,
		(u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].length))
	{
		
		if(read_buf[0] == 0)
		{
			eeprom_clear((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,0,BYTES_EACH_VARIABLE);
			eeprom_write((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,&g_weight.change_threshold,
				(u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].length);
		}
		else if(read_buf[0] != g_weight.change_threshold)
		{
			g_weight.change_threshold =read_buf[0];
		}
#ifdef DEBUG_MACRO_INIT
		printf_string("\neeprom_init,weight:");
		printf_u16_hexStr(read_buf[0]);
		printf_string("\t");
		printf_u16_hexStr(g_weight.change_threshold);
#endif

	}

	//初始化业务上报次数eeprom，数据为0，则把初始化的默认值写入，非0，则改变默认值
	if(eeprom_read((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,read_buf,
		(u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].length))
	{
		
		if(read_buf[0] == 0)
		{
			eeprom_clear((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,0,BYTES_EACH_VARIABLE);
			eeprom_write((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,(u16 *)(&g_bus.report_times),
				(u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].length);
		}
		else if(read_buf[0] != g_bus.report_times)
		{
			g_bus.report_times =read_buf[0];
		}
#ifdef DEBUG_MACRO_INIT
			printf_string("\t upload:");
			printf_u16_hexStr(read_buf[0]);
			printf_string("\t");
			printf_u16_hexStr(g_bus.report_times);
#endif

	}

	//初始化重量调节因子eeprom
	if(eeprom_read((u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].offset_addr,read_buf,
		(u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].length))
	{
		if(read_buf[0] <= 5)
		{
			eeprom_clear((u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].offset_addr,0,BYTES_EACH_VARIABLE);
			eeprom_write((u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].offset_addr,(u16 *)(&g_weight.factor100),
				(u16)g_eeprom[EEP_ID_W_FACTOR100_VALUE].length);
		}
		else if(read_buf[0] != g_weight.factor100)
		{
			g_weight.factor100 =read_buf[0];
		}
#ifdef DEBUG_MACRO_INIT
		printf_string("\t factor:");
		printf_u16_hexStr(read_buf[0]);
		printf_string("\t");
		printf_u16_hexStr(g_weight.factor100);
#endif
	}

	//初始化重量上报时间间隔eeprom，数据为0，则把初始化的默认值写入，非0，则改变默认值
	if(eeprom_read((u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].offset_addr,read_buf,
		(u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].length))
	{
		if(read_buf[0] == 0)
		{
			eeprom_clear((u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].offset_addr,0,BYTES_EACH_VARIABLE);
			eeprom_write((u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].offset_addr,(u16 *)(&g_bus.report_interval),
				(u16)g_eeprom[EEP_ID_W_UPLOAD_INTERVAL].length);
		}
		else if(read_buf[0] != g_weight.factor100)
		{
			g_bus.report_interval =read_buf[0];
		}
#ifdef DEBUG_MACRO_INIT
		printf_string("\t report_interval:");
		printf_u16_hexStr(read_buf[0]);
		printf_string("\t");
		printf_u16_hexStr(g_bus.report_interval);
#endif
	}
}

void eld_eeprom_init()
{	
	u16 read_buf[5];
	if(eeprom_read((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,read_buf,
		(u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].length))
	{
		printf_string("\neeprom_init:");
		printf_u16_hexStr(read_buf[0]);
		if(read_buf[0] != g_weight.change_threshold)
		{
			eeprom_clear((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,0,BYTES_EACH_VARIABLE);
			eeprom_write((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,&g_weight.change_threshold,
				(u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].length);
		}
	}

	if(eeprom_read((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,read_buf,
		(u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].length))
	{
		printf_string("\t");
		printf_u16_hexStr(read_buf[0]);
		if(read_buf[0] != g_bus.report_times)
		{
			eeprom_clear((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,0,BYTES_EACH_VARIABLE);
			eeprom_write((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,(u16 *)(&g_bus.report_times),
				(u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].length);
		}
	}
}


void test(void)
{	
	u8 i;
	u16 eerpom_r[10];
	u16 eerpom_w[5]={1,2,65535,65534,65533}; 
//	eeprom_clear(g_eeprom[EEP_ID_W_CHANGE_THRESHOLD],0,25);

//	eeprom_write(g_eeprom[EEP_ID_W_CHANGE_THRESHOLD],(u16*)eerpom_w,5);
	delay_ms(500);
	if(eeprom_read((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,eerpom_r,5))
	{
		for(i=0;i<5;i++)
		{
			printf_u16_hexStr(eerpom_r[i]);
			printf_string("\t");
		}
	}
	
	printf_string("\nd\n");
}

void old_test()
{
	u8 eeprom_r1[10]={0};
	u8 eeprom_r2[10]={0};

	u16 eerpom_w[5]={1,2,65535,65534,65533};
	memset(eeprom_r1,0,sizeof(eeprom_r1));
	memset(eeprom_r2,0,sizeof(eeprom_r2));
		
	printf_string("\na:");
	EEPROM_WriteWords((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,(u16*)eerpom_w,10);
	printf_string("\tb:");
	delay_ms(500);
	EEPROM_ReadWords((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,(u16 *)eeprom_r2,10);
	printf_string(eeprom_r2);
	printf_string("\nd\n");

	delay_ms(500);
	printf_string("\na:");
	EEPROM_WriteBytes((u16)g_eeprom[EEPROM_ID_RESERVED4].offset_addr,(u8 *)eerpom_w,10);
	printf_string("\tb:");
	delay_ms(500);
	EEPROM_ReadBytes((u16)g_eeprom[EEPROM_ID_RESERVED4].offset_addr,(u8 *)eeprom_r1,10);
	printf_string(eeprom_r1);
}

