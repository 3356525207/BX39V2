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
*\*\file misc.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __MISC_H__
#define __MISC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g003.h"

/** MISC Driving Functions Declaration **/

/** NVIC Init Structure definition **/

typedef struct
{
    IRQn_Type NVIC_IRQChannel; /*!< Specifies the IRQ channel to be enabled or disabled.
                                  This parameter can be a value of @ref IRQn_Type
                                  (For the complete N32G003 Devices IRQ Channels list, please
                                   refer to n32g003.h file) */

    uint8_t NVIC_IRQChannelPriority; /*!< Specifies the priority for the IRQ channel
                                          specified in NVIC_IRQChannel. This parameter can be a value
                                          between 0 and 3 */

    FunctionalState NVIC_IRQChannelCmd; /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                             will be enabled or disabled.
                                             This parameter can be set either to ENABLE or DISABLE */
} NVIC_InitType;


/** MISC driver modules **/
#define AIRCR_VECTKEY_MASK       SCB_AIRCR_VECTKEY /* access key */

/** NVIC Priority **/
#define NVIC_PRIORITY_0          ((uint8_t)0x00)
#define NVIC_PRIORITY_1          ((uint8_t)0x01)
#define NVIC_PRIORITY_2          ((uint8_t)0x02)
#define NVIC_PRIORITY_3          ((uint8_t)0x03)

/** System_Low_Power **/
#define NVIC_LP_SEVONPEND        (SCB_SCR_SEVONPEND)
#define NVIC_LP_SLEEPDEEP        (SCB_SCR_SLEEPDEEP)
#define NVIC_LP_SLEEPONEXIT      (SCB_SCR_SLEEPONEXIT)

/** SysTick_clock_source **/
#define SYSTICK_CLKSOURCE_HCLK_DIV8  (~SysTick_CTRL_CLKSOURCE)
#define SYSTICK_CLKSOURCE_HCLK       (SysTick_CTRL_CLKSOURCE)


void NVIC_Initializes(NVIC_InitType* NVIC_structure_initializes);
void NVIC_System_LowPower_Enable(uint8_t low_power_mode);
void NVIC_System_Low_Power_Disable(uint8_t low_power_mode);
void SysTick_Clock_Source_Set(uint32_t systick_clock_source);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */


