1、功能说明
    1、COMP的输出PA8受输入INP PB0和INM PB1的影响
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                        IAR EWARM 8.50.1
    硬件环境：      基于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSI=48M,AHB=48M,APB1=48M,COMP CLK=48M
        2、端口配置：
                    PB0选择为模拟功能COMP INP
                    PB1选择为模拟功能COMP INM
                    PA8选择为模拟功能COMP OUT
                    PA12选择为IO输出
                    PA13选择为IO输出
        3、COMP：
                    COMP1输入PB1，PB0，输出PA8
    使用方法：
        1、编译后打开调试模式，将PA12连接到PB0，PA13连接到PB1，利用示波器或者逻辑分析仪观察PA8输出波形（波形输出从上电复位开始，5s之后才开始输出）
        2、当软件输出PA12电平大于PA13时，PA8输出高电平，相反时，输出低电平
4、注意事项
    1，比较器输出管脚为PA8，该引脚是调试口SWDIO的复用管脚，故在初始化延时5s后，才开始将SWDIO配置成普通的GPIO使用； 
    2，如果将延时5s注释掉，代码一旦运行，下次会造成无法烧录的问题，请务必注意；


1. Function description
     1. The output PA8 of COMP is affected by the input INP PB0 and INM PB1

2. Use environment
     Software development environment: KEIL MDK-ARM V5.30.0.0
     Hardware environment: Developed based on the development board N32G003F5S7-STB V1.0

3. Instructions for use
     System Configuration;
         1. Clock source:
                     HSI=48M,AHB=48M,APB1=48M,COMP CLK=48M
         2. Port configuration:
                     PB0 is selected as the analog function, COMP INP
                     PB1 is selected as the analog function, COMP INM
                     PA8 is selected as the analog function, COMP OUT
                     PA12 is selected as IO output
                     PA13 is selected as IO output
         3. COMP:
                     COMP input PB0, PB1, output PA8
     Instructions:
         1. After compiling, turn on the debug mode, connect PA12 to PB0, and PA13 to PB1, use an oscilloscope or logic analyzer 
             to observe the output waveform of PA8.(Waveform output starts from power on reset and starts after 5s).
         2. When the software output PA12 level is greater than PA13, PA8 output high level, on the contrary, output low level

4. Matters needing attention
         1. The output pin of the comparator is PA8, which is a multiplexed pin of the debugging port SWDIO. Therefore, SWDIO is not configured for normal GPIO until 5s after the initialization delay;
         2. If the 5s delay is commented out, once the code is run, it will cause the problem that it cannot be burned next time. Please be careful;