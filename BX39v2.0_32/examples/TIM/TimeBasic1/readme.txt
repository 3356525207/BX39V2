1、功能说明
    TIM1 利用更新中断，并产生定时翻转IO
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：      N32G003F5S7-STB V1.0
3、使用说明
    系统配置；
        1、时钟源：
                    AHB=48M,APB=48M,TIM1 CLK=APB
        2、中断：
                    TIM1 更新中断打开，优先级1
        3、端口配置：
                    PA6选择为IO输出
        4、TIM：
                    TIM1使能周期中断
    使用方法：
        1、编译后下载运行
        2、TIM1的周期中断中翻转PA6，可通过示波器或逻辑分析仪查看波形
4、注意事项
    无

1. Function description
     1. TIM1 uses the update interrupt to generate timing rollover IO
2. Use environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware environment:  N32G003F5S7-STB V1.0
3. Instructions for use
     System Configuration;
         1. Clock source:
                     AHB=48M,APB=48M,TIM1 CLK=APB
         2. Interruption:
                     TIM1 update interrupt is turned on, priority level 1
         3. Port configuration:
                     PA6 is selected as IO output
         4. TIM:
                     TIM1 enables periodic interrupts
	       
     Instructions:
         1. Download and run after compilation
         2. Flip PA6 in the cycle interrupt of TIM1, and the waveform can be viewed through an oscilloscope or logic analyzer
4. Matters needing attention
     None