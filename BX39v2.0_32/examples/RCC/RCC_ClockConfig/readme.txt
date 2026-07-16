1、功能说明

    /* 简单描述工程功能 */
    这个例程配置并演示设置不同的系统时钟


2、使用环境

    /* 软件开发环境：当前工程使用的软件工具名称及版本号 */
    IDE工具：KEIL MDK-ARM 5.34.0.0
             IAR EWARM 8.50.1
      
    /* 硬件环境：工程对应的开发硬件平台 */
    开发板：N32G003F5S7-STB V1.0     

        

3、使用说明

    /* 描述相关模块配置方法；例如:时钟，I/O等 */
    USART：TX - PA2，波特率115200
    GPIO：PA13 - 复用为MC0时钟输出

    /* 描述Demo的测试步骤和现象 */
    1.编译后下载程序复位运行；
    2.切换系统时钟源为HSI48M，系统时钟为24M,使用串口打印出当前SYSCLK、HCLK、PCLK等信息，并且使用PA13复用引脚输出HSI时钟，用示波器查看；


4、注意事项
     无

1. Function description

     /* Briefly describe the engineering function */
     This example configures and demonstrates the setting of different system clocks


2. Use environment

     /* Software development environment: the name and version number of the software tool used in the current project */
     IDE tool: KEIL MDK-ARM 5.34.0.0
      
     /* Hardware environment: development hardware platform corresponding to the project */
     Development board: N32G003F5S7-STB V1.0

3. Instructions for use

     /* Describe the configuration method of related modules; for example: clock, I/O, etc. */
     USART: TX-PA2, baud rate 115200
     GPIO: PA13-multiplexed as MC0 clock output

     /* Describe the test steps and phenomena of Demo */
     1. After compiling, download the program to reset and run;
     2. Switch the system clock source to HSI48M, the system clock is 24M, use the serial port to print out the current SYSCLK, HCLK, 
       PCLK and other information, and use PA13 multiplexing pin output HSI clock, with an oscilloscope view;


4. Attention
    None