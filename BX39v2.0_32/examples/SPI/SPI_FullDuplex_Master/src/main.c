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
*\*\file      main.c
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
*/

#include "log.h"
#include "bsp_spi_master.h"

#define BufferSize 32

uint8_t SPI_Buffer_Tx0[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                      0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                      0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};

uint8_t SPI_Buffer_Tx1[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B,
                                      0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
                                      0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70};	

uint8_t SPI_Buffer_Rx0[BufferSize];

__IO uint8_t TxIdx = 0, RxIdx = 0;

volatile TestStatus TransferStatus = FAILED;

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


/**
 *\*\name    main.
 *\*\fun     Main function
 *\*\param   none
 *\*\return  
**/
int main(void)
{
    log_init();
    log_info("This is SPI_Master FullDuplex Soft NSS Demo!\r\n");

    /* SPI configuration */
    SPI_Configuration();

    while(TxIdx < BufferSize)
    {
        /* Pull-down NSS pin */
        GPIO_Pins_Reset(SPI_MASTER_NSS_GPIO, SPI_MASTER_NSS_PIN);
        
        /* Wait for SPI Tx Buffer empty */
        while(SPI_Flag_Status_Get(SPI_FLAG_TE) == RESET);
        
        /* Send data */
        SPI_Data_Transmit(SPI_Buffer_Tx0[TxIdx++]);
        
        /* Wait for data reception */
        while(SPI_Flag_Status_Get(SPI_FLAG_RNE) == RESET);
        
        /* Receive data */
        SPI_Buffer_Rx0[RxIdx++] = SPI_Data_Get();
        
        /* Pull-up NSS pin */
        GPIO_Pins_Set(SPI_MASTER_NSS_GPIO, SPI_MASTER_NSS_PIN);
    }

    /* Disable SPI Master */
    SPI_OFF();

    TransferStatus = Buffercmp(SPI_Buffer_Rx0, SPI_Buffer_Tx1, BufferSize);

    if(TransferStatus == PASSED)
    {
        log_info("Test Passed!\r\n");
    }
    else
    {
        log_info("Test Failed!\r\n");
    }

    while(1)
    {
    }
}


/**
 *\*\name    Buffercmp.
 *\*\fun     Compares two buffers.
 *\*\param   pBuffer1
 *\*\param   pBuffer2
 *\*\param   BufferLength
 *\*\return  FAILED or PASSED
**/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}


