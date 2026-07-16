1、功能说明
     1、SPI 从模式单线中断发送数据
2、使用环境
     软件开发环境：KEIL MDK-ARM 5.30.0.0
                   IAR EWARM 8.50.1
     硬件环境：    基于N32G003F5S7-STB V1.0开发  
3、使用说明
     系统配置：
         1、SystemClock：48MHz
         2、GPIO：（从模式 DEMO 板）SPI: NSS--PA3、SCK--PA15、MISO--PA7
         3、日志打印：从模式 DEMO 板 PA2(TX)，波特率：115200
     使用方法：
         1、编译后下载程序复位运行；
         2、从模式 DEMO 板接好串口打印工具，上电
         3、先松开从DEMO板复位键，再松开主DEMO板复位键，查看打印测试成功
4、注意事项
     需要两块 demo 板配合，一块板子烧录主模式程序，一块板子烧录从模式程序，两块板子需一起上电，连接两块板子的 VCC 和 GND
    “单线”数据线在主设备端为MOSI引脚，在从设备端为MISO引脚

1. Function description
    1. SPI slave mode single-wire interrupt send data
2. Use environment
    Software development environment: KEIL MDK-ARM 5.30.0.0
    Hardware Environment: Developed based on N32G003F5S7-STB V1.0
3. Instructions for use
    System Configuration;
        1. SystemClock：48MHz
        2. GPIO：(Slave mode DEMO board) SPI: NSS--PA3, SCK--PA15, MISO--PA7
        3. Log print: slave mode DEMO board PA2(TX), baud rate: 115200
    Instructions:
        1. After compiling, download the program to reset and run;
        2. The slave mode DEMO board connects the serial port printing tool, power on.
        3. Release the reset key of the Slave DEMO board and then release the reset key of the Master DEMO board, check that the printing test is successful.
4. Matters needing attention
    Two demo boards are required, one board to program the master mode program, one board to program the slave mode program, 
    the two boards need to be powered on together, and connect the VCC and GND of the two boards;
    The "single wire" data lines are MOSI pins on the master side and MISO pins on the slave side.