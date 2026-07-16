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
*\*\file n32g003_rcc.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#ifndef __n32g003_RCC_H
#define __n32g003_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "n32g003.h"

/** RCC_Exported_Types **/

typedef struct
{
  uint32_t SysclkFreq;     /* returns SYSCLK clock frequency expressed in Hz */
  uint32_t HclkFreq;       /* returns HCLK clock frequency expressed in Hz */
  uint32_t PclkFreq;       /* returns PCLK clock frequency expressed in Hz */
  uint32_t Adc1MClkFreq;   /* returns ADC1MHCLK clock frequency expressed in Hz */
  uint32_t AdcClkFreq;    /* returns ADCHCLK clock frequency expressed in Hz */
}RCC_ClocksType;


/** RCC registers bit mask **/
#define HSI_FREQ_48M        (48000000)
#define HSI_FREQ_40M        (40000000)

/** Register shift macro definition **/
#define  RCC_CFG_AHBPRES_OFFSET     (REG_BIT4_OFFSET)
#define  RCC_CFG_APBPRES_OFFSET     (REG_BIT8_OFFSET)
#define  RCC_CFG2_ADC1MPRES_OFFSET  (REG_BIT10_OFFSET)


/* RCC Flag Mask */
#define FLAG_Mask                 ((uint8_t)0x1F)


/** Internal High Speed clock trimming **/
#define RCC_HSITRIM_MASK (~RCC_HSICTRL_HSITRIM)
/** Internal Low Speed clock trimming **/
#define RCC_LSITRIM_MASK (~RCC_LSICTRL_LSITRIM)


/** SYS_clock_source **/
#define RCC_HSICLK_DIV_MASK (~RCC_CFG_SYSPRES)
#define RCC_HSICLK_DIV1     (RCC_CFG_SYSPRES_DIV1)
#define RCC_HSICLK_DIV2     (RCC_CFG_SYSPRES_DIV2)
#define RCC_HSICLK_DIV5     (RCC_CFG_SYSPRES_DIV5)
#define RCC_HSICLK_DIV6     (RCC_CFG_SYSPRES_DIV6)

/** AHB_clock_source **/
#define RCC_SYSCLK_DIV_MASK (~RCC_CFG_AHBPRES)
#define RCC_SYSCLK_DIV1     (RCC_CFG_AHBPRES_DIV1)
#define RCC_SYSCLK_DIV2     (RCC_CFG_AHBPRES_DIV2)
#define RCC_SYSCLK_DIV4     (RCC_CFG_AHBPRES_DIV4)
#define RCC_SYSCLK_DIV8     (RCC_CFG_AHBPRES_DIV8)
#define RCC_SYSCLK_DIV12    (RCC_CFG_AHBPRES_DIV12)
#define RCC_SYSCLK_DIV16    (RCC_CFG_AHBPRES_DIV16)
#define RCC_SYSCLK_DIV24    (RCC_CFG_AHBPRES_DIV24)

/** APB_clock_source **/
#define RCC_APB_DIV_MASK   	(~RCC_CFG_APBPRES)
#define RCC_HCLK_DIV1       (RCC_CFG_APBPRES_DIV1)
#define RCC_HCLK_DIV2       (RCC_CFG_APBPRES_DIV2)
#define RCC_HCLK_DIV4       (RCC_CFG_APBPRES_DIV4)
#define RCC_HCLK_DIV8       (RCC_CFG_APBPRES_DIV8)
#define RCC_HCLK_DIV16      (RCC_CFG_APBPRES_DIV16)

/** RCC_CFGR2_Config **/
#define RCC_TIM1_CLKSRC_MASK    (~RCC_CFG2_TIM1CLKSEL)
#define RCC_TIM1_CLKSRC_PCLK    (RCC_CFG2_TIM1CLKSEL_PCLK)
#define RCC_TIM1_CLKSRC_SYSCLK  (RCC_CFG2_TIM1CLKSEL_SYSCLK)

#define RCC_ADC1MCLK_DIV_MASK   (~RCC_CFG2_ADC1MPRES)
#define RCC_ADC1MCLK_DIV6       (RCC_CFG2_ADC1MPRES_DIV6)
#define RCC_ADC1MCLK_DIV12      (RCC_CFG2_ADC1MPRES_DIV12)
#define RCC_ADC1MCLK_DIV24      (RCC_CFG2_ADC1MPRES_DIV24)
#define RCC_ADC1MCLK_DIV40      (RCC_CFG2_ADC1MPRES_DIV40)
#define RCC_ADC1MCLK_DIV48      (RCC_CFG2_ADC1MPRES_DIV48)

#define RCC_ADC_DIV_MASK        (~RCC_CFG2_ADCPRES)
#define RCC_ADC_DIV1            (RCC_CFG2_ADCPRES_DIV1)
#define RCC_ADC_DIV2            (RCC_CFG2_ADCPRES_DIV2)
#define RCC_ADC_DIV3            (RCC_CFG2_ADCPRES_DIV3)
#define RCC_ADC_DIV4            (RCC_CFG2_ADCPRES_DIV4)
#define RCC_ADC_DIV6            (RCC_CFG2_ADCPRES_DIV6)
#define RCC_ADC_DIV8            (RCC_CFG2_ADCPRES_DIV8)
#define RCC_ADC_DIV10           (RCC_CFG2_ADCPRES_DIV10)
#define RCC_ADC_DIV12           (RCC_CFG2_ADCPRES_DIV12)
#define RCC_ADC_DIV24           (RCC_CFG2_ADCPRES_DIV24)
#define RCC_ADC_DIV32           (RCC_CFG2_ADCPRES_DIV32)
#define RCC_ADC_DIV_OTHERS      (RCC_CFG2_ADCPRES_OTHERS)

#define RCC_TIM6_CLKSRC_MASK  			(~RCC_CFG2_TIM6CLKSEL)
#define RCC_TIM6_CLKSRC_PCLK  			(RCC_CFG2_TIM6CLKSEL_PCLK)
#define RCC_TIM6_CLKSRC_LSI   			(RCC_CFG2_TIM6CLKSEL_LSI)

/** Peripheral Reset**/
#define RCC_RST_AFIORST       (RCC_PRST_AFIORST) 
#define RCC_RST_BEEPRST       (RCC_PRST_BEEPRST) 
#define RCC_RST_GPIOARST      (RCC_PRST_GPIOARST) 
#define RCC_RST_GPIOBRST      (RCC_PRST_GPIOBRST) 
#define RCC_RST_UART1RST      (RCC_PRST_UART1RST) 
#define RCC_RST_UART2RST      (RCC_PRST_UART2RST) 
#define RCC_RST_I2CRST        (RCC_PRST_I2CRST) 
#define RCC_RST_PWRRST        (RCC_PRST_PWRRST) 
#define RCC_RST_ADCRST        (RCC_PRST_ADCRST) 
#define RCC_RST_SPIRST        (RCC_PRST_SPIRST) 
#define RCC_RST_TIM1RST       (RCC_PRST_TIM1RST) 
#define RCC_RST_TIM3RST       (RCC_PRST_TIM3RST) 
#define RCC_RST_TIM6RST       (RCC_PRST_TIM6RST) 
#define RCC_RST_COMPRST       (RCC_PRST_COMPRST) 

/** Peripheral Enable **/

/** AHB_peripheral **/
#define RCC_AHB_PERIPH_CRC    (RCC_AHBPCLKEN_CRCEN)
#define RCC_AHB_PERIPH_ADC    (RCC_AHBPCLKEN_ADCEN)

/** APB_peripheral **/
#define RCC_APB_PERIPH_AFIO       (RCC_APBPCLKEN_AFIOEN)
#define RCC_APB_PERIPH_BEEPER     (RCC_APBPCLKEN_BEEPEREN)
#define RCC_APB_PERIPH_IOPA       (RCC_APBPCLKEN_IOPAEN)
#define RCC_APB_PERIPH_IOPB       (RCC_APBPCLKEN_IOPBEN)
#define RCC_APB_PERIPH_COMP       (RCC_APBPCLKEN_COMPEN)
#define RCC_APB_PERIPH_COMP_FILT  (RCC_APBPCLKEN_COMPFILTEN)
#define RCC_APB_PERIPH_UART1      (RCC_APBPCLKEN_UART1EN)
#define RCC_APB_PERIPH_UART2      (RCC_APBPCLKEN_UART2EN)
#define RCC_APB_PERIPH_I2C        (RCC_APBPCLKEN_I2CEN)
#define RCC_APB_PERIPH_PWR        (RCC_APBPCLKEN_PWREN)
#define RCC_APB_PERIPH_SPI        (RCC_APBPCLKEN_SPIEN)
#define RCC_APB_PERIPH_TIM1       (RCC_APBPCLKEN_TIM1EN)
#define RCC_APB_PERIPH_TIM3       (RCC_APBPCLKEN_TIM3EN)
#define RCC_APB_PERIPH_TIM6       (RCC_APBPCLKEN_TIM6EN)
#define RCC_APB_PERIPH_IWDG       (RCC_APBPCLKEN_IWDGEN)

/** RCC_Exported_Constants **/

/** Register shift macro definition **/
#define  RCC_FLAG_OFFSET             (REG_BIT5_OFFSET)
#define  RCC_CTRL_HSITRIM_OFFSET     (REG_BIT4_OFFSET)
#define  RCC_CTRL_LSITRIM_OFFSET     (REG_BIT2_OFFSET)


/** Clock_source_to_output_on_MCO_pin **/
#define RCC_MCO_MASK   (~RCC_CFG_MCO)
#define RCC_MCO_NOCLK  (RCC_CFG_MCO_NOCLK)
#define RCC_MCO_LSI    (RCC_CFG_MCO_LSI)
#define RCC_MCO_HSI    (RCC_CFG_MCO_HSI)

#define RCC_MCO_CLK_DIV_MASK   (~RCC_CFG_MCOPRES)
#define RCC_MCO_CLK_DIV1       (RCC_CFG_MCOPRES_DIV1)
#define RCC_MCO_CLK_DIV2       (RCC_CFG_MCOPRES_DIV2)
#define RCC_MCO_CLK_DIV3       (RCC_CFG_MCOPRES_DIV3)
#define RCC_MCO_CLK_DIV4       (RCC_CFG_MCOPRES_DIV4)
#define RCC_MCO_CLK_DIV5       (RCC_CFG_MCOPRES_DIV5)
#define RCC_MCO_CLK_DIV6       (RCC_CFG_MCOPRES_DIV6)
#define RCC_MCO_CLK_DIV7       (RCC_CFG_MCOPRES_DIV7)
#define RCC_MCO_CLK_DIV8       (RCC_CFG_MCOPRES_DIV8)
#define RCC_MCO_CLK_DIV9       (RCC_CFG_MCOPRES_DIV9)
#define RCC_MCO_CLK_DIV10      (RCC_CFG_MCOPRES_DIV10)
#define RCC_MCO_CLK_DIV11      (RCC_CFG_MCOPRES_DIV11)
#define RCC_MCO_CLK_DIV12      (RCC_CFG_MCOPRES_DIV12)
#define RCC_MCO_CLK_DIV13      (RCC_CFG_MCOPRES_DIV13)
#define RCC_MCO_CLK_DIV14      (RCC_CFG_MCOPRES_DIV14)
#define RCC_MCO_CLK_DIV15      (RCC_CFG_MCOPRES_DIV15)
#define RCC_MCO_CLK_DIV32      (RCC_CFG_MCOPRES_DIV32)

/** RCC_Flag **/
#define RCC_FLAG_HSI40MRDF               ((uint8_t)0x20)
#define RCC_FLAG_HSI48MRDF               ((uint8_t)0x21)
#define RCC_FLAG_LSIRDF                  ((uint8_t)0x41)
#define RCC_FLAG_LSIDETFF                ((uint8_t)0x58)
#define RCC_FLAG_PINRST                  ((uint8_t)0x63)
#define RCC_FLAG_PORRST                  ((uint8_t)0x64)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x65)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x66)
#define RCC_FLAG_LVRRST                  ((uint8_t)0x67)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x68)
#define RCC_FLAG_EMCCLAMPF               ((uint8_t)0x6B)

/** RCC Reset Flag clear**/
#define RCC_REMOVE_RESET_FLAG (RCC_CTRLSTS_RMRSTF)

void RCC_Reset(void);
void RCC_HSI_Calibration_Value_Set(uint8_t calibration_value);
void RCC_LSI_Calibration_Value_Set(uint8_t calibration_value);
void RCC_Sysclk_Config(uint32_t HSI_div);
void RCC_Hclk_Config(uint32_t sysclk_div);
void RCC_Pclk_Config(uint32_t hclk_div);
void RCC_TIM1_Clock_Config(uint32_t timer_clksrc);
void RCC_ADC_1M_Clock_Config(uint32_t ADC1M_prescaler);
void RCC_ADC_Clock_Config(uint32_t ADC_prescaler);
void RCC_TIM6_Clock_Config(uint32_t timer_clksrc);
void RCC_Clocks_Frequencies_Value_Get(RCC_ClocksType* RCC_clocks);
void RCC_AHB_Peripheral_Clock_Enable(uint32_t AHB_periph);
void RCC_AHB_Peripheral_Clock_Disable(uint32_t AHB_periph);
void RCC_APB_Peripheral_Clock_Enable(uint32_t APB_periph);
void RCC_APB_Peripheral_Clock_Disable(uint32_t APB_periph);
void RCC_Peripheral_Reset(uint32_t Periph);
void RCC_MCO_Prescaler_Config(uint32_t MCO_prescaler);
void RCC_MCO_Source_Config(uint32_t MCO_source);
FlagStatus RCC_Flag_Status_Get(uint8_t RCC_flag);
void RCC_Reset_Flag_Clear(void);
void RCC_EMC_Reset_Disable(void);
void RCC_EMC_Reset_Enable(void);
void RCC_EMC_Detect_Disable(void);
void RCC_EMC_Detect_Enable(void);
void RCC_LSI_Detect_Disable(void);
void RCC_LSI_Detect_Enable(void);
uint16_t RCC_LSI_Clocks_Frequencies_Get(void);


#ifdef __cplusplus
}
#endif

#endif /* __N32G003_RCC_H__ */


