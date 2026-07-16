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
*\*\file n32g003_comp.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
 
#include "n32g003_comp.h"
#include "n32g003_rcc.h"


/** COMP Driving Functions Declaration**/


/**
*\*\name    COMP_Reset.
*\*\fun     Reset the COMP registers.
*\*\return  none
**/
void COMP_Reset(void)
{
	RCC_Peripheral_Reset(RCC_RST_COMPRST);
}

/**
*\*\name    COMP_Initializes_Structure.
*\*\fun     Fills all COMP_initstruct member with default value.
*\*\param   COMP_initstruct :
*\*\          - Blking
*\*\          - Hyst
*\*\          - PolRev
*\*\          - OutSel
*\*\          - SubSel
*\*\          - InpSel
*\*\          - InmSel
*\*\          - FilterEn
*\*\          - ClkPsc
*\*\          - SampWindow
*\*\          - Threshold
*\*\          - En
*\*\return  none
**/
void COMP_Initializes_Structure(COMP_InitType* COMP_initstruct)
{
    /* Reset COMP init structure parameters values */ 
    /* Initialize the Blking */
    COMP_initstruct->Blking            = COMP_BLANKING_NO;
    /* Initialize the Hyst */
    COMP_initstruct->Hyst              = COMP_HYST_NO;
    /* Initialize the PolRev */
    COMP_initstruct->PolRev            = COMP_OUTPOL_NFLIP;
    /* Initialize the OutSel */
    COMP_initstruct->OutSel            = COMP_OUTSEL_RES;
	/* Initialize the SubSel */
    COMP_initstruct->SubSel            = COMP_SUB_NO;
    /* Initialize the InpSel */
    COMP_initstruct->InpSel            = COMP_INPSEL_RES;
    /* Initialize the InmSel */
    COMP_initstruct->InmSel            = COMP_INMSEL_RES;
    /* Initialize the FilterEn */
    COMP_initstruct->FilterEn          = DISABLE;
    /* Initialize the ClkPsc */
    COMP_initstruct->ClkPsc            = 0;
    /* Initialize the SampWindow */
    COMP_initstruct->SampWindow        = 0;
    /* Initialize the Threshold */
    COMP_initstruct->Threshold         = 0;
    /* Initialize the En */
    COMP_initstruct->En                = DISABLE;
}

/**
*\*\name    COMP_Filter_SampWindow_Config.
*\*\fun     Configures the COMP filter sampwindow with a correct value. 
*\*\param   sampwin;_value:
*\*\          - from 0 to 31.
*\*\return  none
**/
void COMP_Filter_SampWindow_Config(uint8_t sampwin_value)
{
	/* Clear COMP_FILC SAMPW[4:0] bits */
	COMP->FILC &= COMP_FILTER_SAMPW_MASK;
	/* Set COMP_FILC SAMPW[4:0] bits */
	COMP->FILC |= sampwin_value << REG_BIT6_OFFSET;
}

/**
*\*\name    COMP_Filter_Threshold_Config.
*\*\fun     Configures the COMP filter Threshold with a correct value. 
*\*\param   Threshold_value:
*\*\          - Value must be greater than SAMPW / 2 .
*\*\return  none
**/
void COMP_Filter_Threshold_Config(uint8_t Threshold_value)
{
	/* Clear COMP_FILC THRESH[4:0] bits */
	COMP->FILC &= COMP_FILTER_THRESHOLD_MASK;
	/* Set COMP_FILC THRESH[4:0] bits */
	COMP->FILC |= Threshold_value << REG_BIT1_OFFSET;
    
}

/**
*\*\name    COMP_Filter_Enable.
*\*\fun     Configures COMP filter enable.
*\*\          - Before the COMP filter is enabled, the clock of the COMP filter must be turned on.
*\*\          - Configure RCC_APB1PCLKEN. COMPFILTEN = 1 to enable COMP filter, the RCC driver function can configure this.
*\*\return  none
**/
void COMP_Filter_Enable(void)
{
	/* Set COMP_FILC FILTER bits */
	COMP->FILC |= COMP_FILTER_ENABLE;
}

/**
*\*\name    COMP_Filter_Disable.
*\*\fun     Configures COMP filter disable.
*\*\return  none
**/
void COMP_Filter_Disable(void)
{
	/* Clear COMP_FILC FILTER bits */
	COMP->FILC &= COMP_FILTER_DISABLE;
}

/**
*\*\name    COMP_Filter_Clock_Prescale_Config.
*\*\fun     Configures The COMP low filter prescale . 
*\*\param   clkpsc_value:
*\*\        Value can be set from 0 to 65535.
*\*\return  none
**/
void COMP_Filter_Clock_Prescale_Config(uint16_t clkpsc_value)
{
	/* Clear COMP_FILP CLKPSC[15:0] bits */
	COMP->FILP &= COMP_FILTER_CLKPSC_MASK;
	/* Set COMP_FILP CLKPSC[15:0] bits */
	COMP->FILP |= clkpsc_value;
}

/**
*\*\name    COMP_Blking_Soucre_Config.
*\*\fun     Configures TIM1 output signal to control COMP Blking. 
*\*\param   blking_mode :
*\*\          - COMP_BLANKING_NO
*\*\          - COMP_BLANKING_TIM1_OC5
*\*\return  none
**/
void COMP_Blking_Soucre_Config(uint32_t blking_mode)
{
	/* Clear COMP_CTRL BLKING bit */
	COMP->CTRL &= COMP_BLANKING_MASK;
	/* Set COMP_CTRL BLKING bit */
	COMP->CTRL |= blking_mode;
}

/**
*\*\name    COMP_Subtract_Level_Config.
*\*\fun     Configures COMP Subtract level.
*\*\param   sub_mode :
*\*\          - COMP_SUB_NO  
*\*\          - COMP_SUB_100mV    
*\*\          - COMP_SUB_200mV     
*\*\          - COMP_SUB_300mV      
*\*\return  none
**/
void COMP_Subtract_Level_Config(uint32_t sub_mode)
{
	/* Clear COMP_CTRL CMPVOS[1:0] bits */
	COMP->CTRL &= COMP_SUB_MASK;
	/* Set COMP_CTRL CMPVOS[1:0] bits */
	COMP->CTRL |= sub_mode;
}

/**
*\*\name    COMP_Hysteresis_Level_Config.
*\*\fun     Configures COMP hysteresis level.
*\*\param   hyst_mode :
*\*\          - COMP_HYST_NO   
*\*\          - COMP_HYST_LOW     5.1mV-level
*\*\          - COMP_HYST_MID     15mV-level
*\*\          - COMP_HYST_HIGH    25mV-level   
*\*\return  none
**/
void COMP_Hysteresis_Level_Config(uint32_t hyst_mode)
{
	/* Clear COMP_CTRL HYST[1:0] bits */
	COMP->CTRL &= COMP_HYST_MASK;
	/* Set COMP_CTRL HYST[1:0] bits */
	COMP->CTRL |= hyst_mode;
}

/**
*\*\name    COMP_Output_Polarity_Config.
*\*\fun     Configures COMP output signal polarity overturn or not.
*\*\param   output_pol :
*\*\          - COMP_OUTPOL_FLIP   
*\*\          - COMP_OUTPOL_NFLIP   
*\*\return  none
**/
void COMP_Output_Polarity_Config(uint32_t output_pol)
{
	/* Clear COMP_CTRL POL bits */
	COMP->CTRL &= COMP_OUTPOL_MASK;
	/* Set COMP_CTRL POL bits */
	COMP->CTRL |= output_pol;
}

/**
*\*\name    COMP_InpSel_Config.
*\*\fun     Configures COMP inpsel.
*\*\param   vpsel :
*\*\             comp inp sel 
*\*\            - COMP_INPSEL_PB0                   
*\*\            - COMP_INPSEL_PA5                                                           
*\*\return  none
**/
void COMP_InpSel_Config(uint32_t vpsel)
{
	/* Clear COMP_CTRL INPSEL bit */
	COMP->CTRL &= COMP_INPSEL_MASK;
	/* Set COMP_CTRL INPSEL bit */
	COMP->CTRL |= vpsel;
}

/**
*\*\name    COMP_InmSel_Config.
*\*\fun     Configures COMP inmsel.
*\*\param   vpsel :
*\*\             comp inm sel 
*\*\            - COMP_INMSEL_PB1                   
*\*\            - COMP_INMSEL_PA4                                                           
*\*\return  none
**/
void COMP_InmSel_Config(uint32_t vpsel)
{
	/* Clear COMP_CTRL INMSEL bit */
	COMP->CTRL &= COMP_INMSEL_MASK;
	/* Set COMP_CTRL INMSEL bit */
	COMP->CTRL |= vpsel;
}

/**
*\*\name    COMP_Output_Trigger_Config.
*\*\fun     Configures which Timer input must be connected with the comparator output.
*\*\param   outtrgsel :
*\*\             comp out trig 
*\*\            - COMP_OUTSEL_NO
*\*\            - COMP_OUTSEL_TIM1_BKIN                
*\*\            - COMP_OUTSEL_TIM1_IC1  
*\*\            - COMP_OUTSEL_TIM1_OCREFCLEAR        
*\*\            - COMP_OUTSEL_TIM3_IC1              
*\*\            - COMP_OUTSEL_TIM3_OCREFCLEAR                                 
*\*\return  none
**/
void COMP_Output_Trigger_Config(uint32_t outtrgsel)
{
	/* Clear COMP_CSR OUTSEL[3:0] bits */
	COMP->CTRL &= COMP_OUTSEL_MASK;
	/* Set COMP_CSR OUTSEL[3:0] bits */ 
	COMP->CTRL |= outtrgsel;
}

/**
*\*\name    COMP_ON.
*\*\fun     Enable COMP.
*\*\return  none
**/
void COMP_ON(void)
{
   /* Set the COMP_CTRL EN bit to wake up the COMP from power down mode */
     COMP->CTRL |= COMP_ENABLE;
}

/**
*\*\name    COMP_OFF.
*\*\fun     Disable COMP.
*\*\return  none
**/
void COMP_OFF(void)
{
   /* Clear the COMP_CTRL EN bit, The COMP go to power down mode */
     COMP->CTRL &= COMP_DISABLE;
}

/**
*\*\name    COMP_Lock_Config.
*\*\fun     Configures COMP will be Locked.
*\*\return  none
**/
void COMP_Lock_Config(void)
{
    COMP->LOCK |= COMP_LOCK ;
}

/**
*\*\name    COMP_Interrupt_Enable.
*\*\fun     Configures COMP interrupt enable.
*\*\param   IntEn :
*\*\          - COMP_INTEN
*\*\return  none
**/
void COMP_Interrupt_Enable(uint32_t inten)
{
    COMP->INTEN |= inten;
}

/**
*\*\name    COMP_Interrupt_Disable.
*\*\fun     Configures COMP interrupt disable.
*\*\param   IntEn :
*\*\          - COMP_INTEN
*\*\return  none
**/
void COMP_Interrupt_Disable(uint32_t inten)
{
    COMP->INTEN &= ~inten;
}

/**
*\*\name    COMP_Interrupt_Status_Get.
*\*\fun     Get COMP interrupt Status.
*\*\return 
*\*\          - COMP_INTSTS
**/
uint8_t COMP_Interrupt_Status_Get(void)
{
    return COMP->INTSTS;
}

/**
*\*\name    COMP_Interrupt_Status_Clear.
*\*\fun     Clear COMP interrupt flag.
*\*\return  none
**/
void COMP_Interrupt_Status_Clear(void)
{
    COMP->INTSTS &= COMP_INTS_CLEAR;
}

/**
*\*\name    COMP_Output_Status_Get.
*\*\fun     Get COMP Output Status.
*\*\return  FlagStatus:
*\*\          - SET
*\*\          - RESET
**/
FlagStatus COMP_Output_Status_Get(void)
{
    return (COMP->CTRL & COMP_OUT) ? SET : RESET;
}

/**
*\*\name    COMP_Filter_Control_Config.
*\*\fun     Configures the COMP filter control value.
*\*\param   sw :
*\*\          - ENABLE
*\*\          - DISABLE
*\*\param   threshnum :
*\*\          - Threshold Value need > SampWindow/2.      
*\*\param   sampwindow :
*\*\          -  Value can be set from 0 to 31.
*\*\return  none
**/
void COMP_Filter_Control_Config(uint32_t sw, uint8_t threshold , uint8_t sampwindow)
{
    COMP_Filter_SampWindow_Config(sampwindow);
    COMP_Filter_Threshold_Config(threshold);
    if(sw == ENABLE)
    {
        COMP_Filter_Enable();
    }
    else
    {
        COMP_Filter_Disable();
    }
}

/**
*\*\name    COMP_Initializes.
*\*\fun     Initializes the COMP according to COMP_initstruct.
*\*\param   COMP_initstruct :
*\*\          - Blking
*\*\            - COMP_BLANKING_NO
*\*\            - COMP_BLANKING_TIM1_OC5
*\*\          - Hyst
*\*\            - COMP_HYST_NO   
*\*\            - COMP_HYST_LOW    
*\*\            - COMP_HYST_MID    
*\*\            - COMP_HYST_HIGH   
*\*\          - PolRev
*\*\            - COMP_OUTPOL_FLIP             
*\*\            - COMP_OUTPOL_NFLIP              
*\*\          - OutSel  
*\*\             comp out trig 
*\*\            - COMP_OUTSEL_NO
*\*\            - COMP_OUTSEL_TIM1_BKIN                 
*\*\            - COMP_OUTSEL_TIM1_IC1   
*\*\            - COMP_OUTSEL_TIM1_OCREFCLEAR        
*\*\            - COMP_OUTSEL_TIM3_IC1              
*\*\            - COMP_OUTSEL_TIM3_OCREFCLEAR  
*\*\          - SubSel
*\*\            - COMP_SUB_NO             
*\*\            - COMP_SUB_100mV 
*\*\            - COMP_SUB_200mV  
*\*\            - COMP_SUB_300mV                              
*\*\          - InpSel                
*\*\             comp inp sel 
*\*\            - COMP_INPSEL_PB0                   
*\*\            - COMP_INPSEL_PA5                                                         
*\*\          - InmSel
*\*\             comp inm sel            
*\*\            - COMP_INMSEL_PB1                   
*\*\            - COMP_INMSEL_PA4                                             
*\*\          - En
*\*\            - ENABLE
*\*\            - DISABLE
*\*\          - SampWindow
*\*\            - SampWindow Value ranges from 0~31. 
*\*\          - Threshold
*\*\            - Threshold Value need > SampWindow/2.
*\*\          - FilterEn
*\*\            - ENABLE
*\*\            - DISABLE
*\*\          - ClkPsc
*\*\            - ClkPsc Value ranges from 0~36635. 
*\*\return  none
**/
void COMP_Initializes(COMP_InitType* COMP_initstruct)
{
    /** filter configures **/
    COMP_Filter_SampWindow_Config(COMP_initstruct->SampWindow);
    COMP_Filter_Threshold_Config( COMP_initstruct->Threshold);
    if(COMP_initstruct->FilterEn == ENABLE)
    {
        COMP_Filter_Enable();
    }   
    else
    {
        COMP_Filter_Disable();
    }
    /** filter clock Prescale configures **/
    COMP_Filter_Clock_Prescale_Config(COMP_initstruct->ClkPsc);
    
    /** ctrl configures**/
    COMP_Blking_Soucre_Config(COMP_initstruct->Blking);
    COMP_Hysteresis_Level_Config(COMP_initstruct->Hyst);
    COMP_Output_Polarity_Config(COMP_initstruct->PolRev);
	COMP_Subtract_Level_Config(COMP_initstruct->SubSel);
    COMP_InpSel_Config(COMP_initstruct->InpSel);
    COMP_InmSel_Config(COMP_initstruct->InmSel);
    COMP_Output_Trigger_Config(COMP_initstruct->OutSel);

    /** comp enable or disable configures*/
    if (COMP_initstruct->En == ENABLE)
        COMP_ON();
    else
        COMP_OFF();
}

