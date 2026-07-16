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
*\*\file bsp_led.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "bsp_led.h"

/**
 *\*\name   LED_Initialize.
 *\*\fun    Initialize the specified LED.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
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
 *\*\return  none.
**/
void LED_Initialize(GPIO_Module* GPIOx, uint16_t pin)
{
	/* Define a structure of type GPIO_InitType */
	GPIO_InitType GPIO_InitStructure;
	
	/* Enable LED related GPIO peripheral clock */
	if(GPIOx == GPIOA)
	{
		RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
	}
	else if(GPIOx == GPIOB)
	{
		RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
	}
	else
	{
		/* ... */
	}
	
	if(pin < GPIO_PIN_ALL)
	{
		/* Assign default value to GPIO_InitStructure structure */
		GPIO_Structure_Initialize(&GPIO_InitStructure);
		
		/* Select the GPIO pin to control */
		GPIO_InitStructure.Pin          = pin;
		/* Set pin mode to general push-pull output */
		GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_OUT_PP;
		/* Set the pin drive current to 4MA*/
		GPIO_InitStructure.GPIO_Current = GPIO_HIGH_DREIVE;
		/* Initialize GPIO */
		GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);
	}
}

/**
 *\*\name   LED_Toggle.
 *\*\fun    GPIOx Specifies the led port to be toggled.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
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
**/
void LED_Toggle(GPIO_Module* GPIOx, uint16_t pin)
{
    GPIO_Pin_Toggle(GPIOx, pin);
}


/**
 *\*\name   LED_On.
 *\*\fun    GPIOx Specifies the led port to be set on.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
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
**/
void LED_On(GPIO_Module* GPIOx,uint16_t pin)
{
    GPIO_Pins_Set(GPIOx, pin);
}


/**
 *\*\name   LED_Off.
 *\*\fun    GPIOx Specifies the led port to be set off.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
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
**/
void LED_Off(GPIO_Module* GPIOx,uint16_t pin)
{
    GPIO_Pins_Reset(GPIOx, pin);
}

