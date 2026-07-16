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
*\*\file system_n32g003.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "n32g003_flash.h"


/**
*\*\name    FLASH_Latency_Set
*\*\fun     Sets the code latency value.
*\*\note    This function can be used for N32G003 devices.
*\*\param   flash_latency :
*\*\            - FLASH_LATENCY_0    FLASH Zero Latency cycle, 0 < SYSCLK <= 24MHz      
*\*\            - FLASH_LATENCY_1    FLASH One Latency cycle, 24MHz < SYSCLK<= 48MHz
*\*\return  none
**/

void FLASH_Latency_Set(uint32_t flash_latency)
{
    uint32_t temp_value = 0;

    /* Read the AC register */
    temp_value = FLASH->AC;

    /* Sets the Latency value */
    temp_value &= (~FLASH_LATENCY_MASK); 
    temp_value |= flash_latency;

    /* Write the AC register */
    FLASH->AC = temp_value;
}

/**
*\*\name    FLASH_Latency_Get
*\*\fun     Get the code latency value.
*\*\note    This function can be used for N32G003 devices.
*\*\param   none
*\*\return  FLASH_LATENCY :
*\*\            - FLASH_LATENCY_0    FLASH Zero Latency cycle, 0 < SYSCLK <= 24MHz      
*\*\            - FLASH_LATENCY_1    FLASH One Latency cycle, 24MHz < SYSCLK <= 48MHz
**/
uint8_t FLASH_Latency_Get(void)
{
    /* Read the AC register */
    return (uint8_t)(FLASH->AC & FLASH_LATENCY_MASK);
}


/**
*\*\name    FLASH_Prefetch_Buffer_Enable
*\*\fun     Enables the Prefetch Buffer.
*\*\note    This function can be used for N32G003 devices. 
*\*\param   none 
*\*\return  none
**/
void FLASH_Prefetch_Buffer_Enable(void)
{
    /* Enable the Prefetch Buffer */
    FLASH->AC |= FLASH_PREFETCHBUF_EN;
}

/**
*\*\name    FLASH_Prefetch_Buffer_Disable
*\*\fun     Disables the Prefetch Buffer.
*\*\note    This function can be used for N32G003 devices.
*\*\param   none
*\*\return  none
**/
void FLASH_Prefetch_Buffer_Disable(void)
{
    /* Disable the Prefetch Buffer */
    FLASH->AC &= FLASH_PREFETCHBUF_DIS;
}

/**
*\*\name   FLASH_Unlock
*\*\fun    Unlocks the FLASH Program Erase Controller.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return none
**/
void FLASH_Unlock(void)
{
    FLASH->KEY = FLASH_KEY1;
    FLASH->KEY = FLASH_KEY2;
}



/**
*\*\name   FLASH_Lock
*\*\fun    Locks the FLASH Program Erase Controller.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return none
**/
void FLASH_Lock(void)
{
    FLASH->CTRL |= FLASH_CTRL_SET_LOCK;
}


/**
*\*\name   Option_Bytes_Unlock
*\*\fun    Unlocks the Option_Bytes Program Erase Controller.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return none
**/
void Option_Bytes_Unlock(void)
{
    FLASH->OPTKEY = FLASH_KEY1;
    FLASH->OPTKEY = FLASH_KEY2;
}


/**
*\*\name   Option_Bytes_Lock
*\*\fun    Locks the Option_Bytes Program Erase Controller.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return none
**/
void Option_Bytes_Lock(void)
{
    /* Set the FLASH_CTRL_SET_OPTWE Bit to lock */
    FLASH->CTRL &= (~FLASH_CTRL_SET_OPTWE);
}


/**
*\*\name   Option_Bytes_Lock_Status_Get
*\*\fun    Get the Option Bytes lock status.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return FlagStatus :
*\*\            - SET
*\*\            - RESET
**/
FlagStatus Option_Bytes_Lock_Status_Get(void)
{
    if ((FLASH->CTRL & FLASH_CTRL_SET_OPTWE) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


/**
*\*\name   FLASH_One_Page_Erase
*\*\fun    Erases a specified FLASH page.
*\*\note   This function can be used for N32G003 devices.
*\*\param  page_address :
*\*\            - It ranges from 0x08000000 to 0x080075FF
*\*\return FLASH_STS : 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG 
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_One_Page_Erase(uint32_t page_address)
{
    FLASH_STS status_value = FLASH_EOP;
    
    if((page_address < 0x08000000) || (page_address > 0x080075FF))
    {
        /* The programming address out of range */
        return FLASH_ERR_ADD;
    }
    else
    {
        /*No process*/
    }

   /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CTRL |= FLASH_CTRL_SET_PER;
        page_address = page_address & 0x8007FFF;
        FLASH->ADD = page_address;
        FLASH->CTRL |= FLASH_CTRL_SET_START;

        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

        /* Disable the PER Bit */
        FLASH->CTRL &= FLASH_CTRL_RESET_PER;
    }
    else
    {
        /*No process*/
    }

    /* Return the Erase status_value */
    return status_value;
}


/**
*\*\name   FLASH_Mass_Erase
*\*\fun    Erases all FLASH pages.
*\*\note   This function can be used for all N32G003 devices.
*\*\param  none
*\*\return FLASH_STS : 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG 
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Mass_Erase(void)
{
    FLASH_STS status_value = FLASH_EOP;

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        /* if the previous operation is completed, proceed to erase all pages */
        FLASH->CTRL |= FLASH_CTRL_SET_MER;
        FLASH->CTRL |= FLASH_CTRL_SET_START;

        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

        /* Disable the MER Bit */
        FLASH->CTRL &= FLASH_CTRL_RESET_MER;
    }
    else
    {
        /*No process*/
    }

    /* Return the Erase status_value */
    return status_value;
}


/**
*\*\name   FLASH_48Page_Mass_Erase
*\*\fun    Erases first 48 pages FLASH.
*\*\note   This function can be used for all N32G003 devices.
*\*\param  none
*\*\return FLASH_STS : 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG 
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_48Page_Mass_Erase(void)
{
    FLASH_STS status_value = FLASH_EOP;

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        /* if the previous operation is completed, proceed to erase all pages */
        FLASH->CTRL |= FLASH_CTRL_SET_MMER;
        FLASH->CTRL |= FLASH_CTRL_SET_START;

        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

        /* Disable the MER Bit */
        FLASH->CTRL &= FLASH_CTRL_RESET_MMER;
    }
    else
    {
        /*No process*/
    }

    /* Return the Erase status_value */
    return status_value;
}



/**
*\*\name   FLASH_Word_Program
*\*\fun    Programs a word at a specified address.
*\*\note   This function can be used for N32G003 devices.
*\*\param  address :
*\*\            - It ranges from 0x08000000 to 0x080075FF
*\*\param  data :
*\*\            - It ranges from 0x00000000 to 0xFFFFFFFF
*\*\return FLASH_STS : 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_ERR_ADD
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Word_Program(uint32_t address, uint32_t data)
{
    FLASH_STS status_value  = FLASH_EOP;

    if((address & FLASH_WORD_LENGTH) != RESET)
    {
        /* The programming address is not a multiple of 4 */
        return FLASH_ERR_ADD;
    }
    else
    {
        /*No process*/
    }
    
    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        /* if the previous operation is completed, proceed to program the new word */
        FLASH->CTRL |= FLASH_CTRL_SET_PG;

        *(__IO uint32_t*)address = (uint32_t)data;
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);

        /* Disable the PG Bit */
        FLASH->CTRL &= FLASH_CTRL_RESET_PG;
    }
    else
    {
        /*No process*/
    }

    /* Return the Program status_value */
    return status_value;
}


/**
*\*\name   FLASH_Option_Bytes_Erase
*\*\fun    Erases the FLASH option bytes.
*\*\note   This functions erases all option bytes except the Read protection (RDP).
*\*\       This function can be used for N32G003 devices.
*\*\param  none
*\*\return FLASH_STS : 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG 
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_ERR_RDP2
*\*\            - FLASH_ERR_ADD
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Option_Bytes_Erase(void)
{
    FLASH_STS status_value = FLASH_EOP;

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();

        /* if the previous operation is completed, proceed to erase the option bytes */
        FLASH->CTRL |= FLASH_CTRL_SET_OPTER;
        FLASH->CTRL |= FLASH_CTRL_SET_START;
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

        if (status_value == FLASH_EOP)
        {
            /* Clears the FLASH's pending flags */
            FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);

            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
        }
        else
        {
            if (status_value != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            }
            else
            {
                /*No process*/
            }
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the erase status_value */
    return status_value;
}


/**
*\*\name   FLASH_Option_Bytes_User_RDP1_Program
*\*\fun    Programs the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_PD / 
*\*\       LOCK_BOOT / PA0_NRST / RDP1.
*\*\note   This function can be used for N32G003 devices.
*\*\param  option_byte_rdp1 :
*\*\        - FLASH_OB_RDP1_ENABLE
*\*\        - FLASH_OB_RDP1_DISABLE
*\*\param  option_byte_iwdg :
*\*\        - FLASH_OB_IWDG_SW Software IWDG selected
*\*\        - FLASH_OB_IWDG_HW Hardware IWDG selected
*\*\param  option_byte_stop :
*\*\        - FLASH_OB_STOP_NORST No reset generated when entering in STOP
*\*\        - FLASH_OB_STOP_RST Reset generated when entering in STOP
*\*\param  option_byte_PD :
*\*\        - FLASH_OB_PD_NORST No reset generated when entering in PD
*\*\        - FLASH_OB_PD_RST Reset generated when entering in PD
*\*\param  option_byte_lock_boot :
*\*\        - FLASH_OB_LOCK_BOOT First 3KB FLASH LOCK
*\*\        - FLASH_OB_NOLOCK_BOOT First 3KB FLASH NOLOCK
*\*\param  option_byte_PA0_NRST :
*\*\        - FLASH_OB_PA0_IO PA0 is general pin
*\*\        - FLASH_OB_PA0_NRST PA0 is RESET pin
*\*\return FLASH_STS : 
*\*\        - FLASH_BUSY
*\*\        - FLASH_ERR_PG 
*\*\        - FLASH_ERR_WRP
*\*\        - FLASH_EOP
*\*\        - FLASH_ERR_RDP2
*\*\        - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Option_Bytes_User_RDP1_Program(uint8_t option_byte_rdp1, uint16_t option_byte_iwdg, \
                                               uint16_t option_byte_stop, uint16_t option_byte_PD, \
                                               uint16_t option_byte_lock_boot, uint16_t option_byte_PA0_NRST)
{
    FLASH_STS status_value = FLASH_EOP;

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }

    Option_Bytes_Unlock();

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        /* Enable the Option Bytes Programming operation */
        FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;
        /* Restore the last read protection Option Byte value */
        OBT->USER_RDP =
            (uint32_t)option_byte_rdp1
            | (((uint32_t)(option_byte_iwdg | option_byte_stop | option_byte_PD | option_byte_lock_boot 
            | option_byte_PA0_NRST)) << FLASH_USER_RDP_OFFSET);
        
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);
        if (status_value != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
        }
        else
        {
            /*No process*/
        }
    }
    else
    {
        /*No process*/
    }

    /* Return the Option Byte program status_value */
    return status_value;
}

/**
*\*\name   FLASH_Option_Bytes_DATA_Program
*\*\fun    Programs a half word at a specified Option Byte Data0 and Data1.
*\*\note   This function can be used for N32G003 devices.
*\*\param  option_byte_data0:
*\*\                - 0x00 to 0xFF
*\*\param  option_byte_data1:
*\*\                - 0x00 to 0xFF
*\*\return FLASH_STS: The returned value can be: 
*\*\                - FLASH_BUSY
*\*\                - FLASH_ERR_PG
*\*\                - FLASH_ERR_WRP
*\*\                - FLASH_EOP
*\*\                - FLASH_ERR_RDP2
*\*\                - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Option_Bytes_DATA_Program(uint8_t option_byte_data0,uint8_t option_byte_data1)
{
    FLASH_STS status_value = FLASH_EOP;

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }
    
    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();
        /* Enables the Option Bytes Programming operation */
        FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;
        OBT->Data1_Data0 = (((uint32_t)option_byte_data1) << FLASH_DATA0_DATA1_OFFSET) \
                          | ((uint32_t)option_byte_data0);

        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);
        if (status_value != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
        }
        else
        {
            /*No process*/
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the Option Byte Data Program status_value */
    return status_value;
}


/**
*\*\name   FLASH_Option_Bytes_User2_User3_Program
*\*\fun    Programs a half word at a specified Option Byte USER2 and USER3.
*\*\note   This function can be used for N32G003 devices.
*\*\param  option_byte_user2:
*\*\                - 0x00 to 0xFF  LVR filter control count value
*\*\param  option_byte_LVR_cnt :
*\*\                - 0x00 to 0x0F  LVR gear selection
*\*\param  option_byte_LVR :
*\*\                - FLASH_OB_USER_LVR_ENABLE LVR enable
*\*\                - FLASH_OB_USER_LVR_DISABLE LVR disable
*\*\param  option_byte_LVR_filter :
*\*\                - FLASH_OB_USER_LVRFIL_ENABLE LVR filter enable
*\*\                - FLASH_OB_USER_LVRFIL_DISABLE LVR filter disable
*\*\param  option_byte_LVR_reset :
*\*\                - FLASH_OB_USER_LVRRST_ENABLE LVR reset enable
*\*\                - FLASH_OB_USER_LVRRST_DISABLE LVR reset disable
*\*\return FLASH_STS: The returned value can be: 
*\*\                - FLASH_BUSY
*\*\                - FLASH_ERR_PG
*\*\                - FLASH_ERR_WRP
*\*\                - FLASH_EOP
*\*\                - FLASH_ERR_RDP2
*\*\                - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Option_Bytes_User2_User3_Program(uint8_t option_byte_user2,uint8_t option_byte_LVR_cnt, \
                                                uint8_t option_byte_LVR,uint8_t option_byte_LVR_filter,uint8_t option_byte_LVR_reset)  
{
    FLASH_STS status_value = FLASH_EOP;
 
    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);
    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }
    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();
        /* Enables the Option Bytes Programming operation */
        FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;
		OBT->USER3_USER2 = (uint32_t)option_byte_user2 | (((uint32_t)(option_byte_LVR_cnt | option_byte_LVR | option_byte_LVR_filter | option_byte_LVR_reset)) << FLASH_USER2_USER3_OFFSET);

        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);
        if (status_value != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
        }
        else
        {
            /*No process*/
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the Option Byte Data Program status_value */
    return status_value;
}


/**
*\*\name   FLASH_Option_Bytes_User4_RDP2_Program
*\*\fun    Programs a half word at a specified Option Byte Data address.
*\*\note   This function can be used for N32G003 devices.
*\*\param  option_byte_rdp2:
*\*\            - FLASH_OB_RDP2_ENABLE
*\*\            - FLASH_OB_RDP2_DISABLE
*\*\param  option_byte_user4:
*\*\            - 0x00 to 0xFF
*\*\return FLASH_STS: The returned value can be: 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_ERR_RDP2
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Option_Bytes_User4_RDP2_Program(uint8_t option_byte_rdp2,uint8_t option_byte_user4)
{
    FLASH_STS status_value = FLASH_EOP;
 
    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }
    
    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);
    
    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();
        /* Enables the Option Bytes Programming operation */
        FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;
		OBT->USER4_RDP2 = (((uint32_t)option_byte_user4) << FLASH_DATA0_DATA1_OFFSET) \
                          | ((uint32_t)option_byte_rdp2);
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(PROGRAM_TIMEOUT);
        if (status_value != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
        }
        else
        {
            /*No process*/
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the Option Byte Data Program status_value */
    return status_value;
}



/**
*\*\name   FLASH_Read_Out_Protection_L1_Enable
*\*\fun    Enables the read out protection.
*\*\note   If the user has already programmed the other option bytes before calling
*\*\       this function, he must re-program them since this function erases all option bytes.
*\*\       This function can be used for N32G003 devices.
*\*\param  none
*\*\return FLASH_STS : 
*\*\        - FLASH_BUSY
*\*\        - FLASH_ERR_PG
*\*\        - FLASH_ERR_WRP
*\*\        - FLASH_EOP
*\*\        - FLASH_ERR_RDP2
*\*\        - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Read_Out_Protection_L1_Enable(void)
{
    uint32_t user_temp;
    FLASH_STS status_value = FLASH_EOP;

    user_temp = ((FLASH_OB_USER_MSK & FLASH->OB) << FLASH_OB_TO_USER_OFFSET);

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();
        FLASH->CTRL |= FLASH_CTRL_SET_OPTER;
        FLASH->CTRL |= FLASH_CTRL_SET_START;
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);
        if (status_value == FLASH_EOP)
        {
            /* Clears the FLASH's pending flags */
            FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);

            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;
            /*enable new state of the ReadOut Protection*/
            OBT->USER_RDP = (FLASH_USER_MASK & user_temp);
            /* Wait for last operation to be completed */
            status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

            if (status_value != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
            }
            else
            {
                /*No process*/
            }
        }
        else
        {
            if (status_value != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            }
            else
            {
                /*No process*/
            }
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the protection operation status_value */
    return status_value;
}


/**
*\*\name   FLASH_Read_Out_Protection_L1_Disable
*\*\fun    Disables the read out protection.
*\*\note   If the user has already programmed the other option bytes before calling
*\*\       this function, he must re-program them since this function erases all option bytes.
*\*\       This function can be used for N32G003 devices.
*\*\param  none
*\*\return FLASH_STS : 
*\*\        - FLASH_BUSY
*\*\        - FLASH_ERR_PG
*\*\        - FLASH_ERR_WRP
*\*\        - FLASH_EOP
*\*\        - FLASH_ERR_RDP2
*\*\        - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Read_Out_Protection_L1_Disable(void)
{
    uint32_t user_temp;
    FLASH_STS status_value = FLASH_EOP;

    user_temp = ((FLASH_OB_USER_MSK & FLASH->OB) << FLASH_OB_TO_USER_OFFSET);

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;        
    }
    else
    {
        /*No process*/
    }

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();
        FLASH->CTRL |= FLASH_CTRL_SET_OPTER;
        FLASH->CTRL |= FLASH_CTRL_SET_START;
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);
        if (status_value == FLASH_EOP)
        {
            /* Clears the FLASH's pending flags */
            FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);

            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;
            /* Disables the read out protection */
            OBT->USER_RDP = ((FLASH_L1_RDP_KEY & FLASH_RDP1_MASK) | user_temp);
            /* Wait for last operation to be completed */
            status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

            if (status_value != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
            }
            else
            {
                /*No process*/
            }
        }
        else
        {
            if (status_value != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            }
            else
            {
                /*No process*/
            }
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the protection operation status_value */
    return status_value;
}


/**
*\*\name   FLASH_Read_Out_Protection_L2_Enable
*\*\fun    Enables or disables the read out protection L2.
*\*\note   If the user has already programmed the other option bytes before calling
*\*\       this function, he must re-program them since this function erases all option bytes.
*\*\       This function can be used for N32G003 devices.
*\*\param  none
*\*\return FLASH_STS : 
*\*\        - FLASH_BUSY
*\*\        - FLASH_ERR_PG 
*\*\        - FLASH_ERR_WRP
*\*\        - FLASH_EOP
*\*\        - FLASH_ERR_RDP2
*\*\        - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Read_Out_Protection_L2_Enable(void)
{
    uint32_t user_temp;
    FLASH_STS status_value = FLASH_EOP;

    user_temp = ((FLASH_OB_USER_MSK & FLASH->OB) << FLASH_OB_TO_USER_OFFSET);
  
    /* Get the actual read protection L1 Option Byte value */
    if (FLASH_Read_Out_Protection_Status_Get() == RESET)
    {
        user_temp |= (FLASH_L1_RDP_KEY & FLASH_RDP1_MASK);
    }
    else
    {
        /*No process*/
    }
    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_Read_Out_Protection_L2_Status_Get() != RESET)
    {
        return FLASH_ERR_RDP2;
    }
    else
    {
        /*No process*/
    }

    /* Clears the FLASH's pending flags */
    FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

    if (status_value == FLASH_EOP)
    {
        Option_Bytes_Unlock();
        FLASH->CTRL |= FLASH_CTRL_SET_OPTER;
        FLASH->CTRL |= FLASH_CTRL_SET_START;
        /* Wait for last operation to be completed */
        status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);
        if (status_value == FLASH_EOP)
        {
            /* Clears the FLASH's pending flags */
            FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG); 

            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= FLASH_CTRL_SET_OPTPG;

            OBT->USER_RDP = user_temp;
            /* Wait for last operation to be completed */
            status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);

            if (status_value == FLASH_EOP)
            {
                /* Clears the FLASH's pending flags */
                FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);

                /* Enables the read out protection L2 */
                OBT->USER4_RDP2 = FLASH_L2_RDP_KEY;

                /* Wait for last operation to be completed */
                status_value = FLASH_Last_Operation_Wait(ERASE_TIMEOUT);
            }
            else
            {
                /*No process*/
            }

            if (status_value != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTPG;
            }
            else
            {
                /*No process*/
            }
        }
        else
        {
            if (status_value != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= FLASH_CTRL_RESET_OPTER;
            }
            else
            {
                /*No process*/
            }
        }
    }
    else
    {
        /*No process*/
    }
    /* Return the protection operation status_value */
    return status_value;
}


/**
*\*\name   FLASH_Option_Bytes_User_Get
*\*\fun    Returns the FLASH User Option Bytes values.
*\*\note   This function can be used for N32G003 devices.
*\*\param  option_byte_bit
*\*\            - FLASH_OB_IWDG_SW
*\*\            - FLASH_OB_STOP_NORST
*\*\            - FLASH_OB_PD_NORST
*\*\            - FLASH_OB_NOLOCK_BOOT
*\*\            - FLASH_OB_PA0_NRST
*\*\return FlagStatus :
*\*\            - SET
*\*\            - RESET
**/
FlagStatus FLASH_Option_Bytes_User_Get(uint32_t option_byte_bit)
{
    if(((FLASH->OB >> FLASH_OB_OFFSET) & option_byte_bit) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


/**
*\*\name   FLASH_Option_Bytes_User2_Get
*\*\fun    Returns the FLASH Option Bytes LVRCNT values.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return LVRCNT:
*\*\            - 0x00 to 0xFF.
**/
uint32_t FLASH_Option_Bytes_User2_Get(void)
{
    /* Return the User Option Byte2 */
    return (uint32_t)(FLASH->USER & FLASH_USER_LVRCNT_MSK);
}


/**
*\*\name   FLASH_User_LVRLS_Get
*\*\fun    Returns the FLASH Option Bytes LVRLS values.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return LVRLS:
*\*\            - 0x00 to 0x0F.
**/
uint32_t FLASH_User_LVRLS_Get(void)
{
    /* Return the Option Byte User3 LVRLS */
    return (uint32_t)((FLASH->USER & FLASH_USER_LVRLS_MSK) >> FLASH_USER3_OFFSET);
}


/**
*\*\name   FLASH_Option_Bytes_User3_Get
*\*\fun    Returns the FLASH Option Bytes User3 values.
*\*\note   This function can be used for N32G003 devices.
*\*\param  option_byte_bit
*\*\            - FLASH_OB_USER_LVR_DISABLE
*\*\            - FLASH_OB_USER_LVRFIL_DISABLE
*\*\            - FLASH_OB_USER_LVRRST_DISABLE
*\*\return FlagStatus :
*\*\            - SET
*\*\            - RESET
**/
FlagStatus FLASH_Option_Bytes_User3_Get(uint32_t option_byte_bit)
{
    if(((FLASH->USER >> FLASH_USER3_OFFSET) & option_byte_bit) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
*\*\name   FLASH_Option_Bytes_User4_Get
*\*\fun    Returns the FLASH Option Bytes LVRCNT values.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return LVRCNT:
*\*\            - 0x00 to 0xFF.
**/
uint32_t FLASH_Option_Bytes_User4_Get(void)
{
    /* Return the Option Byte User4 */
    return (uint32_t)((FLASH->USER & FLASH_USER_POR_DELAY_MSK) >> FLASH_USER4_OFFSET);
}


/**
*\*\name   FLASH_Read_Out_Protection_Status_Get
*\*\fun    Checks whether the FLASH Read Out Protection status_value is set or not.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return FlagStatus :
*\*\        - SET
*\*\        - RESET
**/
FlagStatus FLASH_Read_Out_Protection_Status_Get(void)
{
    if ((FLASH->OB & FLASH_RDPRTL1_MSK) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


/**
*\*\name   FLASH_Read_Out_Protection_L2_Status_Get
*\*\fun    Checks whether the FLASH Read Out Protection L2 status_value is set or not.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return FlagStatus :
*\*\        - SET
*\*\        - RESET
**/
FlagStatus FLASH_Read_Out_Protection_L2_Status_Get(void)
{
    if ((FLASH->OB & FLASH_RDPRTL2_MSK) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}





/**
*\*\name   FLASH_Prefetch_Buffer_Status_Get
*\*\fun    Checks whether the FLASH Prefetch Buffer status_value is set or not.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return FlagStatus :
*\*\        - SET
*\*\        - RESET
**/
FlagStatus FLASH_Prefetch_Buffer_Status_Get(void)
{
    if ((FLASH->AC & FLASH_PRFTBS_MSK) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}



/**
*\*\name   FLASH_Interrupt_Enable
*\*\fun    Enables the specified FLASH interrupts.
*\*\note   This function can be used for N32G003 devices.
*\*\param  flash_interrupts :
*\*\        - FLASH_INT_ERROR    FLASH Error Interrupt
*\*\        - FLASH_INT_EOP      FLASH end of operation Interrupt
*\*\return none
**/
void FLASH_Interrupt_Enable(uint32_t flash_interrupts)
{
		
    /* Enable the interrupt sources */
    FLASH->CTRL |= flash_interrupts;
}


/**
*\*\name   FLASH_Interrupt_Disable
*\*\fun    Disables the specified FLASH interrupts.
*\*\note   This function can be used for N32G003 devices.
*\*\param  flash_interrupts :
*\*\        - FLASH_INT_ERROR    FLASH Error Interrupt
*\*\        - FLASH_INT_EOP      FLASH end of operation Interrupt
*\*\return none
**/
void FLASH_Interrupt_Disable(uint32_t flash_interrupts)
{
    /* Disable the interrupt sources */
    FLASH->CTRL &= ~(uint32_t)flash_interrupts;
}




/**
*\*\name   FLASH_Flag_Status_Get
*\*\fun    Checks whether the specified FLASH flag is set or not.
*\*\note   This function can be used for N32G003 devices.
*\*\param  flash_flag :
*\*\        - FLASH_FLAG_BUSY      FLASH Busy flag
*\*\        - FLASH_FLAG_PGERR     FLASH Program error flag
*\*\        - FLASH_FLAG_WRPERR    FLASH Write protected error flag
*\*\        - FLASH_FLAG_EOP       FLASH End of Operation flag
*\*\return FlagStatus :
*\*\        - SET
*\*\        - RESET
**/
FlagStatus FLASH_Flag_Status_Get(uint32_t flash_flag)
{
    if ((FLASH->STS & flash_flag) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


/**
*\*\name   FLASH_Option_Bytes_Flag_Get
*\*\fun    Checks whether the specified FLASH flag is set or not.
*\*\note   This function can be used for N32G003 devices.
*\*\param  flash_flag :
*\*\        - FLASH_FLAG_OBERR FLASH Option Byte error flag
*\*\return FlagStatus :
*\*\        - SET
*\*\        - RESET
**/
FlagStatus FLASH_Option_Bytes_Flag_Get(uint32_t flash_flag)
{
    if ((FLASH->OB & flash_flag) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


/**
*\*\name   FLASH_Flag_Status_Clear
*\*\fun    Clears the FLASH's status flags.
*\*\note   This function can be used for N32G003 devices.
*\*\param  flash_flag :
*\*\        - FLASH_FLAG_PGERR     FLASH Program error flag
*\*\        - FLASH_FLAG_WRPERR    FLASH Write protected error flag
*\*\        - FLASH_FLAG_EOP       FLASH End of Operation flag
*\*\return none
**/
void FLASH_Flag_Status_Clear(uint32_t flash_flag)
{
    /* Clear the flags */
    FLASH->STS |= flash_flag;
}



/**
*\*\name   FLASH_Status_Get
*\*\fun    Returns the FLASH_STS.
*\*\note   This function can be used for N32G003 devices.
*\*\param  none
*\*\return FLASH_STS :
*\*\        - FLASH_BUSY
*\*\        - FLASH_ERR_PG 
*\*\        - FLASH_ERR_WRP
*\*\        - FLASH_EOP
**/
FLASH_STS FLASH_Status_Get(void)
{
    FLASH_STS status_value = FLASH_EOP;

    if ((FLASH->STS & FLASH_FLAG_BUSY) == FLASH_FLAG_BUSY)
    {
        status_value = FLASH_BUSY;
    }
    else
    {
        if ((FLASH->STS & FLASH_FLAG_PGERR) != RESET)
        {
            status_value = FLASH_ERR_PG;
        }
        else
        {
            if ((FLASH->STS & FLASH_FLAG_WRPERR) != RESET)
            {
                status_value = FLASH_ERR_WRP;
            }
            else
            {
                status_value = FLASH_EOP;
            }
        }
    }

    /* Return the FLASH_STS */
    return status_value;
}


/**
*\*\name   FLASH_Last_Operation_Wait
*\*\fun    Waits for a Flash operation to complete or a timeout to occur.
*\*\note   This function can be used for N32G003 devices.
*\*\param  timeout :
*\*\            - ERASE_TIMEOUT
*\*\            - PROGRAM_TIMEOUT
*\*\return FLASH_STS: The returned value can be: 
*\*\            - FLASH_BUSY
*\*\            - FLASH_ERR_PG 
*\*\            - FLASH_ERR_WRP
*\*\            - FLASH_EOP
*\*\            - FLASH_TIMEOUT
**/
FLASH_STS FLASH_Last_Operation_Wait(uint32_t timeout)
{
    FLASH_STS status_value = FLASH_EOP;

    /* Check for the Flash status_value */
    status_value = FLASH_Status_Get();
    /* Wait for a Flash operation to complete or a timeout to occur */
    while ((status_value == FLASH_BUSY) && (timeout != 0x00))
    {
        status_value = FLASH_Status_Get();
        timeout--;
    }
    if (timeout == 0x00)
    {
        status_value = FLASH_TIMEOUT;
    }
    /* Return the operation status_value */
    return status_value;
}


/**
*\*\name   FLASH_VTOR_Address_Set
*\*\fun    Set interrupt vector offset address.
*\*\note   This function can be used for N32G003 devices.
*\*\param  VTOR_address :
*\*\            - 0x00000000~0x7FFFFFFF
*\*\param  VTOR_cmd :
*\*\            - FLASH_VTOR_ENABLE
*\*\            - FLASH_VTOR_DISABLE
*\*\return none
**/
void FLASH_VTOR_Address_Set(uint32_t VTOR_address,uint32_t VTOR_cmd)
{
    /* Set VTOR address */
    FLASH->VTOR = (VTOR_cmd | VTOR_address);
}



