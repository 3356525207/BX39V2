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
*\*\file main.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "main.h"
#include "log.h"

#define FLASH_PAGE_SIZE        ((uint16_t)0x200)
#define FLASH_WRITE_START_ADDR ((uint32_t)0x08003000)
#define FLASH_WRITE_END_ADDR   ((uint32_t)0x08003200)

int main(void)
{
    uint32_t Counter_Num = 0;
    uint32_t Erase_Data  = 0xCDEF89AB;
    /* USART Init */
    log_init();
    printf("Flash Program Test Start\r\n");
    /* Program FLASH */

    /* Unlocks the FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Erase */
    if (FLASH_EOP != FLASH_One_Page_Erase(FLASH_WRITE_START_ADDR))
    {
        while(1)
        {
            printf("Flash EraseOnePage Error. Please Deal With This Error Promptly\r\n");
        }
    }

    /* Program */
    for (Counter_Num = 0; Counter_Num < FLASH_PAGE_SIZE; Counter_Num += 4)
    {
        if (FLASH_EOP != FLASH_Word_Program(FLASH_WRITE_START_ADDR + Counter_Num, Erase_Data))
        {
            while(1)
            {
                printf("Flash ProgramWord Error. Please Deal With This Error Promptly\r\n");
            }
        }
    }

    /* Locks the FLASH Program Erase Controller */
    FLASH_Lock();

    /* Check */
    for (Counter_Num = 0; Counter_Num < FLASH_PAGE_SIZE; Counter_Num += 4)
    {
        if (Erase_Data != (*(__IO uint32_t*)(FLASH_WRITE_START_ADDR + Counter_Num)))
        {
            printf("Flash Program Test Failed\r\n");
            break;
        }
    }

    printf("Flash Program Test End\r\n");

    while (1)
    {
    }
}


