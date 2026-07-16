1、功能说明
    1、SPI 全双工软件 NSS 模式发送接收数据
2、使用环境
     软件开发环境：KEIL MDK-ARM 5.30.0.0
                   IAR EWARM 8.50.1
     硬件环境：    基于N32G003F5S7-STB V1.0开发
3、使用说明
     系统配置：
         1、SystemClock：48MHz
         2、GPIO：SPI: NSS--PA3 SCK--PA15 MISO--PA7 MOSI--PA6
     使用方法：
        1、编译后下载程序复位运行；
        2、先松开从机复位键，再松开主机复位键，主机发数据从机接收，之后从机发数据主机接收，最后检查TransferStatus为PASSED；
4、注意事项
     无

1. Function description
    1. SPI full duplex software NSS mode to send and receive data
2. Use environment
    Software development environment: KEIL MDK-ARM 5.30.0.0
    Hardware Environment: Developed based on N32G003F5S7-STB V1.0
3. Instructions for use
    System Configuration;
        1. SystemClock：48MHz
        2. GPIO：SPI: NSS--PA3 SCK--PA15 MISO--PA7 MOSI--PA6
    Instructions:
        1. After compiling, download the program to reset and run;
        2. First release the slave reset button, then release the host reset button, the master sends data to the slave to receive, 
           then the slave sends data to the host receives, and finally check that TransferStatus is PASSED;
4. Matters needing attention
    No