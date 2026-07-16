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
#include "bsp_delay.h"


#define COMPX_FILC_SAMPW_30       	30
#define COMPX_FILC_THRESH_16		16

void RCC_Configuration(void);
void GPIO_Configuration(void);
void COMP_Configuratoin(void);
void ChangeVmVp(void);

/**
*\*\name    main.
*\*\fun     main program.
*\*\return  none
**/
int main(void)
{
	/* cannot be commented out -------------------------------------------------*/
	SysTick_Delay_Ms(5*1000);
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();

    /* COMP configuration ------------------------------------------------------*/
    COMP_Configuratoin();
    while (1)
    {
        ChangeVmVp();
    }
}

/**
*\*\name    ChangeVmVp.
*\*\fun     Self Generate Puls ,by skip line connect to vp and vm if need.
*\*\return  none
**/
void ChangeVmVp(void)
{
    GPIO_Pins_Set(GPIOA,GPIO_PIN_12);
	GPIO_Pins_Reset(GPIOA,GPIO_PIN_13);
    {
        uint32_t i = 0;
        while (i++ < 1000)
            ;
    }
	GPIO_Pins_Reset(GPIOA,GPIO_PIN_12);
	GPIO_Pins_Set(GPIOA,GPIO_PIN_13);
    {
        uint32_t i = 0;
        while (i++ < 1000)
            ;
    }
}

/**
*\*\name    COMP_Configuratoin.
*\*\fun     Configures the COMP.
*\*\return  none
**/
void COMP_Configuratoin(void)
{
    COMP_InitType COMP_Initial;

    /*Initial comp*/
    COMP_Initializes_Structure(&COMP_Initial);
    COMP_Initial.InpSel     = COMP_INPSEL_PB0;
    COMP_Initial.InmSel     = COMP_INMSEL_PB1;
	COMP_Initial.SubSel     = COMP_SUB_NO;
	
	COMP_Initial.FilterEn	= ENABLE;
    COMP_Initial.SampWindow = COMPX_FILC_SAMPW_30;
    COMP_Initial.Threshold  = COMPX_FILC_THRESH_16;
	COMP_Initial.En			= DISABLE;
    COMP_Initializes( &COMP_Initial);
	
	/*Enable comp*/
    COMP_ON();
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
	/* Enable System clocks */
	RCC_Sysclk_Config(RCC_HSICLK_DIV1);
	/* Enable COMP,COMPFILT clocks */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_COMP | RCC_APB_PERIPH_COMP_FILT );

    /* Enable GPIOA, GPIOB, AFIO */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA | RCC_APB_PERIPH_IOPB | RCC_APB_PERIPH_AFIO);
}

/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\return  none
**/
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    // INP PB0
    GPIO_InitStructure.Pin            = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_ANALOG;
    GPIO_InitStructure.GPIO_Current   = GPIO_LOW_DREIVE;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    // INM PB1
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);

    // OutSel
    // PA8
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin            = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_COMP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    // GPIO PA12 PA13
    GPIO_InitStructure.Pin            = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Current   = GPIO_LOW_DREIVE;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}



