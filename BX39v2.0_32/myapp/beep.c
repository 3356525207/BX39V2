#include "beep.h"
#include "bsp_delay.h"

/* BEEPER 引脚: PA14, AF7 */
#define BEEPER_GPIO_PORT    GPIOA
#define BEEPER_GPIO_PIN     GPIO_PIN_14
#define BEEPER_GPIO_AF      GPIO_AF7_BEEPER

static uint32_t s_beep_default_freq = BEEPER_FREQ_2KHZ;

/**
 * 初始化 BEEPER 外设（无源蜂鸣器，PA14 输出）
 * 默认频率 2KHz
 */
void Beep_Init(void)
{
    GPIO_InitType GPIO_InitStruct;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_BEEPER);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);

    GPIO_Structure_Initialize(&GPIO_InitStruct);
    GPIO_InitStruct.Pin            = BEEPER_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.GPIO_Alternate = BEEPER_GPIO_AF;
    GPIO_Peripheral_Initialize(BEEPER_GPIO_PORT, &GPIO_InitStruct);

    BEEPER_Reset();
    BEEPER_Frequency_Select(BEEPER_FREQ_2KHZ);
    BEEPER_Inverted_Enable();
}

/**
 * 设置默认频率（供 Beep_Freq 恢复用）
 */
static void beep_set_freq(uint32_t freq)
{
    BEEPER_Frequency_Select(freq);
}

/**
 * 蜂鸣器发声指定时长
 * 通过 Beep_Tem → ISR 驱动 BEEPER，避免与 ISR 争抢硬件寄存器
 */
void Beep(uint16_t TIME)
{
    Beep_Tem = TIME;
    while (Beep_Tem > 0);
}

/**
 * 蜂鸣器以指定频率发声指定时长，结束后恢复默认频率
 */
void Beep_Freq(uint16_t TIME, uint32_t freq)
{
    beep_set_freq(freq);
    Beep_Tem = TIME;
    while (Beep_Tem > 0);
    beep_set_freq(s_beep_default_freq);
}
