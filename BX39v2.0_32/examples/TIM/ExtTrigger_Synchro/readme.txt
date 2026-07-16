1、功能说明
    1、TIM1 CH2门控CH1和TIM3
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置：
        1、时钟源：
                    AHB=48M,APB=48M,TIM1 CLK=48M,TIM3 CLK=48M
        2、端口配置：
                    PA11选择为TIM1 CH1输出
                    PA7选择为TIM1 CH2输入
                    PA1选择为TIM3 CH1输出
        3、TIM：
                    TIM1 CH2 门控CH1，门控TIM3
    使用方法：
        1、编译后打开调试模式，PA7给高电平，用示波器或者逻辑分析仪观察TIM1 CH1,TIM3 CH1的波形
        2、TIM1 CH2高电平定时器开始计数，低电平停止
4、注意事项
    无

1. Function description
     1. TIM1 CH2 gated CH1 and TIM3
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for Use
    System configuration：
        1. Clock source:
                     AHB=48M, APB=48M,  TIM1 CLK=48M, TIM3 CLK=48M
        2. Port configuration:
                     PA11 is selected as TIM1 CH1 output
                     PA7 is selected as TIM1 CH2 input
                     PA1 is selected as TIM3 CH1 output
        3. TIM:
            TIM1 CH2 Gated CH1, Gated TIM3
     Instructions:
    1. Open debugging mode after compilation, PA7 supply high level and observe TIM1 CH1, TIM3 CH1 waveforms with an oscilloscope or logic analyzer
         2. TIM1 CH2 high level timer starts counting, low level stops
4. Matters needing attention
    None