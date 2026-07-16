1、功能说明
    1、ADC采样内部通道Vrefint
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                         IAR EWARM 8.50.1
    硬件环境：      基于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=48M/12,ADC 1M CLK=HSI/48
        2、ADC：
                    ADC连续转换、软件触发、12位数据右对齐，ADC通道9即内部参考电压数据
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue,添加到watch窗口观察，这个值就是Vrefint的采样值
4、注意事项
    1.当系统采样时钟HSI为48M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48)，分频需要配置成RCC_ADC1MCLK_DIV48
    2.当系统采样时钟HSI为40M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40)，分频需要配置成RCC_ADC1MCLK_DIV40



1. Function description
    1. ADC samples the internal channel Vrefint

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the evaluation board N32G003F5S7-STB V1.0

3. Instructions for use
    System Configuration;
        1. Clock source:
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=48M/12,ADC 1M CLK=HSI/48
        2. ADC:
                    ADC configuration: continuous conversion, software trigger, 12-bit data right-aligned, ADC channel 9 is the analog voltage data of the internal reference voltage.
    Instructions:
        1. Open the debug mode after compiling, add the variables ADCConvertedValue to the watch window for observation，This value is the Vrefint sample value
4. Matters needing attention
    1.When the system sampling clock HSI is 48M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV48
    2.When the system sampling clock HSI is 40M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV40