1、功能说明
    1、TIM3 CH1 CH2 达到CC值后输出翻转，并且比较值累加
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置；
        1、时钟源：
                    AHB=48M,APB=48M,TIM3 CLK=48M
        2、中断：
                    TIM3 比较中断打开，优先级0
        3、端口配置：
                    PA1选择为TIM3的CH1输出
                    PA2选择为TIM3的CH2输出
        4、TIM：
                    TIM3 配置好CH1 CH2 的比较值输出翻转，并打开比较中断
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM3 的CH1 CH2的波形
        2、每当达到比较值时，输出翻转，并且再增加同样的比较值，波形占空比为50%
4、注意事项
       开发板默认PA2,PA3跳帽接到NSLINK的虚拟串口，若工程中PA2，PA3不作串口，用作其他用途，须拔掉串口跳帽。

1. Function description
    1. After TIM3 CH1 CH2 reaches the CC value, the output is flipped, and the comparison value is accumulated
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for use
    System Configuration;
        1. Clock source:
	    AHB=48M,APB=48M,TIM3 CLK=48M
        2. Interrupt:
                    TIM3 compare interrupt on, priority 0
        3. Port configuration:
                    PA1 is selected as the CH1 output of TIM3
                    PA2 is selected as the CH2 output of TIM3
        4. TIM:
                    TIM3 configures the comparison value output of CH1 CH2 to flip, and open the comparison interrupt
    Instructions:
        1. Open the debug mode after compiling, and observe the waveforms of CH1 CH2 of TIM3 with an oscilloscope or logic analyzer
        2. Whenever the comparison value is reached, the output is flipped, and the same comparison value is increased again, and the waveform duty cycle is 50%
4. Matters needing attention
    By default, the PA2 and PA3 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA2 and PA3 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.
