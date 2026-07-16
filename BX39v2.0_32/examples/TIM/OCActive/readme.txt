1、功能说明
    1、TIM3 CH1 CH2 达到CC值后，输出ACTIVE电平
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置；
        1、时钟源：
                    AHB=48M,APB=48M,TIM3 CLK=48M
        2、端口配置：
                    PA1选择为TIM3 CH1输出
                    PA2选择为TIM3 CH2输出
                    PA3选择为IO 输出
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM3 CH1 CH2的波形
        2、定时器运行到CC1 CC2之后，对应通道的输出变为Active
4、注意事项
       开发板默认PA2、PA3跳帽接到NSLINK的虚拟串口，若工程中PA2、PA3不作串口，用作其他用途，须拔掉串口跳帽。

1. Function description
     1. After TIM3 CH1 CH2 reaches the CC value, it outputs ACTIVE level
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for use
     System Configuration;
         1. Clock source:
                     AHB=48M, APB=48M, TIM3 CLK=48M
         2. Port configuration:
                     PA1 is selected as TIM3 CH1 output
                     PA2 is selected as TIM3 CH2 output
                     PA3 is selected as IO output
     Instructions:
        1. Open the debug mode after compiling, and observe the waveforms of TIM2 CH1 CH2 with an oscilloscope or logic analyzer
        2. After the timer runs to CC1 CC2, the output of the corresponding channel becomes Active
4. Matters needing attention
         By default, the PA2 and PA3 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA2 and PA3 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.
