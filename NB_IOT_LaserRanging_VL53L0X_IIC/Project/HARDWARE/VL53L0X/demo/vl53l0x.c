#include "vl53l0x.h"
#include "usart.h"

VL53L0X_Dev_t vl53l0x_dev;//�豸I2C���ݲ���
VL53L0X_DeviceInfo_t vl53l0x_dev_info;//�豸ID�汾��Ϣ
vu16 ranging_data = 0;//����������
g_ranging_type g_ranging;


//VL53L0X������ģʽ����
//0��Ĭ��;1:�߾���;2:������;3:����
mode_data Mode_data[4]=
{
    {(FixPoint1616_t)(0.25*65536), 
	 (FixPoint1616_t)(18*65536),
	 33000,
	 14,
	 10},//Ĭ��
		
	{(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(18*65536),
	 200000, 
	 14,
	 10},//�߾���
		
    {(FixPoint1616_t)(0.1*65536) ,
	 (FixPoint1616_t)(60*65536),
	 33000,
	 18,
	 14},//������
	
    {(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(32*65536),
	 20000,
	 14,
	 10},//����
};


//Ƭѡ��gpio����
void vl53l0x_init_xsh_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//��ʼ��vl53l0x
//dev:�豸I2C�����ṹ��
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev)
{
	raning_mode_type mode =HIGH_ACCURACY;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_Dev_t *pMyDevice = dev;
		
	pMyDevice->I2cDevAddr = VL53L0X_Addr;//I2C��ַ(�ϵ�Ĭ��0x52)
	pMyDevice->comms_type = 1;           //I2Cͨ��ģʽ
	pMyDevice->comms_speed_khz = 400;    //I2Cͨ������

	vl53l0x_init_xsh_gpio();
	VL53L0X_i2c_init();//��ʼ��IIC����
	
	VL53L0X_Xshut=0;	//ʧ��VL53L0X
	delay_ms(30);
	VL53L0X_Xshut=1;	//ʹ��VL53L0X,�ô��������ڹ���
	delay_ms(30);
	
    Status = vl53l0x_Addr_set(pMyDevice,0x54);		//����VL53L0X������I2C��ַ
    if(Status!=VL53L0X_ERROR_NONE) 
		return Status;
	Status = VL53L0X_DataInit(pMyDevice);	//�豸��ʼ��
	if(Status!=VL53L0X_ERROR_NONE) 
		return Status;
	delay_ms(2);
	Status = VL53L0X_GetDeviceInfo(pMyDevice,&vl53l0x_dev_info);//��ȡ�豸ID��Ϣ
    if(Status!=VL53L0X_ERROR_NONE) 
		return Status;

	vl53l0x_set_mode(&vl53l0x_dev,mode);
	
}

//
void test_vl5310x_init(void)
{
	while(vl53l0x_init(&vl53l0x_dev)) {}				//vl53l0x��ʼ��
//	while(vl53l0x_set_mode(&vl53l0x_dev,mode))	{}	//���þ���ģʽ
	Uart1_SendStr("VL53L0X OK\r\n");
}

//API������Ϣ��ӡ
//Status�����鿴VL53L0X_Error�����Ķ���
void print_pal_error(VL53L0X_Error Status)
{
	char buf[VL53L0X_MAX_STRING_LENGTH];
	VL53L0X_GetPalErrorString(Status,buf);//����Status״̬��ȡ������Ϣ�ַ���
//    printf("API Status: %i : %s\r\n",Status, buf);//��ӡ״̬�ʹ�����Ϣ
}

//ģʽ�ַ�����ʾ
//mode:0-Ĭ��;1-�߾���;2-������;3-����
void mode_string(u8 mode,char *buf)
{
	switch(mode)
	{
		case DEFAULT_MODE: strcpy(buf,"Default");        break;
		case HIGH_ACCURACY: strcpy(buf,"High Accuracy"); break;
		case LONG_RANGE: strcpy(buf,"Long Range");       break;
		case HIGH_SPEED: strcpy(buf,"High Speed");       break;
	}

}

//����VL53L0X�豸I2C��ַ
//dev:�豸I2C�����ṹ��
//newaddr:�豸��I2C��ַ
VL53L0X_Error vl53l0x_Addr_set(VL53L0X_Dev_t *dev,uint8_t newaddr)
{
	uint16_t Id;
	uint8_t FinalAddress;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	u8 sta=0x00;
	
	FinalAddress = newaddr;
	
	if(FinalAddress==dev->I2cDevAddr)//���豸I2C��ַ��ɵ�ַһ��,ֱ���˳�
		return VL53L0X_ERROR_NONE;
	//�ڽ��е�һ���Ĵ�������֮ǰ����I2C��׼ģʽ(400Khz)
	Status = VL53L0X_WrByte(dev,0x88,0x00);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		sta=0x01;//����I2C��׼ģʽ����
		goto set_error;
	}
	//����ʹ��Ĭ�ϵ�0x52��ַ��ȡһ���Ĵ���
	Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		sta=0x02;//��ȡ�Ĵ�������
		goto set_error;
	}
	if(Id == 0xEEAA)
	{
		//�����豸�µ�I2C��ַ
		Status = VL53L0X_SetDeviceAddress(dev,FinalAddress);
		if(Status!=VL53L0X_ERROR_NONE) 
		{
			sta=0x03;//����I2C��ַ����
			goto set_error;
		}
		//�޸Ĳ����ṹ���I2C��ַ
		dev->I2cDevAddr = FinalAddress;
		//����µ�I2C��ַ��д�Ƿ�����
		Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
		if(Status!=VL53L0X_ERROR_NONE) 
		{
			sta=0x04;//��I2C��ַ��д����
			goto set_error;
		}	
	}
	set_error:
	if(Status!=VL53L0X_ERROR_NONE)
	{
	}
	if(sta!=0)
	{
//	  printf("sta:0x%x\r\n",sta);
	}
	return Status;
}

//vl53l0x��λ����
//dev:�豸I2C�����ṹ��
void vl53l0x_reset(VL53L0X_Dev_t *dev)
{
	uint8_t addr;
	addr = dev->I2cDevAddr;//�����豸ԭI2C��ַ
    VL53L0X_Xshut=0;//ʧ��VL53L0X
	delay_ms(30);
	VL53L0X_Xshut=1;//ʹ��VL53L0X,�ô��������ڹ���(I2C��ַ��ָ�Ĭ��0X52)
	delay_ms(30);	
	dev->I2cDevAddr=0x52;
	vl53l0x_Addr_set(dev,addr);//����VL53L0X������ԭ���ϵ�ǰԭI2C��ַ
	VL53L0X_DataInit(dev);	
}

void test_vl5310x_judge(void)
{
	g_ranging.sta=NO_DETECTION;
	if(ranging_data < RANGING_LIMIT_LOW)
	{
		g_ranging.sta = HAVE_DETECTION;
	}
}

//��ຯ������	
void test_vl5310x_handle(void)
{
	vl53l0x_general_start(&vl53l0x_dev);
	test_vl5310x_judge();
//	delay_ms(500);	
	delay_ms(50);

}


