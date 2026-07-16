1、功能说明
     此例程展示 IO 控制 LED 闪烁

2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                        IAR EWARM 8.50.1
     硬件环境：基于N32G003F5S7-STB V1.0开发

3、使用说明
      /* 描述相关模块配置方法；例如:时钟，I/O等 */
      1、 SystemClock：48MHz
      2、 GPIO：PA6、 PA7、PA10 控制 LED(D1、D2、D3) 闪烁

     /* 描述Demo的测试步骤和现象 */
     1、编译后下载程序复位运行；
     2、LED(D1、D2、D3) 闪烁；

4、注意事项
    当端口为高电平时，LED1和LED2亮，但是LED3灭


1. Function description
    This example shows the IO control LED blinking

2. Use environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G003F5S7-STB V1.0

3. Instructions for use
    /* Describe related module configuration methods; for example: clock, I/O, etc. */
    1. SystemClock：48MHz
    2. GPIO: PA6, PA7, PA10 control LED (D1, D2, D3) to blink

   /* Describe the test steps and phenomena of the Demo */
   1. After compiling, download the program to reset and run;；
   2. LED (D1, D2, D3) flashes;

4. Matters needing attention
   LED1 and LED2 are always on when the port is at high level, but LED3 is always off when the port is at high level

