1、功能说明
    1、PD模式的进入和唤醒退出。


2、使用环境

    软件开发环境：KEIL MDK-ARM Professional Version 5.34
                          IAR EWARM 8.50.1

    硬件环境：最小系统板N32G003F5S7-STB_V1.0


3、使用说明

    系统配置：
        - 系统时钟 = 48MHz

    使用方法：
        在KEIL下编译后烧录到评估板，串接电流表，上电后过一会，电流明显变小。按下WAKEUP按键，电流回到了mA级别。过了一会又降到uA。
        如果开启打印输出，可以看到串口输出“This is a demo of entering and exiting PD mode.”“Enter PD mode”，唤醒后输出“This is a demo of entering and exiting PD mode.”，说明PD唤醒后从0地址开始执行。
            


4、注意事项
    无


1. Function description
    1. Entry and wake-up exit of PD mode.


2. Use environment


    Software development environment: KEIL MDK-ARM Professional Version 5.34
                                                             IAR EWARM 8.50.1

    Hardware environment: minimum system board N32G003F5S7-STB_V1.0


3. Instructions for use

    System Configuration：
        -System clock = 48MHz

    Instructions:
        After compiling under KEIL, it is burned to the evaluation board, and the ammeter is connected in series. After a while, the current becomes significantly smaller. Press the WAKEUP button and the current returns to mA level. After a while it drops to uA again.
        If you turn on the print output, you can see that the serial port outputs “This is a demo of entering and exiting PD mode.”“Enter PD mode”, and after waking up, it outputs "This is a demo of entering and exiting PD mode.", indicating that the PD starts to execute from address 0 after waking up.
            

4. Matters needing attention
    none


