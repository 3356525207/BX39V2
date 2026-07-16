#include "aip1640.h"
#include "bsp_delay.h"

uint8_t display_buffer[16] = {0};

const uint8_t SEG_TABLE[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
    0x00
};

const uint8_t SEG_TABLE2[] = {
    0x40, 0x54, 0x1D, 0x6F, 0x77, 0x7C, 0x39, 0x5E,
    0x79, 0x71, 0x76, 0x74, 0x33, 0x73, 0x9C, 0x78
};

void aip1640_init(void)
{
    GPIO_InitType GPIO_InitStruct;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);

    GPIO_Structure_Initialize(&GPIO_InitStruct);
    GPIO_InitStruct.Pin        = AIP1640_CLK_PIN | AIP1640_DIN_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_MODE_OUT_PP;

    GPIO_Peripheral_Initialize(AIP1640_CLK_PORT, &GPIO_InitStruct);

    AIP1640_CLK_H();
    AIP1640_DIN_H();
}

/**
 * @brief  Microsecond delay (simple loop, ~48MHz)
 */
void aip1640_delay_us(uint16_t us)
{
    volatile uint32_t i;
    while (us--)
    {
        for (i = 0; i < 12; i++)  /* ~1us @48MHz (calibrated empirically) */
        {
            __NOP();
        }
    }
}

void aip1640_start(void)
{
    AIP1640_DIN_H();
    AIP1640_CLK_H();
    aip1640_delay_us(2);
    AIP1640_DIN_L();
    aip1640_delay_us(2);
}

void aip1640_stop(void)
{
    AIP1640_DIN_L();
    aip1640_delay_us(2);
    AIP1640_CLK_H();
    aip1640_delay_us(2);
    AIP1640_DIN_H();
    aip1640_delay_us(2);
}

void aip1640_write_data(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        AIP1640_CLK_L();
        aip1640_delay_us(2);
        AIP1640_DIN_WR(dat & 0x01);
        aip1640_delay_us(2);
        AIP1640_CLK_H();
        aip1640_delay_us(4);
        dat >>= 1;
    }
    AIP1640_CLK_L();
    aip1640_delay_us(2);
}

void aip1640_display(void)
{
    uint8_t i;

    aip1640_start();
    aip1640_write_data(0x40);
    aip1640_stop();

    aip1640_start();
    aip1640_write_data(0xC0);
    for (i = 0; i < 16; i++)
        aip1640_write_data(display_buffer[i]);
    aip1640_stop();

    aip1640_start();
    aip1640_write_data(0x8f);
    aip1640_stop();
}

void aip1640_Display_Number5(uint32_t renum)
{
    uint8_t re_digit0, re_digit1, re_digit2, re_digit3, re_digit4;

    re_digit0 = renum % 10;
    re_digit1 = (renum / 10) % 10;
    re_digit2 = (renum / 100) % 10;
    re_digit3 = (renum / 1000) % 10;
    re_digit4 = (renum / 10000) % 10;

    display_buffer[0] = SEG_TABLE[re_digit4];
    display_buffer[1] = SEG_TABLE[re_digit3];
    display_buffer[2] = SEG_TABLE[re_digit2];
    display_buffer[3] = SEG_TABLE[re_digit1];
    display_buffer[4] = SEG_TABLE[re_digit0];
}
