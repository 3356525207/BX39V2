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
*\*\file misc.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "misc.h"

/** MISC Driving Functions Declaration **/

/**
*\*\name    NVIC_Initializes
*\*\fun     Initializes the NVIC peripheral according to the specified parameters in the NVIC_structure_initializes.
*\*\param   NVIC_structure_initializes :
*\*\              - NVIC_IRQChannel :
*\*\                   -  IRQn_Type    N32G003 Interrupt Number Definition
*\*\              - NVIC_IRQChannelPriority :
*\*\                   -  NVIC_PRIORITY_0
*\*\                   -  NVIC_PRIORITY_1
*\*\                   -  NVIC_PRIORITY_2
*\*\                   -  NVIC_PRIORITY_3
*\*\              - NVIC_IRQChannelCmd :
*\*\                   -  ENABLE
*\*\                   -  DISABLE
*\*\return  none
**/
void NVIC_Initializes(NVIC_InitType* NVIC_structure_initializes)
{
    /* Set Interrupt Priority */
    NVIC_SetPriority(NVIC_structure_initializes->NVIC_IRQChannel, NVIC_structure_initializes->NVIC_IRQChannelPriority);

    if (NVIC_structure_initializes->NVIC_IRQChannelCmd != DISABLE)
    {
        /* Enable the Selected IRQ Channels */
        NVIC_EnableIRQ(NVIC_structure_initializes->NVIC_IRQChannel);
    }
    else
    {
        /* Disable the Selected IRQ Channels */
        NVIC_DisableIRQ(NVIC_structure_initializes->NVIC_IRQChannel);
    }
}

/**
*\*\name    NVIC_System_Low_Power_Enable
*\*\fun     Selects the condition for the system to enter low power mode.
*\*\param   low_power_mode Specifies the new mode for the system to enter low power mode.
*\*\        This parameter can be one of the following values:
*\*\            - NVIC_LP_SEVONPEND
*\*\            - NVIC_LP_SLEEPDEEP
*\*\            - NVIC_LP_SLEEPONEXIT
*\*\return  none
**/
void NVIC_System_LowPower_Enable(uint8_t low_power_mode)
{
    SCB->SCR |= low_power_mode;
}

/**
*\*\name    NVIC_System_Low_Power_Disable
*\*\fun     Selects the condition for the system to enter low power mode.
*\*\param   low_power_mode :
*\*\            - NVIC_LP_SEVONPEND
*\*\            - NVIC_LP_SLEEPDEEP
*\*\            - NVIC_LP_SLEEPONEXIT
*\*\return  none
**/
void NVIC_System_Low_Power_Disable(uint8_t low_power_mode)
{
    SCB->SCR &= (uint32_t)(~(uint32_t)low_power_mode);
}

/**
*\*\name  SysTick_Clock_Source_Set
*\*\fun   Configures the SysTick clock source.
*\*\param systick_clock_source :
*\*\       - SYSTICK_CLKSOURCE_HCLK_DIV8    External clock selected as SysTick clock source.
*\*\       - SYSTICK_CLKSOURCE_HCLK         AHB clock selected as SysTick clock source.
*\*\return  none
**/
void SysTick_Clock_Source_Set(uint32_t systick_clock_source)
{
    if (systick_clock_source == SYSTICK_CLKSOURCE_HCLK)
    {
        SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;
    }
    else
    {
        SysTick->CTRL &= SYSTICK_CLKSOURCE_HCLK_DIV8;
    }
}

