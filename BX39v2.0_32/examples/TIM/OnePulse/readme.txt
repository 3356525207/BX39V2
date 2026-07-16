1、功能说明
    1、TIM3 CH2上升沿触发CH1输出一个单脉冲
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置；
        1、时钟源：
                    AHB=48M,APB=48M,TIM3 CLK=48M
        2、端口配置：
                    PA1选择为TIM3的CH1输出
                    PA2选择为TIM3的CH2输入
                    PA3选择为IO输出
        3、TIM：
                    TIM3 配置CH2上升沿触发CH1输出一个单脉冲
    使用方法：
        1、编译后打开调试模式，PA3连接PA2，用示波器或者逻辑分析仪观察TIM3 的CH1 的波形
        2、将变量gSendTrigEn添加到watch窗口，默认gSendTrigEn=0，每次手动修改gSendTrigEn=1,将会看到在TIM3 CH1端口输出一个单脉冲
4、注意事项
       开发板默认PA2,PA3跳帽接到NSLINK的虚拟串口，若工程中PA2，PA3不作串口，用作其他用途，须拔掉串口跳帽。

1. Function description
     1. The rising edge of TIM3 CH2 triggers CH1 to output a single pulse
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for use
     System Configuration;
         1. Clock source:
                     AHB=48M,APB=48M,TIM3 CLK=48M
         2. Port configuration:
                     PA1 is selected as the CH1 output of TIM3
                     PA2 is selected as the CH2 input of TIM3
                     PA3 is selected as IO output
         3. TIM:
                     TIM3 configures CH2 rising edge to trigger CH1 to output a single pulse
     Instructions:
         1. Open the debug mode after compiling, connect PA3 to PA2, and observe the waveform of CH1 of TIM3 with an oscilloscope or logic analyzer
         2. Add the variable gSendTrigEn to the watch window, the default gSendTrigEn=0, every time you manually modify gSendTrigEn=1, you will see a single pulse output on the TIM3 CH1 port
4. Matters needing attention
    By default, the PA2 and PA3 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA2 and PA3 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.
