1、功能说明
    1、ADC规则采样PA2,PA5引脚的模拟电压，在TIM1 CC2事件下触发采样;
    2、在转换完成中断中读取转换结果
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                         IAR EWARM 8.50.1
    硬件环境：      基于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=24M,ADC 1M CLK=HSI/48,TIM1 CLK= 48M
        2、中断：
                    ADC规则通道转换结果完成会进入中断，中断优先级设置为0。
                    中断函数中将规则通道的结果读取到ADCConvertedValue[5]数组，并翻转PA6电平
        3、端口配置：
                    PA2选择为模拟功能ADC转换通道
                    PA5选择为模拟功能ADC转换通道
                    PA6选择为通用IO输出
                    PA7选择为TIM1 CH2 PWM输出

        4、ADC：
                    ADC TIM1 CC2触发、12位数据右对齐，规则转换通道PA2，PA5的模拟电压数据
        6、TIM：
                    TIM1开启CH2输出，CH2用作触发ADC转换        
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue[5]添加到watch窗口观察
        2、通过改变PA2 PA5引脚的电压，在每个CC2事件发生时转换一次规则通道，变量储存在对应数组中。同时在PA7 可以看到TIM1 CH2的 PWM 波形
4、注意事项
    1.当系统采样时钟HSI为48M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48)，分频需要配置成RCC_ADC1MCLK_DIV48
    2.当系统采样时钟HSI为40M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40)，分频需要配置成RCC_ADC1MCLK_DIV40
	3.J17的跳线帽需要拔掉


1. Function description
    1. The ADC regularly samples the analog voltage of the PA2,PA5 pin, and triggers sampling under the TIM1 CC2 event;
    2. Read the conversion result in the conversion completion interrupt.

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the development board N32G003F5S7-STB V1.0 V1.0

3. Instructions for use
    System Configuration;
        1. Clock source:
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=24M,ADC 1M CLK=HSI/48,TIM1 CLK= 48M
        2. Interrupt:
                    ADC regular conversion result complete will enter interrupt function, interrupt priority level set to 0.
                    In the interrupt function, the regular result is read into the ADCConvertedValue[5] array, and the PA6 level is flipped
        3. Port configuration:
                    PA2 is selected as the analog function, ADC conversion channel
                    PA5 is selected as the analog function, ADC conversion channel
                    PA6 is selected as general IO output
                    PA7 is selected as TIM1 CH2 PWM output
		5. ADC:
                    ADC configuration: TIM1 CC2 trigger, 12-bit data right-aligned, analog voltage data of regular conversion channel PA2,PA5 Pin
        6. TIM:
                    TIM1 turns on CH2 output, and CH2 is used to trigger ADC conversion
    Instructions:
        1. Open the debug mode after compiling and add the variables ADCConvertedValue[5] to the watch window for observation
        2. By changing the voltage of the PA2 PA5 pin,regular channel are converted once every CC2 event occurs, 
		   and the variables are stored in the corresponding array.At the same time, you can see the PWM waveform of TIM1 CH2 on PA7

4. Matters needing attention
    1.When the system sampling clock HSI is 48M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV48
    2.When the system sampling clock HSI is 40M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV40
	3.J17 jumper cap needs to be removed