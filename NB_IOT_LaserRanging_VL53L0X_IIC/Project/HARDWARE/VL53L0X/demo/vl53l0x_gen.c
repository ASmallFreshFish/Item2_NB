#include "vl53l0x_gen.h"
#include "usart.h"
#include "delay.h"

VL53L0X_RangingMeasurementData_t vl53l0x_data;//测距测量结构体
extern vu16 ranging_data;//保存测距数据

//VL53L0X 测量模式配置
//dev:设备I2C参数结构体
//mode: 0:默认;1:高精度;2:长距离;3:高速
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode)
{
	
	 VL53L0X_Error status = VL53L0X_ERROR_NONE;
	 uint8_t VhvSettings;
	 uint8_t PhaseCal;
	 uint32_t refSpadCount;
	 uint8_t isApertureSpads;
	
	 vl53l0x_reset(dev);//复位vl53l0x(频繁切换工作模式容易导致采集距离数据不准，需加上这一代码)
	 status = VL53L0X_StaticInit(dev);

	status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal);//Ref参考校准
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads);//执行参考SPAD管理
	if(status!=VL53L0X_ERROR_NONE) goto error;
    delay_ms(2);	
		
	status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);//使能单次测量模式
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,1);//使能SIGMA范围检查
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//使能信号速率范围检查
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//设定SIGMA范围
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//设定信号速率范围范围
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);//设定完整测距最长时间
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod);//设定VCSEL脉冲周期
	if(status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod);//设定VCSEL脉冲周期范围

	error://错误信息
	if(status!=VL53L0X_ERROR_NONE)
	{
		Uart1_SendStr("set mode error!!!\r\n");
		return status;
	}
	return status;
	
}	

//VL53L0X 单次距离测量函数
//dev:设备I2C参数结构体
//pdata:保存测量数据结构体
VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *dev,VL53L0X_RangingMeasurementData_t *pdata,char *buf)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t RangeStatus;
	
	status = VL53L0X_PerformSingleRangingMeasurement(dev, pdata);//执行单次测距并获取测距测量数据
	if(status !=VL53L0X_ERROR_NONE) 
		return status;
   
	RangeStatus = pdata->RangeStatus;//获取当前测量状态
    memset(buf,0x00,VL53L0X_MAX_STRING_LENGTH);
	VL53L0X_GetRangeStatusString(RangeStatus,buf);//根据测量状态读取状态字符串
	
	ranging_data = pdata->RangeMilliMeter;//保存最近一次测距测量数据
	
    return status;
}

void vl53l0x_general_start_mode(VL53L0X_Dev_t *dev,u8 mode)
{
	static char buf[VL53L0X_MAX_STRING_LENGTH];//测试模式字符串字符缓冲区
	VL53L0X_Error Status=VL53L0X_ERROR_NONE;//工作状态
	u8 temp_str[5];
	
	if(Status==VL53L0X_ERROR_NONE)
	{
		Status = vl53l0x_start_single_test(dev,&vl53l0x_data,buf);//执行一次测量

		hex_to_str(&ranging_data,temp_str,2);
		Uart1_SendStr(temp_str);
		UART1_send_byte('\n');
	}
}	

void vl53l0x_general_start(VL53L0X_Dev_t *dev)
{
	static char buf[VL53L0X_MAX_STRING_LENGTH];//测试模式字符串字符缓冲区
	VL53L0X_Error Status=VL53L0X_ERROR_NONE;//工作状态
	u8 temp_str[5];
	
	if(Status==VL53L0X_ERROR_NONE)
	{
		Status = vl53l0x_start_single_test(dev,&vl53l0x_data,buf);//执行一次测量

		hex_to_str(&ranging_data,temp_str,2);
		Uart1_SendStr(temp_str);
		UART1_send_byte('\n');
	}
}

