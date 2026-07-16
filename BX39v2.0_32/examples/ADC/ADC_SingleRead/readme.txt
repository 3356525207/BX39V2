1、功能说明
    1、ADC1采样转换PA2 PA12引脚的模拟电压
    2、采用软件触发一次，采集一次的方式
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                         IAR EWARM 8.50.1
			                     
    硬件环境：      基于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=48M/12,ADC 1M CLK=HSI/48
        2、端口配置：
                    PA2选择为模拟功能ADC转换通道0
                    PA12选择为模拟功能ADC转换通道1
        3、ADC：
                    ADC软件触发转换、12位数据右对齐，转换PA2 PA12的模拟电压数据
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue添加到watch窗口观察
        2、通过改变PA2 PA12引脚的电压，可以看到转换结果变量同步改变
4、注意事项
    1.当系统采样时钟HSI为48M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48)，分频需要配置成RCC_ADC1MCLK_DIV48
    2.当系统采样时钟HSI为40M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40)，分频需要配置成RCC_ADC1MCLK_DIV40


1. Function description
    1. ADC1 samples and converts the analog voltage of PA2 PA12 pin
    2. Use software to trigger once and collect once

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the development board N32G003F5S7-STB V1.0 

3. Instructions for use
    System Configuration;
        1. Clock source:
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=48M/12,ADC 1M CLK=HSI/48
        2. Port configuration:
                    PA2 is selected as analog function, ADC conversion channel 0
                    PA12 is selected as analog function, ADC conversion channel 1
        3. ADC:
                    ADC configuration: software trigger, 12-bit data is right-aligned, and converts the analog voltage data of PA2 PA12
    Instructions:
        1. Open the debug mode after compiling and add the variable ADCConvertedValue to the watch window for observation
        2. By changing the voltage of the PA2 PA12 pin, you can see that the conversion result variable changes synchronously

4. Matters needing attention
    1.When the system sampling clock HSI is 48M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV48
    2.When the system sampling clock HSI is 40M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV40