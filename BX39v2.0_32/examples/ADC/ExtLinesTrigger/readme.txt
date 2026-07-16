1、功能说明
    1、ADC规则通道采样PA2、PA5引脚的模拟电压
    2、在转换完成中断中读取转换结果
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.30.0.0
                         IAR EWARM 8.50.1
    硬件环境：      基于于N32G003F5S7-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=24M,ADC 1M CLK=HSI/48,TIM1 CLK= 48M
        2、中断：
                    ADC规则转换结果完成进入中断，中断优先级0
                    中断处理接收转换结果到ADCConvertedValue[5]数组，并翻转PA6电平
        3、端口配置：
                    PA2选择为模拟功能ADC转换通道
                    PA5选择为模拟功能ADC转换通道
                    PA6选择为通用IO输出
                    PA4选择为通用IO输出
                    PA7选择为外部EXTI事件上升沿触发
        4、ADC：
                    ADC规则通道扫描模式、EXTI7触发、12位数据右对齐，转换通道PA2和PA5的模拟电压数据
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue[5]添加到watch窗口观察
        2、连接PA7与PA4,通过PA7给上升沿可以触发规则通道数据采样；
4、注意事项
    1.当系统采样时钟HSI为48M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48)，分频需要配置成RCC_ADC1MCLK_DIV48
    2.当系统采样时钟HSI为40M时，RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40)，分频需要配置成RCC_ADC1MCLK_DIV40


1. Function description
    1. The ADC regular channel samples the analog voltage of the PA2 and PA5 pins.
    2. Read the conversion result in the conversion completion interrupt.

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the development board N32G003F5S7-STB V1.0

3. Instructions for use
    System Configuration;
        1. Clock source:
                    HSI=48M,AHB=48M,APB1=48M,ADC CLK=24M,ADC 1M CLK=HSI/48,TIM1 CLK= 48M
        2. Interrupt:
                    ADC regular conversion result completion will enter interrupt function, interrupt priority level set to 0.
                    In the interrupt function,the regular result is read into the ADCConvertedValue[5] array, and flips PA6 level.
        3. Port configuration:
                    PA2 is selected as the analog function, ADC conversion channel
                    PA5 is selected as the analog function, ADC conversion channel
                    PA6 is selected as general IO output
                    PA4 is selected as general IO output
					PA7 is selected as external EXTI event rising edge trigger
        4. ADC:
                    ADC regular channel scan mode, EXTI7 trigger, 12-bit data right-aligned, conversion of analog voltage data of channels PA2 and PA5
    Instructions:
        1. Open the debug mode after compiling, add the variables ADCConvertedValue[5] to the watch window for observation
        2. Pin PA7 is connected with PA4. The regular channel data sampling can be triggered by the rising edge of PA7.

4. Matters needing attention
    1.When the system sampling clock HSI is 48M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV48
    2.When the system sampling clock HSI is 40M, RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV40), frequency division needs to be configured as RCC_ ADC1MCLK_ DIV40