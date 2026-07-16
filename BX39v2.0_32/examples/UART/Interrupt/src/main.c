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
#include <stdio.h>
#include "main.h"

#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define TxBufferSize2 (countof(TxBuffer2) - 1)
#define RxBufferSize1 TxBufferSize2
#define RxBufferSize2 TxBufferSize1

#define countof(a) (sizeof(a) / sizeof(*(a)))

UART_InitType UART_InitStructure;
uint8_t TxBuffer1[] = "UART Interrupt Example: UARTy -> UARTz using Interrupt";
uint8_t TxBuffer2[] = "UART Interrupt Example: UARTz -> UARTy using Interrupt";
uint8_t RxBuffer1[RxBufferSize1];
uint8_t RxBuffer2[RxBufferSize2];
__IO uint8_t TxCounter1         = 0x00;
__IO uint8_t TxCounter2         = 0x00;
__IO uint8_t RxCounter1         = 0x00;
__IO uint8_t RxCounter2         = 0x00;
uint8_t NbrOfDataToTransfer1    = TxBufferSize1;
uint8_t NbrOfDataToTransfer2    = TxBufferSize2;
uint8_t NbrOfDataToRead1        = RxBufferSize1;
uint8_t NbrOfDataToRead2        = RxBufferSize2;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;

/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    /* System Clocks Configuration */
    RCC_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* UARTy and UARTz configuration */
    UART_Structure_Initializes(&UART_InitStructure);
    UART_InitStructure.BaudRate   = 115200;
    UART_InitStructure.WordLength = UART_WL_8B;
    UART_InitStructure.StopBits   = UART_STPB_1;
    UART_InitStructure.Parity     = UART_PE_NO;
    UART_InitStructure.Mode       = UART_MODE_RX | UART_MODE_TX;

    /* Configure UARTy and UARTz */
    UART_Initializes(UARTy, &UART_InitStructure);
    UART_Initializes(UARTz, &UART_InitStructure);

    /* Enable UARTy Receive and Transmit interrupts */
    UART_Interrput_Enable(UARTy, UART_INT_RXDNE);
    UART_Interrput_Enable(UARTy, UART_INT_TXDE);

    /* Enable UARTz Receive and Transmit interrupts */
    UART_Interrput_Enable(UARTz, UART_INT_RXDNE);
    UART_Interrput_Enable(UARTz, UART_INT_TXDE);

    /* Enable the UARTy and UARTz */
    UART_Enable(UARTy);
    UART_Enable(UARTz);

    /* Wait until end of transmission from UARTy to UARTz */
    while (RxCounter2 < RxBufferSize2)
    {
    }

    /* Wait until end of transmission from USARTz to UARTy */
    while (RxCounter1 < RxBufferSize1)
    {
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, RxBufferSize1);
    /* TransferStatus1 = PASSED, if the data transmitted from UARTz and
       received by UARTy are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from UARTz and
       received by UARTy are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, RxBufferSize2);
    /* TransferStatus2 = PASSED, if the data transmitted from UARTy and
       received by UARTz are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from UARTy and
       received by UARTz are different */

    while (1)
    {
    }
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\param   none
*\*\return  none
**/
void RCC_Configuration(void)
{
    /* Enable GPIO clock */
    RCC_APB_Peripheral_Clock_Enable(UARTy_GPIO_CLK | UARTz_GPIO_CLK);
    /* Enable UARTy and UARTz Clock */
    UARTy_APBxClkCmd(UARTy_CLK);
    UARTz_APBxClkCmd(UARTz_CLK);
}

/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\param   none
*\*\return  none
**/
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_Structure_Initialize(&GPIO_InitStructure);
   
    /* Configure UARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = UARTy_TxPin;    
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = UARTy_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(UARTy_GPIO, &GPIO_InitStructure);

    /* Configure UARTz Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = UARTz_TxPin;
    GPIO_InitStructure.GPIO_Alternate = UARTz_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(UARTz_GPIO, &GPIO_InitStructure); 

    /* Configure UARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = UARTy_RxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate = UARTy_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(UARTy_GPIO, &GPIO_InitStructure);    

    /* Configure UARTz Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = UARTz_RxPin;
    GPIO_InitStructure.GPIO_Alternate = UARTz_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(UARTz_GPIO, &GPIO_InitStructure);        
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures the nested vectored interrupt controller.
*\*\param   none
*\*\return  none
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the UARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel         = UARTy_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_0;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    /* Enable the UARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel         = UARTz_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

/**
*\*\name    Buffercmp.
*\*\fun     Compares two buffers.
*\*\param   pBuffer1
*\*\param   pBuffer2
*\*\param   BufferLength
*\*\return  PASSED or FAILED
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


