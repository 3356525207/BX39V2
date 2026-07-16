1、功能说明
    1. 此例程显示读取端口的高低电平，并控制LED（D1、D2）闪烁
    2. 控制IO端口电平翻转

2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                        IAR EWARM 8.50.1
     硬件环境：基于N32G003F5S7-STB V1.0开发

3、使用说明
      /* 描述相关模块配置方法；例如:时钟，I/O等 */
      1、 SystemClock：48MHz
      2、 GPIO：选择PA11作为读取电平端口，PA6、PA7控制指示灯（D1、D2）闪烁

     /* 描述Demo的测试步骤和现象 */
     1、编译后下载程序复位运行；
     2、检查PA11端口电平：如果为高电平，LED(D2)亮；如果为低电平，LED(D1)亮，将PA8/PA9轮流连接到LED(D3)，如果LED3端口为低电平，LED(D3)亮

4、注意事项
    1、当检测到 PA11 端口处于高电平时，关闭 SWD，当 PA11 端口处于低电平时，打开 SWD
    2、当端口处于高电平时，LED1 和 LED2 始终亮起，但当端口处于高电平时，LED3 始终熄灭

1. Function description
    1. This routine shows the high and low levels of the read port and controls the LED (D1、D2) to flash
    2. Control IO port level flip

2. Use environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G003F5S7-STB V1.0

3. Instructions for use
    /* Describe related module configuration methods; for example: clock, I/O, etc. */
    1. SystemClock：48MHz
    2. GPIO: PA11 is selected as the read level port, PA6, PA7 control LED (D1, D2) to flash

   /* Describe the test steps and phenomena of the Demo */
    1. After compiling, download the program to reset and run;
    2. Check the level of the PA11 port:
       If it is high level, LED (D2) is always on;
       If it is low level, the LED (D1) is always on, and the PA8/PA9 are connected to LED(D3) in turn, if LED3 port is low, LED(D3) will on

4. Matters needing attention
   1. When it is detected that the PA11 port is at a high level, close SWD, and when the PA11 port is at a low level, open the SWD
   2. LED1 and LED2 are always on when the port is at high level, but LED3 is always off when the port is at high level

