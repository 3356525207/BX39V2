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
#include "bsp_spi_flash.h"


#define TxBufferSize1          (countof(TxBuffer1) - 1)
#define RxBufferSize1          (countof(TxBuffer1) - 1)
#define countof(a)             (sizeof(a) / sizeof(*(a)))
#define BufferSize             (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x700000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

uint8_t Tx_Buffer[] = "Thank you for using the Nationas Development Board\r\nhttps://www.nationstech.com";
uint8_t Rx_Buffer[BufferSize];

uint32_t DeviceID = 0;
uint32_t FlashID = 0;
TestStatus TransferStatus1 = FAILED;

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


/**
 *\*\name    Delay.
 *\*\fun     Delay function
 *\*\param   nCount
 *\*\return  
**/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/**
 *\*\name    main.
 *\*\fun     Main function
 *\*\param   none
 *\*\return  
**/
int main(void)
{
    log_init();    
    log_info("\r\nThis is a serial SPI flash experiment!\r\n");

    /* SPI configuration */
    SPI_Configuration();

    /* Read Flash Device ID */
    DeviceID = SPI_Flash_Read_Device_ID();
    
    /* Read SPI Flash ID */
    FlashID = SPI_Flash_Read_ID();
    
    printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
    
    /* Verify the SPI Flash ID */
    if (FlashID == SPI_Flash_ID) 
    {	
        printf("\r\nSPI FLASH detected!\r\n");
        
        /* Erase the SPI Flash sector that will be written, Flash should be erased before wirting */
        SPI_Flash_Sector_Erase(FLASH_SectorToErase);	 	 
        
        /* Write the data from the send buffer to Flash */
        SPI_Flash_Buffer_Write(Tx_Buffer, FLASH_WriteAddress, BufferSize);
        printf("\r\nThe data written is£º\r\n%s", Tx_Buffer);
        
        /* Read the data just written and put it into the receive buffer */
        SPI_Flash_Buffer_Read(Rx_Buffer, FLASH_ReadAddress, BufferSize);
        printf("\r\nThe data read out is£º\r\n%s", Rx_Buffer);
        
        /* Check whether the data written is equal to the data read */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
        
        if(PASSED == TransferStatus1 )
        {    
            printf("\r\nThe SPI Flash test succeeded!\n\r");
        }
        else
        {        
            printf("\r\nThe SPI Flash test failed!\n\r");
        }
    }
    else
    {    
        printf("\r\nFailed to read the external FLASH ID!\n\r");
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


