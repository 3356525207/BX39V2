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
*\*\file n32g003_flash.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef __N32G003_FLASH_H
#define __N32G003_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "n32g003.h" 

/** FLASH Status **/
typedef enum
{ 
    FLASH_BUSY = 1,
    FLASH_ERR_PG,
    FLASH_ERR_WRP,
    FLASH_EOP,
    FLASH_ERR_RDP2,
    FLASH_ERR_ADD,
    FLASH_TIMEOUT	
}FLASH_STS;

/** Flash Access Control Register bits **/
#define FLASH_PRFTBS_MSK  ((uint32_t)FLASH_AC_PRFTBFSTS)
/** Prefetch_Buffer_Enable_Disable **/
#define FLASH_PREFETCHBUF_EN              ((uint32_t)FLASH_AC_PRFTBFEN)            /* FLASH Prefetch Buffer Enable */
#define FLASH_PREFETCHBUF_DIS             (~((uint32_t)FLASH_AC_PRFTBFEN))         /* FLASH Prefetch Buffer Disable */

/** Flash_Latency **/
#define FLASH_LATENCY_0     ((uint32_t)FLASH_AC_LATENCY_0) /* FLASH Zero Latency cycle */
#define FLASH_LATENCY_1     ((uint32_t)FLASH_AC_LATENCY_1) /* FLASH One Latency cycle */
#define FLASH_LATENCY_MASK  ((uint32_t)FLASH_AC_LATENCY)

/** FLASH Keys **/
#define FLASH_KEY1   ((uint32_t)0x45670123)
#define FLASH_KEY2   ((uint32_t)0xCDEF89AB)

/** Flash Control Register bits **/
#define FLASH_CTRL_SET_PG       ((uint32_t)FLASH_CTRL_PG)
#define FLASH_CTRL_RESET_PG     (~((uint32_t)FLASH_CTRL_PG))
#define FLASH_CTRL_SET_PER      ((uint32_t)FLASH_CTRL_PER)
#define FLASH_CTRL_RESET_PER    (~((uint32_t)FLASH_CTRL_PER))
#define FLASH_CTRL_SET_MER      ((uint32_t)FLASH_CTRL_MER)
#define FLASH_CTRL_RESET_MER    (~((uint32_t)FLASH_CTRL_MER))
#define FLASH_CTRL_SET_OPTPG    ((uint32_t)FLASH_CTRL_OPTPG)
#define FLASH_CTRL_RESET_OPTPG  (~((uint32_t)FLASH_CTRL_OPTPG))
#define FLASH_CTRL_SET_OPTER    ((uint32_t)FLASH_CTRL_OPTER)
#define FLASH_CTRL_RESET_OPTER  (~((uint32_t)FLASH_CTRL_OPTER))
#define FLASH_CTRL_SET_MMER     ((uint32_t)FLASH_CTRL_MMER)
#define FLASH_CTRL_RESET_MMER   (~((uint32_t)FLASH_CTRL_MMER))
#define FLASH_CTRL_SET_START    ((uint32_t)FLASH_CTRL_START)
#define FLASH_CTRL_SET_LOCK     ((uint32_t)FLASH_CTRL_LOCK)
#define FLASH_CTRL_SET_OPTWE    ((uint32_t)FLASH_CTRL_OPTWRE)

/** Option byte **/
#define FLASH_L1_RDP_KEY        ((uint32_t)0xFFFF00A5)  
#define FLASH_L2_RDP_KEY        ((uint32_t)0xFFFF33CC)
#define FLASH_OB_DATA0_MASK      (FLASH_OB_DATA0_MSK)
#define FLASH_OB_DATA1_MASK      (FLASH_OB_DATA1_MSK)


/** OB Register related definition**/
#define FLASH_USER_MASK    (FLASH_USER_USER)
#define FLASH_RDP1_MASK    (FLASH_RDP_RDP1)
#define FLASH_RDP2_MASK    ((uint32_t)0xFF00FFFF)
#define FLASH_FLAG_OBERR   (FLASH_OB_OBERR)


/** FLASH Mask **/
#define FLASH_RDPRTL1_MSK           ((uint32_t)FLASH_OB_RDPRT1)
#define FLASH_RDPRTL2_MSK           ((uint32_t)FLASH_OB_RDPRT2)
#define FLASH_OB_USER_MSK           ((uint32_t)FLASH_OB_USER)

/** Option_Bytes_RDPx **/
#define FLASH_OB_RDP1_ENABLE            ((uint8_t)0x00) /* Enable RDP1 */
#define FLASH_OB_RDP1_DISABLE           ((uint8_t)0xA5) /* DISABLE RDP1 */

#define FLASH_OB_RDP2_ENABLE            ((uint8_t)0xCC) /* Enable RDP2 */
#define FLASH_OB_RDP2_DISABLE           ((uint8_t)0x00) /* Disable RDP2 */

/** Option_Bytes_IWatchdog **/
#define FLASH_OB_IWDG_SW                ((uint16_t)0x0001) /* Software IWDG selected */
#define FLASH_OB_IWDG_HW                ((uint16_t)0x0000) /* Hardware IWDG selected */

/** Option_Bytes_nRST_STOP **/
#define FLASH_OB_STOP_NORST             ((uint16_t)0x0002) /* No reset generated when entering in STOP */
#define FLASH_OB_STOP_RST               ((uint16_t)0x0000) /* Reset generated when entering in STOP */

/** Option_Bytes_nRST_PD **/
#define FLASH_OB_PD_NORST               ((uint16_t)0x0004) /* No reset generated when entering in PD */
#define FLASH_OB_PD_RST                 ((uint16_t)0x0000) /* Reset generated when entering in PD */

/** Option_Bytes_LOCK_BOOT **/
#define FLASH_OB_LOCK_BOOT              ((uint16_t)0x0008) /* First 3KB FLASH LOCK*/
#define FLASH_OB_NOLOCK_BOOT            ((uint16_t)0x0000) /* First 3KB FLASH NOLOCK */

/** Option_Bytes_PA0_IO **/
#define FLASH_OB_PA0_IO                 ((uint16_t)0x0000) /* PA0 is general pin */
#define FLASH_OB_PA0_NRST               ((uint16_t)0x0010) /* PA0 is RESET pin */

/** Option_Bytes_USER_LVREN **/
#define FLASH_OB_USER_LVR_ENABLE        ((uint16_t)0x0000) /* LVR enable */
#define FLASH_OB_USER_LVR_DISABLE       ((uint16_t)0x0010) /* LVR disable */

/** Option_Bytes_USER_LVRFIL **/
#define FLASH_OB_USER_LVRFIL_ENABLE     ((uint16_t)0x0000) /* LVR filter enable */
#define FLASH_OB_USER_LVRFIL_DISABLE    ((uint16_t)0x0020) /* LVR filter disable */

/** Option_Bytes_USER_LVRFIL **/
#define FLASH_OB_USER_LVRRST_ENABLE     ((uint16_t)0x0000) /* LVR reset enable */
#define FLASH_OB_USER_LVRRST_DISABLE    ((uint16_t)0x0040) /* LVR reset disable */


/** FLASH USER Mask **/
#define FLASH_USER_LVRCNT_MSK           ((uint32_t)FLASH_USER_LVRCNT)
#define FLASH_USER_LVRLS_MSK            ((uint32_t)FLASH_USER_LVRLS)
#define FLASH_USER_POR_DELAY_MSK        ((uint32_t)FLASH_USER_POR_DELAY)


#define FLASH_USER_RDP_OFFSET           (REG_BIT16_OFFSET)
#define FLASH_OB_TO_USER_OFFSET         (REG_BIT14_OFFSET)
#define FLASH_OB_OFFSET                 (REG_BIT2_OFFSET)
#define FLASH_DATA0_DATA1_OFFSET        (REG_BIT16_OFFSET)
#define FLASH_USER2_USER3_OFFSET        (REG_BIT16_OFFSET)
#define FLASH_OB_DATA0_OFFSET           (REG_BIT10_OFFSET)
#define FLASH_OB_DATA1_OFFSET           (REG_BIT18_OFFSET)
#define FLASH_USER3_OFFSET              (REG_BIT8_OFFSET)
#define FLASH_USER4_OFFSET              (REG_BIT16_OFFSET)


/** Delay definition **/
#define ERASE_TIMEOUT   ((uint32_t)0x000B0000)
#define PROGRAM_TIMEOUT ((uint32_t)0x00002000)	 

#define FLASH_WORD_LENGTH   ((uint32_t)0x00000003)


/**  FLASH_Interrupts **/
#define FLASH_INT_ERROR    ((uint32_t)FLASH_CTRL_ERRITE) /* PGERR WRPERR ERROR error interrupt source */
#define FLASH_INT_EOP      ((uint32_t)FLASH_CTRL_EOPITE) /* End of FLASH Operation Interrupt source */

/** FLASH_Flags **/
#define FLASH_FLAG_BUSY     ((uint32_t)FLASH_STS_BUSY) /* FLASH Busy flag */
#define FLASH_FLAG_PGERR    ((uint32_t)FLASH_STS_PGERR) /* FLASH Program error flag */
#define FLASH_FLAG_WRPERR   ((uint32_t)FLASH_STS_WRPERR) /* FLASH Write protected error flag */
#define FLASH_FLAG_EOP      ((uint32_t)FLASH_STS_EOP) /* FLASH End of Operation flag */
#define FLASH_FLAG_SPERR    ((uint32_t)FLASH_STS_SPRERR)  /* FLASH Special Pattern Read error */
#define FLASH_FLAG_CFGERR   ((uint32_t)FLASH_STS_CFGERR)  /* FLASH Configuration registers error when checking */

/** FLASH_STS_CLRFLAG **/
#define FLASH_STS_CLRFLAG   (FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_EOP)


/** FLASH_VTOR_CLRFLAG **/
#define FLASH_VTOR_ENABLE   ((uint32_t)FLASH_VTOR_EN) /* FLASH Busy flag */
#define FLASH_VTOR_DISABLE  ((uint32_t)0x00000000) /* FLASH Busy flag */

/** Delay definition **/
#define ERASE_TIMEOUT   ((uint32_t)0x000B0000)
#define PROGRAM_TIMEOUT ((uint32_t)0x00002000)

#define FLASH_WORD_LENGTH   ((uint32_t)0x00000003)



/** Functions used for N32G003 devices **/
void FLASH_Latency_Set(uint32_t flash_latency);
uint8_t FLASH_Latency_Get(void);
void FLASH_Prefetch_Buffer_Enable(void);
void FLASH_Prefetch_Buffer_Disable(void);
void FLASH_Unlock(void);
void FLASH_Lock(void);
void Option_Bytes_Unlock(void);
void Option_Bytes_Lock(void);
FlagStatus Option_Bytes_Lock_Status_Get(void);
FLASH_STS FLASH_One_Page_Erase(uint32_t page_address);
FLASH_STS FLASH_Mass_Erase(void);
FLASH_STS FLASH_48Page_Mass_Erase(void);
FLASH_STS FLASH_Word_Program(uint32_t address, uint32_t data);
FLASH_STS FLASH_Option_Bytes_Erase(void);
FLASH_STS FLASH_Option_Bytes_User_RDP1_Program(uint8_t option_byte_rdp1, uint16_t option_byte_iwdg, \
                                               uint16_t option_byte_stop, uint16_t option_byte_PD, \
                                               uint16_t option_byte_lock_boot, uint16_t option_byte_PA0_NRST);
FLASH_STS FLASH_Option_Bytes_DATA_Program(uint8_t option_byte_data0,uint8_t option_byte_data1);
FLASH_STS FLASH_Option_Bytes_User2_User3_Program(uint8_t option_byte_user2,uint8_t option_byte_LVR_cnt, \
                                                uint8_t option_byte_LVR,uint8_t option_byte_LVR_filter,uint8_t option_byte_LVR_reset);
FLASH_STS FLASH_Option_Bytes_User4_RDP2_Program(uint8_t option_byte_rdp2,uint8_t option_byte_user4);
FLASH_STS FLASH_Read_Out_Protection_L1_Enable(void);
FLASH_STS FLASH_Read_Out_Protection_L1_Disable(void);
FLASH_STS FLASH_Read_Out_Protection_L2_Enable(void);
FlagStatus FLASH_Option_Bytes_User_Get(uint32_t option_byte_bit);
uint32_t FLASH_Option_Bytes_User2_Get(void);
uint32_t FLASH_User_LVRLS_Get(void);
FlagStatus FLASH_Option_Bytes_User3_Get(uint32_t option_byte_bit);
uint32_t FLASH_Option_Bytes_User4_Get(void);
FlagStatus FLASH_Read_Out_Protection_Status_Get(void);
FlagStatus FLASH_Read_Out_Protection_L2_Status_Get(void);
FlagStatus FLASH_Prefetch_Buffer_Status_Get(void);
void FLASH_Interrupt_Enable(uint32_t flash_interrupts);
void FLASH_Interrupt_Disable(uint32_t flash_interrupts);
FlagStatus FLASH_Flag_Status_Get(uint32_t flash_flag);
FlagStatus FLASH_Option_Bytes_Flag_Get(uint32_t flash_flag);
void FLASH_Flag_Status_Clear(uint32_t flash_flag);
FLASH_STS FLASH_Status_Get(void);
FLASH_STS FLASH_Last_Operation_Wait(uint32_t timeout);
void FLASH_VTOR_Address_Set(uint32_t VTOR_address,uint32_t VTOR_cmd);

#ifdef __cplusplus
}
#endif

#endif /* __N32G003_FLASH_H */

