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
*\*\file n32g003_pwr.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */

#ifndef __N32G003_PWR_H
#define __N32G003_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "n32g003.h"

/*** PWR Structure Definition Start ***/

/** PWR wakeup PINx enmu definition **/
typedef enum
{
    WAKEUP_PIN1 = 0x0100,
    WAKEUP_PIN2 = 0x0200,
} WAKEUP_PINX;
    
/*** PWR Structure Definition End ***/

/*** PWR Macro Definition Start ***/

/** PWR CTRL register bit mask definition **/
#define PWR_CTRL_PLS_MASK   (~((uint32_t)PWR_CTRL_PLS))
#define PWR_CTRL_PDSTP_MASK (~((uint32_t)PWR_CTRL_PDSTP))
#define PWR_CTRL_PDRS_MASK  (~((uint32_t)PWR_CTRL_PDRS))
    
/** PWR CTRL2 register bit mask definition **/
#define PWR_CTRL2_LVRLS_MASK (~((uint32_t)PWR_CTRL2_LVRLS))

/** PWR CTRLSTS register bit mask definition **/
#define PWR_CTRLSTS_WKUPPOL_MASK (~((uint32_t)PWR_CTRLSTS_WKUPPOL))

/** PWR CTRL5 register bit mask definition **/
#define PWR_CTRL5_STPMRSEL_MASK (~((uint32_t)PWR_CTRL5_STPMRSEL))

/** PWR PVD enable definition **/
#define PWR_PVD_ENABLE ((uint32_t)PWR_CTRL_PVDEN)

/** PWR PVD interrupt enable definition **/
#define PWR_PVD_INT_ENABLE ((uint32_t)PWR_CTRL_PVDITEN)

/** PWR PVD Filter enable definition **/
#define PWR_PVD_FIL_ENABLE ((uint32_t)PWR_CTRL_PVDFILEN)

/** PWR LVR enable definition **/
#define PWR_LVR_ENABLE ((uint32_t)PWR_CTRL2_LVREN)

/** PWR LVR reset enable definition **/
#define PWR_LVR_RST_ENABLE ((uint32_t)PWR_CTRL2_LVRRSTEN)

/** PWR LVR Filter enable definition **/
#define PWR_LVR_FIL_ENABLE ((uint32_t)PWR_CTRL2_LVRFILEN)

/** PWR wakup PIN polarity definition **/
#define PWR_PIN_RISING  ((uint32_t)0x00000001)
#define PWR_PIN_FALLING ((uint32_t)0x00000000)

/** PWR clear WKUPF definition **/
#define PWR_CLEAR_WKUPF ((uint32_t)PWR_CTRL_CLRWKUPF)

/** PWR PVD output Flag definition **/
#define PWR_PVD_OUTPUT_FLAG ((uint32_t)PWR_CTRLSTS_PVDO)

/** PWR PD Flag definition **/
#define PWR_DBGPD_FLAG ((uint32_t)PWR_CTRLSTS_DBGPDF)

/** PWR PA2/PA1wakeup Flag definition **/
#define PWR_WKUP_FLAG ((uint32_t)PWR_CTRLSTS_WKUPF)

/** PWR LVR output Flag definition **/
#define PWR_LVR_OUTPUT_FLAG ((uint32_t)PWR_CTRL2_LVRO)

/** PWR PD mode definition **/
#define PWR_PD_MODE ((uint32_t)PWR_CTRL_PDSTP)

/** PWR control HSI definition **/
#define PWR_CONTROL_HSI ((uint32_t)PWR_CTRL3_HSIPWR)

/** PWR control PDR select signal definition **/
#define PWR_CONTROL_PDR_SEL ((uint32_t)PWR_CTRL3_PDRSEL)

/** PWR HSI 40M trim definition **/
#define PWR_HSI_TRIM_40M ((uint32_t)PWR_CTRL3_HSISEL)

/** PWR enable VDDD output voltage select definition **/
#define PWR_VDDD_OUTPUT_SEL_EN ((uint32_t)PWR_CTRL6_STPMREN)

/** PWR PVD threshold level definition **/
#define PWR_PVD_LEVEL_1V8 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    & PWR_CTRL_PLS_1 \
                                    & PWR_CTRL_PLS_2 \
                                    & PWR_CTRL_PLS_3))    /* 1.8v PWR_CTRL bit[8:5]:0000 */
                                    
#define PWR_PVD_LEVEL_2V0 ((uint32_t)PWR_CTRL_PLS_0)      /* 2.0v PWR_CTRL bit[8:5]:0001 */

#define PWR_PVD_LEVEL_2V2 ((uint32_t)PWR_CTRL_PLS_1)      /* 2.2v PWR_CTRL bit[8:5]:0010 */

#define PWR_PVD_LEVEL_2V4 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_1))    /* 2.4v PWR_CTRL bit[8:5]:0011 */
                                    
#define PWR_PVD_LEVEL_2V6 ((uint32_t)PWR_CTRL_PLS_2)      /* 2.6v PWR_CTRL bit[8:5]:0100 */

#define PWR_PVD_LEVEL_2V8 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_2))    /* 2.8v PWR_CTRL bit[8:5]:0101 */
                                    
#define PWR_PVD_LEVEL_3V0 ((uint32_t)(PWR_CTRL_PLS_1 \
                                    | PWR_CTRL_PLS_2))    /* 3.0v PWR_CTRL bit[8:5]:0110 */
                                    
#define PWR_PVD_LEVEL_3V2 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_1 \
                                    | PWR_CTRL_PLS_2))    /* 3.2v PWR_CTRL bit[8:5]:0111 */
                                    
#define PWR_PVD_LEVEL_3V4 ((uint32_t)PWR_CTRL_PLS_3)      /* 3.4v PWR_CTRL bit[8:5]:1000 */

#define PWR_PVD_LEVEL_3V6 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_3))    /* 3.6v PWR_CTRL bit[8:5]:1001 */
                                    
#define PWR_PVD_LEVEL_3V8 ((uint32_t)(PWR_CTRL_PLS_1 \
                                    | PWR_CTRL_PLS_3))    /* 3.8v PWR_CTRL bit[8:5]:1010 */
                                    
#define PWR_PVD_LEVEL_4V0 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_1 \
                                    | PWR_CTRL_PLS_3))    /* 4.0v PWR_CTRL bit[8:5]:1011 */
                                    
#define PWR_PVD_LEVEL_4V2 ((uint32_t)(PWR_CTRL_PLS_2 \
                                    | PWR_CTRL_PLS_3))    /* 4.2v PWR_CTRL bit[8:5]:1100 */
                                    
#define PWR_PVD_LEVEL_4V4 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_2 \
                                    | PWR_CTRL_PLS_3))    /* 4.4v PWR_CTRL bit[8:5]:1101 */
                                    
#define PWR_PVD_LEVEL_4V6 ((uint32_t)(PWR_CTRL_PLS_1 \
                                    | PWR_CTRL_PLS_2 \
                                    | PWR_CTRL_PLS_3))    /* 4.6v PWR_CTRL bit[8:5]:1110 */
                                    
#define PWR_PVD_LEVEL_4V8 ((uint32_t)(PWR_CTRL_PLS_0 \
                                    | PWR_CTRL_PLS_1 \
                                    | PWR_CTRL_PLS_2 \
                                    | PWR_CTRL_PLS_3))    /* 4.8v PWR_CTRL bit[8:5]:1111 */     

/** PWR LVR threshold level definition **/
#define PWR_LVR_LEVEL_1V8 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    & PWR_CTRL2_LVRLS_1 \
                                    & PWR_CTRL2_LVRLS_2 \
                                    & PWR_CTRL2_LVRLS_3)) /* 1.8v PWR_CTRL2 bit[14:11]:0000 */
                                    
#define PWR_LVR_LEVEL_2V0 ((uint32_t)PWR_CTRL2_LVRLS_0)   /* 2.0v PWR_CTRL2 bit[14:11]:0001 */

#define PWR_LVR_LEVEL_2V2 ((uint32_t)PWR_CTRL2_LVRLS_1)   /* 2.2v PWR_CTRL2 bit[14:11]:0010 */

#define PWR_LVR_LEVEL_2V4 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_1)) /* 2.4v PWR_CTRL2 bit[14:11]:0011 */
                                    
#define PWR_LVR_LEVEL_2V6 ((uint32_t)PWR_CTRL2_LVRLS_2)   /* 2.6v PWR_CTRL2 bit[14:11]:0100 */

#define PWR_LVR_LEVEL_2V8 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_2)) /* 2.8v PWR_CTRL2 bit[14:11]:0101 */
                                    
#define PWR_LVR_LEVEL_3V0 ((uint32_t)(PWR_CTRL2_LVRLS_1 \
                                    | PWR_CTRL2_LVRLS_2)) /* 3.0v PWR_CTRL2 bit[14:11]:0110 */
                                    
#define PWR_LVR_LEVEL_3V2 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_1 \
                                    | PWR_CTRL2_LVRLS_2)) /* 3.2v PWR_CTRL2 bit[14:11]:0111 */
                                    
#define PWR_LVR_LEVEL_3V4 ((uint32_t)PWR_CTRL2_LVRLS_3)   /* 3.4v PWR_CTRL2 bit[14:11]:1000 */

#define PWR_LVR_LEVEL_3V6 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_3)) /* 3.6v PWR_CTRL2 bit[14:11]:1001 */
                                    
#define PWR_LVR_LEVEL_3V8 ((uint32_t)(PWR_CTRL2_LVRLS_1 \
                                    | PWR_CTRL2_LVRLS_3)) /* 3.8v PWR_CTRL2 bit[14:11]:1010 */
                                    
#define PWR_LVR_LEVEL_4V0 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_1 \
                                    | PWR_CTRL2_LVRLS_3)) /* 4.0v PWR_CTRL2 bit[14:11]:1011 */
                                    
#define PWR_LVR_LEVEL_4V2 ((uint32_t)(PWR_CTRL2_LVRLS_2 \
                                    | PWR_CTRL2_LVRLS_3)) /* 4.2v PWR_CTRL2 bit[14:11]:1100 */
                                    
#define PWR_LVR_LEVEL_4V4 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_2 \
                                    | PWR_CTRL2_LVRLS_3)) /* 4.4v PWR_CTRL2 bit[14:11]:1101 */
                                    
#define PWR_LVR_LEVEL_4V6 ((uint32_t)(PWR_CTRL2_LVRLS_1 \
                                    | PWR_CTRL2_LVRLS_2 \
                                    | PWR_CTRL2_LVRLS_3)) /* 4.6v PWR_CTRL2 bit[14:11]:1110 */
                                    
#define PWR_LVR_LEVEL_4V8 ((uint32_t)(PWR_CTRL2_LVRLS_0 \
                                    | PWR_CTRL2_LVRLS_1 \
                                    | PWR_CTRL2_LVRLS_2 \
                                    | PWR_CTRL2_LVRLS_3)) /* 4.8v PWR_CTRL2 bit[14:11]:1111 */

/** PWR PDR trigger level definition **/
#define PWR_PDR_LEVEL_0V8 ((uint32_t)PWR_CTRL_PDRS_0)   /* 0.8v PWR_CTRL bit[10:9]:01 */

#define PWR_PDR_LEVEL_1V0 ((uint32_t)PWR_CTRL_PDRS_1)   /* 1.0v PWR_CTRL bit[10:9]:10 */

#define PWR_PDR_LEVEL_1V2 ((uint32_t)(PWR_CTRL_PDRS_0 \
                                    | PWR_CTRL_PDRS_1)) /* 0.2v PWR_CTRL bit[10:9]:11 */
                                    
/** PWR VDDD output voltage definition **/
#define PWR_VDDD_OUTPUT_1V5 ((uint32_t)PWR_CTRL5_STPMRSEL_0) /* 1.5v PWR_CTRL5 bit[3:2]:01 */

#define PWR_VDDD_OUTPUT_1V0 ((uint32_t)PWR_CTRL5_STPMRSEL_1) /* 1.0v PWR_CTRL5 bit[3:2]:10 */

#define PWR_VDDD_OUTPUT_1V2 ((uint32_t)(PWR_CTRL5_STPMRSEL_0 \
                                    | PWR_CTRL5_STPMRSEL_1)) /* 0.2v PWR_CTRL5 bit[3:2]:11 */

/** PWR STOP mode entry definition **/
#define PWR_STOP_ENTRY_WFI ((uint8_t)0x01) /* enter STOP mode with WFI instruction */
#define PWR_STOP_ENTRY_WFE ((uint8_t)0x02) /* enter STOP mode with WFE instruction */

/** PWR PD mode entry definition **/
#define PWR_PD_ENTRY_WFI ((uint8_t)0x01) /* enter PD mode with WFI instruction */
#define PWR_PD_ENTRY_WFE ((uint8_t)0x02) /* enter PD mode with WFE instruction */

/** PWR DBG_CTRL definition **/
#define PWR_DBG_SLEEP ((uint32_t)DBG_CTRL_SLEEP)
#define PWR_DBG_STOP  ((uint32_t)DBG_CTRL_STOP)
#define PWR_DBG_PD    ((uint32_t)DBG_CTRL_PD)
#define PWR_DBG_IWDG  ((uint32_t)DBG_CTRL_IWDGSTP)
#define PWR_DBG_TIM1  ((uint32_t)DBG_CTRL_TIM1STP)
#define PWR_DBG_TIM3  ((uint32_t)DBG_CTRL_TIM3STP)
#define PWR_DBG_TIM6  ((uint32_t)DBG_CTRL_TIM6STP)

/** PWR registers write protection keys definition **/
#define PWR_CTRL2_KEYS ((uint32_t)0xA5000000)
#define PWR_CTRL4_KEYS ((uint32_t)0x01753603)

/** PWR bit shift number definition **/
#define PWR_BIT_SHIFT_2  ((uint32_t)REG_BIT2_OFFSET)
#define PWR_BIT_SHIFT_10 ((uint32_t)REG_BIT10_OFFSET)

/*** PWR Macro Definition End ***/

/*** PWR Driving Functions Declaration ***/

void PWR_Reset(void);
void PWR_PVD_Enable(void);
void PWR_PVD_Disable(void);
void PWR_PVD_Interrupt_Enable(void);
void PWR_PVD_Interrupt_Disable(void);
void PWR_PVD_Filter_Enable(void);
void PWR_PVD_Filter_Disable(void);
void PWR_PVD_Level_Config(uint32_t level);
void PWR_LVR_Enable(void);
void PWR_LVR_Disable(void);
void PWR_LVR_Reset_Enable(void);
void PWR_LVR_Reset_Disable(void);
void PWR_LVR_Filter_Enable(void);
void PWR_LVR_Filter_Disable(void);
void PWR_LVR_Level_Config(uint32_t level);
void PWR_Wakeup_Pin_Enable(WAKEUP_PINX pin, uint32_t polarity);
void PWR_Wakeup_Pin_Disable(WAKEUP_PINX pin);
void PWR_STOP_Mode_Enter(uint8_t enter_mode);
void PWR_PD_Mode_Enter(uint8_t enter_mode);
void PWR_HSI_40M_Trim_Select(void);
void PWR_PDR_Trigger_Level_Config(uint32_t level);
void PWR_VDDD_Output_Config(uint32_t voltage_output);
void PWR_CTRL2_write_Protection_Enable(void);
void PWR_CTRL4_write_Protection_Enable(void);
FlagStatus PWR_Flag_Status_Get(uint32_t flag);
void PWR_Flag_Status_Clear(uint32_t flag);
void DBG_Peripheral_ON(uint32_t DBG_Periph);
void DBG_Peripheral_OFF(uint32_t DBG_Periph);

#ifdef __cplusplus
}
#endif

#endif /* __N32G003_PWR_H */

