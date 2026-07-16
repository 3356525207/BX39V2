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
*\*\file      n32g003_spi.c
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
**/

#include "n32g003_spi.h"
#include "n32g003_rcc.h"

/* SPI Driving Functions Declaration */

/**
*\*\name    SPI_Reset.
*\*\fun     Reset the SPI registers.
*\*\param   none
*\*\return  none
**/
void SPI_Reset(void)
{
    /* SPI Reset */
    RCC_Peripheral_Reset(RCC_RST_SPIRST);
}


/**
*\*\name    SPI_ON.
*\*\fun     SPI turn ON.
*\*\param   none
*\*\return  none
**/
void SPI_ON(void)
{
    /* Enable SPI */
    SPI->CTRL1 |= SPI_TURN_ON;
}


/**
*\*\name    SPI_OFF.
*\*\fun     SPI turn OFF.
*\*\param   none
*\*\return  none
**/
void SPI_OFF(void)
{
    /* Disable SPI */
    SPI->CTRL1 &= SPI_TURN_OFF;
}


/**
*\*\name    SPI_Initializes_Structure.
*\*\fun     Fills each SPI_InitStruct member with its default value.
*\*\param   SPI_InitStruct :
*\*\          - DataDirection
*\*\          - SpiMode
*\*\          - DataLen
*\*\          - CLKPOL
*\*\          - CLKPHA
*\*\          - NSS      
*\*\          - BaudRatePres     
*\*\          - FirstBit      
*\*\return  none
**/
void SPI_Initializes_Structure(SPI_InitType* SPI_InitStruct)
{
    /* Initialize the DataDirection member */
    SPI_InitStruct->DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    /* initialize the SpiMode member */
    SPI_InitStruct->SpiMode       = SPI_MODE_SLAVE;
    /* initialize the DataLen member */
    SPI_InitStruct->DataLen       = SPI_DATA_SIZE_8BITS;
    /* Initialize the CLKPOL member */
    SPI_InitStruct->CLKPOL        = SPI_CLKPOL_LOW;
    /* Initialize the CLKPHA member */
    SPI_InitStruct->CLKPHA        = SPI_CLKPHA_FIRST_EDGE;
    /* Initialize the NSS member */
    SPI_InitStruct->NSS           = SPI_NSS_HARD;
    /* Initialize the BaudRatePres member */
    SPI_InitStruct->BaudRatePres  = SPI_BR_PRESCALER_2;
    /* Initialize the FirstBit member */
    SPI_InitStruct->FirstBit      = SPI_FB_MSB;
}


/**
*\*\name    SPI_DataDirection_Config.
*\*\fun     Config the SPI DataDirection.
*\*\param   DataDirection :
*\*\          - SPI_DIR_DOUBLELINE_FULLDUPLEX
*\*\          - SPI_DIR_DOUBLELINE_RONLY_TX    
*\*\          - SPI_DIR_DOUBLELINE_RONLY_RX   
*\*\          - SPI_DIR_SINGLELINE_RX        
*\*\          - SPI_DIR_SINGLELINE_TX
*\*\return  none
**/
void SPI_DataDirection_Config(uint16_t DataDirection)
{  
    /* Clear SPI_CTRL1[15:14] and [10] bits */
    SPI->CTRL1 &= SPI_DATADIRECTION_MASK;
    /* Set SPI_CTRL1[15:14] and [10] bits */
    SPI->CTRL1 |= DataDirection;
}


/**
*\*\name    SPI_SpiMode_Config.
*\*\fun     Config the SPI mode.
*\*\param   SpiMode :
*\*\          - SPI_MODE_MASTER
*\*\          - SPI_MODE_SLAVE
*\*\return  none
**/
void SPI_SpiMode_Config(uint16_t SpiMode)
{
    /* Clear SPI_CTRL1 MSEL bits */
    SPI->CTRL1 &= SPI_MODE_MASK;
    /* Set SPI_CTRL1 MSEL bits */
    SPI->CTRL1 |= SpiMode;
}


/**
*\*\name    SPI_DataLen_Config.
*\*\fun     Config the SPI DataLen.
*\*\param   DataLen :
*\*\          - SPI_DATA_SIZE_16BITS
*\*\          - SPI_DATA_SIZE_8BITS
*\*\return  none
*\*\note    Must be used with SPI OFF.
**/
void SPI_DataLen_Config(uint16_t DataLen)
{
    /* Clear SPI_CTRL1 DATFF bits */
    SPI->CTRL1 &= SPI_DATALEN_MASK;
    /* Set SPI_CTRL1 DATFF bits */
    SPI->CTRL1 |= DataLen;
}


/**
*\*\name    SPI_CLKPOL_Config.
*\*\fun     Config the SPI CLKPOL.
*\*\param   CLKPOL :
*\*\          - SPI_CLKPOL_LOW
*\*\          - SPI_CLKPOL_HIGH
*\*\return  none
**/
void SPI_CLKPOL_Config(uint16_t CLKPOL)
{
    /* Clear SPI_CTRL1 CLKPOL bits */
    SPI->CTRL1 &= SPI_CLKPOL_MASK;
    /* Set SPI_CTRL1 CLKPOL bits */
    SPI->CTRL1 |= CLKPOL;
}


/**
*\*\name    SPI_CLKPHA_Config.
*\*\fun     Config the SPI CLKPHA.
*\*\param   CLKPHA :
*\*\          - SPI_CLKPHA_FIRST_EDGE
*\*\          - SPI_CLKPHA_SECOND_EDGE
*\*\return  none
**/
void SPI_CLKPHA_Config(uint16_t CLKPHA)
{
    /* Clear SPI_CTRL1 CLKPHA bits */
    SPI->CTRL1 &= SPI_CLKPHA_MASK;
    /* Set SPI_CTRL1 CLKPHA bits */
    SPI->CTRL1 |= CLKPHA;
}


/**
*\*\name    SPI_NSS_Config.
*\*\fun     Config the SPI NSS.
*\*\param   NSS :
*\*\          - SPI_NSS_SOFT
*\*\          - SPI_NSS_HARD
*\*\return  none
**/
void SPI_NSS_Config(uint16_t NSS)
{
    /* Clear SPI_CTRL1 SSMEN bits */
    SPI->CTRL1 &= SPI_NSS_MASK;
    /* Set SPI_CTRL1 SSMEN bits */
    SPI->CTRL1 |= NSS;
}


/**
*\*\name    SPI_BaudRatePres_Config.
*\*\fun     Config the SPI BaudRatePres.
*\*\param   BaudRatePres :
*\*\          - SPI_BR_PRESCALER_2
*\*\          - SPI_BR_PRESCALER_4
*\*\          - SPI_BR_PRESCALER_8
*\*\          - SPI_BR_PRESCALER_16
*\*\          - SPI_BR_PRESCALER_32
*\*\          - SPI_BR_PRESCALER_64
*\*\          - SPI_BR_PRESCALER_128
*\*\          - SPI_BR_PRESCALER_256
*\*\return  none
**/
void SPI_BaudRatePres_Config(uint16_t BaudRatePres)
{
    /* Clear SPI_CTRL1 BR[2:0] bits */
    SPI->CTRL1 &= SPI_BAUDRATEPRES_MASK;
    /* Set SPI_CTRL1 BR[2:0] bits */
    SPI->CTRL1 |= BaudRatePres;
}


/**
*\*\name    SPI_FirstBit_Config.
*\*\fun     Config the SPI FirstBit.
*\*\param   FirstBit :
*\*\          - SPI_FB_MSB
*\*\          - SPI_FB_LSB
*\*\return  none
**/
void SPI_FirstBit_Config(uint16_t FirstBit)
{
    /* Clear SPI_CTRL1 LSBFF bits */
    SPI->CTRL1 &= SPI_FIRSTBIT_MASK;
    /* Set SPI_CTRL1 LSBFF bits */
    SPI->CTRL1 |= FirstBit;
}


/**
*\*\name    SPI_Initializes.
*\*\fun     Initializes the SPI according to SPI_InitStruct.
*\*\param   SPI_InitStruct :
*\*\          - DataDirection
*\*\            - SPI_DIR_DOUBLELINE_FULLDUPLEX
*\*\            - SPI_DIR_DOUBLELINE_RONLY_TX    
*\*\            - SPI_DIR_DOUBLELINE_RONLY_RX    
*\*\            - SPI_DIR_SINGLELINE_RX        
*\*\            - SPI_DIR_SINGLELINE_TX        
*\*\          - SpiMode
*\*\            - SPI_MODE_MASTER
*\*\            - SPI_MODE_SLAVE 
*\*\          - DataLen
*\*\            - SPI_DATA_SIZE_16BITS
*\*\            - SPI_DATA_SIZE_8BITS     
*\*\          - CLKPOL
*\*\            - SPI_CLKPOL_LOW 
*\*\            - SPI_CLKPOL_HIGH
*\*\          - CLKPHA
*\*\            - SPI_CLKPHA_FIRST_EDGE 
*\*\            - SPI_CLKPHA_SECOND_EDGE
*\*\          - NSS
*\*\            - SPI_NSS_SOFT
*\*\            - SPI_NSS_HARD
*\*\          - BaudRatePres
*\*\            - SPI_BR_PRESCALER_2  
*\*\            - SPI_BR_PRESCALER_4  
*\*\            - SPI_BR_PRESCALER_8  
*\*\            - SPI_BR_PRESCALER_16 
*\*\            - SPI_BR_PRESCALER_32 
*\*\            - SPI_BR_PRESCALER_64 
*\*\            - SPI_BR_PRESCALER_128
*\*\            - SPI_BR_PRESCALER_256
*\*\          - FirstBit
*\*\            - SPI_FB_MSB
*\*\            - SPI_FB_LSB
*\*\return  none
**/
void SPI_Initializes(SPI_InitType* SPI_InitStruct)
{
    SPI_DataDirection_Config(SPI_InitStruct->DataDirection);
    SPI_SpiMode_Config(SPI_InitStruct->SpiMode);
    SPI_DataLen_Config(SPI_InitStruct->DataLen);
	SPI_CLKPHA_Config(SPI_InitStruct->CLKPHA);
    SPI_CLKPOL_Config(SPI_InitStruct->CLKPOL);
    SPI_NSS_Config(SPI_InitStruct->NSS);
    SPI_BaudRatePres_Config(SPI_InitStruct->BaudRatePres);
    SPI_FirstBit_Config(SPI_InitStruct->FirstBit);
}


/**
*\*\name    SPI_Set_Nss_Level.
*\*\fun     Configures internally by software the NSS pin for the selected SPI.
*\*\param   SPI_NSS_Internal_Soft :
*\*\          - SPI_NSS_HIGH
*\*\          - SPI_NSS_LOW
*\*\return  none
**/
void SPI_Set_Nss_Level(uint16_t SPI_NSS_Internal_Soft)
{
    if(SPI_NSS_Internal_Soft == SPI_NSS_HIGH)
    {
        /* Set NSS pin internally by software */
        SPI->CTRL1 |= SPI_NSS_HIGH;
    }
    else
    {
        /* Reset NSS pin internally by software */
        SPI->CTRL1 &= SPI_NSS_LOW;
    }
}

/**
*\*\name    SPI_NSS_Output_Enable.
*\*\fun     Enables the NSS output for the selected SPI.
*\*\return  none
**/
void SPI_NSS_Output_Enable(void)
{
    /* Set the SPI_CTRL2 SSOEN bit to enable NSS output */
    SPI->CTRL2 |= SPI_NSS_OUTPUT_ENABLE;
}

/**
*\*\name    SPI_NSS_Output_Disable.
*\*\fun     Disable the NSS output for the selected SPI.
*\*\return  none
**/
void SPI_NSS_Output_Disable(void)
{
    /* Clean the SPI_CTRL2 SSOEN bit to disable SS output */
    SPI->CTRL2 &= SPI_NSS_OUTPUT_DISABLE;
}


/**
*\*\name    SPI_Interrupts_Enable.
*\*\fun     Enable SPI interrupts.     
*\*\param   spi_interrupt :
*\*\          - SPI_INT_TE    SPI_CTRL2
*\*\          - SPI_INT_RNE   SPI_CTRL2
*\*\          - SPI_INT_ERR   SPI_CTRL2
*\*\return  none
**/
void SPI_Interrupts_Enable(uint8_t spi_interrupt)
{
    /* Set the SPI_CTRL2 TEINTEN/RNEINTEN/ERRINTEN bit to enable SPI interrupts */
    SPI->CTRL2 |= spi_interrupt;
}

/**
*\*\name    SPI_Interrupts_Disable.
*\*\fun     Disable SPI interrupts.           
*\*\param   spi_interrupt :
*\*\          - SPI_INT_TE    SPI_CTRL2
*\*\          - SPI_INT_RNE   SPI_CTRL2
*\*\          - SPI_INT_ERR   SPI_CTRL2
*\*\return  none
**/
void SPI_Interrupts_Disable(uint8_t spi_interrupt)
{
    /* Clean the SPI_CTRL2 TEINTEN/RNEINTEN/ERRINTEN bit to disable SPI interrupts */
    SPI->CTRL2 &= (~spi_interrupt);
}


/**
*\*\name    SPI_Data_Transmit.
*\*\fun     Transmits a Data through the SPI peripheral.            
*\*\param   Data: data to be transmitted 
*\*\return  none
**/
void SPI_Data_Transmit(uint16_t Data)
{
    /* Write in the SPI_DAT register the data to be sent */
    SPI->DAT = Data;
}


/**
*\*\name    SPI_Data_Get.
*\*\fun     Get SPI data from SPI_DAT register.           
*\*\return  The data in the SPI_DAT register
**/
uint16_t SPI_Data_Get(void)
{
    /* Return the data in the SPI_DAT register */
    return SPI->DAT;
}


/**
*\*\name    SPI_Flag_Status_Get.
*\*\fun     Get SPI flag status.            
*\*\param   spi_flag :
*\*\          - SPI_FLAG_TE
*\*\          - SPI_FLAG_RNE
*\*\          - SPI_FLAG_BUSY
*\*\          - SPI_FLAG_OVER
*\*\          - SPI_FLAG_MODERR
*\*\return  SET or RESET
**/
FlagStatus SPI_Flag_Status_Get(uint8_t spi_flag)
{
    return (((SPI->STS & spi_flag) == spi_flag) ? SET : RESET);  
}

/**
*\*\name    SPI_Interrupt_Flag_Status_Get.
*\*\fun     Get SPI interrupt flag status.            
*\*\param   spi_flag :
*\*\          - SPI_INT_FLAG_RNE 
*\*\          - SPI_INT_FLAG_TE   
*\*\          - SPI_INT_FLAG_MODERR  
*\*\          - SPI_INT_FLAG_OVER
*\*\          - SPI_INT_FLAG_ERR 
*\*\return  SET or RESET
**/
FlagStatus SPI_Interrupt_Flag_Status_Get(uint16_t spi_flag)
{
    /* Interrupt flag */
    switch (spi_flag)
    {
        case SPI_INT_FLAG_TE:
            if((SPI->CTRL2 & SPI_INT_TE) == SPI_INT_TE && (SPI->STS & SPI_INT_FLAG_TE) == SPI_INT_FLAG_TE)
            {
                return SET;
            }
            else
            {
                return RESET;
            }
			
        case SPI_INT_FLAG_RNE:
            if((SPI->CTRL2 & SPI_INT_RNE) && (SPI->STS & SPI_INT_FLAG_RNE))
            {
                return SET;
            }
            else
            {
                return RESET;
            }
			
        case SPI_INT_FLAG_ERR:  
        case SPI_INT_FLAG_MODERR:   
        case SPI_INT_FLAG_OVER:
            if((SPI->CTRL2 & SPI_INT_ERR) && (SPI->STS & spi_flag))
            {
                return SET;
            }
            else
            {
                return RESET;
            }
			
        default:
           return RESET;
    }
}


/**
*\*\name    SPI_Clear_Flag_Status.
*\*\fun     Get SPI flag status.           
*\*\param   none   
*\*\return  none
**/
void SPI_OVER_Flag_Clear(void)
{
    volatile u8 temp_vlue;

    /* read SPI_DAT and SPI_STS to Clear the Flag*/
    temp_vlue = SPI->DAT;
    temp_vlue = SPI->STS;
}


