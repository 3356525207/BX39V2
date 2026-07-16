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

/***
*\*\file n32g003_exti.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
***/

#ifndef __N32G003_EXTI_H
#define __N32G003_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "n32g003.h"
	 
/*** EXTI Structure Definition Start ***/

/** EXTI mode enumeration **/

typedef enum
{
    EXTI_Mode_Interrupt = 0x00,
    EXTI_Mode_Event = 0x04
} EXTI_ModeType;


/** EXTI Trigger enumeration **/

typedef enum
{
    EXTI_Trigger_Rising 				= 0x08,
    EXTI_Trigger_Falling 				= 0x0C,  
    EXTI_Trigger_Rising_Falling = 0x10
} EXTI_TriggerType;


/** EXTI Init Structure definition **/

typedef struct
{
    uint32_t EXTI_Line;  /* Specifies the EXTI lines to be enabled or disabled. */                                 
   
    EXTI_ModeType EXTI_Mode; /* Specifies the mode for the EXTI lines. */                                   

    EXTI_TriggerType EXTI_Trigger; /* Specifies the trigger signal active edge for the EXTI lines. */
                                         
    FunctionalState EXTI_LineCmd; /* Specifies the new state of the selected EXTI lines. */ 
} EXTI_InitType;

/*** EXTI Structure Definition End ***/


/*** EXTI Macro Definition Start ***/

/** EXTI_Lines **/
#define EXTI_LINENONE ((uint32_t)0x00000) /* No interrupt selected */

#define EXTI_LINE0  ((uint32_t)0x00001) /* External interrupt line 0 Connected to the PA0 */
#define EXTI_LINE1  ((uint32_t)0x00002) /* External interrupt line 1 Connected to the PA1 */
#define EXTI_LINE2  ((uint32_t)0x00004) /* External interrupt line 2 Connected to the PA2 */
#define EXTI_LINE3  ((uint32_t)0x00008) /* External interrupt line 3 Connected to the PA3 */
#define EXTI_LINE4  ((uint32_t)0x00010) /* External interrupt line 4 Connected to the PA4 */
#define EXTI_LINE5  ((uint32_t)0x00020) /* External interrupt line 5 Connected to the PA5 */
#define EXTI_LINE6  ((uint32_t)0x00040) /* External interrupt line 6 Connected to the PA6 */
#define EXTI_LINE7  ((uint32_t)0x00080) /* External interrupt line 7 Connected to the PA7 */
#define EXTI_LINE8  ((uint32_t)0x00100) /* External interrupt line 8 Connected to the PA8 */
#define EXTI_LINE9  ((uint32_t)0x00200) /* External interrupt line 9 Connected to the PA9 */
#define EXTI_LINE10 ((uint32_t)0x00400) /* External interrupt line 10 Connected to the PA10 */
#define EXTI_LINE11 ((uint32_t)0x00800) /* External interrupt line 11 Connected to the PA11 */
#define EXTI_LINE12 ((uint32_t)0x01000) /* External interrupt line 12 Connected to the PA12 */
#define EXTI_LINE13 ((uint32_t)0x02000) /* External interrupt line 13 Connected to the PA13 */
#define EXTI_LINE14 ((uint32_t)0x04000) /* External interrupt line 14 Connected to the PA14 */
#define EXTI_LINE15 ((uint32_t)0x08000) /* External interrupt line 15 Connected to the PA15 */
#define EXTI_LINE16 ((uint32_t)0x10000) /* External interrupt line 16 Connected to the PB0 */
#define EXTI_LINE17 ((uint32_t)0x20000) /* External interrupt line 17 Connected to the PB1 */
#define EXTI_LINE18 ((uint32_t)0x40000) /* External interrupt line 18 Connected to the PVD Output */                                    
#define EXTI_LINE19 ((uint32_t)0x80000) /* External interrupt line 19 Connected to the TIM6 interrupt */


/*** EXTI Macro Definition End ***/

/** EXTI Driving Functions Declaration **/

void EXTI_Reset(void);
void EXTI_Peripheral_Initializes(EXTI_InitType* EXTI_InitStruct);
void EXTI_Work_Mode_Config(uint32_t exti_line,uint32_t exti_mode);
void EXTI_Trigger_Config(uint32_t exti_line,uint32_t exti_trigger);
void EXTI_LineCmd_Disable(uint32_t exti_line,uint32_t exti_linecmd,uint32_t exti_mode);
void EXTI_Structure_Initializes(EXTI_InitType* EXTI_InitStruct);
void EXTI_Software_Interrupt_Trigger(uint32_t exti_line);
FlagStatus EXTI_Flag_Status_Get(uint32_t exti_line);
void EXTI_Flag_Status_Clear(uint32_t exti_line);
ITStatus EXTI_Interrupt_Status_Get(uint32_t exti_line);
void EXTI_Interrupt_Status_Clear(uint32_t exti_line);

#ifdef __cplusplus
}
#endif

#endif /* __N32G003_EXTI_H__ */

