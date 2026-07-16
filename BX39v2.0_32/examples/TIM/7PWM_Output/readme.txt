1、功能说明
    1、TIM1输出3路互补波形和一路CH4波形
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置:
        1、时钟源：
                    AHB=48M,APB=48M,TIM1 CLK=48M
        2、端口配置：
                    PA11选择为TIM1 CH1输出
                    PA7选择为TIM1 CH2输出
                    PA1选择为TIM1 CH3输出
                    PA5选择为TIM1 CH1N输出
                    PA6选择为TIM1 CH2N输出
                    PA2选择为TIM1 CH3N输出
                    PA3选择为TIM1 CH4输出
        3、TIM：
                    TIM1 3路互补输出,CH4输出
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM1的输出波形
        2、输出波形TIM1 3路互补加一路CH4
4、注意事项
       开发板默认PA2、PA3跳帽接到NSLINK的虚拟串口，若工程中PA2、PA3不作串口，用作其他用途，须拔掉串口跳帽。

1. Function description
    1. TIM1 output 3 complementary waveforms and 1 CH4 waveform
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for Use
    System configuration;
        1. Clock source:
            AHB=48M, APB=48M, TIM1 CLK=48M
        2. Port configuration:
                    PA11 is selected as TIM1 CH1 output
                    PA7 is selected as TIM1 CH2 output
                    PA1 is selected as TIM1 CH3 output
                    PA5 is selected as TIM1 CH1N output
                    PA6 is selected as TIM1 CH2N output
                    PA2 is selected as TIM1 CH3N output
                    PA3 is selected as TIM1 CH4 output
        3. TIM:
            TIM1   3 complementary outputs, CH4 output
    Instructions:
        1. Open debugging mode after compilation and use an oscilloscope or logic analyzer to observe the output waveform of TIM1
        2. Output waveform TIM1 3-way complementary plus one CH4
4. Matters needing attention
       By default, the PA2 and PA3 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA2 and PA3 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.
