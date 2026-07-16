1、功能说明
1、STOP进入和唤醒退出示例。


2、使用环境

    软件开发环境：KEIL MDK-ARM Professional Version 5.34
                          IAR EWARM 8.50.1

    硬件环境：最小系统板N32G003F5S7-STB_V1.0  


3、使用说明

    系统配置；
        - 系统时钟 = 48MHz

    使用方法：
        在KEIL下编译后烧录到评估板，上电输出打印“This is a demo of entering and exiting STOP mode.”。过了一会输出打印“Entry STOP mode”，表明进入STOP了。
        按下WAKEUP按键后，串口又输出“Exit STOP mode”，表明MCU被唤醒了，从停止的位置执行。


4、注意事项
    无


1. Function description
    1. Example of STOP entry and wake-up exit.


2. Use environment

    Software development environment: KEIL MDK-ARM Professional Version 5.34
                                                             IAR EWARM 8.50.1

    Hardware environment: minimum system board N32G003F5S7-STB_V1.0


3. Instructions for use

    System Configuration;
        -System clock = 48MHz

    Instructions:
        After compiling under KEIL and burning it to the evaluation board, the output prints "This is a demo of entering and exiting STOP mode." after power-on. After a while, the output prints "Entry STOP mode", indicating that it has entered STOP.
        After pressing the WAKEUP button, the serial port outputs "Exit STOP mode" again, indicating that the MCU is awakened，Execute from the stopped position.


4. Matters needing attention
    none
