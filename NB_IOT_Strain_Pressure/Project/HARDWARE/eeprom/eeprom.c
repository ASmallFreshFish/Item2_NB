#include "head_include.h"

eeprom_data_type g_eeprom[]={{0x0,1},{0x64,1},{0xC8,1},{0x12C,1},{0x190,1}};

 /*
 //////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif	

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ����zjw���˴�����Ӧ�þ���ҳ��
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<EEPROM_BASE_ADDR||(WriteAddr>=(EEPROM_BASE_ADDR+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-EEPROM_BASE_ADDR;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+EEPROM_BASE_ADDR,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+EEPROM_BASE_ADDR);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+EEPROM_BASE_ADDR,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}

*/



/********************************************************************************
���԰��ֽں��ֶ�ȡ��
********************************************************************************/
//���ֽڶ�����������EEPROM�ﰴ4�ֽڶ���洢��
void EEPROM_ReadBytes(u16 Addr,u8 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	while(Length--){
		*Buffer++=*wAddr++;
	}	
}

//���ֶ�����������EEPROM�ﰴ4�ֽڶ���洢��
void EEPROM_ReadWords(u16 Addr,u16 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	while(Length--){
		*Buffer++=*wAddr++;
	}	
}

//���ֽ�д�룬������EEPROM�ﰴ4�ֽڶ���洢��
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

//����д�룬������EEPROM�ﰴ4�ֽڶ���洢��
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


//��IDΪƫ�ƣ�ÿ��˫�ֽ������ID��100���ֽ�
//eg��eeprom_clear(0x0,0,25);
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

//����д�룬������EEPROM�ﰴ4�ֽڶ���洢��
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
EEPROMӦ�ú���
******************************************/
//�ϵ��һ��eeprom��ֵΪ0����Ĭ��ֵд��ȥ��ֵ�����Һ�Ĭ��ֵ��һ������eepromֵ�޸�Ĭ��ֵ��
void eeprom_clear_variable()
{	
	eeprom_clear((u16)g_eeprom[EEP_ID_W_CHANGE_THRESHOLD].offset_addr,0,BYTES_EACH_VARIABLE);
	eeprom_clear((u16)g_eeprom[EEP_ID_W_UPLOAD_TIMES].offset_addr,0,BYTES_EACH_VARIABLE);
}

//�ϵ��һ��eeprom��ֵΪ0����Ĭ��ֵд��ȥ��ֵ�����Һ�Ĭ��ֵ��һ������eepromֵ�޸�Ĭ��ֵ��
void eeprom_init()
{	
	u16 read_buf[5];

	//��ʼ�������仯����ֵeeprom������Ϊ0����ѳ�ʼ����Ĭ��ֵд�룬��0����ı�Ĭ��ֵ
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

	//��ʼ��ҵ���ϱ�����eeprom������Ϊ0����ѳ�ʼ����Ĭ��ֵд�룬��0����ı�Ĭ��ֵ
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

	//��ʼ��������������eeprom
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

	//��ʼ�������ϱ�ʱ����eeprom������Ϊ0����ѳ�ʼ����Ĭ��ֵд�룬��0����ı�Ĭ��ֵ
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

