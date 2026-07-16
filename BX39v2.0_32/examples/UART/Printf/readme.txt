1、功能说明

    该测例演示了UARTx与PC间通过查询检测标识实现的基础通信。
    重定向printf函数至UARTx，并使用printf函数输出消息至上位机。
    UARTx可以是UART1或UART2。


2、使用环境

    软件开发环境：KEIL MDK-ARM Professional Version 5.34
                          IAR EWARM 8.50.1

    硬件环境：最小系统板N32G003F5S7-STB_V1.0


3、使用说明
	
    系统时钟配置如下：
    - 系统时钟 = 48MHz
    
    USART配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - 接收器和发送器使能
    
    UART引脚连接如下：
    - UART1_Tx.PA14
    或
    - UART2_Tx.PA2
    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，查看串口打印信息


4、注意事项



1. Function description

    The test example demonstrates the basic communication between UARTx and PC through the query detection identifier.
    Redirect the printf function to UARTx and use the printf function to output the message to the top machine.
    UARTx can be UART1 or UART2.


2. Use environment

    Software development environment: KEIL MDK-ARM Professional Version 5.34
                                                             IAR EWARM 8.50.1

    Hardware environment: minimum system board N32G003F5S7-STB_V1.0


3. Instructions for use

    The system clock configuration is as follows:
    -System clock = 48MHz
    
    The USART configuration is as follows:
    -Baud rate = 115200 baud
    -Word length = 8 data bits
    -1 stop bit
    -Verification control disabled
    -Receiver and transmitter enable
    
    The UART pin connections are as follows:
    - UART1_Tx.PA14
    or
    - UART2_Tx.PA2

    
    Test steps and phenomena:
    - Demo is compiled in KEIL environment and downloaded to MCU
    - Reset and run. Check the serial port information


4. Matters needing attention