1、功能说明

    此例程展示了作为主机使用I2C中断收发数据。   

2、使用环境

   软件开发环境：
        IDE工具：KEIL MDK-ARM 5.34.0.0
	         IAR EWARM 8.50.1
    
    硬件环境：
        开发板：N32G003F5S7-STB V1.0


3、使用说明
	
    1、主时钟：48MHz
    2、I2C 配置：
            SCL   -->  PA4          
            SDA   -->  PA5         
            CLOCK:100KHz
            
   3、UART1配置：
            TX  -->  PA2   
            RX  -->  PA3          
            波特率：115200
        

    4、测试步骤与现象
        a，跳线连接从机I2C
        b，编译下载代码复位运行
        c，从串口看打印信息，验证结果

4、注意事项
    SCL及SDA必须接上拉

1. Function description

     This example demonstrates the use of I2C interrupts as a master to send and receive data.

2. Use environment

    Software development environment:
         IDE tool: KEIL MDK-ARM 5.34.0.0
    
     Hardware environment:
         Development board: N32G003F5S7-STB V1.0


3. Instructions for use

     1. Main clock: 48MHz
     2. I2C configuration:
             SCL --> PA4          
             SDA --> PA5          
             CLOCK: 100KHz
            
    3. UART1 configuration:
             TX --> PA2
             RX --> PA3
             Baud rate: 115200
        

     4. Test steps and phenomena
         a, jumper wire to connect slave I2C
         b, compile and download the code, reset and run
         c, view the print information from the serial port and verify the result

4. Matters needing attention
     SCL and SDA must be pulled up