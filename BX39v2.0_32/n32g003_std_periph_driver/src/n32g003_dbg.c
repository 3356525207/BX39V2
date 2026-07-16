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
*\*\file n32g003_dbg.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32G003_DBG_H__
#define __N32G003_DBG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g003.h"

#define IDCODE_DEVID_MASK ((uint32_t)0x00000FFF)

/**
*\*\name    UCID_Get.
*\*\fun     get unique customer id.
*\*\param   UCIDbuf
*\*\return  none
**/
void UCID_Get(uint8_t *UCIDbuf)
{
    uint8_t num = 0;
    uint32_t* ucid_addr = (void*)0;
    uint32_t temp = 0;

    ucid_addr = (uint32_t*)UCID_BASE;
    
    for (num = 0; num < UCID_LENGTH;)
    {
        temp = *(__IO uint32_t*)(ucid_addr++); 
        UCIDbuf[num++] = (temp & 0xFF);
        UCIDbuf[num++] = (temp & 0xFF00) >> 8;
        UCIDbuf[num++] = (temp & 0xFF0000) >> 16;
        UCIDbuf[num++] = (temp & 0xFF000000) >> 24;
    }
}

/**
*\*\name    UID_Get.
*\*\fun     get unique device id.
*\*\param   UIDbuf
*\*\return  none
**/
void UID_Get(uint8_t *UIDbuf)
{
    uint8_t num = 0;
    uint32_t* uid_addr = (void*)0;
    uint32_t temp = 0;
    
    uid_addr = (uint32_t*)UID_BASE;
        
    for (num = 0; num < UID_LENGTH;)
    {
        temp = *(__IO uint32_t*)(uid_addr++);
        UIDbuf[num++] = (temp & 0xFF);
        UIDbuf[num++] = (temp & 0xFF00) >> 8;
        UIDbuf[num++] = (temp & 0xFF0000) >> 16;
        UIDbuf[num++] = (temp & 0xFF000000) >> 24;
    }
}

/**
*\*\name    DBGMCU_ID_Get.
*\*\fun     get debug MCU id.
*\*\param   DBGIDbuf
*\*\return  none
**/
void DBGMCU_ID_Get(uint8_t *DBGIDbuf)
{
    uint8_t num = 0;
    uint32_t* dbgid_addr = (void*)0;
    uint32_t temp = 0;
    
    dbgid_addr = (uint32_t*)DBGMCU_ID_BASE;
        
    for (num = 0; num < DBGMCU_ID_LENGTH;)
    {
        temp = *(__IO uint32_t*)(dbgid_addr++);
        DBGIDbuf[num++] = (temp & 0xFF);
        DBGIDbuf[num++] = (temp & 0xFF00) >> 8;
        DBGIDbuf[num++] = (temp & 0xFF0000) >> 16;
        DBGIDbuf[num++] = (temp & 0xFF000000) >> 24;
    }
}


#ifdef __cplusplus
}
#endif

#endif /*__N32G003_DBG_H__ */
