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
*\*\file n32g003_adc.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32G003_ADC_H__
#define __N32G003_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "n32g003.h"

/** ADC init structure definition **/
typedef struct
{
    FunctionalState MultiChEn;      /* Specifies whether the conversion is performed in
                                       Scan (multichannels) or Single (one channel) mode */

    FunctionalState ContinueConvEn; /* Specifies whether the conversion is performed in
                                       Continuous or Single mode */

    uint32_t ExtTrigSelect;         /* Defines the external trigger used to start the analog
                                       to digital conversion of regular channels */

    uint32_t DatAlign;              /* Specifies whether the ADC data alignment is left or right */

    uint32_t ChsNumber;             /* Specifies the number of ADC channels that will be converted
                                       using the sequencer for regular channel group */
} ADC_InitType;
/** ADC scan conversion define **/
#define ADC_MULTCH_ENABLE                    ((uint32_t)(0x00001000)) /* ADC_CTRL2 SCANMD bits */ 
#define ADC_MULTCH_DISABLE                   ((uint32_t)(~0x00001000)) 

/** ADC continue conversion define **/
#define ADC_CTU_ENABLE                       ((uint32_t)(0x00000002)) /* ADC_CTRL2 CTU bits */
#define ADC_CTU_DISABLE                      ((uint32_t)(~0x00000002)) 

/** ADC external trigger sources for regular channels conversion define **/
#define ADC_EXT_TRIGCONV_REGULAR_MASK        ((uint32_t)(~(0x00000078|0x00000080))) /* ADC_CTRL2 EXTRSEL[3:0] bits Mask */
#define ADC_EXT_TRIGCONV_REGULAR_T1_CC1      ((uint32_t)(0x00000000|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_T1_CC2      ((uint32_t)(0x00000008|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_T1_CC3      ((uint32_t)(0x00000010|0x00000080))
#define ADC_EXT_TRIGCONV_REGULAR_T1_CC4      ((uint32_t)(0x00000018|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_T1_TRGO     ((uint32_t)(0x00000020|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_T3_TRGO     ((uint32_t)(0x00000028|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_T3_CC1      ((uint32_t)(0x00000030|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_T3_CC2      ((uint32_t)(0x00000038|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_EXT_INT11   ((uint32_t)(0x00000040|0x00000080)) 
#define ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH    ((uint32_t)0x00000048) 

/** ADC data alignment define **/
#define ADC_ALIG_MASK                        ((uint32_t)(~0x00000004)) /* ADC_CTRL2 ALIG bits Mask */
#define ADC_DAT_ALIGN_R                      ((uint32_t)0x00000000)
#define ADC_DAT_ALIGN_L                      ((uint32_t)0x00000004)

/** ADC regular channel sequence length define **/
#define ADC_REGULAR_LEN_MSAK                 ((uint32_t)(~0x00000E00)) /* ADC_CTRL2 LEN[2:0] bits Mask */
#define ADC_REGULAR_LEN_1                    ((uint32_t)0x00000000)
#define ADC_REGULAR_LEN_2                    ((uint32_t)0x00000200)
#define ADC_REGULAR_LEN_3                    ((uint32_t)0x00000400)
#define ADC_REGULAR_LEN_4                    ((uint32_t)0x00000600)
#define ADC_REGULAR_LEN_5                    ((uint32_t)0x00000800)

/** ADC channels define **/
#define ADC_SQR_MASK                         ((uint32_t)~0x000f0000)
#define ADC_SQR_OFFSET                       ((uint32_t)0X10)
#define ADC_CH_0                             ((uint8_t)0x00)
#define ADC_CH_1                             ((uint8_t)0x01)
#define ADC_CH_2                             ((uint8_t)0x02)
#define ADC_CH_3                             ((uint8_t)0x03)
#define ADC_CH_4                             ((uint8_t)0x04)
#define ADC_CH_5                             ((uint8_t)0x05)
#define ADC_CH_6                             ((uint8_t)0x06)
#define ADC_CH_7                             ((uint8_t)0x07)
#define ADC_CH_8                             ((uint8_t)0x08)
#define ADC_CH_9                             ((uint8_t)0x09)
#define ADC_CH_10                            ((uint8_t)0x0A)
#define ADC_CH_INT_VBG                       ((uint8_t)ADC_CH_9)
#define ADC_CH_INT_VDDA                      ((uint8_t)ADC_CH_10)

#define ADC_Channel_00_PA1                   ((uint8_t)0x00)
#define ADC_Channel_01_PA2                   ((uint8_t)0x01)
#define ADC_Channel_02_PA5                   ((uint8_t)0x02)
#define ADC_Channel_03_PA10                  ((uint8_t)0x03)
#define ADC_Channel_04_PA12                  ((uint8_t)0x04)
#define ADC_Channel_05_PA13                  ((uint8_t)0x05)
#define ADC_Channel_06_PA14                  ((uint8_t)0x06)
#define ADC_Channel_07_PB0                   ((uint8_t)0x07)
#define ADC_Channel_08_PB1                   ((uint8_t)0x08)
#define ADC_Channel_09_VBG                   ((uint8_t)0x09)
#define ADC_Channel_10_VDDA                  ((uint8_t)0x0A)
/** ADC converter operation define **/
#define ADC_TURN_ON                          ((uint32_t)0x00000001)    /* A/D Converter ON / OFF */
#define ADC_TURN_OFF                         ((uint32_t)(~0x00000001)) 

/** ADC interrupts define **/
#define ADC_INT_ENDC                         ((uint32_t)0x00000010)   /* Interrupt enable for EOC */
#define ADC_INT_AWD                          ((uint32_t)0x00000020)   /* Analog Watchdog interrupt enable */
#define ADC_INT_ENDCA                        ((uint32_t)0x00000020)  /* Interrupt enable for any EOC */
#define ADC_SET_INT_TYPE_ENDCA               ((uint8_t) 0x01)  /* Interrupt enable for any EOC */
#define ADC_SET_INT_TYPE_OTHER               ((uint8_t) 0x00)  /* Interrupt enable for other */


#define ADC_EXTTRIGCONV_REGULAR_ENABLE		 ((uint32_t)ADC_CTRL2_EXTRTRIG)
#define ADC_EXTTRIGCONV_REGULAR_DISABLE		 ((uint32_t)(~ADC_CTRL2_EXTRTRIG))

#define ADC_EXTRTRIG_SWSTRRCH_ENABLE         ((uint32_t)(ADC_CTRL2_EXTRTRIG | ADC_CTRL2_SWSTRRCH))
#define ADC_EXTRTRIG_SWSTRRCH_DISABLE        ((uint32_t)(~(ADC_CTRL2_EXTRTRIG | ADC_CTRL2_SWSTRRCH)))
#define ADC_EXTRTRIG_SWSTRRCH_GET_STS        ((uint32_t)ADC_CTRL2_SWSTRRCH)


/** ADC sampling time define **/
#define ADC_SAMP_TIME_MASK                   	 ((uint32_t)~0x0000001F)
#define ADC_SAMP_TIME_RESERVED                   ((uint32_t)0x0)
#define ADC_SAMP_TIME_8CYCLES                    ((uint32_t)0x1)
#define ADC_SAMP_TIME_12CYCLES                   ((uint32_t)0x2)
#define ADC_SAMP_TIME_14CYCLES                   ((uint32_t)0x3)
#define ADC_SAMP_TIME_20CYCLES                   ((uint32_t)0x4)
#define ADC_SAMP_TIME_26CYCLES                   ((uint32_t)0x5)
#define ADC_SAMP_TIME_30CYCLES                   ((uint32_t)0x6)
#define ADC_SAMP_TIME_42CYCLES                   ((uint32_t)0x7)
#define ADC_SAMP_TIME_56CYCLES                   ((uint32_t)0x8)
#define ADC_SAMP_TIME_72CYCLES                   ((uint32_t)0x9)
#define ADC_SAMP_TIME_88CYCLES                   ((uint32_t)0xA)
#define ADC_SAMP_TIME_120CYCLES                  ((uint32_t)0xB)
#define ADC_SAMP_TIME_182CYCLES                  ((uint32_t)0xC)
#define ADC_SAMP_TIME_240CYCLES                  ((uint32_t)0xD)
#define ADC_SAMP_TIME_380CYCLES                  ((uint32_t)0xE)
#define ADC_SAMP_TIME_760CYCLES                  ((uint32_t)0xF)
#define ADC_SAMP_TIME_1520CYCLES                 ((uint32_t)0x10)
#define ADC_SAMP_TIME_3040CYCLES                 ((uint32_t)0x11)

/** ADC analog watchdog mode define **/
#define ADC_ANALOG_WTDG_MODE_MASK            ((uint32_t)(~0x00000040))
#define ADC_ANALOG_WTDG_SINGLE_MODE          ((uint32_t)0x00000040)/* Enable the watchdog on a single channel in scan mode */
#define ADC_ANALOG_WTDG_SCAN_MODE            ((uint32_t)0x00000000)


/** ADC analog watchdog single mode channel define**/
#define ADC_ANALOG_WTDG_SINGLE_CH_MASK       ((uint32_t)(~0x0000000F))/* AWDG_CH[4:0] bits (Analog watchdog channel select bits) */
#define ADC_ANALOG_WTDG_SINGLE_CH0           ((uint8_t)0X00)
#define ADC_ANALOG_WTDG_SINGLE_CH1           ((uint8_t)0X01)
#define ADC_ANALOG_WTDG_SINGLE_CH2           ((uint8_t)0X02)
#define ADC_ANALOG_WTDG_SINGLE_CH3           ((uint8_t)0X03)
#define ADC_ANALOG_WTDG_SINGLE_CH4           ((uint8_t)0X04)
#define ADC_ANALOG_WTDG_SINGLE_CH5           ((uint8_t)0X05)
#define ADC_ANALOG_WTDG_SINGLE_CH6           ((uint8_t)0X06)
#define ADC_ANALOG_WTDG_SINGLE_CH7           ((uint8_t)0X07)
#define ADC_ANALOG_WTDG_SINGLE_CH8           ((uint8_t)0X08)
#define ADC_ANALOG_WTDG_SINGLE_CH9           ((uint8_t)0X09)
#define ADC_ANALOG_WTDG_SINGLE_CH10          ((uint8_t)0X0A)

/** ADC ADC Analog watchdog on regular channels define **/
#define ADC_ANALOG_WTDG_REGULAR              ((uint32_t)0x00000080)    /* ADC_CTRL1 AWDGERCH bit */

/** ADC flags definition **/
#define ADC_RUN_FLAG                         ((uint8_t)0x01)
#define ADC_RD_FLAG                          ((uint8_t)0x02)

#define ADC_INT_FLAG_AWDG                    ((uint8_t)0x01)
#define ADC_INT_FLAG_ENDC                    ((uint8_t)0x02)
#define ADC_INT_FLAG_ENDCA                   ((uint8_t)0x08)


#define ADC_FLAG_AWDG                        ((uint8_t)0x01)
#define ADC_FLAG_ENDC                        ((uint8_t)0x02)
#define ADC_FLAG_STR                         ((uint8_t)0x04)
#define ADC_FLAG_ENDCA                       ((uint8_t)0x08)
/** ADC flags ex definition **/
#define ADC_FLAG_BUF_RDY                     ((uint8_t)0x04)
#define ADC_FLAG_RDY                         ((uint8_t)0x08)
#define ADC_FLAG_PD_RDY                      ((uint8_t)0x10)


/** ADC regular data register define **/
#define ADC_DATA_OFFSET0                      ((uint8_t)0x00)
#define ADC_DATA_OFFSET1                      ((uint8_t)0X01)
#define ADC_DATA_OFFSET2                      ((uint8_t)0X02)
#define ADC_DATA_OFFSET3                      ((uint8_t)0X03)
#define ADC_DATA_OFFSET4                      ((uint8_t)0X04)

/** ADC internal input buffer  definition **/ 
#define ADC_IN_BUFFER_ENABLE      			  ((uint32_t)0x00000002)    /*ADC internal input buffer enable.*/
#define ADC_IN_BUFFER_DISABLE     			  ((uint32_t)(~0x00000002))

/* ADC data registers read define */
#define ADC_DATA_REG_READ(data, num)     	  (*(uint32_t*)(&data+num))



/** ADC Driving Functions Declaration **/

void ADC_Reset(void);

void ADC_Multichannels_Enable(void);
void ADC_Multichannels_Disable(void);

void ADC_Continue_Conversion_Enable(void);
void ADC_Continue_Conversion_Disable(void);

void ADC_Regular_Group_External_Trigger_Source_Config(uint32_t external_trigger_sources);
void ADC_Data_Alignment_Config(uint32_t data_alignment);
void ADC_Regular_Channels_Number_Config(uint32_t channels_number);

void ADC_Initializes(ADC_InitType* ADC_initstruct);
void ADC_Initializes_Structure(ADC_InitType* ADC_initstruct);

void ADC_ON(void);
void ADC_OFF(void);

void ADC_Interrupts_Enable(u8 eoc_any, uint32_t adc_interrupt);
void ADC_Interrupts_Disable(u8 eoc_any, uint32_t adc_interrupt);

FlagStatus ADC_Regular_Channels_Software_Conversion_Operation(uint32_t conversion_operation);

void ADC_Channel_Sample_Time_Config(uint8_t sample_time);
void ADC_External_Trigger_Conversion_Config(uint32_t group_operation);

uint16_t ADC_Regular_Conversion_Data_Get(void);
uint16_t ADC_Regular_Group_Conversion_Data_Get(uint8_t number);

void ADC_Analog_Watchdog_Mode_Channel_Config(uint32_t mode, uint8_t channel);
void ADC_Analog_Watchdog_Enable(uint32_t wcdg_mode);
void ADC_Analog_Watchdog_Disable(uint32_t wcdg_mode);
void ADC_Analog_Watchdog_HighThresholds_Config(uint16_t high_thresholds);
void ADC_Analog_Watchdog_LowThresholds_Config(uint16_t low_thresholds);

FlagStatus ADC_INTFlag_Status_Get(uint8_t adc_flag);
FlagStatus ADC_Flag_Status_Get(uint8_t selflag, uint8_t adc_runflag, uint8_t adc_rdflag);
void ADC_INTFlag_Status_Clear(uint8_t adc_flag);
void ADC_Flag_Status_Clear(uint8_t adc_flag);

void ADC_Regular_Sequence_Single_Config(uint8_t channel);
void ADC_Regular_Sequence_Multi_Config(uint8_t channel,uint8_t number);

void ADC_Internal_Input_Buffer_Enable(void);
void ADC_Internal_Input_Buffer_Disable(void);

#ifdef __cplusplus
}
#endif

#endif /*__N32G003_ADC_H__ */

