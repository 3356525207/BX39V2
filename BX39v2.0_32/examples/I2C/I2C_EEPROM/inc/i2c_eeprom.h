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
*\*\file i2c_eeprom.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef __I2C_EEPROM_H__
#define __I2C_EEPROM_H__

#include "n32g003.h"
#include <stdio.h>

typedef enum i2c_state
{
    COMM_DONE       = 0, /// done successfully
    COMM_PRE        = 1,
    COMM_IN_PROCESS = 2,
    COMM_EXIT       = 3 /// exit since failure
} I2C_STATE;

typedef enum i2c_direction
{
    Transmitter = 0x00,
    Receiver    = 0x01
} I2C_DIRECTION;

/**
 * Select PROCESS MODE
 * 0: polling
 * 1: interrupt
 */
#define PROCESS_MODE 0

#define I2C_TEST

#ifdef I2C_TEST
#define I2Cx I2C
#define I2Cx_SCL_PIN GPIO_PIN_4
#define I2Cx_SDA_PIN GPIO_PIN_5
#define GPIOx        GPIOA

#define I2Cx_peripheral_clk_en() RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_I2C)
#define I2Cx_scl_clk_en()        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA)
#define I2Cx_sda_clk_en()        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA)
#endif

#define TEST_EEPROM_SIZE 256
#define TEST_EEPROM_ADDR 0x00
#define I2C_Speed        400000
#define EEPROM_ADDRESS   0xA0
#define I2C_PageSize     8 /// eeprom IC type AT24C02
#define sEE_FLAG_TIMEOUT ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT ((uint32_t)(100 * sEE_FLAG_TIMEOUT))

/** Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER 150
#define FALSE                 0
#define TRUE                  1

void I2C_EE_Init(void);
void I2C_EE_WriteBuffer(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WriteOnePage(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WriteOnePageCompleted(void);

void I2C_EE_ReadBuffer(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitOperationIsCompleted(void);
void I2C_EE_WaitEepromStandbyState(void);
void IIC_RCCReset(void);

#endif /* __I2C_EEPROM_H__ */
