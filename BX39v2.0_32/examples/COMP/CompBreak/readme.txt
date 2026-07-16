1、功能说明
    1、COMP1的输出刹车TIM1 的互补信号，COMP OUT变低后恢复TIM1 波形
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                    IAR EWARM 8.50.1
    硬件环境：      基于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                   HSI=48M,AHB=48M,APB1=48M,COMP CLK=48M,TIM1 CLK=48M
        2、端口配置：
                    PB0选择为模拟功能COMP INP
                    PB1选择为模拟功能COMP INM
                    PA8选择为模拟功能COMP OUT
                    PA12选择为IO输出
                    PA13选择为IO输出
                    PA6选择为TIM1 CH1输出
                    PA5选择为TIM1 CH1N输出
                 
        3、TIM：
                    TIM1开启CH1 CH1N 输出,COMP作为刹车输入
        4、COMP：
                    COMP1输出触发TIM1 刹车，无输出时恢复TIM1 输出
    使用方法：
        1、编译后打开调试模式，将PA12连接到PB0，PA13连接到PB1，利用示波器或者逻辑分析仪观察TIM1输出波形（波形输出从上电复位开始，5s之后才开始输出）
        2、当软件输出PA12电平大于PA13时，TIM波形消失，相反时，波形正常输出.
4、注意事项
    1，比较器输出管脚为PA8，该引脚是调试口SWDIO的复用管脚，故在初始化延时5s后，才开始将SWDIO配置成普通的GPIO使用； 
    2，如果将延时5s注释掉，代码一旦运行，下次会造成无法烧录的问题，请务必注意；



1. Function description
    1. The output of COMP1 brakes the complementary signal of TIM1, and the waveform of TIM1 is restored after COMP OUT goes low.

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the evaluation board N32G003F5S7-STB V1.0

3. Instructions for use
    System Configuration;
        1. Clock source:
                    HSI=48M,AHB=48M,APB1=48M,COMP CLK=48M,TIM1 CLK=48M
        2. Port configuration:
                    PB0 is selected as the analog function, COMP INP
                    PB1 is selected as the analog function, COMP INM
                    PA8 is selected as the analog function, COMP OUT
                    PA12 is selected as IO output
                    PA13 is selected as IO output
                    PA6 is selected as TIM1 CH1 output
                    PA5 is selected as TIM1 CH1N output
                  
        3. TIM:
                    TIM1 turns on CH1 CH1N output, COMP is used as brake input
        4. COMP:
                    COMP1 output triggers TIM1 brake, and resumes TIM1 output when there is no output
    Instructions:
        1. Open the debug mode after compiling, connect PA12 to PB0 and PA13 to PB1, use an oscilloscope or logic analyzer 
            to observe the output waveform of TIM1(Waveform output starts from power on reset and starts after 5s)
        2. When the software output PA12 level is greater than PA13, the TIM waveform disappears, on the contrary, the waveform is output normally

4. Matters needing attention
        1. The output pin of the comparator is PA8, which is a multiplexed pin of the debugging port SWDIO. Therefore, SWDIO is not configured for normal GPIO until 5s after the initialization delay;
        2. If the 5s delay is commented out, once the code is run, it will cause the problem that it cannot be burned next time. Please be careful;