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
#include "n32g003.h"
#include "log.h"


RCC_ClocksType RCC_ClockFreq;

#define SYSCLK_SRC_HSI48M     0
#define SYSCLK_SRC_HSI40M     1


/**
*\*\name   PrintfClockInfo.
*\*\fun    Print clock information.
*\*\param  none  
*\*\return  none. 
**/
void PrintfClockInfo(const char* msg)
{
     uint32_t TimeDelay = 0xFFFF;
    /* reinit after sysclk changed */
    log_init(); 
    log_info("-----------------\n");
    log_info("%s:\n", msg);
    RCC_Clocks_Frequencies_Value_Get(&RCC_ClockFreq);
    log_info("SYSCLK: %d\n", RCC_ClockFreq.SysclkFreq);
    log_info("HCLK: %d\n", RCC_ClockFreq.HclkFreq);
    log_info("PCLK: %d\n", RCC_ClockFreq.PclkFreq);
    /* Wait for the data to be sent */
    while(--TimeDelay) 
    {
    }
}

/**
*\*\name   RCC_MCO_Pin_Config.
*\*\fun    Configure MCO pin.
*\*\param  none  
*\*\return  none. 
**/
void RCC_MCO_Pin_Config(void)                    
{
    GPIO_InitType GPIO_InitStructure;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA|RCC_APB_PERIPH_AFIO);	
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_NO_PULL;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_MCO;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name   SetSysClock.
*\*\brief  Selects HSI as System clock source and configure HCLK, PCLK
*\*\         prescalers.
*\*\parm  src:
*\*\        - SYSCLK_SRC_HSI48M 
*\*\        - SYSCLK_SRC_HSI40M 
*\*\parm  freq(Hz):
*\*\        - 48000000 
*\*\        - 40000000
*\*\        - 24000000 
*\*\        - 8000000 
**/
void SetSysClock(uint8_t src, uint32_t freq)
{
    uint32_t status_value = 0, counter_value = 0;
    uint32_t sys_div = 0;
	
    
    /* Set Latency before switching the clock to prevent the chip flying  */
    FLASH_Latency_Set(FLASH_LATENCY_1);
    switch (freq)
    {
      case 8000000:
        if(src == SYSCLK_SRC_HSI40M)
        {
            sys_div = RCC_HSICLK_DIV5;
        }
        else
        {
            sys_div = RCC_HSICLK_DIV6;
        }
        break;
      case 24000000:
        sys_div = RCC_HSICLK_DIV2;
        break;
      case 40000000:
        sys_div = RCC_HSICLK_DIV1;
        break;
      case 48000000:
        sys_div = RCC_HSICLK_DIV1;
        break;
      default:
        /* Input parameter out of range */
        while(1);
       
    }	
        
    /* HCLK = SYSCLK */
    RCC_Hclk_Config(RCC_SYSCLK_DIV1);
    /* PCLK = HCLK */
    RCC_Pclk_Config(RCC_HCLK_DIV1);

    if(src == SYSCLK_SRC_HSI48M)
	{

        /* Select HSI 48M*/
        PWR->CTRL3 &= ~PWR_CTRL3_HSISEL;	//48M

        /* Wait till HSI is ready and if Time out is reached exit */
        do
        {
                status_value = RCC->HSICTRL & RCC_HSICTRL_HSI48MRDF;
                counter_value++;
        } while ((status_value == 0) && (counter_value != HSI_STARTUP_TIMEOUT));

        if(status_value == RCC_HSICTRL_HSI48MRDF)
        {
             RCC_Sysclk_Config(sys_div);
        }
        else
        {
            /* clock source fails to start-up, user can add here some code to deal with this error */
            while(1);
        }
    
	}
    else if(src == SYSCLK_SRC_HSI40M)
	{
        /* Select HSI 40M */
        PWR->CTRL3 |= PWR_CTRL3_HSISEL; //40M

        /* Wait till HSI is ready and if Time out is reached exit */
        do
        {
                status_value = RCC->HSICTRL & RCC_HSICTRL_HSI40MRDF;
                counter_value++;
        } while ((status_value == 0) && (counter_value != HSI_STARTUP_TIMEOUT));

        if(status_value == RCC_HSICTRL_HSI40MRDF)
        {
             RCC_Sysclk_Config(sys_div);
                            
        }
        else
        {
            /* clock source fails to start-up, user can add here some code to deal with this error */
            while(1);
        }
	 }
	 else
	 {
         /* Input parameter out of range */
		 while(1);
	 }
     
     /* Flash wait state 0: SYSCLK <= 24M 1: SYSCLK <= 48M */
    if(freq <= SYSCLK_FREQ_24M)
    {
        /*8M\24M */
        FLASH_Latency_Set(FLASH_LATENCY_0);
    }
    else
    {
        /*40M\48M */
        FLASH_Latency_Set(FLASH_LATENCY_1);
    }
}

int main(void)
{
    log_init();
    log_info("-----------------\nRCC_ClockConfig Demo.\n");
    PrintfClockInfo("After reset");
    
    /* Switch the system clock */
    SetSysClock(SYSCLK_SRC_HSI48M,24000000);
	PrintfClockInfo("HSI48M");
	
    /* Output HSI clock on MCO pin */
    RCC_MCO_Pin_Config();
    RCC_MCO_Prescaler_Config(RCC_MCO_CLK_DIV4); 
	RCC_MCO_Source_Config(RCC_MCO_HSI);
      
    while (1);
}


