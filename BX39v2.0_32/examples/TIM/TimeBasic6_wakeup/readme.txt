1、功能说明
    TIM6 利用更新中断，唤醒STOP模式，并产生定时翻转IO
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：      N32G003F5S7-STB V1.0
3、使用说明
    系统配置；
        1、时钟源：
                    AHB=48M,APB=48M,TIM6 CLK=LSI=32kHz
        2、中断：
                    TIM6 更新中断打开，优先级1
        3、端口配置：
                    PA6选择为IO输出
        4、TIM：
                    TIM6使能周期中断
        5、UART：
                   TX：PA2
                   波特率：115200
    使用方法：
        1、编译后下载运行，进入STOP后通过更新中断唤醒，通过打印查看状态
        2、TIM6的周期中断中翻转PA6，可通过示波器或逻辑分析仪查看波形
4、注意事项
    无

1. Function description
     1. TIM6 uses the update interrupt to wakeup STOP mode and generate timing rollover IO
2. Use environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware environment:  N32G003F5S7-STB V1.0
3. Instructions for use
     System Configuration;
         1. Clock source:
                     AHB=48M,APB=48M,TIM6 CLK=LSI=32kHz
         2. Interruption:
                     TIM6 update interrupt is turned on, priority level 1
         3. Port configuration:
                     PA6 is selected as IO output
         4. TIM:
                     TIM6 enables periodic interrupts
         5. UART:
                      TX：PA2
                      Baud rate: 115200
	       
     Instructions:
         1. Download and run after compilation, wake up by interrupting the update after entering STOP, and check the status through printing
         2. Flip PA6 in the cycle interrupt of TIM6, and the waveform can be viewed through an oscilloscope or logic analyzer
4. Matters needing attention
     None