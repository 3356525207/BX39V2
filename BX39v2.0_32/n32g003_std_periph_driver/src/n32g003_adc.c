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
*\*\file n32g003_adc.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "n32g003_adc.h"
#include "n32g003_rcc.h"

static RCC_ClocksType RCC_clocks;

static void Delay_8ADC_Colck(uint32_t SysClk, uint32_t adc_clk);

/**
*\*\name    ADC_Reset.
*\*\fun     Reset the ADC registers.
*\*\return  none
**/
void ADC_Reset(void)
{
	RCC_Peripheral_Reset(RCC_RST_ADCRST);
}


/**
*\*\name    ADC_Multichannels_Enable.
*\*\fun     Configures the ADC according conversion is performed in
*\*\        Scan (multichannels) mode.
*\*\return  none
**/
void ADC_Multichannels_Enable(void)
{
    /* Set ADC_CTRL1 SCANMD bit */
    ADC->CTRL2 |= ADC_MULTCH_ENABLE;
}
/**
*\*\name    ADC_Multichannels_Disable.
*\*\fun     Configures the ADC according conversion is performed in
*\*\        Single (one channel) mode.
*\*\return  none
**/
void ADC_Multichannels_Disable(void)
{
    /* Clear ADC_CTRL1 SCANMD bit */
    ADC->CTRL2 &= ADC_MULTCH_DISABLE;
}

/**
*\*\name    ADC_Continue_Conversion_Enable.
*\*\fun     Configures the ADC according conversion is performed in
*\*\        Continuous mode.
*\*\return  none
**/
void ADC_Continue_Conversion_Enable(void)
{
    /* Set ADC_CTRL2 CTU bit */
    ADC->CTRL2 |= ADC_CTU_ENABLE;
}

/**
*\*\name    ADC_Continue_Conversion_Disable.
*\*\fun     Configures the ADC according conversion is performed in
*\*\        Single mode.
*\*\return  none
**/
void ADC_Continue_Conversion_Disable(void)
{
    /* Clear ADC_CTRL2 CTU bit */
    ADC->CTRL2 &= ADC_CTU_DISABLE;
}

/**
*\*\name    ADC_Regular_Group_External_Trigger_Source_Config.
*\*\fun     Configures the ADC external trigger source for regular group channels.
*\*\param   external_trigger_sources :
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC1                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC2                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC3                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC4                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_TRGO               
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T3_TRGO                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T3_CC1   
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T3_CC2   
*\*\          - ADC_EXT_TRIGCONV_REGULAR_EXT_INT11   
*\*\          - ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH              
*\*\return  none
**/
void ADC_Regular_Group_External_Trigger_Source_Config(uint32_t external_trigger_sources)
{
    /* Clear ADC_CTRL2 EXTRSEL[3:0] and EXTRTRIG bit */
    ADC->CTRL2 &= ADC_EXT_TRIGCONV_REGULAR_MASK;
    /* Set ADC_CTRL2 EXTRSEL[3:0] and EXTRTRIG bit */
    ADC->CTRL2 |= external_trigger_sources;
}

/**
*\*\name    ADC_Data_Alignment_Config.
*\*\fun     Configures the ADC data alignment is left or right.
*\*\param   data_alignment :
*\*\          - ADC_DAT_ALIGN_R
*\*\          - ADC_DAT_ALIGN_L
*\*\return  none
**/
void ADC_Data_Alignment_Config(uint32_t data_alignment)
{
    /* Clear ADC_CTRL2 ALIG bit */
    ADC->CTRL2 &= ADC_ALIG_MASK;
    /* Set ADC_CTRL2 ALIG bit */
    ADC->CTRL2 |= data_alignment;
}

/**
*\*\name    ADC_Regular_Channels_Number_Config.
*\*\fun     Configures the ADC total number of conversion channels.
*\*\param   channels_number :
*\*\          - ADC_REGULAR_LEN_1
*\*\          - ADC_REGULAR_LEN_2
*\*\          - ADC_REGULAR_LEN_3
*\*\          - ADC_REGULAR_LEN_4
*\*\          - ADC_REGULAR_LEN_5
*\*\return  none
**/
void ADC_Regular_Channels_Number_Config(uint32_t channels_number)
{
    /* Clear ADC_RSEQ1 LEN[3:0] bit */
    ADC->CTRL2 &= ADC_REGULAR_LEN_MSAK;
    /* Set ADC_RSEQ1 LEN[3:0] bit */
    ADC->CTRL2 |= channels_number;
}



/**
*\*\name    ADC_Initializes.
*\*\fun     Initializes the ADC according to ADC_initstruct.
*\*\param   ADC_initstruct :
*\*\          - MultiChEn
*\*\            - DISABLE
*\*\            - ENABLE
*\*\          - ContinueConvEn
*\*\            - DISABLE
*\*\            - ENABLE
*\*\          - ExtTrigSelect
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC1                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC2                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC3                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_CC4                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T1_TRGO               
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T3_TRGO                
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T3_CC1   
*\*\          - ADC_EXT_TRIGCONV_REGULAR_T3_CC2   
*\*\          - ADC_EXT_TRIGCONV_REGULAR_EXT_INT11   
*\*\          - ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH              
*\*\          - DatAlign
*\*\            - ADC_DAT_ALIGN_R
*\*\            - ADC_DAT_ALIGN_L
*\*\          - ChsNumber
*\*\            - ADC_REGULAR_LEN_1
*\*\            - ADC_REGULAR_LEN_2
*\*\            - ADC_REGULAR_LEN_3
*\*\            - ADC_REGULAR_LEN_4
*\*\            - ADC_REGULAR_LEN_5
*\*\return  none
**/
void ADC_Initializes(ADC_InitType* ADC_initstruct)
{
    if(ADC_initstruct->MultiChEn)
    {
        ADC_Multichannels_Enable();
    }
    else
    {
        ADC_Multichannels_Disable();
    }

    if(ADC_initstruct->ContinueConvEn)
    {
        ADC_Continue_Conversion_Enable();
    }
    else
    {
        ADC_Continue_Conversion_Disable();
    }

    ADC_Regular_Group_External_Trigger_Source_Config(ADC_initstruct->ExtTrigSelect);
    ADC_Data_Alignment_Config(ADC_initstruct->DatAlign);
    ADC_Regular_Channels_Number_Config(ADC_initstruct->ChsNumber);
    
    RCC_Clocks_Frequencies_Value_Get(&RCC_clocks);
}


/**
*\*\name    ADC_Initializes_Structure.
*\*\fun     Fills each ADC_initstruct member with its default value.
*\*\param   ADC_initstruct :
*\*\          - MultiChEn
*\*\          - ContinueConvEn
*\*\          - ExtTrigSelect
*\*\          - DatAlign
*\*\          - ChsNumber
*\*\return  none
**/
void ADC_Initializes_Structure(ADC_InitType* ADC_initstruct)
{
    /* Reset ADC init structure parameters values */
    /* initialize the MultiChEn member */
    ADC_initstruct->MultiChEn      = DISABLE;
    /* Initialize the ContinueConvEn member */
    ADC_initstruct->ContinueConvEn = DISABLE;
    /* Initialize the ExtTrigSelect member */
    ADC_initstruct->ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_T1_CC1;
    /* Initialize the DatAlign member */
    ADC_initstruct->DatAlign       = ADC_DAT_ALIGN_R;
    /* Initialize the ChsNumber member */
    ADC_initstruct->ChsNumber      = ADC_REGULAR_LEN_1;
}

/**
*\*\name    ADC_ON.
*\*\fun     ADC turn ON.
*\*\return  none
**/
void ADC_ON(void)
{
    /* Set the ADC_CTRL2 ON bit to wake up the ADC from power down mode */
    ADC->CTRL2 |= ADC_TURN_ON;
}

/**
*\*\name    ADC_OFF.
*\*\fun     ADC turn OFF.
*\*\return  none
**/
void ADC_OFF(void)
{
    /* Clear the ADC_CTRL2 ON bit, then ADC go to power down mode */
    ADC->CTRL2 &= ADC_TURN_OFF;
}

/**
*\*\name    ADC_Interrupts_Enable.
*\*\fun     Enable ADC interrupts.
*\*\param   eoc_any :
*\*\          - ADC_SET_INT_TYPE_ENDCA
*\*\          - ADC_SET_INT_TYPE_OTHER
*\*\param   ADC_SET_INT_TYPE_ENDCA:
*\*\ 		  - ADC_INT_ENDCA     ADC_CTRL3
*\*\param   ADC_SET_INT_TYPE_OTHER:
*\*\          - ADC_INT_ENDC      ADC_CTRL1
*\*\          - ADC_INT_AWD       ADC_CTRL1  
*\*\return  none
**/
void ADC_Interrupts_Enable(u8 eoc_any, uint32_t adc_interrupt)
{
    if(eoc_any == ADC_SET_INT_TYPE_ENDCA)
    {
       /* Set the ADC_CTRL3 ADC_INT_ENDCA bit to enable ADC interrupts */
        ADC->CTRL3 |= adc_interrupt;
    }
    else
    {
         /* Set the ADC_CTRL1 ADC_INT_ENDC/ADC_INT_AWD bit to enable ADC interrupts */
        ADC->CTRL1 |= adc_interrupt;
    }
}

/**
*\*\name    ADC_Interrupts_Disable.
*\*\fun     Disable ADC interrupts.
*\*\param   eoc_any :
*\*\          - ADC_SET_INT_TYPE_ENDCA
*\*\          - ADC_SET_INT_TYPE_OTHER
*\*\param   ADC_SET_INT_TYPE_ENDCA:
*\*\ 		  - ADC_INT_ENDCA     ADC_CTRL3
*\*\param   ADC_SET_INT_TYPE_OTHER:
*\*\          - ADC_INT_ENDC      ADC_CTRL1
*\*\          - ADC_INT_AWD       ADC_CTRL1          
*\*\return  none
**/
void ADC_Interrupts_Disable(u8 eoc_any, uint32_t adc_interrupt)
{
    if(eoc_any == ADC_SET_INT_TYPE_ENDCA)
    {
       /* Clear the ADC_CTRL3 ADC_INT_ENDCA bit to disable ADC interrupts */
        ADC->CTRL3 &= (~adc_interrupt);
    }
    else
    {
         /* Clear the ADC_CTRL1 ADC_INT_ENDC/ADC_INT_AWD bit to disable ADC interrupts */
        ADC->CTRL1 &= (~adc_interrupt);
    }
}

/**
*\*\name    ADC_Regular_Channels_Software_Conversion_Operation.
*\*\fun     ADC regular channels software conversion operation.
*\*\param   conversion_operation :
*\*\          - ADC_EXTRTRIG_SWSTRRCH_ENABLE
*\*\          - ADC_EXTRTRIG_SWSTRRCH_DISABLE
*\*\          - ADC_EXTRTRIG_SWSTRRCH_GET_STS
*\*\return  FlagStatus:
*\*\          - SET
*\*\          - RESET
**/
FlagStatus ADC_Regular_Channels_Software_Conversion_Operation(uint32_t conversion_operation)
{
    if(conversion_operation == ADC_EXTRTRIG_SWSTRRCH_ENABLE)
    {
        /* Enable the ADC external trigger conversion mode for regular channels */
        ADC->CTRL2 |= conversion_operation;
    }
    else if(conversion_operation == ADC_EXTRTRIG_SWSTRRCH_DISABLE)
    {
        /* Disable the ADC external trigger conversion mode for regular channels */
        ADC->CTRL2 &= conversion_operation;
    }
    else if(conversion_operation == ADC_EXTRTRIG_SWSTRRCH_GET_STS)
    {
        /* Get ADC_CTRL2 SWSTRRCH bit */
        if ((ADC->CTRL2 & conversion_operation) != (uint32_t)RESET)
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    return RESET;
}
         
/**
*\*\name    ADC_Channel_Sample_Time_Config.
*\*\fun     Configures ADC channel sample time.
*\*\param   sample_time :
*\*\          - ADC_SAMP_TIME_8CYCLES
*\*\          - ADC_SAMP_TIME_12CYCLES
*\*\          - ADC_SAMP_TIME_14CYCLES  
*\*\          - ADC_SAMP_TIME_20CYCLES                
*\*\          - ADC_SAMP_TIME_26CYCLES                   
*\*\          - ADC_SAMP_TIME_30CYCLES                   
*\*\          - ADC_SAMP_TIME_42CYCLES                   
*\*\          - ADC_SAMP_TIME_56CYCLES                   
*\*\          - ADC_SAMP_TIME_72CYCLES                  
*\*\          - ADC_SAMP_TIME_88CYCLES                   
*\*\          - ADC_SAMP_TIME_120CYCLES                  
*\*\          - ADC_SAMP_TIME_182CYCLES                  
*\*\          - ADC_SAMP_TIME_240CYCLES                  
*\*\          - ADC_SAMP_TIME_380CYCLES                 
*\*\          - ADC_SAMP_TIME_760CYCLES                  
*\*\          - ADC_SAMP_TIME_1520CYCLES                 
*\*\          - ADC_SAMP_TIME_3040CYCLES      
*\*\return  none
**/
void ADC_Channel_Sample_Time_Config(uint8_t sample_time)
{
	ADC->SAMPT &= (uint32_t)(ADC_SAMP_TIME_MASK);
	ADC->SAMPT |= (uint32_t)(sample_time);
}

/**
*\*\name    ADC_External_Trigger_Conversion_Config.
*\*\fun     ADC conversion through external trigger enable.
*\*\param   group_operation :
*\*\          - ADC_EXTTRIGCONV_REGULAR_ENABLE
*\*\          - ADC_EXTTRIGCONV_REGULAR_DISABLE
*\*\return  none
**/
void ADC_External_Trigger_Conversion_Config(uint32_t group_operation)
{
    if(group_operation == ADC_EXTTRIGCONV_REGULAR_ENABLE)
    {
        /* Enable ADC regular group channels conversion on external event */
        ADC->CTRL2 |= group_operation;
    }
    else if(group_operation == ADC_EXTTRIGCONV_REGULAR_DISABLE)
    {
        /* Disables ADC regular group channels conversion on external event */
        ADC->CTRL2 &= group_operation;
    }
}
/**
*\*\name    ADC_Regular_Conversion_Data_Get.
*\*\fun     Get ADC regular conversion data.
*\*\return  ADC regular group conversion data.
**/
uint16_t ADC_Regular_Conversion_Data_Get(void)
{
    /* Return the conversion value */
    return (uint16_t)ADC->DAT0;
}
/**
*\*\name    ADC_Regular_Group_Conversion_Data_Get.
*\*\fun     Get ADC regular group conversion data.
*\*\param   number :
*\*\          - 0~4
*\*\return  ADC regular group conversion data.
**/
uint16_t ADC_Regular_Group_Conversion_Data_Get(uint8_t number)
{
    /* Return the conversion value */
	return ADC_DATA_REG_READ(ADC->DAT0,number);
}

/**
*\*\name    ADC_Analog_Watchdog_Mode_Channel_Config.
*\*\fun     Configures ADC analog watchdog single mode or scan mode,
*\*\        and single mode channel operation.
*\*\param   mode :
*\*\          - ADC_ANALOG_WTDG_SINGLE_MODE
*\*\          - ADC_ANALOG_WTDG_SCAN_MODE
*\*\param   channel :
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH0
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH1
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH2
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH3
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH4
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH5
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH6
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH7
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH8
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH9
*\*\          - ADC_ANALOG_WTDG_SINGLE_CH10
*\*\return  none
**/
void ADC_Analog_Watchdog_Mode_Channel_Config(uint32_t mode, uint8_t channel)
{
    /* ADC analog watchdog single mode */
    if(mode == ADC_ANALOG_WTDG_SINGLE_MODE)
    {
        ADC->CTRL1 |= mode;
        /* Clear ADC analog watchdog single mode channel */
        ADC->CTRL1 &= ADC_ANALOG_WTDG_SINGLE_CH_MASK;
        /* Select ADC analog watchdog single mode channel */
        ADC->CTRL1 |= (uint32_t)channel;
    }
    /* ADC analog watchdog scan mode */
    else
    {
        ADC->CTRL1 &= ADC_ANALOG_WTDG_MODE_MASK;
    }
}

/**
*\*\name    ADC_Analog_Watchdog_Enable.
*\*\fun     Enable ADC Analog watchdog.
*\*\param   wcdg_mode :
*\*\          - ADC_ANALOG_WTDG_REGULAR
*\*\return  none
**/
void ADC_Analog_Watchdog_Enable(uint32_t wcdg_mode)
{
    /* Set the ADC_CTRL1 AWDGERCH bit */
    ADC->CTRL1 |= wcdg_mode;
}

/**
*\*\name    ADC_Analog_Watchdog_Disable.
*\*\fun     Disable ADC Analog watchdog.
*\*\param   wcdg_mode :
*\*\          - ADC_ANALOG_WTDG_REGULAR
*\*\return  none
**/
void ADC_Analog_Watchdog_Disable(uint32_t wcdg_mode)
{
    /* Clean the ADC_CTRL1 AWDGERCH bit */
    ADC->CTRL1 &= (~wcdg_mode);
}

/**
*\*\name    ADC_Analog_Watchdog_HighThresholds_Config.
*\*\fun     Configures the high thresholds of the analog watchdog.
*\*\param   high_thresholds : 12bit high thresholds value(0 ~ 0xFFF)
*\*\return  none
**/
void ADC_Analog_Watchdog_HighThresholds_Config(uint16_t high_thresholds)
{
    /* Set the ADC high thresholds */
    ADC->WDGHIGH = high_thresholds;
}

/**
*\*\name    ADC_Analog_Watchdog_LowThresholds_Config.
*\*\fun     Configures the low thresholds of the analog watchdog.
*\*\param   low_thresholds : 12bit low thresholds value(0 ~ 0xFFF)
*\*\return  none
**/
void ADC_Analog_Watchdog_LowThresholds_Config(uint16_t low_thresholds)
{
    /* Set the ADC low thresholds */
    ADC->WDGLOW = low_thresholds;
}




/**
*\*\name    ADC_INTFlag_Status_Get.
*\*\fun     Get ADC flag Status.
*\*\param   adc_flag :
*\*\          - ADC_INT_FLAG_AWDG
*\*\          - ADC_INT_FLAG_ENDC
*\*\          - ADC_INT_FLAG_ENDCA
*\*\return  SET or RESET
**/
FlagStatus ADC_INTFlag_Status_Get(uint8_t adc_flag)
{
    /* Check the status of ADC flag */
    if ((ADC->STS & adc_flag) != (uint8_t)RESET)
    {
        if(adc_flag == ADC_INT_FLAG_ENDC)
        {
            Delay_8ADC_Colck(RCC_clocks.SysclkFreq,RCC_clocks.AdcClkFreq);
        }
        /* ADC_FLAG is set */
        return SET;
    }
    else
    {
        /* ADC_FLAG is reset */
        return RESET;
    }
}

/**
*\*\name    ADC_Flag_Status_Get.
*\*\fun     Get ADC flag Status.
*\*\param   selflag :
*\*\          - ADC_RUN_FLAG
*\*\          - ADC_RD_FLAG
*\*\param   adc_runflag :
*\*\          - ADC_FLAG_AWDG
*\*\          - ADC_FLAG_ENDC
*\*\          - ADC_FLAG_STR
*\*\          - ADC_FLAG_ENDCA
*\*\param   adc_rdflag :
*\*\          - ADC_FLAG_BUF_RDY
*\*\          - ADC_FLAG_RDY
*\*\          - ADC_FLAG_PD_RDY
*\*\return  SET or RESET
**/
FlagStatus ADC_Flag_Status_Get(uint8_t selflag, uint8_t adc_runflag, uint8_t adc_rdflag)
{
    if(selflag == ADC_RUN_FLAG)
    {    
        /* Check the status of ADC flag */
        if ((ADC->STS & adc_runflag) != (uint8_t)RESET)
        {
            if(adc_runflag == ADC_FLAG_ENDC)
            {
                Delay_8ADC_Colck(RCC_clocks.SysclkFreq,RCC_clocks.AdcClkFreq);
            }
            /* ADC_FLAG is set */
            return SET;
        }
        else
        {
            /* ADC_FLAG is reset */
            return RESET;
        }
    }
    else
    {
        if ((ADC->CTRL3 & adc_rdflag) != (uint8_t)RESET)
        {
            /* ADC_FLAG is set */
            return SET;
        }
        else
        {
            /* ADC_FLAG is reset */
            return RESET;
        }
    }
}

/**
*\*\name    ADC_INTFlag_Status_Clear.
*\*\fun     Clear ADC flag Status.
*\*\param   adc_flag :
*\*\          - ADC_INT_FLAG_AWDG
*\*\          - ADC_INT_FLAG_ENDC
*\*\          - ADC_INT_FLAG_ENDCA
*\*\return  SET or RESET
**/
void ADC_INTFlag_Status_Clear(uint8_t adc_flag)
{
    /* Clear the selected ADC flag Status */
    ADC->STS &= ~(uint32_t)adc_flag;
}

/**
*\*\name    ADC_Flag_Status_Clear.
*\*\fun     Clear ADC flag Status.
*\*\param   adc_flag :
*\*\          - ADC_FLAG_AWDG
*\*\          - ADC_FLAG_ENDC
*\*\          - ADC_FLAG_STR
*\*\          - ADC_FLAG_ENDCA
*\*\return  SET or RESET
**/
void ADC_Flag_Status_Clear(uint8_t adc_flag)
{
    /* Clear the selected ADC flag Status */
    ADC->STS &= ~(uint32_t)adc_flag;
}

/**
*\*\name    ADC_Regular_Sequence_Single_Config.
*\*\fun     Configures ADC channel Single in regular sequence.
*\*\param   channel :
*\*\          - ADC_CH_0
*\*\          - ADC_CH_1
*\*\          - ADC_CH_2
*\*\          - ADC_CH_3
*\*\          - ADC_CH_4
*\*\          - ADC_CH_5
*\*\          - ADC_CH_6
*\*\          - ADC_CH_7
*\*\          - ADC_CH_8
*\*\          - ADC_CH_9
*\*\          - ADC_CH_10
*\*\return  none
**/
void ADC_Regular_Sequence_Single_Config(uint8_t channel)
{
    /* ADC regular sequence register configures channelx */
    ADC->DAT0 &= ADC_SQR_MASK;
	ADC->DAT0 |= (channel<<ADC_SQR_OFFSET);
}

/**
*\*\name    ADC_Regular_Sequence_Multi_Config.
*\*\fun     Configures ADC channel Multi in regular sequence.
*\*\param   channel :
*\*\          - ADC_CH_0
*\*\          - ADC_CH_1
*\*\          - ADC_CH_2
*\*\          - ADC_CH_3
*\*\          - ADC_CH_4
*\*\          - ADC_CH_5
*\*\          - ADC_CH_6
*\*\          - ADC_CH_7
*\*\          - ADC_CH_8
*\*\          - ADC_CH_9
*\*\          - ADC_CH_10
*\*\param   number :
*\*\          - ADC_DATA_OFFSET0
*\*\          - ADC_DATA_OFFSET1
*\*\          - ADC_DATA_OFFSET2
*\*\          - ADC_DATA_OFFSET3
*\*\          - ADC_DATA_OFFSET4
*\*\return  none
**/
void ADC_Regular_Sequence_Multi_Config(uint8_t channel,uint8_t number)
{
    /* ADC regular sequence register configures channelx */
	if(number == ADC_DATA_OFFSET0)
	{
		ADC->DAT0 &= ADC_SQR_MASK;
		ADC->DAT0 |= (channel<<ADC_SQR_OFFSET);
	}
	else if(number == ADC_DATA_OFFSET1)
	{
		ADC->DAT1 &= ADC_SQR_MASK;
		ADC->DAT1 |= (channel<<ADC_SQR_OFFSET);
	} 
	else if(number == ADC_DATA_OFFSET2)
	{
		ADC->DAT2 &= ADC_SQR_MASK;
		ADC->DAT2 |= (channel<<ADC_SQR_OFFSET);
	} 
	else if(number == ADC_DATA_OFFSET3)
	{
		ADC->DAT3 &= ADC_SQR_MASK;
		ADC->DAT3 |= (channel<<ADC_SQR_OFFSET);
	} 
	else if(number == ADC_DATA_OFFSET4)
	{
		ADC->DAT4 &= ADC_SQR_MASK;
		ADC->DAT4 |= (channel<<ADC_SQR_OFFSET);
	} 
	
}
/**
*\*\name    ADC_Internal_Input_Buffer_Enable.
*\*\fun     ADC internal input buffer enable
*\*\return  none
**/
void ADC_Internal_Input_Buffer_Enable(void)
{
    /* Enable Vbat monitor */
    ADC->CTRL3 |= ADC_IN_BUFFER_ENABLE;
    
}

/**
*\*\name    ADC_Vbat_Monitor_Disable.
*\*\fun     ADC internal input buffer disable
*\*\return  none
**/
void ADC_Internal_Input_Buffer_Disable(void)
{
    /* Disable Vbat monitor */
    ADC->CTRL3 &= ADC_IN_BUFFER_DISABLE;
   
}

/**
*\*\name  Delay_8ADC_Colck.
*\*\fun   Delay 8 ADC_CLK according to the system clock and adc clock.
*\*\param SysClk ,the system clcok frequency.
*\*\param adc_clk ,the adc operating clcok frequency.
*\*\note: this function is not very precise, if you want to get more precise delay time, please modify this function directly. 
*\*\return  None
 */
static void Delay_8ADC_Colck(uint32_t SysClk, uint32_t adc_clk)
{
    __IO uint16_t Count = 0;
    __IO uint16_t  div = 0;
    uint16_t  temp ;
    div =  (SysClk/adc_clk);
    if(div <= 32)
    {
        temp = 1;
    }
    else
    {
        temp = 2;
    }
    while(Count < temp)  
    {
        __NOP();
        Count++;
    }
}

