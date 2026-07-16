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
*\*\file n32g003_uart.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32G003_UART_H
#define __N32G003_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "n32g003.h"

/*** UART Structure Definition Start ***/

/** UART Init structure definition **/
typedef struct
{
    uint32_t BaudRate;   /* Configures the UART communication baud rate. */

    uint16_t WordLength; /* Specifies the number of data bits transmitted or received in a frame. */

    uint16_t StopBits;   /* Specifies the number of stop bits transmitted. */

    uint16_t Parity;     /* Specifies the parity mode. */

    uint16_t Mode;       /* Specifies wether the Receive or Transmit mode is enabled or disabled. */

} UART_InitType;

/*** UART Macro Definition Start ***/

/** All register bits are configure to 0 **/
#define UART_REG_BIT_MASK ((uint16_t)0x0000)

/** Configures the word length of UART **/
#define UART_WL_MASK   (~UART_CTRL1_WL)     /* word length Mask */
#define UART_WL_8B     (UART_REG_BIT_MASK)  /* 8 bits */
#define UART_WL_9B     (UART_CTRL1_WL)      /* 9 bits */

/** UART STOP bits **/
#define CTRL2_STPB_CLR_MASK (~UART_CTRL2_STPB)    /* UART CTRL2 STOP Bits Mask */
#define UART_STPB_1         (UART_REG_BIT_MASK)   /* 1 STOP bit */
#define UART_STPB_2         (UART_CTRL2_STPB_1)   /* 2 STOP bits */

/** UART parity selection **/
#define UART_PE_MASK        (~(UART_CTRL1_PCEN | UART_CTRL1_PSEL))  /* UART parity Mask */
#define UART_PE_NO          (UART_REG_BIT_MASK)                     /* UART parity disable */
#define UART_PE_EVEN        (UART_CTRL1_PCEN)                       /* Even parity */
#define UART_PE_ODD         (UART_CTRL1_PCEN | UART_CTRL1_PSEL)     /* Odd parity */

/** UART is configured as RX or TX **/
#define UART_MODE_MASK      (~(UART_CTRL1_RXEN | UART_CTRL1_TXEN))  /* UART mode Mask */
#define UART_MODE_RX        (UART_CTRL1_RXEN)                       /* Transmitter enable */
#define UART_MODE_TX        (UART_CTRL1_TXEN)                       /* Receiver enable */

/** UART enable or disable **/
#define CTRL1_UEN_SET   (UART_CTRL1_UEN)    /* UART Enable */
#define CTRL1_UEN_RESET (~UART_CTRL1_UEN)   /* UART Disable */

/** Configure interrupt **/
#define INT_MASK       ((uint16_t)0x001F) /* UART Interrupt Mask */
#define UART_INT_PEF   ((uint16_t)0x0028) /* Parity error interrupt */
#define UART_INT_TXDE  ((uint16_t)0x0727) /* TXDE interrupt */
#define UART_INT_TXC   ((uint16_t)0x0626) /* Transmission complete interrupt */
#define UART_INT_RXDNE ((uint16_t)0x0525) /* RXDEN interrupt */
#define UART_INT_IDLEF ((uint16_t)0x0424) /* IDLE interrupt */
#define UART_INT_OREF  ((uint16_t)0x0325) /* ORE interrupt */

/** UART address Mask **/
#define CTRL2_ADDR_MASK   (~UART_CTRL2_ADDR)

/** Wake up methods **/
#define CTRL1_WUM_MASK         (~UART_CTRL1_WUM)    /* UART WakeUp Method Mask */
#define UART_WUM_IDLELINE      (UART_REG_BIT_MASK)  /* Idle frame wake up */
#define UART_WUM_ADDRMASK      (UART_CTRL1_WUM)     /* Address mark wake up */

/** Receiver wakeup **/
#define CTRL1_RCVWU_SET   (UART_CTRL1_RCVWU)     /* UART mute mode Enable  */
#define CTRL1_RCVWU_RESET (~UART_CTRL1_RCVWU)    /* UART mute mode Disable  */

/** UART Break Character send Mask **/
#define CTRL1_SDBRK_SET   (UART_CTRL1_SDBRK)

/** UART Half-Duplex Enable or Disable **/
#define CTRL3_HDMEN_SET   (UART_CTRL3_HDMEN)    /* UART Half-Duplex Enable  */
#define CTRL3_HDMEN_RESET (~UART_CTRL3_HDMEN)   /* UART Half-Duplex Disable  */

/* Specifies the flag to check */
#define UART_FLAG_TXDE  (UART_STS_TXDE)     /* Transmit data register empty flag */
#define UART_FLAG_TXC   (UART_STS_TXC)      /* Transmission complete flag*/
#define UART_FLAG_RXDNE (UART_STS_RXDNE)    /* Read data register not empty flag*/
#define UART_FLAG_IDLEF (UART_STS_IDLEF)    /* IDLE line detected flag */
#define UART_FLAG_OREF  (UART_STS_OREF)     /* OverRun error flag */
#define UART_FLAG_NEF   (UART_STS_NEF)      /* Noise error flag */
#define UART_FLAG_FEF   (UART_STS_FEF)      /* Framing error flag*/
#define UART_FLAG_PEF   (UART_STS_PEF)      /* Parity error flag */

void UART_Reset(UART_Module* UARTx);
void UART_Initializes(UART_Module* UARTx, UART_InitType* UART_InitStruct);
void UART_Baud_Rate_Config(UART_Module* UARTx,uint32_t buad_rate);
void UART_Word_Length_Config(UART_Module* UARTx,uint16_t word_length);
void UART_Stop_Bits_Config(UART_Module* UARTx,uint16_t stop_bits);
void UART_Parity_Config(UART_Module* UARTx,uint16_t parity);
void UART_Mode_Config(UART_Module* UARTx,uint16_t mode);
void UART_Structure_Initializes(UART_InitType* UART_InitStruct);
void UART_Enable(UART_Module* UARTx);
void UART_Disable(UART_Module* UARTx);
void UART_Interrput_Enable(UART_Module* UARTx, uint16_t UART_interrupt);
void UART_Interrput_Disable(UART_Module* UARTx, uint16_t UART_interrupt);
void UART_Address_Set(UART_Module* UARTx, uint8_t UART_address);
void UART_WakeUp_Mode_Set(UART_Module* UARTx, uint16_t UART_wake_up_mode);
void UART_Receiver_Wakeup_Enable(UART_Module* UARTx);
void UART_Receiver_Wakeup_Disable(UART_Module* UARTx);
void UART_Data_Send(UART_Module* UARTx, uint16_t data);
uint16_t UART_Data_Receive(UART_Module* UARTx);
void UART_Break_Frame_Send(UART_Module* UARTx);
void UART_Half_Duplex_Enable(UART_Module* UARTx);
void UART_Half_Duplex_Disable(UART_Module* UARTx);
FlagStatus UART_Flag_Status_Get(UART_Module* UARTx, uint16_t UART_flag);
void UART_Flag_Clear(UART_Module* UARTx, uint16_t UART_flag);
ITStatus UART_Interrupt_Status_Get(UART_Module* UARTx, uint16_t UART_interrupt);
void UART_Interrupt_Status_Clear(UART_Module* UARTx, uint16_t UART_Interrupt);

#ifdef __cplusplus
}
#endif

#endif /* __N32G003_UART_H */


/******************* (C) COPYRIGHT 2022 NATIONZ *****END OF FILE****/
