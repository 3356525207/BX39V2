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
*\*\file      bsp_spi_flash.c
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
*/

#include "bsp_spi_flash.h"

static __IO uint32_t Timeout = SPI_Long_Wait_Timeout;
static uint16_t Timeout_Call_Back(uint8_t error_code);

/**
*\*\name    SPI_RCC_Configuration
*\*\fun     Enable SPI clock
*\*\param   none
*\*\return  none
**/
static void SPI_RCC_Configuration(void)
{
	/* Enable SPI Clock */
	RCC_APB_Peripheral_Clock_Enable(SPI_MASTER_PERIPH);
	
	/* Enable SPI Port Clock */
	RCC_APB_Peripheral_Clock_Enable(SPI_MASTER_SCK_CLK);
}


/**
*\*\name    SPI_GPIO_Configuration
*\*\fun     Config SPI port
*\*\param   none
*\*\return  none
**/
static void SPI_GPIO_Configuration(void)
{
	GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
	
	/* Using software NSS, so don't need to config the NSS pin */
	/* NSS pin uses control Flash chip hardware selection */
	GPIO_InitStructure.Pin       = SPI_MASTER_NSS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
	GPIO_Peripheral_Initialize(SPI_MASTER_NSS_GPIO, &GPIO_InitStructure);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	/* Configure Master Pins SCK as Alternate Function Push-pull */
	GPIO_InitStructure.Pin            = SPI_MASTER_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_SCK_AF;
	GPIO_Peripheral_Initialize(SPI_MASTER_SCK_GPIO, &GPIO_InitStructure);
	
	/* Configure Master Pins MOSI as Alternate Function Push-pull */
	GPIO_InitStructure.Pin            = SPI_MASTER_MOSI_PIN;
	GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_MOSI_AF;
	GPIO_Peripheral_Initialize(SPI_MASTER_MOSI_GPIO, &GPIO_InitStructure);
	
	/* Configure Master Pins MISO as Input-floating */
	GPIO_InitStructure.Pin            = SPI_MASTER_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
	GPIO_InitStructure.GPIO_Pull      = GPIO_NO_PULL;
	GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_MISO_AF;
	GPIO_Peripheral_Initialize(SPI_MASTER_MISO_GPIO, &GPIO_InitStructure);
}
	

/**
*\*\name    SPI_Configuration
*\*\fun     Config SPI 
*\*\param   none
*\*\return  none
**/
void SPI_Configuration(void)
{
	SPI_InitType SPI_InitStructure;
	
	/* SPI clock configuration */
	SPI_RCC_Configuration();
	
	/* GPIO configuration */
	SPI_GPIO_Configuration();
	
	SPI_Initializes_Structure(&SPI_InitStructure);
	
	/* SPI configuration */
	/* Note: Flash chips only support SPI mode 0 and mode 3 */
	SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
	SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
	SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
	SPI_InitStructure.CLKPOL        = SPI_CLKPOL_HIGH;
	SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
	SPI_InitStructure.NSS           = SPI_NSS_SOFT;
	SPI_InitStructure.FirstBit      = SPI_FB_MSB;
	SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_4;
	SPI_Initializes(&SPI_InitStructure);
	
	/* Set NSS level high */
	SPI_Set_Nss_Level(SPI_NSS_HIGH);
	
	/* Enable SPI Master */
	SPI_ON();
}

/**
*\*\name    SPI_Flash_Send_Byte
*\*\fun     Send 1-byte data
*\*\param   data
*\*\return  Returns the received data
**/
uint8_t SPI_Flash_Send_Byte(uint8_t data)
{
	Timeout = SPI_Flag_Wait_Timeout;
	
	/* Wait for the send buffer to be empty, TE flag is set */
	while(SPI_Flag_Status_Get(SPI_FLAG_TE) == RESET)
	{
		if((Timeout--) == 0)
		{
			return Timeout_Call_Back(0);
		}
	}
	
	/* Write the data to be sent to the send-buffer */
	SPI_Data_Transmit(data);
	
	Timeout = SPI_Flag_Wait_Timeout;
	/* Wait for the send buffer to be empty, TE flag is set */
	while(SPI_Flag_Status_Get(SPI_FLAG_TE) == RESET)
	{
		if((Timeout--) == 0)
		{
			return Timeout_Call_Back(0);
		}
	}
    
	Timeout = SPI_Flag_Wait_Timeout;
	/* Wait for SPI bus idle */
	while(SPI_Flag_Status_Get(SPI_FLAG_BUSY) != RESET)
	{
		if((Timeout--) == 0)
		{
			return Timeout_Call_Back(0);
		}
	}
    
	Timeout = SPI_Flag_Wait_Timeout;
	/* Wait for the receive buffer to be non-empty, RNE flag is set */
	while(SPI_Flag_Status_Get(SPI_FLAG_RNE) == RESET)
	{
		if((Timeout--) == 0)
		{
			return Timeout_Call_Back(1);
		}
	}
	
	/* Returns the received data */
	return SPI_Data_Get();
}


/**
*\*\name    SPI_Flash_Send_Half_World
*\*\fun     Send 2-byte data
*\*\param   half_world
*\*\return  Returns the received data
**/
uint16_t SPI_Flash_Send_Half_World(uint16_t half_world)
{
	Timeout = SPI_Flag_Wait_Timeout;
	
	/* Wait for the send buffer to be empty, TE flag is set */
	while(SPI_Flag_Status_Get(SPI_FLAG_TE) == RESET)
	{
		if((Timeout--) == 0)
		{
			return Timeout_Call_Back(2);
		}
	}
	
	/* Send Half Word through the FLASH_SPI peripheral */
	SPI_Data_Transmit(half_world);
	
	Timeout = SPI_Flag_Wait_Timeout;
	
	/* Wait for the receive buffer to be non-empty, RNE flag is set */
	while(SPI_Flag_Status_Get(SPI_FLAG_RNE) == RESET)
	{
		if((Timeout--) == 0)
		{
			return Timeout_Call_Back(3);
		}
	}
	
	/* Returns the received data */
	return SPI_Data_Get();
}


/**
*\*\name    SPI_Flash_Write_Enable
*\*\fun     Send a write enable command 
*\*\param   none
*\*\return  none
**/
void SPI_Flash_Write_Enable(void)
{
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send a write enable command */
	SPI_Flash_Send_Byte(W25x_Write_Enable);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
}


/**
*\*\name    SPI_Flash_Wait_Write_End
*\*\fun     Wait for Flash to write(WIP) flag clear
*\*\param   none
*\*\return  none
**/
void SPI_Flash_Wait_Write_End(void)
{
	uint8_t flash_status = 0x00;
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send the Read Status Register command */
	SPI_Flash_Send_Byte(W25x_Read_Status_Register);
	
	Timeout = SPI_Flag_Wait_Timeout;
	
	/* Wait for Flash to write(WIP) flag clear */
	do
	{
		/* Read the status registers of the FLASH chip */
		flash_status = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
		
		if((Timeout--) == 0)
		{
			Timeout_Call_Back(4);
			return;
		}
	}while((flash_status & Flash_WIP_Flag) == SET);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
}


/**
*\*\name    SPI_Flash_Power_Down
*\*\fun     Flash power-down
*\*\param   none
*\*\return  none
**/
void SPI_Flash_Power_Down(void)
{
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send power-down command */
	SPI_Flash_Send_Byte(W25x_Power_Down);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
}


/**
*\*\name    SPI_Flash_Wake_Up
*\*\fun     Flash wake-up
*\*\param   none
*\*\return  none
**/
void SPI_Flash_Wake_Up(void)
{
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send wake-up command */
	SPI_Flash_Send_Byte(W25x_Wake_Up);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
}


/**
*\*\name    SPI_Flash_Sector_Erase
*\*\fun     Erase sector
*\*\param   sector_addr: the sector address to erase
*\*\return  none
**/
void SPI_Flash_Sector_Erase(uint32_t sector_addr)
{
	/* Send Flash write enable command */
	SPI_Flash_Write_Enable();
	
	/* Wait for writing complete */
	SPI_Flash_Wait_Write_End();
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Sends sector erase command */
	SPI_Flash_Send_Byte(W25x_Sector_Erase);
	
	/* Sending Erase Sector Address Bit[23:16] */
	SPI_Flash_Send_Byte((sector_addr & 0x00FF0000) >> 16);
	
	/* Sending Erase Sector Address Bit[15:8] */
	SPI_Flash_Send_Byte((sector_addr & 0x0000FF00) >> 8);
	
	/* Sending Erase Sector Address Bit[7:0] */
	SPI_Flash_Send_Byte((sector_addr & 0x000000FF) >> 0);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	/* Wait for erasing complete */
	SPI_Flash_Wait_Write_End();
}


/**
*\*\name    SPI_Flash_Erase
*\*\fun     Flash chip erase
*\*\param   none
*\*\return  none
**/
void SPI_Flash_Erase(void)
{
	/* Send Flash write enable command */
	SPI_Flash_Write_Enable();
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Sends flash erase command */
	SPI_Flash_Send_Byte(W25x_Chip_Erase);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	/* Wait for erasing complete */
	SPI_Flash_Wait_Write_End();
}


/**
*\*\name    SPI_Flash_Page_Write
*\*\fun     SPI flash page write
*\*\param   buffer:         data buffer pointer
*\*\param   write_addr:     write address
*\*\param   NumByteToWrite: write data length
*\*\             --Note£ºMust be less than or equal to Flash_Per_Write_Page_Size
*\*\return  none
**/
void SPI_Flash_Page_Write(u8* buffer, u32 write_addr, u32 NumByteToWrite)
{
	/* Send Flash write enable command */
	SPI_Flash_Write_Enable();
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send page write command */
	SPI_Flash_Send_Byte(W25x_Page_Program);
	
	/* Sending Write Address Bit[23:16] */
	SPI_Flash_Send_Byte((write_addr & 0x00FF0000) >> 16);
	
	/* Sending Write Address Bit[15:8] */
	SPI_Flash_Send_Byte((write_addr & 0x0000FF00) >> 8);
	
	/* Sending Write Address Bit[7:0] */
	SPI_Flash_Send_Byte((write_addr & 0x000000FF) >> 0);
	
	/* Flash page program, The maximum page size per programming is the page size */
	if(NumByteToWrite > Flash_Per_Write_Page_Size)
	{
		NumByteToWrite = Flash_Per_Write_Page_Size;
		Flash_Error("SPI Flash Page Write too large!");
	}
	
	/* Write data */
	while(NumByteToWrite--)
	{
		/* Sends the byte data currently to be written */
		SPI_Flash_Send_Byte(*buffer);
		
		/* Points to the next byte of data */
		buffer++;
	}
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	/* Wait for erasing complete */
	SPI_Flash_Wait_Write_End();
}


/**
*\*\name    SPI_Flash_Buffer_Write
*\*\fun     SPI flash write program
*\*\param   buffer:         data buffer pointer
*\*\param   write_addr:     write address
*\*\param   NumByteToWrite: write data length
*\*\return  none
**/
void SPI_Flash_Buffer_Write(u8* buffer, u32 write_addr, u32 NumByteToWrite)
{
	uint8_t Num_Of_Page      = 0x00;
	uint8_t Num_Of_Remainder = 0x00;
	uint8_t Addr             = 0x00;
	uint8_t count            = 0x00;
	uint8_t temp_value       = 0x00;
	
	/* The mod operation finds the remainder */
	/* If write_addr is an integer multiple of Flash_Per_Write_Page_Size, the Addr value is 0 */
	Addr = write_addr % Flash_Per_Write_Page_Size;
	
	/* Flash write addresses are misaligned with page boundaries */
	/* Need to count the data value just enough to align to the page address */
	count = Flash_Per_Write_Page_Size - Addr;
	
	/* Calculate how many integer pages to write */
	Num_Of_Page = NumByteToWrite / Flash_Per_Write_Page_Size;
	
	/* The mod operation finds the remainder */
	/* Calculate the number of bytes remaining for less than one page */
	Num_Of_Remainder = NumByteToWrite % Flash_Per_Write_Page_Size;
	
	/* write_addr is just aligned with the Flash page boundary */
	if(Addr == 0)
	{
		/* NumByteToWrite < Flash_Per_Write_Page_Size */
		if(Num_Of_Page == 0)
		{
			SPI_Flash_Page_Write(buffer, write_addr, NumByteToWrite);
		}
		/* NumByteToWrite > Flash_Per_Write_Page_Size */
		else
		{
			/* Integer page programming first */
			while(Num_Of_Page--)
			{
				SPI_Flash_Page_Write(buffer, write_addr, Flash_Per_Write_Page_Size);
				buffer     += Flash_Per_Write_Page_Size;
				write_addr += Flash_Per_Write_Page_Size;
			}
			
			/* Have less than one page of extra data, write it down */
			SPI_Flash_Page_Write(buffer, write_addr, Num_Of_Remainder);
		}
	}
	/* write_addr is not aligned with the Flash page boundary */
	else
	{
		/* NumByteToWrite < Flash_Per_Write_Page_Size */
		if(Num_Of_Page == 0)
		{
			/* The remaining page size in the current page is less than Num_Of_Remainder */
			if(count < Num_Of_Remainder)
			{
				temp_value = Num_Of_Remainder - count;
				
				/* Fill the current page first */
				SPI_Flash_Page_Write(buffer, write_addr, count);
				buffer     += count;
				write_addr += count;
				
				/* Then write the rest of the data */
				SPI_Flash_Page_Write(buffer, write_addr, temp_value);
			}
			/* The remaining page size in the current page is larger than Num_Of_Remainder */
			else
			{
				SPI_Flash_Page_Write(buffer, write_addr, NumByteToWrite);
			}
		}
		/* NumByteToWrite > Flash_Per_Write_Page_Size */
		else
		{
			/* Fill the current Flash page first */
			NumByteToWrite  -= count;
			
			/* First fill the current Flash page and calculate how many pages are needed for the remaining data */
			Num_Of_Page      = NumByteToWrite / Flash_Per_Write_Page_Size;
			Num_Of_Remainder = NumByteToWrite % Flash_Per_Write_Page_Size;
			
			SPI_Flash_Page_Write(buffer, write_addr, count);
			buffer     += count;
			write_addr += count;
			
			/* Full-page programming */
			while(Num_Of_Page--)
			{
				SPI_Flash_Page_Write(buffer, write_addr, Flash_Per_Write_Page_Size);
				buffer     += Flash_Per_Write_Page_Size;
			  write_addr += Flash_Per_Write_Page_Size;
			}
			
			/* If there is still data left after the whole page is programmed, program the remaining data */
			if(Num_Of_Remainder != 0)
			{
				SPI_Flash_Page_Write(buffer, write_addr, Num_Of_Remainder);
			}
		}
	}
}


/**
*\*\name    SPI_Flash_Buffer_Read
*\*\fun     Read SPI Flash data
*\*\param   buffer:         data buffer pointer
*\*\param   read_addr:      read address
*\*\param   NumByteToRead:  read data length
*\*\return  none
**/
void SPI_Flash_Buffer_Read(u8* buffer, u32 read_addr, u32 NumByteToRead)
{
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send read command */
	SPI_Flash_Send_Byte(W25x_Read_Data);
	
	/* Sending Read Address Bit[23:16] */
	SPI_Flash_Send_Byte((read_addr & 0x00FF0000) >> 16);
	
	/* Sending Read Address Bit[15:8] */
	SPI_Flash_Send_Byte((read_addr & 0x0000FF00) >> 8);
	
	/* Sending Read Address Bit[7:0] */
	SPI_Flash_Send_Byte((read_addr & 0x000000FF) >> 0);
	
	/* Read data */
	while(NumByteToRead--)
	{
		/* Read 1-byte data */
		*buffer = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
		
		/* Points to the next byte buffer */
		buffer++;
	}
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
}


/**
*\*\name    SPI_Flash_Read_ID
*\*\fun     Read Flash ID
*\*\param   none
*\*\return  Flash ID
**/
uint32_t SPI_Flash_Read_ID(void)
{
	uint32_t temp_value  = 0x00;
	uint32_t temp_value1 = 0x00;
	uint32_t temp_value2 = 0x00;
	uint32_t temp_value3 = 0x00;
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send JEDEC instructions to read the ID */
	SPI_Flash_Send_Byte(W25x_Jedec_Device_ID);
	
	/* Read 3-byte data */
	temp_value1 = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	temp_value2 = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	temp_value3 = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	temp_value = ((temp_value1 << 16) | (temp_value2 << 8) | temp_value3);
	
	return temp_value;
}


/**
*\*\name    SPI_Flash_Read_Device_ID
*\*\fun     Read Flash Device_ID
*\*\param   none
*\*\return  Flash Device_ID
**/
uint32_t SPI_Flash_Read_Device_ID(void)
{
	uint32_t temp_value  = 0x00;
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send JEDEC instructions to read the Device_ID */
	SPI_Flash_Send_Byte(W25x_Divece_ID);
	SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	
	/* Read 1-byte data */
	temp_value = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	return temp_value;
}


/**
*\*\name    SPI_Flash_Start_Read_Sequence
*\*\fun     Initiates a read data byte (READ) sequence from the Flash. 
*\*\        This is done by driving the NSS line low to select the device, 
*\*\        then the READ instruction is transmitted followed by 3 bytes address. 
*\*\        This function exit and keep the NSS line low, so the Flash still being selected. 
*\*\        With this technique the whole content of the Flash is read with a single READ instruction.
*\*\param   read_addr: read address
*\*\return  none
**/
void SPI_Flash_Start_Read_Sequence(u32 read_addr)
{
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send "Read from Memory" instruction */
	SPI_Flash_Send_Byte(W25x_Read_Data);
	
	/* Sending Read Address Bit[23:16] */
	SPI_Flash_Send_Byte((read_addr & 0x00FF0000) >> 16);
	
	/* Sending Read Address Bit[15:8] */
	SPI_Flash_Send_Byte((read_addr & 0x0000FF00) >> 8);
	
	/* Sending Read Address Bit[7:0] */
	SPI_Flash_Send_Byte((read_addr & 0x000000FF) >> 0);
}


/**
*\*\name    SPI_Flash_Read_Byte
*\*\fun     Use SPI to read one byte of data
*\*\param   none
*\*\return  The received data
**/
uint8_t SPI_Flash_Read_Byte(void)
{
	return (SPI_Flash_Send_Byte(Flash_Dummy_Byte));
}

/**
*\*\name    SPI_Flash_Four_Byte_Address_Mode
*\*\fun     SPI Flash Four Byte Address Mode
*\*\param   none
*\*\return  none
**/
void SPI_Flash_Four_Byte_Address_Mode(void)
{
	uint8_t temp_value = 0x00;
	
	/* Pull-down NSS level */
	SPI_Flash_NSS_Low();
	
	/* Send the status register 3 command */
	SPI_Flash_Send_Byte(W25x_Read_Status_Register3);
	
	temp_value = SPI_Flash_Send_Byte(Flash_Dummy_Byte);
	
	/* Pull-up NSS level */
	SPI_Flash_NSS_High();
	
	if((temp_value & 0x01) == 0)
	{
		/* Pull-down NSS level */
	  SPI_Flash_NSS_Low();
		
		/* Enter 4-byte address mode */
		SPI_Flash_Send_Byte(W25x_Four_Byte_Mode);
		
		/* Pull-up NSS level */
	  SPI_Flash_NSS_High();
	}
}


/**
*\*\name    Timeout_Call_Back
*\*\fun     Wait for the timeout to output an error message
*\*\param   error_code
*\*\return  none
**/
static uint16_t Timeout_Call_Back(uint8_t error_code)
{
	/* Wait for the timeout to output an error message */
	Flash_Error("SPI wait timeout! Error_Code = %d", error_code);
	
	return 0;
}







