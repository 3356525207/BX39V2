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
*\*\file n32g003_uart.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "n32g003_uart.h"
#include "n32g003_rcc.h"

/**
*\*\name    UART_Reset
*\*\fun     Reset the UARTx registers.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Reset(UART_Module* UARTx)
{
    if (UARTx == UART1)
    {
        /* UART1 Reset */
		RCC_Peripheral_Reset(RCC_RST_UART1RST);
    }
    else if (UARTx == UART2)
    {
        /* UART2 Reset */
		RCC_Peripheral_Reset(RCC_RST_UART2RST);
    }
}

/**
*\*\name    UART_Initializes
*\*\fun     Initializes the UARTx peripheral according to UART_InitStruct .
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_InitStruct :
*\*\          - buad_rate :
*\*\            - (((buad_rate) > 0) && ((buad_rate) < 0x00337F99))
*\*\          - WordLength
*\*\            - UART_WL_8B
*\*\            - UART_WL_9B
*\*\          - StopBits
*\*\            - UART_STPB_1
*\*\            - UART_STPB_2
*\*\          - Parity
*\*\            - UART_PE_NO
*\*\            - UART_PE_EVEN
*\*\            - UART_PE_ODD
*\*\          - Mode
*\*\            - UART_MODE_RX
*\*\            - UART_MODE_TX
*\*\return  none
**/
void UART_Initializes(UART_Module* UARTx, UART_InitType* UART_InitStruct)
{
	UART_Baud_Rate_Config(UARTx,UART_InitStruct->BaudRate);
    UART_Word_Length_Config(UARTx,UART_InitStruct->WordLength);
    UART_Stop_Bits_Config(UARTx,UART_InitStruct->StopBits);
    UART_Parity_Config(UARTx,UART_InitStruct->Parity);
    UART_Mode_Config(UARTx,UART_InitStruct->Mode);
}

/**
*\*\name    UART_Baud_Rate_Config
*\*\fun     Configure the baud rate of the UART.
*\*\          The baud rate is computed using the following formula:
*\*\          - IntegerDivider = ((PCLKx) / (16 * (UART_InitStruct->BaudRate)))
*\*\          - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   buad_rate :
*\*\          - (((buad_rate) > 0) && ((buad_rate) < 0x00337F99))
*\*\return  none
**/
void UART_Baud_Rate_Config(UART_Module* UARTx,uint32_t buad_rate)
{
	uint32_t temp_value = 0x00, apb_clock_value = 0x00;
	uint32_t integer_divider_value = 0x00;
	uint32_t fractional_divider_value = 0x00;
	RCC_ClocksType RCC_ClocksStatus;
	
	RCC_Clocks_Frequencies_Value_Get(&RCC_ClocksStatus);
	apb_clock_value = RCC_ClocksStatus.PclkFreq;

    /* Determine the integer part */
    integer_divider_value = (((uint32_t)25 * apb_clock_value) / ((uint32_t)4 * buad_rate));
    temp_value = (integer_divider_value / (uint32_t)100) << (uint32_t)4;

    /* Determine the fractional part */
    fractional_divider_value = (((((integer_divider_value - (100 * (temp_value >> 4))) * 16) + 50) / 100));

    /*Determine whether the fractional part needs to carried*/
    if((fractional_divider_value >> 4) == 1){
        temp_value = ((integer_divider_value / 100) + 1) << 4;
    }
    
    /* Implement the fractional part in the register */
    temp_value |= fractional_divider_value & ((uint8_t)0x0F);

	/* Write to UART BRR */
	UARTx->BRCF = (uint16_t)temp_value;
}

/**
*\*\name    UART_Word_Length_Config
*\*\fun     Configure the word length of the UART.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   word_length :
*\*\          - UART_WL_8B
*\*\          - UART_WL_9B
*\*\return  none
**/
void UART_Word_Length_Config(UART_Module* UARTx,uint16_t word_length)
{
    uint32_t temp_value = 0x00;
    temp_value = UARTx->CTRL1;
    /* Clear M bit */
    temp_value &= UART_WL_MASK;
    /* Configure the UART Word Length */
    temp_value |= word_length;
    UARTx->CTRL1 = (uint16_t)temp_value;
}

/**
*\*\name    UART_Stop_Bits_Config
*\*\fun     Configure the stop bits of the UART.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   stop_bits :
*\*\          - UART_STPB_1
*\*\          - UART_STPB_2
*\*\return  none
**/
void UART_Stop_Bits_Config(UART_Module* UARTx,uint16_t stop_bits)
{
    uint32_t temp_value = 0x00;
    temp_value = UARTx->CTRL2;
    /* Clear STOP[13:12] bits */
    temp_value &= CTRL2_STPB_CLR_MASK;
    /* Set STOP[13:12] bits according to stop bits value */
    temp_value |= stop_bits;
    UARTx->CTRL2 = (uint16_t)temp_value;
}

/**
*\*\name    UART_Parity_Config
*\*\fun     Configure the parity of the UART.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   parity :
*\*\          - UART_PE_NO
*\*\          - UART_PE_EVEN
*\*\          - UART_PE_ODD
*\*\return  none
**/
void UART_Parity_Config(UART_Module* UARTx,uint16_t parity)
{
    uint32_t temp_value = 0x00;
    temp_value = UARTx->CTRL1;
    /* Clear parity[10:9] bits */
    temp_value &= UART_PE_MASK;
    /* Configure the UART parity */
    temp_value |= parity;
    UARTx->CTRL1 = (uint16_t)temp_value;
}

/**
*\*\name    UART_Mode_Config
*\*\fun     Configure the mode of the UART.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   mode :
*\*\          - UART_MODE_RX
*\*\          - UART_MODE_TX
*\*\return  none
**/
void UART_Mode_Config(UART_Module* UARTx,uint16_t mode)
{
    uint32_t temp_value = 0x00;
    temp_value = UARTx->CTRL1;
    /* Clear mode[3:2] bits */
    temp_value &= UART_MODE_MASK;
    /* Configure the UART parity */
    temp_value |= mode;
    UARTx->CTRL1 = (uint16_t)temp_value;
}

/**
*\*\name    UART_Structure_Initializes.
*\*\fun     Fills each UART_InitStruct member with its default value.
*\*\param   UART_InitStruct :
*\*\          - BaudRate
*\*\          - WordLength
*\*\          - StopBits
*\*\          - Parity
*\*\          - Mode
*\*\          - HardwareFlowControl
*\*\return  none
**/
void UART_Structure_Initializes(UART_InitType* UART_InitStruct)
{
    /* UART_InitStruct members default value */
    UART_InitStruct->BaudRate   = 9600;
    UART_InitStruct->WordLength = UART_WL_8B;
    UART_InitStruct->StopBits   = UART_STPB_1;
    UART_InitStruct->Parity     = UART_PE_NO;
    UART_InitStruct->Mode       = UART_MODE_RX | UART_MODE_TX;
}

/**
*\*\name    UART_Enable.
*\*\fun     Enables the specified UART peripheral.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Enable(UART_Module* UARTx)
{
    /* Enable the selected UART by setting the UE bit in the CTRL1 register */
    UARTx->CTRL1 |= CTRL1_UEN_SET;
}

/**
*\*\name    UART_Disable.
*\*\fun     Disables the specified UART peripheral.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Disable(UART_Module* UARTx)
{
    /* Disable the selected UART by clearing the UE bit in the CTRL1 register */
    UARTx->CTRL1 &= CTRL1_UEN_RESET;
}

/**
*\*\name    UART_Interrput_Enable.
*\*\fun     Enables the specified UART interrupts.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_interrupt :
*\*\          - UART_INT_PEF
*\*\          - UART_INT_TXDE
*\*\          - UART_INT_TXC
*\*\          - UART_INT_RXDNE
*\*\          - UART_INT_IDLEF
*\*\return  none
**/
void UART_Interrput_Enable(UART_Module* UARTx, uint16_t UART_interrupt)
{
    uint32_t register_value = 0x00, interrupt_position = 0x00, temp_value = 0x00;
    uint32_t base_value = 0x00;

    base_value = (uint32_t)UARTx;

    /* Get the UART register index */
    register_value = (((uint8_t)UART_interrupt) >> 0x5);

    /* Get the interrupt position */
    interrupt_position  = UART_interrupt & INT_MASK;
    temp_value = (((uint32_t)0x01) << interrupt_position);

    if (register_value == 0x01) /* The IT is in CTRL1 register */
    {
        base_value += 0x0C;
    }
    else if (register_value == 0x02) /* The IT is in CTRL2 register */
    {
        base_value += 0x10;
    }
    else /* The IT is in CTRL3 register */
    {
        base_value += 0x14;
    }

    *(__IO uint32_t*)base_value |= temp_value;
}

/**
*\*\name    UART_Interrput_Disable.
*\*\fun     Disables the specified UART interrupts.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_interrupt :
*\*\          - UART_INT_PEF
*\*\          - UART_INT_TXDE
*\*\          - UART_INT_TXC
*\*\          - UART_INT_RXDNE
*\*\          - UART_INT_IDLEF
*\*\return  none
**/
void UART_Interrput_Disable(UART_Module* UARTx, uint16_t UART_interrupt)
{
    uint32_t register_value = 0x00, interrupt_position = 0x00, temp_value = 0x00;
    uint32_t base_value = 0x00;
    base_value = (uint32_t)UARTx;

    /* Get the UART register index */
    register_value = (((uint8_t)UART_interrupt) >> 0x5);

    /* Get the interrupt position */
    interrupt_position  = UART_interrupt & INT_MASK;
    temp_value = (((uint32_t)0x01) << interrupt_position);

    if (register_value == 0x01) /* The IT is in CTRL1 register */
    {
        base_value += 0x0C;
    }
    else if (register_value == 0x02) /* The IT is in CTRL2 register */
    {
        base_value += 0x10;
    }
    else /* The IT is in CTRL3 register */
    {
        base_value += 0x14;
    }

    *(__IO uint32_t*)base_value &= ~temp_value;
}

/**
*\*\name    UART_Address_Set.
*\*\fun     Sets the address of the UART node.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_address :
*\*\          - (((UART_address)>=0x00)&&((UART_address)<0x10))
*\*\return  none
**/
void UART_Address_Set(UART_Module* UARTx, uint8_t UART_address)
{
    /* Clear the UART address */
    UARTx->CTRL2 &= CTRL2_ADDR_MASK;
    /* Set the UART address node */
    UARTx->CTRL2 |= UART_address;
}

/**
*\*\name    UART_WakeUp_Mode_Set.
*\*\fun     Selects the UART WakeUp method.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_wake_up_mode :
*\*\          - UART_WUM_IDLELINE
*\*\          - UART_WUM_ADDRMASK
*\*\return  none
**/
void UART_WakeUp_Mode_Set(UART_Module* UARTx, uint16_t UART_wake_up_mode)
{
    UARTx->CTRL1 &= CTRL1_WUM_MASK;
    UARTx->CTRL1 |= UART_wake_up_mode;
}

/**
*\*\name    UART_Receiver_Wakeup_Enable.
*\*\fun     Enable the UART mute mode.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Receiver_Wakeup_Enable(UART_Module* UARTx)
{
    /* Enable the UART mute mode  by setting the RWU bit in the CTRL1 register */
    UARTx->CTRL1 |= CTRL1_RCVWU_SET;
}

/**
*\*\name    UART_Receiver_Wakeup_Disable.
*\*\fun     Disable the UART mute mode.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Receiver_Wakeup_Disable(UART_Module* UARTx)
{
    /* Disable the UART mute mode by clearing the RWU bit in the CTRL1 register */
    UARTx->CTRL1 &= CTRL1_RCVWU_RESET;
}

/**
*\*\name    UART_Data_Send.
*\*\fun     Transmits single data through the UARTx peripheral.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   data :
*\*\          - The data to transmit.
*\*\return  none
**/
void UART_Data_Send(UART_Module* UARTx, uint16_t data)
{
    /* Transmit Data */
    UARTx->DAT = (data & (uint16_t)0x01FF);
}

/**
*\*\name    UART_Data_Receive.
*\*\fun     Returns the most recent received data by the UARTx peripheral.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  The received data.
**/
uint16_t UART_Data_Receive(UART_Module* UARTx)
{
    /* Receive Data */
    return (uint16_t)(UARTx->DAT & (uint16_t)0x01FF);
}

/**
*\*\name    UART_Break_Frame_Send.
*\*\fun     Transmits break characters.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Break_Frame_Send(UART_Module* UARTx)
{
    /* Send break characters */
    UARTx->CTRL1 |= CTRL1_SDBRK_SET;
}

/**
*\*\name    UART_Half_Duplex_Enable.
*\*\fun     Enables the UART's Half Duplex communication.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Half_Duplex_Enable(UART_Module* UARTx)
{
    /* Enable the Half-Duplex mode by setting the HDSEL bit in the CTRL3 register */
    UARTx->CTRL3 |= CTRL3_HDMEN_SET;
}

/**
*\*\name    UART_Half_Duplex_Disable.
*\*\fun     Disables the UART's Half Duplex communication.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\return  none
**/
void UART_Half_Duplex_Disable(UART_Module* UARTx)
{
    /* Disable the Half-Duplex mode by clearing the HDSEL bit in the CTRL3 register */
    UARTx->CTRL3 &= CTRL3_HDMEN_RESET;
}

/**
*\*\name    UART_Flag_Status_Get.
*\*\fun     Checks whether the specified UART flag is set or not.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_flag :
*\*\          - UART_FLAG_TXDE
*\*\          - UART_FLAG_TXC
*\*\          - UART_FLAG_RXDNE
*\*\          - UART_FLAG_IDLEF
*\*\          - UART_FLAG_OREF
*\*\          - UART_FLAG_NEF
*\*\          - UART_FLAG_FEF
*\*\          - UART_FLAG_PEF
*\*\return  SET or RESET
**/
FlagStatus UART_Flag_Status_Get(UART_Module* UARTx, uint16_t UART_flag)
{
    if ((UARTx->STS & UART_flag) != (uint16_t)RESET)
    {
       return SET;
    }
    else
    {
        return RESET;
    }
}

/**
*\*\name    UART_Flag_Clear.
*\*\fun     Clears the UARTx's pending flags.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_flag :
*\*\          - UART_FLAG_TXC
*\*\          - UART_FLAG_RXDNE
*\*\return  none
**/
void UART_Flag_Clear(UART_Module* UARTx, uint16_t UART_flag)
{
    UARTx->STS = (uint16_t)~UART_flag;
}

/**
*\*\name    UART_Interrupt_Status_Get.
*\*\fun     Checks whether the specified UART interrupt has occurred or not.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_interrupt :
*\*\          - UART_INT_TXDE
*\*\          - UART_INT_TXC
*\*\          - UART_INT_RXDNE
*\*\          - UART_INT_IDLEF
*\*\          - UART_INT_PEF
*\*\          - UART_INT_OREF
*\*\return  SET or RESET
**/
ITStatus UART_Interrupt_Status_Get(UART_Module* UARTx, uint16_t UART_interrupt)
{
    uint32_t bit_position = 0x00, temp_value = 0x00, register_value = 0x00;

    /* Get the UART register index */
    register_value = (((uint8_t)UART_interrupt) >> 0x5);
    /* Get the interrupt position */
    temp_value = UART_interrupt & INT_MASK;
    temp_value = (uint32_t)0x01 << temp_value;

    if (register_value == 0x01) /* The IT  is in CTRL1 register */
    {
        temp_value &= UARTx->CTRL1;
    }
    else if (register_value == 0x02) /* The IT  is in CTRL2 register */
    {
        temp_value &= UARTx->CTRL2;
    }
    else /* The IT  is in CTRL3 register */
    {
        temp_value &= UARTx->CTRL3;
    }

    bit_position = UART_interrupt >> 0x8;
    bit_position = (uint32_t)0x01 << bit_position;
    bit_position &= UARTx->STS;
    if ((temp_value != (uint16_t)RESET) && (bit_position != (uint16_t)RESET))
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
*\*\name    UART_Interrupt_Status_Clear.
*\*\fun     Clears the UARTx's interrupt Status.
*\*\param   UARTx :
*\*\          - UART1
*\*\          - UART2
*\*\param   UART_Interrupt :
*\*\          - UART_INT_TXC
*\*\          - UART_INT_RXDNE
*\*\return  none
**/
void UART_Interrupt_Status_Clear(UART_Module* UARTx, uint16_t UART_Interrupt)
{
    uint16_t bit_position = 0x00, temp_value = 0x00;

    bit_position = UART_Interrupt >> 0x8;
    temp_value   = ((uint16_t)0x01 << (uint16_t)bit_position);
    UARTx->STS   = (uint16_t)~temp_value;
}


