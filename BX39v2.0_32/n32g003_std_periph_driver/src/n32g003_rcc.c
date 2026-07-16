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
*\*\file n32g003_rcc.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#include "n32g003_rcc.h"


/** RCC_Private_Defines **/
static const uint8_t AHBPresTable[16] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 12, 16, 24, 24,24};
static const uint8_t APBPresTable[8] = {1, 1, 1, 1, 2, 4, 8, 16};
static const uint8_t ADCCLKPresTable[16] = {1, 2, 3, 4, 6, 8, 10, 12, 24, 32, 32, 32, 32, 32, 32, 32};
static const uint8_t ADC1MCLKPresTable[4] = {6, 12, 24, 48};


/** RCC Driving Functions Declaration **/

/**
*\*\name    RCC_Reset.
*\*\fun     Reset the RCC registers.
*\*\param   none
*\*\return  none
**/
void RCC_Reset(void)
{
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

}


/**
*\*\name    RCC_HSI_Calibration_Value_Set.
*\*\fun     Adjusts the Internal High Speed oscillator (HSI) calibration value.
*\*\param   calibration_value(the calibration trimming value):
*\*\        This parameter must be a number between 0 and 0x0F
*\*\return  none
**/
void RCC_HSI_Calibration_Value_Set(uint8_t calibration_value)
{
    uint32_t temp_value = 0;

    temp_value = RCC->HSICTRL;
    /* Clear HSITRIM[3:0] bits */
    temp_value &= RCC_HSITRIM_MASK;
    /* Set the HSITRIM[3:0] bits according to HSICalibrationValue value */
    temp_value |= (uint32_t)calibration_value << RCC_CTRL_HSITRIM_OFFSET;
    /* Store the new value */
    RCC->HSICTRL = temp_value;
}

/**
*\*\name    RCC_LSI_Calibration_Value_Set.
*\*\fun     Adjusts the Internal Low Speed oscillator (LSI) calibration value.
*\*\param   calibration_value(the calibration trimming value):
*\*\        This parameter must be a number between 0 and 0x1F
*\*\return  none
**/
void RCC_LSI_Calibration_Value_Set(uint8_t calibration_value)
{
    uint32_t temp_value = 0;

    temp_value = RCC->LSICTRL;
    /* Clear LSITRIM[4:0] bits */
    temp_value &= RCC_LSITRIM_MASK;
    /* Set the LSITRIM[4:0] bits according to LSICalibrationValue value */
    temp_value |= (uint32_t)calibration_value << RCC_CTRL_LSITRIM_OFFSET;
    /* Store the new value */
    RCC->LSICTRL = temp_value;
}

/**
*\*\name    RCC_Sysclk_Config.
*\*\fun     Configures the system clock (SYSCLK).
*\*\When HSI is 48M:
*\*\param   HSI_div(system clock is derived from the HSI clock):
*\*\         - RCC_HSICLK_DIV1      system clock = HSI clock
*\*\         - RCC_HSICLK_DIV2      system clock = HSI clock/2
*\*\         - RCC_HSICLK_DIV6      system clock = HSI clock/6
*\*\When HSI is 40M:
*\*\param   HSI_div(system clock is derived from the HSI clock):
*\*\         - RCC_HSICLK_DIV1      system clock = HSI clock
*\*\         - RCC_HSICLK_DIV5      system clock = HSI clock/5
*\*\return  none
**/
void RCC_Sysclk_Config(uint32_t HSI_div)
{
    uint32_t temp_value = 0;
    temp_value = RCC->CFG;
    /* Clear SYSPRES[1:0] bits */
    temp_value &= RCC_HSICLK_DIV_MASK;
    /* Set SYSPRES[1:0] bits according to HSI_div value */
    temp_value |= HSI_div;
    /* Store the new value */
    RCC->CFG = temp_value;
}

/**
*\*\name    RCC_Hclk_Config.
*\*\fun     Configures the AHB clock (HCLK).
*\*\param   sysclk_div(AHB clock is derived from the system clock (SYSCLK)):
*\*\         - RCC_SYSCLK_DIV1      AHB clock = SYSCLK
*\*\         - RCC_SYSCLK_DIV2      AHB clock = SYSCLK/2
*\*\         - RCC_SYSCLK_DIV4      AHB clock = SYSCLK/4
*\*\         - RCC_SYSCLK_DIV8      AHB clock = SYSCLK/8
*\*\         - RCC_SYSCLK_DIV12     AHB clock = SYSCLK/12
*\*\         - RCC_SYSCLK_DIV16     AHB clock = SYSCLK/16
*\*\         - RCC_SYSCLK_DIV24    AHB clock = SYSCLK/24
*\*\return  none
**/
void RCC_Hclk_Config(uint32_t sysclk_div)
{
    uint32_t temp_value = 0;
    temp_value = RCC->CFG;
    /* Clear AHBPRES[3:0] bits */
    temp_value &= RCC_SYSCLK_DIV_MASK;
    /* Set AHBPRES[3:0] bits according to rcc_sysclk value */
    temp_value |= sysclk_div;
    /* Store the new value */
    RCC->CFG = temp_value;
}


/**
*\*\name    RCC_Pclk_Config.
*\*\fun     Configures the Low Speed APB clock (PCLK1).
*\*\param   hclk_div(APB1 clock is derived from the AHB clock (HCLK)):
*\*\         - RCC_HCLK_DIV1     APB1 clock = HCLK
*\*\         - RCC_HCLK_DIV2     APB1 clock = HCLK/2
*\*\         - RCC_HCLK_DIV4     APB1 clock = HCLK/4
*\*\         - RCC_HCLK_DIV8     APB1 clock = HCLK/8
*\*\         - RCC_HCLK_DIV16    APB1 clock = HCLK/16
*\*\return  none
**/
void RCC_Pclk_Config(uint32_t hclk_div)
{
    uint32_t temp_value = 0;
    temp_value = RCC->CFG;
    /* Clear APB1PRES[2:0] bits */
    temp_value &= RCC_APB_DIV_MASK;
    /* Set APB1PRES[2:0] bits according to hclk_div value */
    temp_value |= hclk_div;
    /* Store the new value */
    RCC->CFG = temp_value;
}


/**
*\*\name    RCC_TIM1_Clock_Config.
*\*\fun     Configures the TIM1 clock source(TIM1CLK).
*\*\param   timer_clksrc(TIM1 clock source):
*\*\         - RCC_TIM1_CLKSRC_PCLK
*\*\         - RCC_TIM1_CLKSRC_SYSCLK
*\*\return  none
**/
void RCC_TIM1_Clock_Config(uint32_t timer_clksrc)
{
    uint32_t temp_value = 0;

    temp_value = RCC->CFG2;
    /* Clear TIMCLK_SEL bits */
    temp_value &= RCC_TIM1_CLKSRC_MASK;
    /* Set TIMCLK_SEL bits according to timer_clksrc value */
    temp_value |= timer_clksrc;

    /* Store the new value */
    RCC->CFG2 = temp_value;
}


/**
*\*\name    RCC_ADC_1M_Clock_Config.
*\*\fun     Configures the ADC 1M clock (ADC1MCLK).
*\*\param   ADC1M_prescaler(ADC1M clock prescaler):
*\*\         - RCC_ADC1MCLK_DIV6     HSI Clock Divided By 6
*\*\         - RCC_ADC1MCLK_DIV12    HSI Clock Divided By 12
*\*\         - RCC_ADC1MCLK_DIV24    HSI Clock Divided By 24
*\*\         - RCC_ADC1MCLK_DIV40    HSI Clock Divided By 40
*\*\         - RCC_ADC1MCLK_DIV48    HSI Clock Divided By 48
*\*\return  none
*\*\note    To ensure normal work of the ADC module, the HSI divide result must be 1Mhz.
**/
void RCC_ADC_1M_Clock_Config(uint32_t ADC1M_prescaler)
{
    uint32_t temp_value = 0;

    temp_value = RCC->CFG2;
		/* Clear ADC1MPRE[1:0] bits */
    temp_value &= RCC_ADC1MCLK_DIV_MASK;
	
    /* Set ADC1MPRE[1:0] bits according to ADC1M_prescaler value */
    temp_value |= ADC1M_prescaler;

    /* Store the new value */
    RCC->CFG2 = temp_value;
}


/**
*\*\name    RCC_ADC_Clock_Config.
*\*\fun     Configures the ADCHCLK prescaler.
*\*\param   ADC_prescaler(ADCHCLK prescaler):
*\*\         - RCC_ADC_DIV1          HSI Clock Divided By 1
*\*\         - RCC_ADC_DIV2          HSI Clock Divided By 2
*\*\         - RCC_ADC_DIV3          HSI Clock Divided By 3
*\*\         - RCC_ADC_DIV4          HSI Clock Divided By 4
*\*\         - RCC_ADC_DIV6          HSI Clock Divided By 6
*\*\         - RCC_ADC_DIV8          HSI Clock Divided By 8
*\*\         - RCC_ADC_DIV10         HSI Clock Divided By 10
*\*\         - RCC_ADC_DIV12         HSI Clock Divided By 12
*\*\         - RCC_ADC_DIV24         HSI Clock Divided By 24
*\*\         - RCC_ADC_DIV32         HSI Clock Divided By 32
*\*\         - RCC_ADC_DIV_OTHERS    HSI Clock Divided By 32
*\*\return  none
**/
void RCC_ADC_Clock_Config(uint32_t ADC_prescaler)
{
    uint32_t temp_value = 0;

    temp_value = RCC->CFG2;
    /* Clear ADCPRES[3:0] bits */
    temp_value &= RCC_ADC_DIV_MASK;
    /* Set ADCPRES[3:0] bits according to ADC_prescaler value */
    temp_value |= ADC_prescaler;

    /* Store the new value */
    RCC->CFG2 = temp_value;
}


/**
*\*\name    RCC_TIM6_Clock_Config.
*\*\fun     Configures the TIM6 clock source(TIM6CLK).
*\*\param   timer_clksrc(TIM6 clock source):
*\*\         - RCC_TIM6_CLKSRC_PCLK
*\*\         - RCC_TIM6_CLKSRC_LSI
*\*\return  none
**/
void RCC_TIM6_Clock_Config(uint32_t timer_clksrc)
{
    uint32_t temp_value = 0;

    temp_value = RCC->CFG2;
    /* Clear TIM6CLK_SEL bits */
    temp_value &= RCC_TIM6_CLKSRC_MASK;
    /* Set TIM6CLK_SEL bits according to timer_clksrc value */
    temp_value |= timer_clksrc;

    /* Store the new value */
    RCC->CFG2 = temp_value;
}


/**
*\*\name    RCC_Clocks_Frequencies_Value_Get.
*\*\fun     Returns the frequencies of different on chip clocks.
*\*\param   RCC_clocks pointer to a RCC_ClocksType structure which will hold
*\*\        the clocks frequencies.
*\*\return  none
*\*\note    The result of this function could be not correct when using
*\*\        fractional value for HSI crystal.
**/
void RCC_Clocks_Frequencies_Value_Get(RCC_ClocksType* RCC_clocks)
{
    uint32_t temp_value = 0;
	  uint32_t presc_value = 0;

    /* Get SYSCLK source
     * -------------------------------------------------------*/
    temp_value = PWR->CTRL3 & PWR_CTRL3_HSISEL;
		 
    switch (temp_value)
    {
        case PWR_CTRL3_HSISEL: /* HSI 40M used as system clock */
            if((RCC->CFG & RCC_CFG_SYSPRES) == RCC_HSICLK_DIV1)
            {
                RCC_clocks->SysclkFreq = SYSCLK_FREQ_40M;
            }
            else
            {
                RCC_clocks->SysclkFreq = SYSCLK_FREQ_8M;
            } 					
            
            /* Get ADCCLK prescaler */
            presc_value = ADCCLKPresTable[(RCC->CFG2 & RCC_CFG2_ADCPRES)];
            /* ADCCLK clock frequency */
            RCC_clocks->AdcClkFreq = HSI_FREQ_40M/presc_value;
            
            /* Get ADC1MCLK prescaler */
            RCC_clocks->Adc1MClkFreq = HSI_FREQ_40M/40;
            
            break;
     
        default:/* HSI 48M used as system clock */
            if((RCC->CFG & RCC_CFG_SYSPRES) == RCC_HSICLK_DIV1)
            {
                RCC_clocks->SysclkFreq = SYSCLK_FREQ_48M;
            }
            else if((RCC->CFG & RCC_CFG_SYSPRES) == RCC_HSICLK_DIV2)
            {
                RCC_clocks->SysclkFreq = SYSCLK_FREQ_24M;
            }
            else
            {
                RCC_clocks->SysclkFreq = SYSCLK_FREQ_8M;
            }
            
            /* Get ADCCLK prescaler */
            presc_value = ADCCLKPresTable[(RCC->CFG2 & RCC_CFG2_ADCPRES)];
            /* ADCCLK clock frequency */
            RCC_clocks->AdcClkFreq = HSI_FREQ_48M/presc_value;
            
            /* Get ADC1MCLK prescaler */
            presc_value = ADC1MCLKPresTable[((RCC->CFG2 & RCC_CFG2_ADC1MPRES) >> RCC_CFG2_ADC1MPRES_OFFSET)];
            /* ADCCLK clock frequency */
            RCC_clocks->Adc1MClkFreq = HSI_FREQ_48M/presc_value;
            break;
    }
		
    /* Get HCLK prescaler */
    presc_value = AHBPresTable[((RCC->CFG & RCC_CFG_AHBPRES) >> RCC_CFG_AHBPRES_OFFSET)];
    /* HCLK clock frequency */
    RCC_clocks->HclkFreq = RCC_clocks->SysclkFreq/presc_value;
		
		/* Get PCLK prescaler */
    presc_value = APBPresTable[((RCC->CFG & RCC_CFG_APBPRES) >> RCC_CFG_APBPRES_OFFSET)];
    /* PCLK clock frequency */
    RCC_clocks->PclkFreq = RCC_clocks->HclkFreq/presc_value;
		

}



/**
*\*\name    RCC_AHB_Peripheral_Clock_Enable.
*\*\fun     Enables the AHB peripheral clock.
*\*\param   AHB_periph (AHB peripheral to gates its clock):
*\*\         - RCC_AHB_PERIPH_CRC
*\*\         - RCC_AHB_PERIPH_ADC
*\*\return  none
**/
void RCC_AHB_Peripheral_Clock_Enable(uint32_t AHB_periph)
{
    RCC->AHBPCLKEN |= AHB_periph;
}

/**
*\*\name    RCC_AHB_Peripheral_Clock_Disable.
*\*\fun     Disables the AHB peripheral clock.
*\*\param   AHB_periph (AHB peripheral to gates its clock):
*\*\         - RCC_AHB_PERIPH_CRC
*\*\         - RCC_AHB_PERIPH_ADC
*\*\return  none
**/
void RCC_AHB_Peripheral_Clock_Disable(uint32_t AHB_periph)
{
    RCC->AHBPCLKEN &= ~AHB_periph;
}


/**
*\*\name    RCC_APB_Peripheral_Clock_Enable.
*\*\fun     Enables the High Speed APB (APB1) peripheral clock.
*\*\param   APB_periph (APB1 peripheral to gates its clock):
*\*\        - RCC_APB_PERIPH_AFIO
*\*\        - RCC_APB_PERIPH_BEEPER
*\*\        - RCC_APB_PERIPH_IOPA
*\*\        - RCC_APB_PERIPH_IOPB
*\*\        - RCC_APB_PERIPH_COMP
*\*\        - RCC_APB_PERIPH_COMP_FILT
*\*\        - RCC_APB_PERIPH_UART1
*\*\        - RCC_APB_PERIPH_UART2
*\*\        - RCC_APB_PERIPH_I2C
*\*\        - RCC_APB_PERIPH_PWR
*\*\        - RCC_APB_PERIPH_SPI
*\*\        - RCC_APB_PERIPH_TIM1
*\*\        - RCC_APB_PERIPH_TIM3
*\*\        - RCC_APB_PERIPH_TIM6
*\*\        - RCC_APB_PERIPH_IWDG
*\*\return none.
**/
void RCC_APB_Peripheral_Clock_Enable(uint32_t APB_periph)
{
    RCC->APBPCLKEN |= APB_periph;
}


/**
*\*\name    RCC_APB_Peripheral_Clock_Disable.
*\*\fun     Disables the High Speed APB (APB1) peripheral clock.
*\*\param   APB_periph (APB1 peripheral to gates its clock):
*\*\        - RCC_APB_PERIPH_AFIO
*\*\        - RCC_APB_PERIPH_BEEPER
*\*\        - RCC_APB_PERIPH_IOPA
*\*\        - RCC_APB_PERIPH_IOPB
*\*\        - RCC_APB_PERIPH_COMP
*\*\        - RCC_APB_PERIPH_COMP_FILT
*\*\        - RCC_APB_PERIPH_UART1
*\*\        - RCC_APB_PERIPH_UART2
*\*\        - RCC_APB_PERIPH_I2C
*\*\        - RCC_APB_PERIPH_PWR
*\*\        - RCC_APB_PERIPH_SPI
*\*\        - RCC_APB_PERIPH_TIM1
*\*\        - RCC_APB_PERIPH_TIM3
*\*\        - RCC_APB_PERIPH_TIM6
*\*\        - RCC_APB_PERIPH_IWDG
*\*\return none.
**/
void RCC_APB_Peripheral_Clock_Disable(uint32_t APB_periph)
{
    RCC->APBPCLKEN &= ~APB_periph;
}


/**
*\*\name    RCC_Peripheral_Reset.
*\*\fun     AHB and APB peripheral reset.
*\*\param   Periph specifies the APB peripheral to reset.
*\*\         - RCC_RST_AFIORST
*\*\         - RCC_RST_BEEPRST
*\*\         - RCC_RST_GPIOARST
*\*\         - RCC_RST_GPIOBRST
*\*\         - RCC_RST_UART1RST
*\*\         - RCC_RST_UART2RST
*\*\         - RCC_RST_I2CRST
*\*\         - RCC_RST_PWRRST
*\*\         - RCC_RST_ADCRST
*\*\         - RCC_RST_SPIRST
*\*\         - RCC_RST_TIM1RST
*\*\         - RCC_RST_TIM3RST
*\*\         - RCC_RST_TIM6RST
*\*\         - RCC_RST_COMPRST
*\*\return none.
**/
void RCC_Peripheral_Reset(uint32_t Periph)
{
    RCC->PRST |= Periph;
    RCC->PRST &= ~Periph;
}

/**
*\*\name   RCC_MCO_Prescaler_Config.
*\*\fun    Configures the MCO clock prescaler.
*\*\param  MCO_prescaler(MCO  clock prescaler): 
*\*\        - RCC_MCO_CLK_DIV1
*\*\        - RCC_MCO_CLK_DIV2 
*\*\        - RCC_MCO_CLK_DIV3 
*\*\        - RCC_MCO_CLK_DIV4 
*\*\        - RCC_MCO_CLK_DIV5 
*\*\        - RCC_MCO_CLK_DIV6 
*\*\        - RCC_MCO_CLK_DIV7 
*\*\        - RCC_MCO_CLK_DIV8 
*\*\        - RCC_MCO_CLK_DIV9 
*\*\        - RCC_MCO_CLK_DIV10
*\*\        - RCC_MCO_CLK_DIV11
*\*\        - RCC_MCO_CLK_DIV12
*\*\        - RCC_MCO_CLK_DIV13
*\*\        - RCC_MCO_CLK_DIV14
*\*\        - RCC_MCO_CLK_DIV15 
*\*\        - RCC_MCO_CLK_DIV32 
*\*\return  none. 
*\*\note  if MCO select LSI,the minimum MCO_prescaler is RCC_MCO_CLK_DIV1
\*\       if MCO select HSI,the minimum MCO_prescaler is RCC_MCO_CLK_DIV2
**/
void RCC_MCO_Prescaler_Config(uint32_t MCO_prescaler)
{
    uint32_t temp_value = 0;

    temp_value = RCC->CFG;
    /* Clear MCOPRE[3:0] bits */
    temp_value &= RCC_MCO_CLK_DIV_MASK;
    /* Set MCOPRE[3:0] bits according to MCO_prescaler value */
    temp_value |= MCO_prescaler;

    /* Store the new value */
    RCC->CFG = temp_value;
}
/**
*\*\name   RCC_MCO_Source_Config.
*\*\fun    Selects the clock source to output on MCO pin.
*\*\param  MCO_source(clock source to output):
*\*\     - RCC_MCO_NOCLK      
*\*\     - RCC_MCO_HSI                 
*\*\     - RCC_MCO_LSI 
*\*\return  none.
**/
void RCC_MCO_Source_Config(uint32_t MCO_source)
{
    uint32_t temp_value = 0;

    temp_value = RCC->CFG;
    /* Clear MCO[1:0] bits */
	temp_value &= RCC_MCO_MASK;
    /* Set MCO[1:0] bits according to MCO_source value */
    temp_value |= MCO_source;

    /* Store the new value */
    RCC->CFG = temp_value;
}




/**
*\*\name    RCC_Flag_Status_Get.
*\*\fun     Checks whether the specified RCC flag is set or not.
*\*\param   RCC_flag:
*\*\	      - RCC_FLAG_HSI40MRDF    HSI 40M oscillator clock ready
*\*\	      - RCC_FLAG_HSI48MRDF    HSI 48M oscillator clock ready
*\*\	      - RCC_FLAG_LSIRDF       LSI 32k oscillator clock ready
*\*\	      - RCC_FLAG_LSIDETFF     LSI 32k detect finish
*\*\	      - RCC_FLAG_PINRST       Pin reset
*\*\	      - RCC_FLAG_PORRST       POR/PDR reset
*\*\	      - RCC_FLAG_SFTRST       Software reset
*\*\	      - RCC_FLAG_IWDGRST      Independent Watchdog reset
*\*\	      - RCC_FLAG_LVRRST       LVR reset
*\*\	      - RCC_FLAG_LPWRRST      Low Power reset
*\*\	      - RCC_FLAG_EMCCLAMPF    EMC Power reset
*\*\return  FlagStatus:
*\*\      	  - SET
*\*\  	      - RESET
**/
FlagStatus RCC_Flag_Status_Get(uint8_t RCC_flag)
{
    uint32_t temp_value = 0;
    uint32_t reg_value  = 0;

    /* Get the RCC register index */
    temp_value = RCC_flag >> RCC_FLAG_OFFSET;
    switch(temp_value)
    {
        case 1: /* The flag to check is in HSICTRL register */
           reg_value = RCC->HSICTRL;
           break;
        case 2: /* The flag to check is in LSICTRL register */
           reg_value = RCC->LSICTRL;
           break;
        default:/* The flag to check is in CTRLSTS register */
            reg_value = RCC->CTRLSTS;
           break;
    }

    /* Get the flag position */
    temp_value = RCC_flag & FLAG_Mask;
    if ((reg_value & ((uint32_t)1 << temp_value)) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


/**
*\*\name    RCC_Reset_Flag_Clear.
*\*\fun     Clears the RCC reset flags.
*\*\param   none
*\*\return  none
**/
void RCC_Reset_Flag_Clear(void)
{
    /* Set RMRSTF bit to clear the reset flags */
    RCC->CTRLSTS |= RCC_REMOVE_RESET_FLAG;
    /* RMRSTF bit should be reset */
    RCC->CTRLSTS &= (~RCC_REMOVE_RESET_FLAG);
}


/**
*\*\name    RCC_EMC_Reset_Disable.
*\*\fun     Disable the RCC's EMC Clamp reset bits.
*\*\param   none
*\*\return  none
**/
void RCC_EMC_Reset_Disable(void)
{
	RCC->EMCCTRL &= (~RCC_EMCCTRL_EMCRSTEN);
}

/**
*\*\name    RCC_EMC_Reset_Enable.
*\*\fun     Enable the RCC's EMC Clamp reset bits.
*\*\param   none
*\*\return  none
**/
void RCC_EMC_Reset_Enable(void)
{
	RCC->EMCCTRL |= RCC_EMCCTRL_EMCRSTEN;
}

/**
*\*\name    RCC_EMC_Detect_Disable.
*\*\fun     Disable the RCC's EMC Clamp detect bits.
*\*\param   none
*\*\return  none
**/
void RCC_EMC_Detect_Disable(void)
{
	RCC->EMCCTRL &= (~RCC_EMCCTRL_EMCDETEN);
}

/**
*\*\name    RCC_EMC_Detect_Enable.
*\*\fun     Enable the RCC's EMC Clamp detect bits.
*\*\param   none
*\*\return  none
**/
void RCC_EMC_Detect_Enable(void)
{
	RCC->EMCCTRL |= RCC_EMCCTRL_EMCDETEN;
}

/**
*\*\name    RCC_LSI_Detect_Disable.
*\*\fun     Disable the LSI detect.
*\*\param   none
*\*\return  none
**/
void RCC_LSI_Detect_Disable(void)
{
	RCC->LSICTRL &= (~RCC_LSICTRL_LSIDETEN);
}

/**
*\*\name    RCC_LSI_Detect_Enable.
*\*\fun     Enable the LSI detect.
*\*\param   none
*\*\return  none
**/
void RCC_LSI_Detect_Enable(void)
{
	RCC->LSICTRL |= RCC_LSICTRL_LSIDETEN;
}

/**
*\*\name    RCC_LSI_Clocks_Frequencies_Get.
*\*\fun     Get the LSI Frequencies.
*\*\param   none
*\*\return  LSIFREQ[15:0] (LSI Frequencies LSICLK(kHz)= 8*HSICLK/LSIFREQ[15:0] (HSICLK is 48000kHz or 40000kHz))
**/
uint16_t RCC_LSI_Clocks_Frequencies_Get(void)
{
	return (uint16_t)((RCC->LSICTRL & RCC_LSICTRL_LSIFREQ) >> 8);
}
/******************* (C) COPYRIGHT 2018 NATIONZ *****END OF FILE****/
