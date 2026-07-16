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
*\*\file n32g003_it.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "n32g003_it.h"
#include "main.h"

__IO uint32_t step = 1;

/** Cortex-M0 Processor Exceptions Handlers **/

/**
*\*\name    NMI_Handler.
*\*\fun     This function handles NMI exception.
*\*\param   none
*\*\return  none 
**/
void NMI_Handler(void)
{
}

/**
*\*\name    HardFault_Handler.
*\*\fun     This function handles Hard Fault exception.
*\*\param   none
*\*\return  none 
**/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
*\*\name    SVC_Handler.
*\*\fun     This function handles SVC exception.
*\*\param   none
*\*\return  none 
**/
void SVC_Handler(void)
{
}

/**
*\*\name    PendSV_Handler.
*\*\fun     This function handles PendSV exception.
*\*\param   none
*\*\return  none 
**/
void PendSV_Handler(void)
{
}

/**
*\*\name    SysTick_Handler.
*\*\fun     This function handles SysTick Handler.
*\*\param   none
*\*\return  none 
**/
void SysTick_Handler(void)
{
    /* Generate TIM1 COM event by software */
    TIM_Event_Generate(TIM1, TIM_EVT_SRC_COM);
}

/* N32G003 Peripherals Interrupt Handlers, interrupt handler's name please refer to the startup file (startup_n32g003.s). */


/**
*\*\name    TIM1_BRK_UP_TRG_COM_IRQHandler.
*\*\fun     This function handles TIM1 Break Update Trigger and commutation interrupts requests.
*\*\param   none
*\*\return  none 
**/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* Clear TIM1 COM pending bit */
    TIM_Interrupt_Status_Clear(TIM1, TIM_INT_COM);

    if (step == 1)
    {
        /* Next step: Step 2 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_3);

        /*  Channel1 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_Enable(TIM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_1);

        /*  Channel2 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_N_Enable(TIM1, TIM_CH_2);
        step++;
    }
    else if (step == 2)
    {
        /* Next step: Step 3 Configuration ---------------------------- */
        /*  Channel2 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_N_Enable(TIM1, TIM_CH_2);

        /*  Channel3 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_Enable(TIM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_3);

        /*  Channel1 configuration */
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_1);
        step++;
    }
    else if (step == 3)
    {
        /* Next step: Step 4 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_Enable(TIM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_3);

        /*  Channel2 configuration */
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_2);

        /*  Channel1 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_N_Enable(TIM1, TIM_CH_1);
        step++;
    }
    else if (step == 4)
    {
        /* Next step: Step 5 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_3);

        /*  Channel1 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_N_Enable(TIM1, TIM_CH_1);

        /*  Channel2 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_Enable(TIM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_2);
        step++;
    }
    else if (step == 5)
    {
        /* Next step: Step 6 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_N_Enable(TIM1, TIM_CH_3);

        /*  Channel1 configuration */
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_1);

        /*  Channel2 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_Enable(TIM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_2);
        step++;
    }
    else
    {
        /* Next step: Step 1 Configuration ---------------------------- */
        /*  Channel1 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_Enable(TIM1, TIM_CH_1);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_2);

        /*  Channel3 configuration */
        TIM_Output_Channel_Mode_Set(TIM1, TIM_OCMODE_PWM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_3);
        TIM_Capture_Compare_Ch_N_Enable(TIM1, TIM_CH_3);

        /*  Channel2 configuration */
        TIM_Capture_Compare_Ch_Disable(TIM1, TIM_CH_2);
        TIM_Capture_Compare_Ch_N_Disable(TIM1, TIM_CH_2);
        step = 1;
    }
}
