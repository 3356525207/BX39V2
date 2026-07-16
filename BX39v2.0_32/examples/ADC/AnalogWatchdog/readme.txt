1、功能说明
    1、ADC采样转换PA2引脚的模拟电压，如果超过模拟看门狗定义的阈值范围，则跳入中断程序
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                         IAR EWARM 8.50.1
    硬件环境：      基于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=48M/12,ADC 1M CLK=HSI/48
        2、ADC：
                    ADC连续转换、软件触发、12位数据右对齐，ADC通道1
		3、端口配置：
                    PA2选择为模拟功能ADC转换通道0
		4、中断：
                    ADC模拟看门狗中断打开，优先级0
    使用方法：
        1、编译后打开调试模式，将变量gCntAwdg添加到watch窗口观察
        2、改变PA2引脚电压值，当电压值超出模拟看门狗定义的0x300（即0.6V）到0xB00（即2.26V）范围外，则进入一次中断，变量做累加操作
4、注意事项
    1.当系统采样时钟HSI为48M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48)，分频需要配置成RCC_ADC1MCLK_DIV48
    2.当系统采样时钟HSI为40M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40)，分频需要配置成RCC_ADC1MCLK_DIV40



1. Function description
    1. ADC samples and converts the analog voltage of the PA2 pin. If it exceeds the threshold range defined by the analog watchdog, then jump into the interrupt program

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the evaluation board N32G003F5S7-STB V1.0

3. Instructions for use
    System Configuration;
        1. Clock source:
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=48M/12,ADC 1M CLK=HSI/48
        2. ADC:
                    ADC configuration: continuous conversion, software trigger, 12-bit data right-aligned, ADC channel 1.
        3. Port configuration:
                    PA2 is selected as analog function, ADC conversion channel 0
        4. Interrupt:
                    ADC analog watchdog interrupt is turned on, priority 0
    Instructions:
        1. Open the debug mode after compiling and add the variable gCntAwdg to the watch window for observation
        2. Change the voltage value of the PA2 pin. When the voltage value exceeds the range defined by the analog watchdog from 0x300 (ie 0.6V) to 0xB00 (ie 2.26V), an interrupt is entered, and the variable is accumulated

4. Matters needing attention
    1.When the system sampling clock HSI is 48M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV48
    2.When the system sampling clock HSI is 40M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV40