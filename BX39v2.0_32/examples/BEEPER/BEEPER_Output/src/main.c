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
#include "log.h"
#include "n32g003_beeper.h"

/**
 *\*\name   BEEPER_GPIO_Config.
 *\*\fun    BEEPER's GPIO configuration.
 *\*\param  none.
 *\*\return none.
**/
static void BEEPER_GPIO_Config(void)
{
	GPIO_InitType GPIO_InitStructure;
	
    /* Enable BEEPER clock */
	RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_BEEPER);
	/* Turn on the port clock of the GPIO that controls the beeper */
	RCC_APB_Peripheral_Clock_Enable(BEEPER_GPIO_CLK | BEEPER_N_GPIO_CLK);
	
	GPIO_Structure_Initialize(&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin            = BEEPER_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.GPIO_Alternate = BEEPER_GPIO_AF;
	GPIO_Peripheral_Initialize(BEEPER_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin            = BEEPER_N_GPIO_PIN;
	GPIO_InitStructure.GPIO_Alternate = BEEPER_N_GPIO_AF;
	GPIO_Peripheral_Initialize(BEEPER_N_GPIO_PORT, &GPIO_InitStructure);
}

/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
    uint32_t TimeDelay = 0xFFFFFF;
    
	log_init();
	log_info("BEEPER Output Test Start\r\n");
    
    /* BEEPER clock and GPIO initialization */
    BEEPER_GPIO_Config();
	
	/* BEEPER Frequency Select, Clock source is LSI 32K */
	BEEPER_Frequency_Select(BEEPER_FREQ_4KHZ);
	
	/* Enable complementary output */
	BEEPER_Inverted_Enable();
	
	/* Enable BEEPER */
	BEEPER_Enable();
	
	/* Delay */
    while(--TimeDelay) 
    {
    }
		
	/* Disable complementary output */
	BEEPER_Inverted_Disable();
    
    TimeDelay = 0xFFFFFF;
    /* Delay */
    while(--TimeDelay) 
    {
    }
	
	/* Disable BEEPER */
	BEEPER_Disable();
		
	log_info("BEEPER Output Test End\r\n");
	
	while(1)
	{
		
	}
}

