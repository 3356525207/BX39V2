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
*\*\file      bsp_spi_master.c
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
*/

#include "bsp_spi_master.h"

/**
*\*\name    SPI_RCC_Configuration
*\*\fun     Enable SPI clock
*\*\param   none
*\*\return  none
**/
static void SPI_RCC_Configuration(void)
{
    /* Enable SPI Clock */
    RCC_APB_Peripheral_Clock_Enable(SPI_MASTER_PERIPH);

    /* Enable SPI_NSS and SPI_SCK Port Clock */
    RCC_APB_Peripheral_Clock_Enable(SPI_MASTER_SCK_CLK);

    /* Enable SPI_MOSI Port Clock */
    RCC_APB_Peripheral_Clock_Enable(SPI_MASTER_MOSI_CLK);

    /* Enable AFIO Clock */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_AFIO);
}


/**
*\*\name    SPI_GPIO_Configuration
*\*\fun     Config SPI port
*\*\param   none
*\*\return  none
**/
static void SPI_GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* Configure Master Pins NSS as Alternate Function Push-pull */
    GPIO_InitStructure.Pin            = SPI_MASTER_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_NSS_AF;
    GPIO_Peripheral_Initialize(SPI_MASTER_NSS_GPIO, &GPIO_InitStructure);

    /* Set NSS mode as high-impedance when idle */
    AFIO_SPI_NSS_Mode_Set(AFIO_SPI_NSS_HIGH_IMPEDANCE);

    /* Configure Master Pins SCK as Alternate Function Push-pull */
    GPIO_InitStructure.Pin            = SPI_MASTER_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_SCK_AF;
    GPIO_Peripheral_Initialize(SPI_MASTER_SCK_GPIO, &GPIO_InitStructure);

    /* Configure Master Pins MOSI as Alternate Function Push-pull */
    GPIO_InitStructure.Pin            = SPI_MASTER_MOSI_PIN;
    GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_MOSI_AF;
    GPIO_Peripheral_Initialize(SPI_MASTER_MOSI_GPIO, &GPIO_InitStructure);
}

/**
*\*\name    SPI_NVIC_Configuration
*\*\fun     Config SPI NVIC
*\*\param   none
*\*\return  none
**/
static void SPI_NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel         = SPI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}


/**
*\*\name    SPI_NVIC_Configuration
*\*\fun     Config SPI NVIC
*\*\param   none
*\*\return  none
**/
void SPI_Configuration(void)
{
    SPI_InitType SPI_InitStructure;

    /* SPI clock configuration */
    SPI_RCC_Configuration();

    /* GPIO configuration */
    SPI_GPIO_Configuration();

    SPI_Initializes_Structure(&SPI_InitStructure);

    /* SPI configuration */
    SPI_InitStructure.DataDirection = SPI_DIR_SINGLELINE_TX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_16BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_HARD;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_4;
    SPI_Initializes(&SPI_InitStructure);

    /* Enable NSS output */
    SPI_NSS_Output_Enable();

    /* NVIC configuration */
    SPI_NVIC_Configuration();

    /* Enable Master TE Interrupt */
    SPI_Interrupts_Enable(SPI_INT_TE);

    /* Enable SPI Master */
    SPI_ON();
}
