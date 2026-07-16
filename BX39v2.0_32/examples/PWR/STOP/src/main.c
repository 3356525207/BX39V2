/**
*     Copyright (c) 2022, Nations Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of Nations Technologies Inc. (Hereinafter 
* referred to as NATIONS). This software, and the product of NATIONS described herein 
* (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NATIONS does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NATIONS and obtain 
* the latest version of this software before placing orders.

*     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NATIONS products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/

/**
*\*\file main.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "log.h"


/**
*\*\name  SetSysClock.
*\*\fun   Selects HSI as System clock source and configure HCLK, PCLK
*\*\      prescalers.
*\*\parm  src:
*\*\        - SYSCLK_SRC_HSI48M 
*\*\        - SYSCLK_SRC_HSI40M 
*\*\parm  freq(Hz):
*\*\        - SYSCLK_FREQ_8M
*\*\        - SYSCLK_FREQ_24M 
*\*\        - SYSCLK_FREQ_40M 
*\*\        - SYSCLK_FREQ_48M 
**/
void SetSysClock(uint8_t src, uint32_t freq)
{
    uint32_t status_value = 0, counter_value = 0;
    uint32_t sys_div = 0;

    /* Set Latency before switching the clock to prevent the chip flying */
    FLASH_Latency_Set(FLASH_LATENCY_1);
    switch (freq)
    {
      case 8000000:
        if(src == SYSCLK_SRC_HSI40M)
        {
            sys_div = RCC_HSICLK_DIV5;
        }
        else
        {
            sys_div = RCC_HSICLK_DIV6;
        }
        break;
      case 24000000:
        sys_div = RCC_HSICLK_DIV2;
        break;
      case 40000000:
        sys_div = RCC_HSICLK_DIV1;
        break;
      case 48000000:
        sys_div = RCC_HSICLK_DIV1;
        break;
      default:
        while(1);
    }
        
    /* HCLK = SYSCLK */
    RCC_Hclk_Config(RCC_SYSCLK_DIV1);
    /* PCLK = HCLK */
    RCC_Pclk_Config(RCC_HCLK_DIV1);

    if(src == SYSCLK_SRC_HSI48M)
    {
        /* Select HSI 48M*/
        PWR->CTRL3 &= ~PWR_CTRL3_HSISEL; //48M

        /* Wait till HSI is ready and if Time out is reached exit */
        do
        {
            status_value = RCC->HSICTRL & RCC_HSICTRL_HSI48MRDF;
            counter_value++;
        } while ((status_value == 0) && (counter_value != HSI_STARTUP_TIMEOUT));

        if(status_value == RCC_HSICTRL_HSI48MRDF)
        {
             RCC_Sysclk_Config(sys_div);
        }
        else
        {
            /* HSI times out, user can add other processing code */
            while(1);
        }
    }
    else if(src == SYSCLK_SRC_HSI40M)
    {
        /* Select HSI 40M */
        PWR->CTRL3 |= PWR_CTRL3_HSISEL; //40M

        /* Wait till HSI is ready and if Time out is reached exit */
        do
        {
            status_value = RCC->HSICTRL & RCC_HSICTRL_HSI40MRDF;
            counter_value++;
        } while ((status_value == 0) && (counter_value != HSI_STARTUP_TIMEOUT));

        if(status_value == RCC_HSICTRL_HSI40MRDF)
        {
             RCC_Sysclk_Config(sys_div);
        }
        else
        {
            /* HSI times out, user can add other processing code */
            while(1);
        }
    }
    else
    {
        /* Clock source selection error, user can add other processing code */
        while(1);
    }
     
     /* Flash wait state 0: SYSCLK <= 24M 1: SYSCLK <= 48M */
    if(freq <= SYSCLK_FREQ_24M)
    {
        /*8M\24M */
        FLASH_Latency_Set(FLASH_LATENCY_0);
    }
    else
    {
        /*40M\48M */
        FLASH_Latency_Set(FLASH_LATENCY_1);
    }
}

/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    /* Insert a long delay */
    delay(100);
    /* Initialize UART */
    log_init();
    log_info("This is a demo of entering and exiting STOP mode.\n");
    /* Enable PWR Clock */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_PWR);
    /* Initialize LEDs */
    LEDInit(LED1);
    LEDInit(LED2);
    LedOff(LED1);
    LedOn(LED2);
    /* Initialize Key button Interrupt to wakeUp stop */
    KeyInputExtiInit(KEY_INPUT_PORT, KEY_INPUT_PIN);
    
    while (1)
    {
        /* Toggles LED1 */
        Ledlink(LED1);
        log_info("Entry STOP mode\n");
        delay(1);
        
        /* Enters STOP mode */
        PWR_STOP_Mode_Enter(PWR_STOP_ENTRY_WFI);

        /* After exiting from STOP mode, bits[12:13] of RCC_CFG register
        will return to reset state, at which time the system clock is HSI
        6 division (HSI=48M) or HSI 5 division (HSI=40M), and the system 
        clock can be reconfigured as needed.*/
        SetSysClock(SYSCLK_SRC_HSI48M, SYSCLK_FREQ_48M);

        log_info("Exit STOP mode\n");
        /* Insert a long delay */
        delay(100);
    }
}

/**
*\*\name    Ledlink.
*\*\fun     Toggles the selected Led.
*\*\param   Pin
*\*\return  none
**/
void Ledlink(uint16_t Pin)
{
    GPIOA->POD ^= Pin;
}

/**
*\*\name    LedOn.
*\*\fun     Turns selected Led on.
*\*\param   Pin
*\*\return  none
**/
void LedOn(uint16_t Pin)
{
    GPIOA->PBSC = Pin;
}

/**
*\*\name    LedOff.
*\*\fun     Turns selected Led Off.
*\*\param   Pin
*\*\return  none
**/
void LedOff(uint16_t Pin)
{
    GPIOA->PBC = Pin;
}

/**
*\*\name    LEDInit.
*\*\fun     Configures LED GPIO.
*\*\param   Pin
*\*\return  none
**/
void LEDInit(uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;

    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name    delay.
*\*\fun     Delay function.
*\*\param   nCount
*\*\return  none
**/
void delay(vu32 nCount)
{
    vu32 index = 0;
    for (index = (34000 * nCount); index != 0; index--)
    {
    }
}

/**
*\*\name    KeyInputExtiInit.
*\*\fun     Configures key GPIO.
*\*\param   GPIOx
*\*\param   Pin
*\*\return  none
**/
void KeyInputExtiInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    EXTI_Structure_Initializes(&EXTI_InitStructure);

    /* Enable the GPIO Clock */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);

    /*Configure the GPIO pin as input floating*/
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE1;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel         = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;

    NVIC_Initializes(&NVIC_InitStructure);
}

