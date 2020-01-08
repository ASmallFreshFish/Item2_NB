# Item2_NB
### <span id="99">目录</span>
###### 1. [time_190424_1203](#1) 
###### 2. [time_190424_1718](#2) 
###### 3. [time_190426_1459](#3) 
###### 4. [time_190505_1706](#4) 
###### 5. [time_190507_1045](#5) 
###### 6. [time_190603_1053](#6) 
###### 7. [time_190625_1810](#7)   
###### 8. [time_190626_1853](#8) 
###### 9. [time_190708_1526](#9) 
###### 10. [time_190708_1810](#10) 
###### 11. [time_190930_1738](#11) 
###### 12. [time_191107_2047](#12) 
###### 13. [time_191219_0945_release](#13) 
###### 14. [time_200108_1807](#14) 
---
---

## <span id="1">*time_190424_1203*</span>
###### [返回目录](#99) 
##### 1.NB_IOT_Gesture：NO !
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM:
    完成检测功能，
    添加看门狗功能！
##### 3.NB_IOT_Gesture_PAJ7620_IIC：
    完成检测功能，
    添加看门狗功能！
##### 4.NB_IOT_Key：NO !
##### 5.NB_IOT_Pressure：NO !
##### 6.NB_IOT_Strain_Pressure：NO !

---
---
## <span id="2">*time_190424_1718*</span>
###### [返回目录](#99) 
##### 1.NB_IOT_Gesture：NO !
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM:
    优化上报数据功能，
    看门狗溢出时间修改为10s！
##### 3.NB_IOT_Gesture_PAJ7620_IIC：
    优化上报数据功能，
    看门狗溢出时间修改为10s！
##### 4.NB_IOT_Key：NO !
##### 5.NB_IOT_Pressure：NO !
##### 6.NB_IOT_Strain_Pressure：
    优化上报数据功能，
    添加看门狗功能，溢出时间为10s！

---
---
## <span id="3">*time_190426_1459*</span>
###### [返回目录](#99) 
##### 1.NB_IOT_Gesture：NO !
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM:
    修改NONE作为一种手势，作为触发信号！
##### 3.NB_IOT_Gesture_PAJ7620_IIC：NO !
##### 4.NB_IOT_Key：NO !
##### 5.NB_IOT_Pressure：NO !
##### 6.NB_IOT_Strain_Pressure：NO !

---
---
## <span id="5">*time_190507_1045*</span>
###### [返回目录](#99) 
##### 1.NB_IOT_Gesture：    NO !
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 3.NB_IOT_Gesture_PAJ7620_IIC：    NO !
##### 4.NB_IOT_Key：    NO !
##### 5.NB_IOT_Pressure：   NO !
##### 6.NB_IOT_Strain_Pressure：    NO !
##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：
    检测距离设定为5cm，建议实际手位置在4cm以内，无遮挡时大于6cm！

---
---
## <span id="8">*time_190626_1853*</span>
###### [返回目录](#99) 
##### 1.NB_IOT_Gesture：    NO !
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 3.NB_IOT_Gesture_PAJ7620_IIC：    NO !
##### 4.NB_IOT_Key：    NO !
##### 5.NB_IOT_Pressure：   NO !
##### 6.NB_IOT_Strain_Pressure：    NO !
##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：   NO!

---
---
## <span id="9">*time_190708_1526*</span>
###### [返回目录](#99) 
##### 1.NB_IOT_Gesture：    NO !
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 3.NB_IOT_Gesture_PAJ7620_IIC：    NO !
##### 4.NB_IOT_Key：    NO !
##### 5.NB_IOT_Pressure：   NO !
##### 6.NB_IOT_Strain_Pressure：    
    增加心跳。
##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：   NO!


---
---
## <span id="11">*time_190930_1738*</span>
###### [返回目录](#99) 
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 6.NB_IOT_Strain_Pressure：    
    1、增加3kg称重传感器对应的调整系数。
    2、将配置放到data_setting.h文件中。
    3、组网过程，蓝灯0.5s闪烁，组网成功，红灯2s闪烁。
##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：   NO!

---
---
## <span id="12">*time_191107_2047*</span>
###### [返回目录](#99) 
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 6.NB_IOT_Strain_Pressure：    
    1、帧格式中增加1个字段（16位），用于上报重量的变化量。
    2、增加增重的上报事件。
    3、优化打印日志函数。
##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：   NO!

---
---
## <span id="13">*time_191219_0945_release*</span>
###### [返回目录](#99) 
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 6.NB_IOT_Strain_Pressure：    
    1、上报帧中增加时间戳；
    2、集成2个压力传感器，上报两个传感器的值；
    3、增加上报次数，次数可以设置；（默认上报3次）
    4、增加下行可设置阈值，需要调试eeprom驱动；（默认2.5g）
    5、增加版本号码1.1。
    6、优化重量判断机制，进行了多次确认。

##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：   NO!

---
---
## <span id="14">*time_200108_1807*</span>
###### [返回目录](#99) 
##### 2.NB_IOT_Gesture_PADS9960_UART_PWM：   NO!
##### 6.NB_IOT_Strain_Pressure：    
    1、上报过程修改，增加了上报时间间隔，且保存到eeprom；
    2、修改时间戳bug，时间戳改成utc时间。
    3、版本号升至1.2
##### 7.NB_IOT_LaserRanging_VL53L0X_IIC：   NO!