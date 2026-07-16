#ifndef _aip1640_H_
#define _aip1640_H_

#include "main.h"

/*--- GPIO Pin Configuration (user-modifiable) ---*/
#define AIP1640_CLK_PORT    GPIOA
#define AIP1640_CLK_PIN     GPIO_PIN_3
#define AIP1640_DIN_PORT    GPIOA
#define AIP1640_DIN_PIN     GPIO_PIN_5

/*--- Macros for pin control ---*/
#define AIP1640_CLK_H()     GPIO_Pins_Set(AIP1640_CLK_PORT, AIP1640_CLK_PIN)
#define AIP1640_CLK_L()     GPIO_Pins_Reset(AIP1640_CLK_PORT, AIP1640_CLK_PIN)
#define AIP1640_DIN_H()     GPIO_Pins_Set(AIP1640_DIN_PORT, AIP1640_DIN_PIN)
#define AIP1640_DIN_L()     GPIO_Pins_Reset(AIP1640_DIN_PORT, AIP1640_DIN_PIN)
#define AIP1640_DIN_WR(x)   do { if (x) GPIO_Pins_Set(AIP1640_DIN_PORT, AIP1640_DIN_PIN); \
                                 else GPIO_Pins_Reset(AIP1640_DIN_PORT, AIP1640_DIN_PIN); } while(0)

extern uint8_t display_buffer[16];

extern const uint8_t SEG_TABLE[];
extern const uint8_t SEG_TABLE2[];

void aip1640_init(void);
void aip1640_delay_us(uint16_t us);
void aip1640_start(void);
void aip1640_stop(void);
void aip1640_write_data(uint8_t dat);
void aip1640_display(void);
void aip1640_Display_Number5(uint32_t renum);

#endif
