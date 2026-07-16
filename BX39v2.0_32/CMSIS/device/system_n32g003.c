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
*\*\file system_n32g003.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#include "n32g003.h"

/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)

   IMPORTANT NOTE:
   ==============
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your
   device's maximum frequency.

   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume
   that:
        - For Low, Medium and High density Value line devices an external 8MHz
          crystal is used to drive the System clock.
        - For Low, Medium and High density devices an external 8MHz crystal is
          used to drive the System clock.
        - For Connectivity line devices an external 25MHz crystal is used to
   drive the System clock. If you are using different crystal you have to adapt
   those functions accordingly.
    */

#define SYSCLK_USE_HSI48M     0
#define SYSCLK_USE_HSI40M     1

/*** Configure system clock frequency ***/
#ifndef SYSCLK_FREQ
#define SYSCLK_FREQ 48000000
#endif


/******* Select one ******
** SYSCLK_USE_HSI48M    **
** SYSCLK_USE_HSI40M  ***/
#ifndef SYSCLK_SRC
#define SYSCLK_SRC SYSCLK_USE_HSI48M
#endif

#if SYSCLK_SRC == SYSCLK_USE_HSI48M

    #if (SYSCLK_FREQ == SYSCLK_FREQ_48M) 
		
        #define SYS_DIV RCC_CFG_SYSPRES_DIV1
		
    #elif (SYSCLK_FREQ == SYSCLK_FREQ_24M) 

         #define SYS_DIV RCC_CFG_SYSPRES_DIV2
		 
    #elif (SYSCLK_FREQ == SYSCLK_FREQ_8M) 

         #define SYS_DIV RCC_CFG_SYSPRES_DIV6
		 
    #else
        #error Cannot make a prescaler to SYSCLK_FREQ.
    #endif

#elif SYSCLK_SRC == SYSCLK_USE_HSI40M

    #if (SYSCLK_FREQ == SYSCLK_FREQ_40M) 
		
        #define SYS_DIV RCC_CFG_SYSPRES_DIV1
		 
    #elif (SYSCLK_FREQ == SYSCLK_FREQ_8M) 

         #define SYS_DIV RCC_CFG_SYSPRES_DIV5
		 
    #else
        #error Cannot make a prescaler to SYSCLK_FREQ.
    #endif

#else
#error wrong value for SYSCLK_SRC
#endif

/* If you want to use VECT_TAB_SRAM, release the comment*/
/* #define VECT_TAB_SRAM  */

/* Vector Table base offset field. This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET 0x0 

/** Clock Definitions **/

/* System Clock Frequency (Core Clock) */
uint32_t SystemCoreClockFrequency = SYSCLK_FREQ; 

const uint8_t AHBPrescTable[16] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 12, 16, 24, 24,24};

static void System_Clock_Set(void);



/**
*\*\name    System_Initializes.
*\*\fun     Setup the microcontroller system
*\*\        Initialize the Embedded Flash Interface, update the
*\*\        SystemCoreClockFrequency variable.
*\*\param   none.
*\*\return  none
*\*\note    This function should be used only after reset.
**/
void System_Initializes(void)
{
   
	/* Reset the RCC clock configuration to the default reset state(for debug purpose) */
	/* Set HSI 48M bit */
	PWR->CTRL3 &= ~((uint16_t)0x0040);	//48M

	/* Set SYSPRES[1:0] bit */
	RCC->CFG |= (uint32_t)0x00003000;

	/* Reset MCO[1:0] APBPRES[2:0] AHBPRES[3:0] bit */
	RCC->CFG &= (uint32_t)0xF9FFF80F;

	/* Reset LSIDETEN bit */
	RCC->LSICTRL &= (uint32_t)0xFFFFFF7F;

	/* Reset TIM1CLKSEL TIM6CLKSEL ADCPRES[3:0]bit */
	RCC->CFG2 &= (uint32_t)0x3FFFFFF0;

	/* Set ADC1MPRES[1:0] ADCPRES[3:0]bit */
	RCC->CFG2 |= (uint32_t)0x00000C01;

	/* Enable Prefetch Buffer */
	FLASH->AC |= (uint32_t)(FLASH_AC_PRFTBFEN);

    /* Configure the System clock frequency, HCLK PCLK prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    System_Clock_Set();

#ifdef VECT_TAB_SRAM
    FLASH->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
    FLASH->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}

/**
*\*\name    System_Clock_Frequency_Update.
*\*\fun     Update SystemCoreClockFrequency variable according to Clock Register Values.
*\*\         The SystemCoreClockFrequency variable contains the core clock (HCLK), it can
*\*\         be used by the user application to setup the SysTick timer or
*\*\         configure other parameters.
*\*\param   none.
*\*\return  none
*\*\note    Each time the core clock (HCLK) changes, this function must be called
*\*\         to update SystemCoreClockFrequency variable value. Otherwise, any
*\*\         configuration based on this variable will be incorrect.
*\*\
*\*\note    The system frequency computed by this function is not the real
*\*\         frequency in the chip. It is calculated based on the predefined
*\*\         constant and the selected clock source.
*\*\         
**/
void System_Core_Clock_Frequency_Update(void)
{
    uint32_t temp_value = 0;
	uint32_t hclkdiv_value = 0;

    /* Get SYSCLK source
     * -------------------------------------------------------*/
    temp_value = PWR->CTRL3 & PWR_CTRL3_HSISEL;
		 
    switch (temp_value)
    {
        case PWR_CTRL3_HSISEL: /* HSI 40M used as system clock */
            if((RCC->CFG & RCC_CFG_SYSPRES) == RCC_CFG_SYSPRES_DIV1)
            {
                SystemCoreClockFrequency = SYSCLK_FREQ_40M;
            }
            else
            {
                SystemCoreClockFrequency = SYSCLK_FREQ_8M;
            }            
            break;
     
        default:/* HSI 48M used as system clock */
            if((RCC->CFG & RCC_CFG_SYSPRES) == RCC_CFG_SYSPRES_DIV1)
            {
                SystemCoreClockFrequency = SYSCLK_FREQ_48M;
            }
            else if((RCC->CFG & RCC_CFG_SYSPRES) == RCC_CFG_SYSPRES_DIV2)
            {
                SystemCoreClockFrequency = SYSCLK_FREQ_24M;
            }
            else
            {
                SystemCoreClockFrequency = SYSCLK_FREQ_8M;
            }
            break;
    }
		
	/* Compute HCLK clock frequency ----------------*/
    /* Get HCLK prescaler */
    hclkdiv_value = AHBPrescTable[((RCC->CFG & RCC_CFG_AHBPRES) >> 4)];
    /* HCLK clock frequency */
    SystemCoreClockFrequency /= hclkdiv_value;
    
}

/**
*\*\name    Systeminit_Flash_Latency_Set
*\*\fun     Sets the code latency value.
*\*\note    This function can be used for N32G003 devices.
*\*\param   flash_latency :
*\*\            - FLASH_AC_LATENCY_0    FLASH Zero Latency cycle, 0 < SYSCLK <= 24MHz      
*\*\            - FLASH_AC_LATENCY_1    FLASH One Latency cycle, 24MHz < SYSCLK <= 48MHz
*\*\return  none
**/

static void Systeminit_Flash_Latency_Set(uint32_t flash_latency)
{
    uint32_t temp_value = 0;

    /* Read the AC register */
    temp_value = FLASH->AC;

    /* Sets the Latency value */
    temp_value &= (~FLASH_LATENCY_MASK); 
    temp_value |= flash_latency;

    /* Write the AC register */
    FLASH->AC = temp_value;
}

/**
*\*\name    System_Clock_Set.
*\*\fun     Configures the System clock frequency, HCLK and PCLK prescalers.
*\*\param   none.
*\*\return  none
**/
static void System_Clock_Set(void)
{
    volatile uint32_t temp_value = 0,status_value = 0;
    volatile uint32_t counter_value = 0;
	
    /* Flash wait state 0: SYSCLK <= 24M 1: SYSCLK <= 48M */		
    if(SYSCLK_FREQ <= SYSCLK_FREQ_24M)
    {
        /*8M\24M */
        Systeminit_Flash_Latency_Set(FLASH_AC_LATENCY_0);
    }
    else
    {
        /*40M\48M */
        Systeminit_Flash_Latency_Set(FLASH_AC_LATENCY_1);
    }
		
	/* HCLK = SYSCLK */
    RCC->CFG |= (uint32_t)RCC_CFG_AHBPRES_DIV1;
		
	/* PCLK = HCLK */
    RCC->CFG |= (uint32_t)RCC_CFG_APBPRES_DIV1;

#if (SYSCLK_SRC == SYSCLK_USE_HSI48M)

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
       RCC_Sysclk_Config(SYS_DIV);
    }
    else
    {
        while(1);
    }
    

#elif (SYSCLK_SRC == SYSCLK_USE_HSI40M)

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
         RCC_Sysclk_Config(SYS_DIV);
    }
    else
    {
        while(1);
    }
#endif

}

