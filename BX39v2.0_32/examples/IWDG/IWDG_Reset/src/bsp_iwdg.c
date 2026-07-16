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
*\*\file bsp_iwdg.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "bsp_iwdg.h"

/**
 *\*\name   IWDG_Config.
 *\*\fun    Configure IWDG.
 *\*\param   IWDG_prescaler :
 *\*\          - IWDG_CONFIG_PRESCALER_DIV4 
 *\*\          - IWDG_CONFIG_PRESCALER_DIV8
 *\*\          - IWDG_CONFIG_PRESCALER_DIV16
 *\*\          - IWDG_CONFIG_PRESCALER_DIV32
 *\*\          - IWDG_CONFIG_PRESCALER_DIV64
 *\*\          - IWDG_CONFIG_PRESCALER_DIV128
 *\*\          - IWDG_CONFIG_PRESCALER_DIV256
 *\*\param   reload_value :
 *\*\          -0x000 ~ 0xFFF
**/
void IWDG_Config(IWDG_CONFIG_PRESCALER IWDG_prescaler, uint16_t reload_value)
{
	/* The timeout may varies due to LSI frequency dispersion */
	/* Disable write protection to IWDG_PREDIV and IWDG_RELV registers */
	IWDG_Write_Protection_Disable();
	
	/* IWDG counter clock */
	IWDG_Prescaler_Division_Set(IWDG_prescaler);
	
	/* Sets IWDG reload value */
	/* Set counter reload value to obtain x second IWDG TimeOut.
     Counter Reload Value Time = x(second)/IWDG counter clock period
                               = x(second) / (LSI/IWDG_prescaler)
    */
	IWDG_Counter_Reload(reload_value);
	
	/* Reload counter with IWDG_PREDIV value in IWDG_RELV register to prevent reset */
	IWDG_Key_Reload();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

/**
 *\*\name    IWDG_Feed.
 *\*\fun     Feed the dog.
 *\*\param   none
 *\*\return  none
**/
void IWDG_Feed(void)
{
	/* Put the value of the reload register into the counter */
	IWDG_Key_Reload();
}

