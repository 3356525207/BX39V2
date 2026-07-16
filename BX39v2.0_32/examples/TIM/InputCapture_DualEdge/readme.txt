1、功能说明
    1、TIM3 CH2引脚 CH2上升沿,CH1下降沿计算频率
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置：
        1、时钟源：
                    AHB=48M,APB=48M,TIM3 CLK=48M
        2、中断：
                    TIM3 CH2上升沿 CH1下降沿中断打开，优先级1
        3、端口配置：
                    PA2选择为TIM3 CH2输入
                    PA3选择为IO 输出
        4、TIM：
                    TIM3 CH2上升沿，CH1下降沿捕获中断打开
    使用方法：
        1、编译后打开调试模式，连接PA2与PA3，将变量TIM3Freq、gOnePulsEn添加到watch窗口
        2、默认gOnePulsEn=0，每次手动给gOnePulsEn=1，此时可以看到TIM3Freq计算出来的频率值
4、注意事项
       开发板默认PA2、PA3跳帽接到NSLINK的虚拟串口，若工程中PA2、PA3不作串口，用作其他用途，须拔掉串口跳帽。

1. Function description
    1. TIM3 CH2 pin, CH2 rising edge, CH1 falling edge to calculate the frequency
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for Use
    System configuration：
        1. Clock source:
            AHB=48M, APB=48M, TIM3 CLK=48M
        2. Interruption:
            TIM3 CH2 rising edge CH1 falling edge interrupt on, priority 1
        3. Port configuration:
            PA2 is selected TIM3 CH2 Input
            PA3 is selected IO Output
        4. TIM:
            TIM3 CH2 rising edge, CH1 falling edge capture interrupt is turned on
Usage method:
    1. Open debugging mode after compilation, connect PA3 to PA2, add variables TIM3Freq, gOnePulsEn to the watch window
    2. Default gOnePulsEn=0, each time you manually give gOnePulsEn=1, you can see the frequency value calculated by TIM3Freq
4. Matters needing attention
        By default, the PA2 and PA3 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA2 and PA3 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.
