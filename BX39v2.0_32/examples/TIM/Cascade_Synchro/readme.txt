1、功能说明
    1、TIM1周期门控TIM3
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：        N32G003F5S7-STB V1.0
3、使用说明
    系统配置:
        1、时钟源：
                AHB=48M,APB=48M,TIM1 CLK=48M,TIM3 CLK=48M
        2、端口配置：
                PA11选择为TIM1 CH1输出
                PA1选择为TIM3 CH1输出
        3、TIM：
                TIM1 的周期门控TIM3
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM1 CH1，TIM3 CH1波形
        2、TIM3 4倍周期于TIM1
4、注意事项
    无

1. Function description
     1. TIM1 cycle gated TIM3
2. Use environment
    Software development environment: 	KEIL MDK-ARM V5.34.0.0
    Hardware environment:    	             	N32G003F5S7-STB V1.0
3. Instructions for Use
    System configuration;
        1. Clock source:
            AHB=48M, APB=48M, TIM1 CLK=48M, TIM3 CLK=48M
        2. Port configuration:
                     PA11 is selected as TIM1 CH1 output
                     PA1 is selected as TIM3 CH1 output
        3. TIM:
                     TIM1 cycle gating TIM3
    Instructions:
        1. Open debugging mode after compilation and observe TIM1 CH1, TIM3 CH1 waveforms with an oscilloscope or logic analyzer
         2. TIM3 4 times cycle TIM1
4. Matters needing attention
    None