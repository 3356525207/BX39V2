1、功能说明
    1、SPI 读、写、擦除 W25Q128
2、使用环境
     软件开发环境：KEIL MDK-ARM 5.30.0.0
                   IAR EWARM 8.50.1
     硬件环境：    基于N32G003F5S7-STB V1.0开发
3、使用说明
     系统配置：
         1、SystemClock：48MHz
         2、GPIO：SPI: SCK--PA15 NSS--PA3 MISO--PA7 MOSI--PA6
            UART: TX--PA2、RX--PA3
     使用方法：
        1、编译后下载程序复位运行
        2、通过串口工具查看结果
4、注意事项
   将N32G003F5S7-STB开发板与N32WB45xL_EVB V1.1上的SPI Flash连接，注意连接VDD和GND

1. Function description
    1. SPI read, write, erase W25Q128
2. Use environment
    Software development environment: KEIL MDK-ARM 5.30.0.0
    Hardware Environment: Developed based on N32G003F5S7-STB V1.0
3. Instructions for use
    System Configuration;
        1. SystemClock：48MHz
        2. GPIO：SPI: SCK--PA15 NSS--PA3 MISO--PA7 MOSI--PA6
           UART: TX--PA2, RX--PA3
    Instructions:
        1. After compiling, download the program to reset and run
        2. View the results through the serial port tool
4. Matters needing attention
   Connect the N32G003F5S7-STB development board with SPI Flash on N32WB45xL_EVB V1.1, taking care to connect VDD and GND