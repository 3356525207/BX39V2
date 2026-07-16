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
__IO uint16_t ADCConvertedValue;

void RCC_Configuration(void);
uint16_t Get_Data(void);

/**
*\*\name    ADC_Initial.
*\*\fun     ADC_Initial program.
*\*\return  none
**/
void ADC_Initial(void)
{
	ADC_Initializes_Structure(&ADC_InitStructure);
    /* ADC configuration ------------------------------------------------------*/
    ADC_InitStructure.MultiChEn      = DISABLE;
    ADC_InitStructure.ContinueConvEn = ENABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;
    ADC_Initializes(&ADC_InitStructure);
	
	/* ADC1 enable Vrefint  */
    ADC_Internal_Input_Buffer_Enable();
	/*ADC_Regular_Sequence_Single_Config  ADC_Channel_09_VBG */
	ADC_Regular_Sequence_Single_Config(ADC_Channel_09_VBG);
	/* Configure channel sample time ----------------------*/
	ADC_Channel_Sample_Time_Config(ADC_SAMP_TIME_380CYCLES);
    /* Enable ADC */
    ADC_ON();
    /* Check ADC Ready */
    while(ADC_Flag_Status_Get(ADC_RD_FLAG,ADC_FLAG_STR,ADC_FLAG_RDY) == RESET)
        ;
    while(ADC_Flag_Status_Get(ADC_RD_FLAG,ADC_FLAG_STR,ADC_FLAG_PD_RDY))
		;
	/*wait ADC Vrefint is ready to use*/
    while(!ADC_Flag_Status_Get(ADC_RD_FLAG,ADC_FLAG_STR,ADC_FLAG_BUF_RDY))
        ;	
	
	/* Start ADC Software Conversion */
	ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);
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
	
	/* ADC configuration ------------------------------------------------------*/
    ADC_Initial(); 
	
    while (1)
    {
		ADCConvertedValue = Get_Data();
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

    /* Enable ADC clocks */
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_ADC);
	RCC_ADC_Clock_Config(RCC_ADC_DIV12);
	
    /* enable ADC 1M clock */
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48);
}


/**
*\*\name    Get_Data.
*\*\fun     Get ADC conversion results.
*\*\return  ADC conversion results
**/
uint16_t Get_Data(void)
{
	uint16_t data = 0;
	
	/* wait end of conversion flag-------------------------*/
	while(ADC_Flag_Status_Get(ADC_RUN_FLAG ,ADC_FLAG_ENDCA ,ADC_FLAG_RDY)==0){
		}
	ADC_Flag_Status_Clear(ADC_FLAG_ENDCA);
	ADC_Flag_Status_Clear(ADC_FLAG_STR);
	data= ADC_Regular_Conversion_Data_Get();
	return data;
}

