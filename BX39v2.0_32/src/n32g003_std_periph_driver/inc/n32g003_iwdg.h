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
*\*\file n32g003_iwdg.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __n32g003_IWDG_H
#define __n32g003_IWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "n32g003.h"   

typedef enum 
{
    IWDG_PVU_FLAG  = IWDG_STS_PVU,
    IWDG_CRVU_FLAG = IWDG_STS_CRVU
}IWDG_STATUS_FLAG;

/** KEY register bit mask **/
#define KEY_RELOAD_COUNTER                 ((uint16_t)0xAAAA)
#define KEY_ENABLE_COUNTER                 ((uint16_t)0xCCCC)
#define IWDG_FREEZE                        ((uint16_t)0xDDDD)
#define IWDG_UNFREEZE                      ((uint16_t)0xDDDD)

/** PREDIV and RELV register write permission **/
typedef enum 
{
    IWDG_WRITE_CONFIG_ENABLE  = 0x5555,
    IWDG_WRITE_CONFIG_DISABLE = 0x0000
}IWDOG_WRITE_CONFIG;


typedef enum 
{
    IWDG_CONFIG_PRESCALER_DIV4   = (~IWDG_PREDIV_PD),
    IWDG_CONFIG_PRESCALER_DIV8   = (IWDG_PREDIV_PD0),
    IWDG_CONFIG_PRESCALER_DIV16  = (IWDG_PREDIV_PD1),
    IWDG_CONFIG_PRESCALER_DIV32  = (IWDG_PREDIV_PD1 | IWDG_PREDIV_PD0),
    IWDG_CONFIG_PRESCALER_DIV64  = (IWDG_PREDIV_PD2),
    IWDG_CONFIG_PRESCALER_DIV128 = (IWDG_PREDIV_PD2 | IWDG_PREDIV_PD0),
    IWDG_CONFIG_PRESCALER_DIV256 = (IWDG_PREDIV_PD2 | IWDG_PREDIV_PD1 | IWDG_PREDIV_PD0)
}IWDG_CONFIG_PRESCALER;

#define IWDG_FREEZE_FLAG    ((uint16_t)IWDG_FREEZE_FREEZE)

void IWDG_Write_Protection_Disable(void);
void IWDG_Write_Protection_Enable(void);
void IWDG_Prescaler_Division_Set(IWDG_CONFIG_PRESCALER IWDG_prescaler);
void IWDG_Counter_Reload(uint16_t reload_value);
void IWDG_Key_Reload(void);
void IWDG_Enable(void);
ErrorStatus IWDG_Freeze_Enable(void);
ErrorStatus IWDG_Restore_From_Freeze(void);
FlagStatus IWDG_Status_Get(IWDG_STATUS_FLAG IWDG_flag);

#ifdef __cplusplus
}
#endif

#endif /* __n32g003_IWDG_H */
