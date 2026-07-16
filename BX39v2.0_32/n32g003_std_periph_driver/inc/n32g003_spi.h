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
*\*\file      n32g003_spi.h
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
**/
#ifndef __N32G003_SPI_H__
#define __N32G003_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g003.h"

/** SPI Init structure definition **/
typedef struct
{
    uint16_t DataDirection; /* Specifies the SPI unidirectional or bidirectional data mode */

    uint16_t SpiMode;       /* Specifies the SPI operating mode */
                            
    uint16_t DataLen;       /* Specifies the SPI data size */

    uint16_t CLKPOL;        /* Specifies the serial clock steady state */
                             
    uint16_t CLKPHA;        /* Specifies the clock active edge for the bit capture */


    uint16_t NSS;           /* Specifies whether the NSS signal is managed by
                               hardware (NSS pin) or by software using the SSI bit */


    uint16_t BaudRatePres;  /* Specifies the Baud Rate prescaler value which will be
                               used to configure the transmit and receive SCK clock */

    uint16_t FirstBit;      /* Specifies whether data transfers start from MSB or LSB bit */
} SPI_InitType;


/** SPI_data_direction **/
#define SPI_DATADIRECTION_MASK        (~(SPI_CTRL1_RONLY | SPI_CTRL1_BIDIROEN | SPI_CTRL1_BIDIRMODE)) /* direction [15:14] and [10] bits Mask */
#define SPI_DIR_DOUBLELINE_FULLDUPLEX ((uint16_t)0x0000)
#define SPI_DIR_DOUBLELINE_RONLY_TX   ((uint16_t)0x0000)
#define SPI_DIR_DOUBLELINE_RONLY_RX   (SPI_CTRL1_RONLY)
#define SPI_DIR_SINGLELINE_RX         (SPI_CTRL1_BIDIRMODE)
#define SPI_DIR_SINGLELINE_TX         (SPI_CTRL1_BIDIRMODE | SPI_CTRL1_BIDIROEN)

/** SPI mode **/
#define SPI_MODE_MASK                 (~SPI_CTRL1_MSEL) /* MSEL bits Mask */
#define SPI_MODE_MASTER               (SPI_CTRL1_MSEL)
#define SPI_MODE_SLAVE                ((uint16_t)0x0000)

/** SPI_data_size **/
#define SPI_DATALEN_MASK              (~SPI_CTRL1_DATFF) /* DATFF bits Mask */
#define SPI_DATA_SIZE_16BITS          (SPI_CTRL1_DATFF)
#define SPI_DATA_SIZE_8BITS           ((uint16_t)0x0000)

/** SPI_Clock_Polarity **/
#define SPI_CLKPOL_MASK               (~SPI_CTRL1_CLKPOL) /* CLKPOL bits Mask */
#define SPI_CLKPOL_LOW                ((uint16_t)0x0000)
#define SPI_CLKPOL_HIGH               (SPI_CTRL1_CLKPOL)

/** SPI_Clock_Phase **/
#define SPI_CLKPHA_MASK               (~SPI_CTRL1_CLKPHA) /* CLKPHA bits Mask */
#define SPI_CLKPHA_FIRST_EDGE         ((uint16_t)0x0000)
#define SPI_CLKPHA_SECOND_EDGE        (SPI_CTRL1_CLKPHA)

/** SPI_Slave_Select_management **/
#define SPI_NSS_MASK                  (~SPI_CTRL1_SSMEN) /* SSMEN bits Mask */
#define SPI_NSS_SOFT                  (SPI_CTRL1_SSMEN)
#define SPI_NSS_HARD                  ((uint16_t)0x0000)

#define SPI_NSS_OUTPUT_ENABLE          SPI_CTRL2_SSOEN
#define SPI_NSS_OUTPUT_DISABLE         (~SPI_CTRL2_SSOEN)

/** SPI_BaudRate_Prescaler **/
#define SPI_BAUDRATEPRES_MASK         (~SPI_CTRL1_BR) /* BR[2:0] bits Mask */
#define SPI_BR_PRESCALER_2            ((uint16_t)0x0000)
#define SPI_BR_PRESCALER_4            (SPI_CTRL1_BR0)
#define SPI_BR_PRESCALER_8            (SPI_CTRL1_BR1)
#define SPI_BR_PRESCALER_16           (SPI_CTRL1_BR1 | SPI_CTRL1_BR0)
#define SPI_BR_PRESCALER_32           (SPI_CTRL1_BR2)
#define SPI_BR_PRESCALER_64           (SPI_CTRL1_BR2 | SPI_CTRL1_BR0)
#define SPI_BR_PRESCALER_128          (SPI_CTRL1_BR2 | SPI_CTRL1_BR1)
#define SPI_BR_PRESCALER_256          (SPI_CTRL1_BR2 | SPI_CTRL1_BR1 | SPI_CTRL1_BR0)

/** SPI_MSB_LSB_transmission **/
#define SPI_FIRSTBIT_MASK             (~SPI_CTRL1_LSBFF) /* LSBFF bits Mask */
#define SPI_FB_MSB                    ((uint16_t)0x0000)
#define SPI_FB_LSB                    (SPI_CTRL1_LSBFF)

/** SPI Converter **/
#define SPI_TURN_ON                   (SPI_CTRL1_SPIEN)  /* SPIEN ON bit */
#define SPI_TURN_OFF                  (~SPI_CTRL1_SPIEN) /* SPIEN ON bit Mask */

/** SPI_NSS_internal_software_management **/
#define SPI_NSS_HIGH                  (SPI_CTRL1_SSEL)
#define SPI_NSS_LOW                   (~SPI_CTRL1_SSEL)

/** SPI_flags_definition **/
#define SPI_FLAG_RNE                  (SPI_STS_RNE)
#define SPI_FLAG_TE                   (SPI_STS_TE)
#define SPI_FLAG_MODERR               (SPI_STS_MODERR)
#define SPI_FLAG_OVER                 (SPI_STS_OVER)
#define SPI_FLAG_BUSY                 (SPI_STS_BUSY)

#define SPI_INT_RNE                   (SPI_CTRL2_RNEINTEN)
#define SPI_INT_TE                    (SPI_CTRL2_TEINTEN)
#define SPI_INT_ERR                   (SPI_CTRL2_ERRINTEN)

#define SPI_INT_FLAG_RNE              (SPI_STS_RNE)
#define SPI_INT_FLAG_TE               (SPI_STS_TE)
#define SPI_INT_FLAG_MODERR           (SPI_STS_MODERR)
#define SPI_INT_FLAG_OVER             (SPI_STS_OVER)
#define SPI_INT_FLAG_ERR              (SPI_STS_MODERR | SPI_STS_OVER)

/** SPI Macro Definition End **/

/** SPI Driving Functions Declaration **/

void SPI_Reset(void);

void SPI_ON(void);
void SPI_OFF(void);

void SPI_Initializes_Structure(SPI_InitType* SPI_InitStruct);

void SPI_DataDirection_Config(uint16_t DataDirection);
void SPI_SpiMode_Config(uint16_t SpiMode);
void SPI_DataLen_Config(uint16_t DataLen);
void SPI_CLKPOL_Config(uint16_t CLKPOL);
void SPI_CLKPHA_Config(uint16_t CLKPHA);
void SPI_NSS_Config(uint16_t NSS);
void SPI_BaudRatePres_Config(uint16_t BaudRatePres);
void SPI_FirstBit_Config(uint16_t FirstBit);
void SPI_Initializes(SPI_InitType* SPI_InitStruct);

void SPI_Set_Nss_Level(uint16_t SPI_NSS_Internal_Soft);
void SPI_NSS_Output_Enable(void);
void SPI_NSS_Output_Disable(void);

void SPI_Data_Transmit(uint16_t Data);
uint16_t SPI_Data_Get(void);

void SPI_Interrupts_Enable(uint8_t spi_interrupt);
void SPI_Interrupts_Disable(uint8_t spi_interrupt);

FlagStatus SPI_Flag_Status_Get(uint8_t spi_flag);
FlagStatus SPI_Interrupt_Flag_Status_Get(uint16_t spi_flag);
void SPI_OVER_Flag_Clear(void);


#ifdef __cplusplus
}
#endif

#endif /*__N32G003_SPI_H__ */

