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
*\*\file      bsp_spi_flash.h
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
*/
#ifndef __BSP_SPI_FLASH_H__
#define __BSP_SPI_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "n32g003.h"
#include <stdio.h>

/* Flash ID Define */
#define SPI_Flash_ID                 0x00EF4018

/* Flash Page Size */
#define Flash_Page_Size              256
#define Flash_Per_Write_Page_Size    256

/* W25x Control Conmmand */
#define W25x_Write_Enable            0x06
#define W25x_Write_Disable           0x04
#define W25x_Read_Status_Register    0x05
#define W25x_Write_Status_Register   0x01
#define W25x_Read_Status_Register3   0x15
#define W25x_Read_Data               0x03
#define W25x_Fast_Read_Data          0x0B
#define W25x_Fast_Read_Dual          0x3B
#define W25x_Page_Program            0x02
#define W25x_Block_Erase             0xD8
#define W25x_Sector_Erase            0x20
#define W25x_Chip_Erase              0xC7
#define W25x_Power_Down              0xB9
#define W25x_Wake_Up                 0xAB
#define W25x_Divece_ID               0xAB
#define W25x_Manufactor_Device_ID    0x90
#define W25x_Jedec_Device_ID         0x9F
#define W25x_Four_Byte_Mode          0xB7

/* Write In Progress (WIP) flag */
#define Flash_WIP_Flag               0x01   
#define Flash_Dummy_Byte             0xFF

	
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
}TestStatus;

/* SPI Macro define */
#define SPI_MASTER                    SPI
#define SPI_MASTER_PERIPH             RCC_APB_PERIPH_SPI

/* SPI_NSS Macro define */
#define SPI_MASTER_NSS_GPIO           GPIOA
#define SPI_MASTER_NSS_CLK            RCC_APB_PERIPH_IOPA
#define SPI_MASTER_NSS_PIN            GPIO_PIN_3

/* SPI_SCK Macro define */
#define SPI_MASTER_SCK_GPIO           GPIOA
#define SPI_MASTER_SCK_CLK            RCC_APB_PERIPH_IOPA
#define SPI_MASTER_SCK_PIN            GPIO_PIN_15
#define SPI_MASTER_SCK_AF             GPIO_AF0_SPI

/* SPI_MISO Macro define */
#define SPI_MASTER_MISO_GPIO          GPIOA
#define SPI_MASTER_MISO_CLK           RCC_APB_PERIPH_IOPA
#define SPI_MASTER_MISO_PIN           GPIO_PIN_7
#define SPI_MASTER_MISO_AF            GPIO_AF0_SPI

/* SPI_MOSI Macro define */
#define SPI_MASTER_MOSI_GPIO          GPIOA
#define SPI_MASTER_MOSI_CLK           RCC_APB_PERIPH_IOPA
#define SPI_MASTER_MOSI_PIN           GPIO_PIN_6
#define SPI_MASTER_MOSI_AF            GPIO_AF0_SPI

/* NSS Control Command */
#define SPI_Flash_NSS_Low()           {SPI_MASTER_NSS_GPIO->PBC = SPI_MASTER_NSS_PIN;}
#define SPI_Flash_NSS_High()          {SPI_MASTER_NSS_GPIO->PBSC = SPI_MASTER_NSS_PIN;}

/* Wait timeout */
#define SPI_Flag_Wait_Timeout         ((uint32_t)0x1000)
#define SPI_Long_Wait_Timeout         ((uint32_t)(10 * SPI_Flag_Wait_Timeout))

/* Debug information output */
#define SPI_Flash_Debug_On            1

#define Flash_Info(fmt,arg...)        printf("<<--Flash-Info-->>"fmt"\n", ##arg)
#define Flash_Error(fmt,arg...)       printf("<<--Flash-Error-->>"fmt"\n", ##arg)
#define Flash_Debug(fmt,arg...)       do{\
	                                       if(SPI_Flash_Debug_On)\
																					 printf("<<--Flash-Debug-->> [%d]"fmt"\n",__LINE__, ##arg);\
																			 }while(0)

	
void SPI_Configuration(void);
uint8_t SPI_Flash_Send_Byte(uint8_t data);
uint16_t SPI_Flash_Send_Half_World(uint16_t half_world);
void SPI_Flash_Write_Enable(void);
void SPI_Flash_Wait_Write_End(void);
void SPI_Flash_Power_Down(void);
void SPI_Flash_Wake_Up(void);
void SPI_Flash_Sector_Erase(uint32_t sector_addr);
void SPI_Flash_Erase(void);
void SPI_Flash_Page_Write(u8* buffer, u32 write_addr, u32 NumByteToWrite);
void SPI_Flash_Buffer_Write(u8* buffer, u32 write_addr, u32 NumByteToWrite);
void SPI_Flash_Buffer_Read(u8* buffer, u32 read_addr, u32 NumByteToRead);
uint32_t SPI_Flash_Read_ID(void);
uint32_t SPI_Flash_Read_Device_ID(void);
void SPI_Flash_Start_Read_Sequence(u32 read_addr);
uint8_t SPI_Flash_Read_Byte(void);																			 
void SPI_Flash_Four_Byte_Address_Mode(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __BSP_SPI_FLASH_H__ */

