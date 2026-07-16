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
*\*\file bsp_SWD.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/


#include "bsp_SWD.h"

/**
 *\*\name   SWD_Function_Initialize.
 *\*\fun    Configures as SWD function.
 *\*\param  none.
 *\*\return none.
**/
void SWD_Function_Initialize(void)
{
    /* Define a structure of type GPIO_InitType */
    GPIO_InitType GPIO_InitStructure;

    RCC_APB_Peripheral_Clock_Enable(SWDIO_CLK);

    /* Assign default value to GPIO_InitStructure structure */
    GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* Configure PA8(SWDIO) as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = SWDIO_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SWDIO;
    /* Initialize GPIO */
    GPIO_Peripheral_Initialize(SWDIO_PORT, &GPIO_InitStructure);

    /* Configure PA9(SWCLK) as alternate function push-pull and pull-down */
    GPIO_InitStructure.Pin            = SWCLK_PIN;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_DOWN;
    /* Initialize GPIO */
    GPIO_Peripheral_Initialize(SWCLK_PORT, &GPIO_InitStructure);
}


/**
 *\*\name   SWD_As_GPIO_Initialize.
 *\*\fun    Configures SWD as GPIO
 *\*\param  none.
 *\*\return none.
**/
void SWD_As_GPIO_Initialize(void)
{
    /* Define a structure of type GPIO_InitType */
    GPIO_InitType GPIO_InitStructure;

    RCC_APB_Peripheral_Clock_Enable(SWDIO_CLK);

    /* Assign default value to GPIO_InitStructure structure */
    GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* Configure PA8(SWDIO) and PA9(SWCLK) as gpio */
    GPIO_InitStructure.Pin            = SWDIO_PIN | SWCLK_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;
    /* Initialize GPIO */
    GPIO_Peripheral_Initialize(SWCLK_PORT, &GPIO_InitStructure);
}



