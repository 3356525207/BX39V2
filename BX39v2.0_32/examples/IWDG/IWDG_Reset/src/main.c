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
#include "bsp_led.h"
#include "bsp_iwdg.h"
#include "bsp_delay.h"


/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
	uint8_t temp = 0;
	
	log_init();
	log_info("\r\n IWDG Demo Reset \r\n");
	
	/* Initialize the LEDs */
	LED_Initialize(LED1_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN);
	
	/* Debug mode IWDG stopped */
	DBG_Peripheral_ON(PWR_DBG_IWDG);
	
	/* Check if the system has resumed from IWDG reset */
    if(RCC_Flag_Status_Get(RCC_FLAG_IWDGRST) != RESET)
	{
		/* IWDGRST flag set */
        /* Turn On LED1 */
        LED_On(LED1_GPIO_PORT, LED1_GPIO_PIN);
        log_info("\r\n Reset By IWDG \r\n");
		
		/* Clear reset flags */
		RCC_Reset_Flag_Clear();
	}		
	else 
	{
		/* IWDG Reset flag is not set */
        /* Turn Off LED1 */
        LED_Off(LED1_GPIO_PORT, LED1_GPIO_PIN);
	}
	
	LED_Off(LED2_GPIO_PORT, LED2_GPIO_PIN);
	
	/* Timeout is equal to 1 / 32000 * 128 * 4080 = ~16s */
	IWDG_Config(IWDG_CONFIG_PRESCALER_DIV128, 0xFF0);  
	
	while(temp < 3)
	{
		/* Toggle LED2 */
		LED_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
		
		/* Delay 14s */
		SysTick_Delay_Ms(14000);
		
		log_info("\r\n Feed the dog i = %d times.\r\n", (temp + 1));
		/* Feed the dog */
		IWDG_Feed();
		
		temp++;
	}
	
	log_info("\r\n Don't feed the dog, after 16s system reset.\r\n");

	while(1)
	{
		/* Turn Off LED2 */
        LED_Off(LED2_GPIO_PORT, LED2_GPIO_PIN);
	}
}





