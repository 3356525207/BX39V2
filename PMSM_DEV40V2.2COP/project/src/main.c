/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 * Copyright (c) 2025, Artery Technology, All rights reserved.
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32m412_416_wk_config.h"
#include "wk_adc.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"
#include "arm_math.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "arm_math.h"
#include "vofa.h"
#include "foc_drv.h"
#include "motor_sensoruse.h"
#include "global_control.h"
#include "ADC-front-end.h"
#include "UART_CAN.h"
#include "flash.h"
#include "HKT7823.h"
#include "kth78xx.h"
#include "thermistor.h"
#include "step_counter.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

tmr_output_config_type tmr_oc_init_structure;
crm_clocks_freq_type crm_clocks_freq_struct = {0};

uint16_t spi_buf;

uint16_t IPM_TMP=0;

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
 * @brief main function.
 * @param  none
 * @retval none
 */
int main(void)
{
  /* add user code begin 1 */

  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config for
     void wk_delay_us(uint32_t delay);
     void wk_delay_ms(uint32_t delay); */
  wk_timebase_init();

  /* init gpio function. */
  wk_gpio_config();

  /* init adc-common function. */
  wk_adc_common_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init adc2 function. */
  wk_adc2_init();

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR
     and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1,
                        (uint32_t)&ADC1->odt,
                        (uint32_t)adc_odt,
                        2);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init spi2 function. */
  wk_spi2_init();

  /* init tmr1 function. */
  wk_tmr1_init();

  /* init tmr4 function. */
  wk_tmr4_init();

  /* add user code begin 2 */
  wk_dma_channel_config(DMA1_CHANNEL1,
                        (uint32_t)&ADC1->odt,
                        (uint32_t)adc_odt,
                        2);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  gpio_bits_write(GPIOC, GPIO_PINS_13, 1);

  wk_delay_ms(800);
  POWER_ENABLE();

  Recovery_Check();

  Global_Init();

  Sensoruse_Init();

  while (dma_flag_get(DMA1_FDT1_FLAG) == RESET)
    ;
  //

  /* WDT (独立看门狗) 初始化: LICK=40kHz, DIV=256, reload=311 → 超时≈2秒
     主循环若超2秒未喂狗则自动复位，防止单片机意外死机 */
  wdt_register_write_enable(TRUE);
  wdt_divider_set(WDT_CLK_DIV_256);
  wdt_reload_value_set(311);
  while(wdt_flag_get(WDT_DIVF_UPDATE_FLAG) == RESET);
  while(wdt_flag_get(WDT_RLDF_UPDATE_FLAG) == RESET);
  wdt_register_write_enable(FALSE);
  wdt_enable();

  /* add user code end 2 */

  while (1)
  {
    /* add user code begin 3 */

    wdt_counter_reload();  /* 喂看门狗，防止异常死机时系统不复位 */

    /*
     * 处理 ISR 推迟的 Flash 写操作：
     * Recovery_MarkStable() 内含 flash_write()，会暂停 Flash 总线
     * 数毫秒。严禁在 ISR 内执行，否则导致 PWM 更新延迟 → 电机顿挫。
     * 此处由主循环在非实时上下文中安全完成写操作。
     */
    if (g_RecoveryWritePending)
    {
        Recovery_MarkStable();
        g_RecoveryWritePending = 0;
    }

//    vofa_send();

    // spi_buf=KTH78_ReadAngle();

    //
    //    while(adc_flag_get(ADC2, ADC_CCE_FLAG) == RESET);
		


//   tempFloat[0] = (float)StepCounter_GetFluctuation(&g_stepCounter);
//   tempFloat[1] = (float)g_stepCounter.step_count;
//   tempFloat[2] = (float)VIBRATINO_1_flag;
//   tempFloat[3] = (float)FocStruct.IqLPF;
//   tempFloat[4] = (float)g_TempProtect.current_temp;
//   tempFloat[5] = (float)obs_speed;

// {
//     uint8_t cmd = 0;
//     if (ParseUartFrame(NULL, &cmd, &reg_RW, &data_lens, data_buf) == 1
//         && cmd == 0x02) {
//         unsigned char d[4];
//         d[0] = 0x55;
//         d[1] = (unsigned char)g_TempProtect.current_temp;
//         d[2] = 0;
//         d[3] = g_Sensoruse.IPM_TEMP;
//         SendResponse(0x02, 0x00, 0x04, d);
//     }
// }


 {
     uint8_t cmd = 0;
     if (ParseUartFrame(NULL, &cmd, &reg_RW, &data_lens, data_buf) == 1
         && cmd == 0x02) {
         unsigned char d[6];
         d[0] = 0x55;
         d[1] = (unsigned char)g_TempProtect.current_temp;
         d[2] = (g_stepCounter.step_count >> 8) & 0xFF;
         d[3] =  g_stepCounter.step_count & 0xFF;
         d[4] = g_Sensoruse.IPM_TEMP;
         d[5] = g_Sensoruse.SYS_error;
         SendResponse(0x02, 0x00, 0x06, d);
     }
 }
    /* add user code end 3 */
  }
}

/* add user code begin 4 */

/* add user code end 4 */
