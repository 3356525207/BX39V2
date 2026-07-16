#include "tim1.h"

/**
 * TIM1 5KHz 定时器中断配置
 * SYSCLK = 48MHz, TIM1_CLK = SYSCLK = 48MHz
 * Prescaler = 48 - 1 = 47   (分频后 1MHz)
 * Period    = 200 - 1 = 199 (1MHz / 200 = 5KHz)
 */
void TIM1_Config(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseInitStruct;
    NVIC_InitType NVIC_InitStruct;

    /* 使能 TIM1 时钟 */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_TIM1);

    /* TIM1 时钟源选择 SYSCLK (48MHz) */
    RCC_TIM1_Clock_Config(RCC_CFG2_TIM1CLKSEL_SYSCLK);

    /* 填写时基参数 */
    TIM_Base_Struct_Initialize(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.Prescaler = 47;     /* 48MHz / 48 = 1MHz */
    TIM_TimeBaseInitStruct.Period    = 199;    /* 1MHz / 200 = 5KHz */
    TIM_TimeBaseInitStruct.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseInitStruct.ClkDiv    = TIM_CLK_DIV1;

    TIM_Base_Initialize(TIM1, &TIM_TimeBaseInitStruct);

    /* 预分频器立即加载模式 */
    TIM_Base_Reload_Mode_Set(TIM1, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    /* 使能更新中断 */
    TIM_Interrupt_Enable(TIM1, TIM_INT_UPDATE);

    /* NVIC 配置 */
    NVIC_InitStruct.NVIC_IRQChannel         = TIM1_BRK_UP_TRG_COM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Initializes(&NVIC_InitStruct);

    /* 启动 TIM1 */
    TIM_On(TIM1);
}
