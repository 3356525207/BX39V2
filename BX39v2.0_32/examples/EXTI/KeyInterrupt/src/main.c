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


/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    /* SystemInit() function has been called by startup file startup_n32g003.s */

    /* Initialize Led as output pushpull mode */
    LedInit(LED1_PORT, LED1_PIN);
    LedInit(LED2_PORT, LED2_PIN);

    /*Initialize key as external line interrupt*/
    KeyInputExtiInit(KEY_INPUT_PORT, KEY_INPUT_PIN);

    /*Turn on Led1*/
    LedOn(LED1_PORT, LED1_PIN);

    while (1)
    {
    }
}


/**
*\*\name    Delay.
*\*\fun     Main program.
*\*\param   count
*\*\return  none
**/
void Delay(uint32_t count)
{
    for (; count > 0; count--)
        ;
}

/**
*\*\name    KeyInputExtiInit.
*\*\fun     Configures key port.
*\*\param  GPIOx :
*\*\          - GPIOA
*\*\          - GPIOB
*\*\param  Pin :
*\*\          - GPIO_PIN_0
*\*\          - GPIO_PIN_1
*\*\          - GPIO_PIN_2
*\*\          - GPIO_PIN_3
*\*\          - GPIO_PIN_4
*\*\          - GPIO_PIN_5
*\*\          - GPIO_PIN_6
*\*\          - GPIO_PIN_7
*\*\          - GPIO_PIN_8
*\*\          - GPIO_PIN_9
*\*\          - GPIO_PIN_10
*\*\          - GPIO_PIN_11
*\*\          - GPIO_PIN_12
*\*\          - GPIO_PIN_13
*\*\          - GPIO_PIN_14
*\*\          - GPIO_PIN_15
*\*\return  none
**/
void KeyInputExtiInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA|RCC_APB_PERIPH_AFIO);
    }
    else if (GPIOx == GPIOB)
    {
		RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB|RCC_APB_PERIPH_AFIO);
    }
    else
    {
        return;
    }

    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_Structure_Initialize(&GPIO_InitStructure);
        GPIO_InitStructure.Pin          = Pin;
        GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
        GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);
    }
  
    /* Configure key EXTI line */
    EXTI_InitStructure.EXTI_Line    = KEY_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /* Set key input interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = KEY_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

/**
*\*\name    LedInit.
*\*\fun     Configures LED GPIO.
*\*\param  GPIOx :
*\*\          - GPIOA
*\*\          - GPIOB
*\*\param  Pin :
*\*\          - GPIO_PIN_0
*\*\          - GPIO_PIN_1
*\*\          - GPIO_PIN_2
*\*\          - GPIO_PIN_3
*\*\          - GPIO_PIN_4
*\*\          - GPIO_PIN_5
*\*\          - GPIO_PIN_6
*\*\          - GPIO_PIN_7
*\*\          - GPIO_PIN_8
*\*\          - GPIO_PIN_9
*\*\          - GPIO_PIN_10
*\*\          - GPIO_PIN_11
*\*\          - GPIO_PIN_12
*\*\          - GPIO_PIN_13
*\*\          - GPIO_PIN_14
*\*\          - GPIO_PIN_15
*\*\return  none
**/
void LedInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
    }
    else
    {
        return;
    }

    /* Configure the GPIO pin as output push-pull */
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_Structure_Initialize(&GPIO_InitStructure);
        GPIO_InitStructure.Pin = Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
        GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);
    }
}


/**
*\*\name    LedOn.
*\*\fun     Turns selected Led on.
*\*\param  GPIOx :
*\*\          - GPIOA
*\*\          - GPIOB
*\*\param  Pin :
*\*\          - GPIO_PIN_0
*\*\          - GPIO_PIN_1
*\*\          - GPIO_PIN_2
*\*\          - GPIO_PIN_3
*\*\          - GPIO_PIN_4
*\*\          - GPIO_PIN_5
*\*\          - GPIO_PIN_6
*\*\          - GPIO_PIN_7
*\*\          - GPIO_PIN_8
*\*\          - GPIO_PIN_9
*\*\          - GPIO_PIN_10
*\*\          - GPIO_PIN_11
*\*\          - GPIO_PIN_12
*\*\          - GPIO_PIN_13
*\*\          - GPIO_PIN_14
*\*\          - GPIO_PIN_15
*\*\return  none
**/
void LedOn(GPIO_Module* GPIOx, uint16_t Pin)
{    
    GPIO_Pins_Set(GPIOx, Pin);
}

/**
*\*\name    LedOff.
*\*\fun     Turns selected Led off.
*\*\param  GPIOx :
*\*\          - GPIOA
*\*\          - GPIOB
*\*\param  Pin :
*\*\          - GPIO_PIN_0
*\*\          - GPIO_PIN_1
*\*\          - GPIO_PIN_2
*\*\          - GPIO_PIN_3
*\*\          - GPIO_PIN_4
*\*\          - GPIO_PIN_5
*\*\          - GPIO_PIN_6
*\*\          - GPIO_PIN_7
*\*\          - GPIO_PIN_8
*\*\          - GPIO_PIN_9
*\*\          - GPIO_PIN_10
*\*\          - GPIO_PIN_11
*\*\          - GPIO_PIN_12
*\*\          - GPIO_PIN_13
*\*\          - GPIO_PIN_14
*\*\          - GPIO_PIN_15
*\*\return  none
**/
void LedOff(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_PBC_Pins_Reset(GPIOx, Pin);
}

/**
*\*\name    LedBlink.
*\*\fun     Toggles the selected Led.
*\*\param  GPIOx :
*\*\          - GPIOA
*\*\          - GPIOB
*\*\param  Pin :
*\*\          - GPIO_PIN_0
*\*\          - GPIO_PIN_1
*\*\          - GPIO_PIN_2
*\*\          - GPIO_PIN_3
*\*\          - GPIO_PIN_4
*\*\          - GPIO_PIN_5
*\*\          - GPIO_PIN_6
*\*\          - GPIO_PIN_7
*\*\          - GPIO_PIN_8
*\*\          - GPIO_PIN_9
*\*\          - GPIO_PIN_10
*\*\          - GPIO_PIN_11
*\*\          - GPIO_PIN_12
*\*\          - GPIO_PIN_13
*\*\          - GPIO_PIN_14
*\*\          - GPIO_PIN_15
*\*\return  none
**/
void LedBlink(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_Pin_Toggle(GPIOx, Pin);
}



