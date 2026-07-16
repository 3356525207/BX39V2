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


ADC_InitType ADC_InitStructure;
__IO uint16_t ADCConvertedValue[5] = {0};

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);

/**
*\*\name    ADC_Initial.
*\*\fun     ADC_Initial program.
*\*\return  none
**/
void ADC_Initial(void)
{
    ADC_Initializes_Structure(&ADC_InitStructure);
    /* ADC configuration ------------------------------------------------------*/
    ADC_InitStructure.MultiChEn      = ENABLE;
    ADC_InitStructure.ContinueConvEn = DISABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_EXT_INT11;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_2;
    ADC_Initializes(&ADC_InitStructure);
	
	ADC_Regular_Sequence_Multi_Config(ADC_Channel_01_PA2,0);
	ADC_Regular_Sequence_Multi_Config(ADC_Channel_02_PA5,1);

	/* Configure channel sample time ----------------------*/
	ADC_Channel_Sample_Time_Config(ADC_SAMP_TIME_8CYCLES);
	
	/* Enable ENDC interrupt */
    ADC_Interrupts_Enable(ADC_SET_INT_TYPE_OTHER, ADC_INT_ENDC);
	
	/* Enable ADC external trigger */
	ADC_External_Trigger_Conversion_Config(ADC_EXTTRIGCONV_REGULAR_ENABLE);
	
    /* Enable ADC */
    ADC_ON();
	
    /* Check ADC Ready */
    while(ADC_Flag_Status_Get(ADC_RD_FLAG,ADC_FLAG_STR,ADC_FLAG_RDY) == RESET)
        ;
    while(ADC_Flag_Status_Get(ADC_RD_FLAG,ADC_FLAG_STR,ADC_FLAG_PD_RDY))
		;	
}

/**
*\*\name    Change_ExtiLevel.
*\*\fun     Generate the period signal for the exti line.
*\*\return  none
**/
void Change_ExtiLevel(void)
{
    uint32_t i = 0;
    GPIO_Pins_Set(GPIOA, GPIO_PIN_4);
    {
        i = 0;
        while (i++ < 100)
            ;
    }
    GPIO_Pins_Reset(GPIOA, GPIO_PIN_4);
    {
        i = 0;
        while (i++ < 10000)
            ;
    }
}
/**
*\*\name    main.
*\*\fun     main program.
*\*\return  none
**/
int main(void)
{
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();
	
	/* NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();
	
	/* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();
	
	/* EXTI configuration ------------------------------------------------------*/
	EXTI_Configuration();
	
	/* ADC configuration -------------------------------------------------------*/
    ADC_Initial(); 
	
	
    while (1)
    {
		Change_ExtiLevel();
    }
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
	/* RCC_ADCHCLK_DIV1*/
	RCC_Hclk_Config(RCC_SYSCLK_DIV1);
    /* Enable GPIOA clocks */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA | RCC_APB_PERIPH_AFIO);  
    /* Enable ADC clocks */
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_ADC);
	RCC_ADC_Clock_Config(RCC_ADC_DIV2);
    /* enable ADC 1M clock */
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48);
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
    /* Configure PA2 as analog input -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	
	/* Configure PA6 as Output Mode -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_6  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	
	/* Configure PA7 as Input Mode -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.GPIO_Pull = GPIO_PULL_DOWN;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    /* Configure PA4 as output Mode -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_4 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name    EXTI_Configuration.
*\*\fun     Configures the different EXTI lines.
*\*\return  none
**/
void EXTI_Configuration(void)
{
    EXTI_InitType EXTI_InitStructure;

    /* Select the EXTI Line7 the GPIO pin source */
    AFIO_ADC_External_Trigger_Set(AFIO_ADC_TRIG_PA7);
    /* EXTI line7 configuration -----------------------------------------------*/
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE7;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures NVIC and Vector Table base location.
*\*\return  none
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}



