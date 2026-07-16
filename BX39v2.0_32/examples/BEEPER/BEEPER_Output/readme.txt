1、功能说明
    此例程展示蜂鸣器模块的用法。
    
2、使用环境

    软件开发环境：KEIL MDK-ARM 5.34.0.0
                   IAR EWARM 8.50.1
    硬件环境：    N32G003F5S7-STB V1.0

3、使用说明
    1、系统时钟：48MHz
    2、Beeper时钟源：LSI 32kHz
    3、输出端口：
	BEEPER_GPIO      PA14   AF_PP  
	BEEPER_N_GPIO  PB0     AF_PP

    4、测试步骤与现象
       a，编译下载代码复位运行
       b，用示波器查看波形，一开始BEEPER_GPIO和BEEPER_N_GPIO输出互补波形，
            一段时间后BEEPER_N_GPIO输出关闭，最后两路输出均关闭  
	
4、注意事项
     无

1. Function description
    This example shows the usage of the buzzer module.
    
2. Use environment
    Software  environment: KEIL MDK-ARM 5.34.0.0
    Hardware environment: N32G003F5S7-STB V1.0

3. Instructions for use
     1. System clock: 48MHz
     2. Beeper clock source: LSI 32kHz
     3. Output port：
	BEEPER_GPIO      PA14   AF_PP  
	BEEPER_N_GPIO  PB0     AF_PP

     4. Test steps and phenomenon
        a. Compile and download the code to reset and run.
        b. View the waveform with an oscilloscope, BEEPER_GPIO and BEEPER_N_GPIO output complementary waveforms at first, 
	BEEPER_N_GPIO output off after a while, and finally both outputs off .

4. Matters needing attention
    None