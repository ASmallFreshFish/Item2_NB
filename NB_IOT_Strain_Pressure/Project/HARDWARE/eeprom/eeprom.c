#include "eeprom.h"

#include "delay.h"
//#include "usart.h"
 
 /*
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





#define PEKEY1	0x89ABCDEF		//FLASH_PEKEYR
#define PEKEY2	0x02030405		//FLASH_PEKEYR

#define EN_INT      	__enable_irq();		//ϵͳ��ȫ���ж�
#define DIS_INT     	__disable_irq();	//ϵͳ��ȫ���ж�


/********************************************************************************/
/*------------------------------------------------------------
 Func: EEPROM���ݰ��ֽڶ���
 Note:
-------------------------------------------------------------*/
void EEPROM_ReadBytes(u16 Addr,u8 *Buffer,u16 Length)
{
	u8 *wAddr;
	wAddr=(u8 *)(EEPROM_BASE_ADDR+Addr);
	while(Length--){
		*Buffer++=*wAddr++;
	}	
}


/*------------------------------------------------------------
 Func: EEPROM���ݶ���
 Note:
-------------------------------------------------------------*/
void EEPROM_ReadWords(u16 Addr,u16 *Buffer,u16 Length)
{
	u32 *wAddr;
	wAddr=(u32 *)(EEPROM_BASE_ADDR+Addr);
	while(Length--){
		*Buffer++=*wAddr++;
	}	
}




/*------------------------------------------------------------
 Func: EEPROM���ݰ��ֽ�д��
 Note:
-------------------------------------------------------------*/
void EEPROM_WriteBytes(u16 Addr,u8 *Buffer,u16 Length)
{
	u8 *wAddr;
	wAddr=(u8 *)(EEPROM_BASE_ADDR+Addr);
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

/*------------------------------------------------------------
 Func: EEPROM���ݰ���д��
 Note: �ֵ�������
-------------------------------------------------------------*/
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



u8 eeprom_r[100]={0};
u8 eerpom_w[]={"zhou jian wen !"}; 
u8 eerpom_w1[100]={0}; 
#define SIZE sizeof(eerpom_w)


void test(void)
{
//	memset(eeprom_r,0,2048*sizeof(eeprom_r));
//	EEPROM_ReadWords(0,eeprom_r,100);

	Uart1_SendStr("\na");
//	EEPROM_WriteBytes(0,eerpom_w,SIZE);
	EEPROM_WriteWords(0x0,(u16*)eerpom_w,SIZE);


	Uart1_SendStr("b");
	delay_ms(1000);

//	EEPROM_ReadBytes(0,eeprom_r,SIZE);
//	Uart1_SendStr("c");

	EEPROM_ReadWords(0,(u16 *)eeprom_r,SIZE);
	Uart1_SendStr("d\n");
	
//	EEPROM_ReadWords(0,(u16*)eeprom_r,SIZE);
}


































