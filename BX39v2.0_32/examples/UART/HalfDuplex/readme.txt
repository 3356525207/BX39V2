1、功能说明

    该测例演示了UARTy与UARTz间通过查询检测标识，实现半双工模式的基础通信。
    首先，UARTy发送TxBuffer1数据至UARTz，UARTz接收数据存至RxBuffer2。
    随后，UARTz发送TxBuffer2数据至UARTy，UARTy接收数据存至RxBuffer1。
    最后，分别比较两组接收数据与发送数据，比较结果存入TransferStatus1变量和TransferStatus2变量。
    UARTy和UARTz可以是UART1和UART2。

2、使用环境

    软件开发环境：KEIL MDK-ARM Professional Version 5.34
                          IAR EWARM 8.50.1

    硬件环境：最小系统板N32G003F5S7-STB_V1.0

3、使用说明
	
    系统时钟配置如下：
    - 系统时钟 = 48MHz
    
    UART配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - 接收器和发送器使能
    - 半双工模式使能
    
    UART引脚连接如下：
    - UART1_Tx.PA14    <------->   UART2_Tx.PA2

    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行后，依次查看变量TransferStatus1和TransferStatus2，其中，
      PASSED为测试通过，FAILED为测试异常

4、注意事项
    



1. Function description

    This test example demonstrates the basic communication between UARTy and UARTz by querying and detecting identifications in half-duplex mode.
    First, UARTy sends TxBuffer1 data to UARTz, and UARTz receives data and stores it in RxBuffer2.
    Subsequently, UARTz sends TxBuffer2 data to UARTy, and UARTy receives data to RxBuffer1.
    Finally, compare the two groups of received data and sent data respectively, and store the comparison results in the TransferStatus1 variable and the TransferStatus2 variable.
    UARTy and UARTz can be UART1 and UART2.


2. Use environment

    Software development environment: KEIL MDK-ARM Professional Version 5.34
                                                             IAR EWARM 8.50.1

    Hardware environment: minimum system board N32G003F5S7-STB_V1.0


3. Instructions for use

    The system clock configuration is as follows:
    -System clock = 48MHz
    
    The UART configuration is as follows:
    -Baud rate = 115200 baud
    -Word length = 8 data bits
    -1 stop bit
    -Verification control disabled
    -Receiver and transmitter enable
    -Half-duplex mode enabled
    
    The UART pin connections are as follows:
    -UART1_Tx.PA14 <-------> UART2_Tx.PA2

    
    Test steps and phenomena:
    -After the Demo is compiled in the KEIL environment, download it to the MCU
    -After resetting the operation, check the variables TransferStatus1 and TransferStatus2 in turn, among them,
      PASSED means the test passed, FAILED means the test is abnormal


4. Matters needing attention
    