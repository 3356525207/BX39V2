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
*\*\file bsp_led.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "n32g003.h"

//************************************************************************************//
/* Define the LED that is connected GPIO*/
// LED1
#define LED1_GPIO_PORT GPIOA
#define LED1_GPIO_CLK  RCC_APB_PERIPH_IOPA
#define LED1_GPIO_PIN  GPIO_PIN_6
// LED2
#define LED2_GPIO_PORT GPIOA
#define LED2_GPIO_CLK  RCC_APB_PERIPH_IOPA
#define LED2_GPIO_PIN  GPIO_PIN_7
// LED3
#define LED3_GPIO_PORT GPIOA
#define LED3_GPIO_CLK  RCC_APB_PERIPH_IOPA
#define LED3_GPIO_PIN  GPIO_PIN_10

#define digitalHi(p, i)                  \
    {                                    \
        p->PBSC = i;                     \
    } // output high
#define digitalLo(p, i)                  \
    {                                    \
        p->PBC = i;                      \
    } // output low
#define digitalToggle(p, i)              \
    {                                    \
        p->POD ^= i;                     \
    } // toggle

#define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF    digitalLo(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_ON     digitalHi(LED1_GPIO_PORT, LED1_GPIO_PIN)

#define LED2_TOGGLE digitalToggle(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF    digitalLo(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_ON     digitalHi(LED2_GPIO_PORT, LED2_GPIO_PIN)

#define LED3_TOGGLE digitalToggle(LED3_GPIO_PORT, LED3_GPIO_PIN)
//#define LED3_OFF    digitalLo(LED3_GPIO_PORT, LED3_GPIO_PIN)
//#define LED3_ON     digitalHi(LED3_GPIO_PORT, LED3_GPIO_PIN)

// In order to adapt to the N32G003F5S7-STB development board, 
// LED3 makes this special configuration.
#define LED3_OFF    digitalHi(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_ON     digitalLo(LED3_GPIO_PORT, LED3_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __BSP_LED_H__ */
