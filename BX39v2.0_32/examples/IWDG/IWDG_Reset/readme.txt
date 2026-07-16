1、功能说明
	IWDG复位功能

2、使用环境

	软件开发环境：KEIL MDK-ARM V5.25
	         IAR EWARM 8.50.1
    硬件环境：基于评估板N32G003F5S7-STB V1.0开发

3、使用说明
	
	系统配置；
		1、IWDG时钟源：LSI/128
	    2、超时时间值：16s (4ms * 4080)
        3、指示灯：LED1(PA6) LED2(PA7)

	测试步骤及现象：
	    1、在 KEIL 下编译后，将其编程到评估板。上电后，串口打印并喂狗三次，指示灯LED2不断闪烁。这意味着IWDG正常喂狗，代码正常运行
        2、三次喂狗后不再喂狗，大约16s后IWDG复位

4、注意事项
	1、如果通过烧录器仿真，需要开启DBG_Peripheral_ON(PWR_DBG_IWDG);


1. Function description
   IWDG reset function.

2. Use environment

	Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G003F5S7-STB V1.0

3. Instructions for use
	
	System Configuration：
		1. IWDG clock source: LSI/128
        2. Timeout value: 16s (4ms * 4080)
        3. Indicator light: LED1(PA6) LED2(PA7)

	Instructions:
	1. After compiling under KEIL, program it to the evaluation board. After power-on, the serial port prints and 
	   feeds the dog three times, and the indicator LED2 keeps flashing. It means that IWDG feeds the dog normally and the code runs normally.
	2. If the dog is not fed after three feedings, about 16s the IWDG resets.

4. Matters needing attention
      If you want to suspend IWDG during debugging, you need to turn on DBG_Peripheral_ON(PWR_DBG_IWDG);