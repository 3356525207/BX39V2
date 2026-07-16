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
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_SWD.h"


/**
 *\*\name   Delay.
 *\*\fun    Inserts a delay time.
 *\*\param  count:
 *\*\          - any number
 *\*\return none.
**/
void Delay(uint32_t count)
{
    for (; count > 0; count--)
        ;
}

/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
    /*Initialize Led1 and Led2 as output push-pull mode*/
    LED_Initialize(LED1_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN);

    Key_Input_Initialize(KEY1_PORT, KEY1_PIN);

    while(1)
    {
        if(GPIO_Input_Pin_Data_Get(KEY1_PORT, KEY1_PIN) == PIN_RESET)
        {
            /* Turn on Led1 */
            LED_On(LED1_GPIO_PORT, LED1_GPIO_PIN);
            
            /* Turn off Led2 */
            LED_Off(LED2_GPIO_PORT, LED2_GPIO_PIN);
            
            /* Disable the SWD Debug Port */
            SWD_As_GPIO_Initialize();
            
            while(1)
            {
                /* Toggle SWDIO pin */
                if(1 - GPIO_Input_Pin_Data_Get(SWDIO_PORT, SWDIO_PIN))
                {
                    GPIO_Pins_Set(SWDIO_PORT, SWDIO_PIN);
                    
                }
                else 
                {
                    GPIO_PBC_Pins_Reset(SWDIO_PORT, SWDIO_PIN);
                }
                /* Insert delay */
                Delay(0x8FFFF);
                
                /* Toggle SWCLK pin */
                if(1 - GPIO_Input_Pin_Data_Get(SWCLK_PORT, SWCLK_PIN))
                {
                    GPIO_Pins_Set(SWCLK_PORT, SWCLK_PIN);
                }
                else 
                {
                    GPIO_PBC_Pins_Reset(SWCLK_PORT, SWCLK_PIN);
                }
                /* Insert delay */
                Delay(0x8FFFF);
                
                if(GPIO_Input_Pin_Data_Get(KEY1_PORT, KEY1_PIN) == PIN_SET)
                {
                    break;
                }
            }
        }
        else
        {
            /* Enablethe SWD Debug Port */
            SWD_Function_Initialize();
            
            /* Turn on Led2 */
            LED_On(LED2_GPIO_PORT, LED2_GPIO_PIN);
            
            /* Turn off Led1 */
            LED_Off(LED1_GPIO_PORT, LED1_GPIO_PIN);
            
            while(1)
            {
                if(GPIO_Input_Pin_Data_Get(KEY1_PORT, KEY1_PIN) == PIN_RESET)
                {
                    break;
                }
            }
        }
    }
}

