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
*\*\file n32g003_pwr.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#include "n32g003_pwr.h"
#include "n32g003_rcc.h"

/** PWR Private Defines **/

/** PWR Driving Functions Declaration **/

/**
*\*\name    PWR_Reset.
*\*\fun     Deinitializes the PWR peripheral registers to their default reset values.
*\*\param   none
*\*\return  none
**/
void PWR_Reset(void)
{
    RCC_Peripheral_Reset(RCC_RST_PWRRST);
}

/**
*\*\name    PWR_PVD_Enable.
*\*\fun     Enables the Power Voltage Detector(PVD).
*\*\param   none
*\*\return  none
**/
void PWR_PVD_Enable(void)
{
    PWR->CTRL |= PWR_PVD_ENABLE;
}

/**
*\*\name    PWR_PVD_Disable.
*\*\fun     Disables the Power Voltage Detector(PVD).
*\*\param   none
*\*\return  none
**/
void PWR_PVD_Disable(void)
{
    PWR->CTRL &= (~PWR_PVD_ENABLE);
}

/**
*\*\name    PWR_PVD_Interrupt_Enable.
*\*\fun     Enables the Power Voltage Detector Interrupt.
*\*\param   none
*\*\return  none
**/
void PWR_PVD_Interrupt_Enable(void)
{
    PWR->CTRL |= PWR_PVD_INT_ENABLE;
}

/**
*\*\name    PWR_PVD_Interrupt_Disable.
*\*\fun     Disables the Power Voltage Detector Interrupt.
*\*\param   none
*\*\return  none
**/
void PWR_PVD_Interrupt_Disable(void)
{
    PWR->CTRL &= (~PWR_PVD_INT_ENABLE);
}

/**
*\*\name    PWR_PVD_Filter_Enable.
*\*\fun     Enables the Power Voltage Detector Filter.
*\*\param   none
*\*\return  none
**/
void PWR_PVD_Filter_Enable(void)
{
    PWR->CTRL |= PWR_PVD_FIL_ENABLE;
}

/**
*\*\name    PWR_PVD_Filter_Disable.
*\*\fun     Disables the Power Voltage Detector Filter.
*\*\param   none
*\*\return  none
**/
void PWR_PVD_Filter_Disable(void)
{
    PWR->CTRL &= (~PWR_PVD_FIL_ENABLE);
}

/**
*\*\name    PWR_PVD_Level_Config.
*\*\fun     Configures the voltage threshold detected by the Power Voltage Detector(PVD).
*\*\param   level :
*\*\          - PWR_PVD_LEVEL_1V8
*\*\          - PWR_PVD_LEVEL_2V0
*\*\          - PWR_PVD_LEVEL_2V2
*\*\          - PWR_PVD_LEVEL_2V4
*\*\          - PWR_PVD_LEVEL_2V6
*\*\          - PWR_PVD_LEVEL_2V8
*\*\          - PWR_PVD_LEVEL_3V0
*\*\          - PWR_PVD_LEVEL_3V2
*\*\          - PWR_PVD_LEVEL_3V4
*\*\          - PWR_PVD_LEVEL_3V6
*\*\          - PWR_PVD_LEVEL_3V8
*\*\          - PWR_PVD_LEVEL_4V0
*\*\          - PWR_PVD_LEVEL_4V2
*\*\          - PWR_PVD_LEVEL_4V4
*\*\          - PWR_PVD_LEVEL_4V6
*\*\          - PWR_PVD_LEVEL_4V8
*\*\return  none
**/
void PWR_PVD_Level_Config(uint32_t level)
{
    uint32_t temp_value = 0;
    temp_value = PWR->CTRL;
    /* Clear PLS[8:5] bits bit */
    temp_value &= PWR_CTRL_PLS_MASK;
    /* Set PLS[8:5] bits according to level value */
    temp_value |= level;
    /* Store the new value */
    PWR->CTRL = temp_value;
}

/**
*\*\name    PWR_LVR_Enable.
*\*\fun     Enables the Low Voltage Detector(LVR).
*\*\param   none
*\*\return  none
**/
void PWR_LVR_Enable(void)
{
    PWR->CTRL2 |= PWR_LVR_ENABLE;
}

/**
*\*\name    PWR_LVR_Disable.
*\*\fun     Disables the Low Voltage Detector(LVR).
*\*\param   none
*\*\return  none
**/
void PWR_LVR_Disable(void)
{
    PWR->CTRL2 &= (~PWR_LVR_ENABLE);
}

/**
*\*\name    PWR_LVR_Reset_Enable.
*\*\fun     Enables the Low Voltage Detector reset.
*\*\param   none
*\*\return  none
**/
void PWR_LVR_Reset_Enable(void)
{
    PWR->CTRL2 |= PWR_LVR_RST_ENABLE;
}

/**
*\*\name    PWR_LVR_Reset_Disable.
*\*\fun     Disables the Low Voltage Detector reset.
*\*\param   none
*\*\return  none
**/
void PWR_LVR_Reset_Disable(void)
{
    PWR->CTRL2 &= (~PWR_LVR_RST_ENABLE);
}

/**
*\*\name    PWR_LVR_Filter_Enable.
*\*\fun     Enables the Low Voltage Detector Filter.
*\*\param   none
*\*\return  none
**/
void PWR_LVR_Filter_Enable(void)
{
    PWR->CTRL2 |= PWR_LVR_FIL_ENABLE;
}

/**
*\*\name    PWR_LVR_Filter_Disable.
*\*\fun     Disables the Low Voltage Detector Filter.
*\*\param   none
*\*\return  none
**/
void PWR_LVR_Filter_Disable(void)
{
    PWR->CTRL2 &= (~PWR_LVR_FIL_ENABLE);
}

/**
*\*\name    PWR_LVR_Level_Config.
*\*\fun     Configures the voltage threshold detected by the Low Voltage Detector(LVR).
*\*\param   level :
*\*\          - PWR_LVR_LEVEL_1V8
*\*\          - PWR_LVR_LEVEL_2V0
*\*\          - PWR_LVR_LEVEL_2V2
*\*\          - PWR_LVR_LEVEL_2V4
*\*\          - PWR_LVR_LEVEL_2V6
*\*\          - PWR_LVR_LEVEL_2V8
*\*\          - PWR_LVR_LEVEL_3V0
*\*\          - PWR_LVR_LEVEL_3V2
*\*\          - PWR_LVR_LEVEL_3V4
*\*\          - PWR_LVR_LEVEL_3V6
*\*\          - PWR_LVR_LEVEL_3V8
*\*\          - PWR_LVR_LEVEL_4V0
*\*\          - PWR_LVR_LEVEL_4V2
*\*\          - PWR_LVR_LEVEL_4V4
*\*\          - PWR_LVR_LEVEL_4V6
*\*\          - PWR_LVR_LEVEL_4V8
*\*\return  none
**/
void PWR_LVR_Level_Config(uint32_t level)
{
    uint32_t temp_value = 0;
    temp_value = PWR->CTRL2;
    /* Clear LVRS[14:11] bits*/
    temp_value &= PWR_CTRL2_LVRLS_MASK;
    /* Set LVRS[14:11] bits according to level value */
    temp_value |= level;
    /* Store the new value */
    PWR->CTRL2 = temp_value;
}

/**
*\*\name    PWR_Wakeup_Pin_Enable.
*\*\fun     Enables the Wakeup Pin functionality.
*\*\param   pin :
*\*\          - WAKEUP_PIN1 PA1
*\*\          - WAKEUP_PIN2 PA2
*\*\param   polarity :
*\*\          - PWR_PIN_RISING
*\*\          - PWR_PIN_FALLING
*\*\return  none
**/
void PWR_Wakeup_Pin_Enable(WAKEUP_PINX pin, uint32_t polarity)
{
    uint32_t temp_value = 0;
    temp_value = PWR->CTRLSTS;
    /* Clear WKUPPOL bit*/
    temp_value &= (PWR_CTRLSTS_WKUPPOL_MASK);
    /* Clear WKUP1EN or WKUP0EN bit*/
    temp_value &= (~(uint32_t)pin);
    /* Set wakeup polarity value */
    temp_value |= (polarity << PWR_BIT_SHIFT_10);
    /* Set wakeup value */
    temp_value |= pin;
    /* Store the new value */
    PWR->CTRLSTS = temp_value;    
}

/**
*\*\name    PWR_Wakeup_Pin_Disable.
*\*\fun     Disables the WakeUp Pin functionality.
*\*\param   pin :
*\*\          - WAKEUP_PIN1 PA1
*\*\          - WAKEUP_PIN2 PA2
*\*\return  none
**/
void PWR_Wakeup_Pin_Disable(WAKEUP_PINX pin)
{
    uint32_t temp_value = 0;
    temp_value = PWR->CTRLSTS;
    /* Set wakeup value */
    temp_value &= (~(uint32_t)pin);
    /* Store the new value */
    PWR->CTRLSTS = temp_value;    
}

/**
*\*\name    PWR_STOP_Mode_Enter.
*\*\fun     Enters STOP mode.
*\*\param   enter_mode :
*\*\          - PWR_STOP_ENTRY_WFI (enter STOP mode with WFI instruction)
*\*\          - PWR_STOP_ENTRY_WFE (enter STOP mode with WFE instruction)
*\*\return  none
**/
void PWR_STOP_Mode_Enter(uint8_t enter_mode)
{
    uint32_t temp_value = 0;
    temp_value = PWR->CTRL;
    /* Clear PDSTP bits */
    temp_value &= PWR_CTRL_PDSTP_MASK;
    /* Store the new value */
    PWR->CTRL = temp_value;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    /* Select STOP mode entry */
    if (enter_mode == PWR_STOP_ENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);
}

/**
*\*\name    PWR_PD_Mode_Enter.
*\*\fun     Enters PD mode.
*\*\param   enter_mode :
*\*\          - PWR_PD_ENTRY_WFI (enter PD mode with WFI instruction)
*\*\          - PWR_PD_ENTRY_WFE (enter PD mode with WFE instruction)
*\*\return  none
**/
void PWR_PD_Mode_Enter(uint8_t enter_mode)
{
    /* Clear Wake-up flag */
    PWR->CTRL |= PWR_CLEAR_WKUPF;
    /* Clear PDS bits */
    PWR->CTRL &= PWR_CTRL_PDSTP_MASK;
    /* Select PD mode */
    PWR->CTRL |= PWR_PD_MODE;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    /* This option is used to ensure that store operations are completed */
#if defined(__CC_ARM)
    __force_stores();
#endif
    /* Select PD mode entry */
    if (enter_mode == PWR_PD_ENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
*\*\name    PWR_HSI_40M_Trim_Select.
*\*\fun     PWR control selects the HSI frequency as 40M.
*\*\param   none
*\*\return  none
**/
void PWR_HSI_40M_Trim_Select(void)
{
    /* Enable the HSI is controlled by the PWR */
    PWR->CTRL3 |= PWR_CONTROL_HSI;
    /* Select the HSI frequency as 40M */
    PWR->CTRL3 |= PWR_HSI_TRIM_40M;
}

/**
*\*\name    PWR_PDR_Trigger_Level_Config.
*\*\fun     VDDD PDR trigger level configuration in STOP mode.
*\*\param   level :
*\*\          - PWR_PDR_LEVEL_0V8
*\*\          - PWR_PDR_LEVEL_1V0
*\*\          - PWR_PDR_LEVEL_1V2
*\*\return  none
**/
void PWR_PDR_Trigger_Level_Config(uint32_t level)
{
    uint32_t temp_value = 0;
    /* Enable PDR select signal is controlled by the PWR */
    PWR->CTRL3 |= PWR_CONTROL_PDR_SEL;
    
    temp_value = PWR->CTRL;
    /* Clear PDRS[10:9] bits*/
    temp_value &= PWR_CTRL_PDRS_MASK;
    /* Set PDRS[10:9] bits according to level value */
    temp_value |= level;
    /* Store the new value */
    PWR->CTRL = temp_value;
}

/**
*\*\name    PWR_VDDD_Output_Config.
*\*\fun     After the chip enters STOP mode, configure the output voltage of VDDD.
*\*\param   voltage_output :
*\*\          - PWR_VDDD_OUTPUT_1V5
*\*\          - PWR_VDDD_OUTPUT_1V0
*\*\          - PWR_VDDD_OUTPUT_1V2
*\*\return  none
*\*\note    When configuring PWR_VDDD_OUTPUT_1V0 and PWR_VDDD_OUTPUT_1V2, you must
*\*\        first call the PWR_PDR_Trigger_Level_Config() function with the 
*\*\        parameter PWR_PDR_LEVEL_0V8.
**/
void PWR_VDDD_Output_Config(uint32_t voltage_output)
{
    uint32_t temp_value = 0;
    /* Enable VDDD output voltage select */
    PWR->CTRL6 |= PWR_VDDD_OUTPUT_SEL_EN;
    
    temp_value = PWR->CTRL5;
    /* Clear STPMRSEL[3:2] bits*/
    temp_value &= PWR_CTRL5_STPMRSEL_MASK;
    /* Set STPMRSEL[3:2] bits according to level value */
    temp_value |= voltage_output;
    /* Store the new value */
    PWR->CTRL5 = temp_value;    
}


/**
*\*\name    PWR_CTRL2_write_Protection_Enable.
*\*\fun     Enable the PWR_CTRL2 registers write protection.
*\*\param   none
*\*\return  none
**/
void PWR_CTRL2_write_Protection_Enable(void)
{
    /* Enable the write protection for PWR_CTRL2 registers */
    PWR->CTRL2 = PWR_CTRL2_KEYS;
}

/**
*\*\name    PWR_CTRL4_write_Protection_Enable.
*\*\fun     Enable the PWR_CTRL4 registers write protection.
*\*\param   none
*\*\return  none
**/
void PWR_CTRL4_write_Protection_Enable(void)
{
    /* Enable the write protection for PWR_CTRL4 registers */
    PWR->CTRL4 = PWR_CTRL4_KEYS;
}

/**
*\*\name    PWR_Flag_Status_Get.
*\*\fun     Checks whether the specified PWR flag is set or not.
*\*\param   flag :
*\*\          - PWR_WKUP_FLAG
*\*\          - PWR_DBGPD_FLAG
*\*\          - PWR_PVD_OUTPUT_FLAG
*\*\          - PWR_LVR_OUTPUT_FLAG
*\*\return  SET or RESET.
**/
FlagStatus PWR_Flag_Status_Get(uint32_t flag)
{
    if (flag == PWR_LVR_OUTPUT_FLAG)
    {
        /* Check the status of the PWR flag */
        if ((PWR->CTRL2 & flag) == (uint32_t)RESET)
        {
            /* PWR Flag is reset */
            return RESET;
        }
        else
        {
            /* PWR Flag is set */
            return SET;
        }
    }
    else
    {
        /* Check the status of the PWR flag */
        if ((PWR->CTRLSTS & flag) == (uint32_t)RESET)
        {
            /* PWR Flag is reset */
            return RESET;
        }
        else
        {
            /* PWR Flag is set */
            return SET;
        }
    }
}

/**
*\*\name    PWR_Flag_Status_Clear.
*\*\fun     Clears the PWR's pending flags.
*\*\param   flag :
*\*\          - PWR_WKUP_FLAG
*\*\          - PWR_DBGPD_FLAG
*\*\return  none
**/
void PWR_Flag_Status_Clear(uint32_t flag)
{
    if(flag == PWR_DBGPD_FLAG)
    {
        PWR->CTRL |= (flag << PWR_BIT_SHIFT_2);
    }
    else
    {
        PWR->CTRL |= (flag << PWR_BIT_SHIFT_2);
    }
}

/**
*\*\name    DBG_Peripheral_ON.
*\*\fun     Configures the specified peripheral run when the MCU under Debug mode.
*\*\param   DBG_Periph :
*\*\          - PWR_DBG_STOP    Keep debugger connection during STOP mode
*\*\          - PWR_DBG_PD      Keep debugger connection during PD mode
*\*\          - PWR_DBG_IWDG    IWDG stopped when Core is halted
*\*\          - PWR_DBG_TIM1    TIM1 counter stopped when Core is halted
*\*\          - PWR_DBG_TIM3    TIM3 counter stopped when Core is halted
*\*\          - PWR_DBG_TIM6    TIM6 counter stopped when Core is halted
*\*\return  none
**/
void DBG_Peripheral_ON(uint32_t DBG_Periph)
{
    PWR->DBG_CTRL |= DBG_Periph;
}

/**
*\*\name    DBG_Peripheral_OFF.
*\*\fun     Configures the specified peripheral run or stop when the MCU under Debug mode.
*\*\param   DBG_Periph :
*\*\          - PWR_DBG_STOP    Keep debugger connection during STOP mode
*\*\          - PWR_DBG_PD      Keep debugger connection during PD mode
*\*\          - PWR_DBG_IWDG    IWDG stopped when Core is halted
*\*\          - PWR_DBG_TIM1    TIM1 counter stopped when Core is halted
*\*\          - PWR_DBG_TIM3    TIM3 counter stopped when Core is halted
*\*\          - PWR_DBG_TIM6    TIM6 counter stopped when Core is halted
*\*\return  none
**/
void DBG_Peripheral_OFF(uint32_t DBG_Periph)
{
    PWR->DBG_CTRL &= (~DBG_Periph);
}

