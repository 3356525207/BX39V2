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
*\*\file n32g003_comp.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32G003_COMP_H
#define __N32G003_COMP_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "n32g003.h"
#include <stdbool.h> 
	 
/** Bit operation definition **/
#define SetBitMsk(reg, bit, msk) ((reg) = ((reg) & ~(msk) | (bit)))
#define ClrBit(reg, bit)         ((reg) &= ~(bit))
#define SetBit(reg, bit)         ((reg) |= (bit))
#define GetBit(reg, bit)         ((reg) & (bit))	 
	 
/** COMP init structure definition **/
typedef struct
{
    /* ctrl define */         
    uint32_t Blking;             /* Specifies which timer can control the comp output blanking with its capture event */
    uint32_t Hyst;               /* Specifies the comp hysteresis level with low/medium/high level */
    uint32_t PolRev;             /* Specifies the comp output polarity */
    uint32_t OutSel;             /* Specifies which timer input that can be connecte to the comp output */
    uint32_t SubSel;             /* Specifies the comp subtract level with 0/100mv/200mv/300mv level */
	uint32_t InpSel;             /* Specifies the comp inpsel */
    uint32_t InmSel;             /* Specifies the comp inmsel */
    uint32_t En;                 /* enable or disable the comp */

    /* filter define */
    uint8_t SampWindow;          /* Initializes comp sampwindow value ~5bit */
    uint8_t Threshold;           /* ~5bit ,need > SampWindow/2 */
    uint8_t FilterEn;            /* enable or disable the comp filter */

    /* filter prescale */
    uint16_t ClkPsc;             /* Initializes comp clkpsc value ~5bit */
} COMP_InitType;

/** COMP blanking definition **/
#define COMP_BLANKING_MASK                ((uint32_t)(~COMP_CTRL_BLKING))
#define COMP_BLANKING_NO                  ((uint32_t)0x00000000)
#define COMP_BLANKING_TIM1_OC5            ((uint32_t)COMP_CTRL_BLKING)

/** COMP hysteresis definition **/
#define COMP_HYST_MASK                    ((uint32_t)(~COMP_CTRL_HYST))
#define COMP_HYST_NO                      ((uint32_t)0x00000000)
#define COMP_HYST_LOW                     ((uint32_t)COMP_CTRL_HYST_0)
#define COMP_HYST_MID                     ((uint32_t)COMP_CTRL_HYST_1)
#define COMP_HYST_HIGH                    ((uint32_t)(COMP_CTRL_HYST_0 | COMP_CTRL_HYST_1))

/** COMP output polarity definition **/
#define COMP_OUTPOL_MASK                  ((uint32_t)(~COMP_CTRL_POL))
#define COMP_OUTPOL_FLIP                  ((uint32_t)COMP_CTRL_POL)
#define COMP_OUTPOL_NFLIP                 ((uint32_t)0x00000000)

/** COMP subtract definition **/
#define COMP_SUB_MASK                     ((uint32_t)(~COMP_CTRL_CMPVOS))
#define COMP_SUB_NO                       ((uint32_t)0x00000000)
#define COMP_SUB_100mV                    ((uint32_t)COMP_CTRL_CMPVOS_0)
#define COMP_SUB_200mV                    ((uint32_t)COMP_CTRL_CMPVOS_1)
#define COMP_SUB_300mV                    ((uint32_t)(COMP_CTRL_CMPVOS_0 | COMP_CTRL_CMPVOS_1))

/** COMP non inverting input definition **/
#define COMP_INPSEL_MASK                  ((uint32_t)(~COMP_CTRL_INPSEL))
#define COMP_INPSEL_RES                   ((uint32_t)0x00000000) 
 /** comp inp sel **/  
#define COMP_INPSEL_PB0                  ((uint32_t)0x00000000)
#define COMP_INPSEL_PA5                  ((uint32_t)0x00000004)

/** COMP inverting input definition **/
#define COMP_INMSEL_MASK                  ((uint32_t)(~COMP_CTRL_INMSEL))
#define COMP_INMSEL_RES                   ((uint32_t)0x00000000)
/** comp inm sel **/
#define COMP_INMSEL_PB1                  ((uint32_t)0x00000000)
#define COMP_INMSEL_PA4                  ((uint32_t)0x00000002)

/** COMP output connection definition **/
#define COMP_OUTSEL_MASK                  ((uint32_t)(~COMP_CTRL_OUTTRG))
#define COMP_OUTSEL_RES                   ((uint32_t)0x00000000)
/** comp out trig **/
#define COMP_OUTSEL_NO                   ((uint32_t)0x00000000)
#define COMP_OUTSEL_TIM1_BKIN            ((uint32_t)0x00000080)
#define COMP_OUTSEL_TIM1_IC1             ((uint32_t)0x00000100)
#define COMP_OUTSEL_TIM1_OCREFCLEAR      ((uint32_t)0x00000180)
#define COMP_OUTSEL_TIM3_IC1             ((uint32_t)0x00000200)
#define COMP_OUTSEL_TIM3_OCREFCLEAR      ((uint32_t)0x00000280)

/** COMP switch definition **/
#define COMP_ENABLE                       ((uint32_t)COMP_CTRL_EN)
#define COMP_DISABLE                      ((uint32_t)(~COMP_CTRL_EN))

/** COMP filter definition **/
#define COMP_FILTER_SAMPW_MASK            ((uint16_t)(~COMP_FILC_SAMPW)) /* Low filter sample window size. Number of samples to monitor is SAMPWIN+1. */
#define COMP_FILTER_THRESHOLD_MASK        ((uint16_t)(~COMP_FILC_THRESH)) /* For proper operation, the value of THRESHOLD must be greater than SAMPWIN / 2. */

#define COMP_FILTER_ENABLE                ((uint16_t)COMP_FILC_FILEN) /* Filter enable. */
#define COMP_FILTER_DISABLE               ((uint16_t)(~COMP_FILC_FILEN)) /* Filter disable. */ 

/** COMP filter prescale definition **/
#define COMP_FILTER_CLKPSC_MASK           ((uint16_t)(~COMP_FILP_CLKPSC)) /* Low filter sample clock prescale.Number of system clocks between samples = CLK_PRE_CYCLE + 1, e.g. */

/** COMP lock definition**/
#define COMP_LOCK_MSK                    ((uint16_t)(~COMP_LOCK_CMPLK)) /* COMP Lock bit */
#define COMP_LOCK                    	 ((uint16_t)COMP_LOCK_CMPLK) /* COMP Lock bit */

/** COMP interrupt enable definition **/
#define COMP_INTEN_MSK                    ((uint16_t)(~COMP_INTEN_CMPIEN))      
#define COMP_INTEN                        ((uint16_t)COMP_INTEN_CMPIEN) /* This bit control Interrput enable of COMP. */   

/** COMP interrupt status clear definition **/ 
#define COMP_INTS_CLEAR		              ((uint32_t)(~COMP_INTSTS_CMPIS))

/** COMP output status definition**/
#define COMP_OUT_MASK                     	  ((uint32_t)(~COMP_CTRL_OUT)) 
#define COMP_OUT                     	  	  ((uint32_t)COMP_CTRL_OUT) 

/** COMP Driving Functions Declaration **/
void COMP_Reset(void);

void COMP_Initializes_Structure(COMP_InitType* COMP_initstruct);
void COMP_Filter_SampWindow_Config(uint8_t sampwin_value);
void COMP_Filter_Threshold_Config(uint8_t Threshold_value);
void COMP_Filter_Enable(void);
void COMP_Filter_Disable(void);
void COMP_Filter_Clock_Prescale_Config(uint16_t clkpsc_value);
void COMP_Blking_Soucre_Config(uint32_t blking_mode);
void COMP_Hysteresis_Level_Config(uint32_t hyst_mode);
void COMP_Output_Polarity_Config(uint32_t output_pol);
void COMP_InpSel_Config(uint32_t vpsel);
void COMP_InmSel_Config(uint32_t vpsel);
void COMP_Subtract_Level_Config(uint32_t sub_mode);
void COMP_Output_Trigger_Config(uint32_t outtrgsel);

void COMP_ON(void);
void COMP_OFF(void);

void COMP_Lock_Config(void);
void COMP_Interrupt_Enable(uint32_t inten);
void COMP_Interrupt_Disable(uint32_t inten);
uint8_t COMP_Interrupt_Status_Get(void);
void COMP_Interrupt_Status_Clear(void);

FlagStatus COMP_Output_Status_Get(void);

void COMP_Filter_Control_Config(uint32_t sw, uint8_t threshold , uint8_t sampwindow);

void COMP_Initializes(COMP_InitType* COMP_initstruct);

#ifdef __cplusplus
}
#endif

#endif /* __N32G003_COMP_H */

