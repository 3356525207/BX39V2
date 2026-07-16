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
*\*\file main.c
*\*\author Nations 
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "main.h"
#include "bsp_delay.h"


TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
TIM_BDTRInitType TIM_BDTRInitStructure;
uint16_t TimerPeriod   = 0;
uint16_t Channel1Pulse = 0;

#define COMPX_FILC_SAMPW_30       	30
#define COMPX_FILC_THRESH_16		16

void RCC_Configuration(void);
void GPIO_Configuration(void);
void COMP_Configuratoin(void);
void ChangeVmVp(void);
void GPIO_TimConfiguration(void);
void TIM_Intial(TIM_Module* TIMx);

/**
*\*\name    main.
*\*\fun     main program.
*\*\return  none
**/
int main(void)
{
	/* cannot be commented out -------------------------------------------------*/
	SysTick_Delay_Ms(5*1000);
	
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();
	GPIO_TimConfiguration();
	
	/* tim pwm configuration ------------------------------------------------------*/
    TIM_Intial(TIM1);

    /* COMP configuration ------------------------------------------------------*/
    COMP_Configuratoin();
    while (1)
    {
        ChangeVmVp();
    }
}


/**
*\*\name    ChangeVmVp.
*\*\fun     Self Generate Puls ,by skip line connect to vp and vm if need.
*\*\return  none
**/
void ChangeVmVp(void)
{
    GPIO_Pins_Set(GPIOA,GPIO_PIN_12);
	GPIO_Pins_Reset(GPIOA,GPIO_PIN_13);
    {
        uint32_t i = 0;
        while (i++ < 1000)
            ;
    }
	GPIO_Pins_Reset(GPIOA,GPIO_PIN_12);
	GPIO_Pins_Set(GPIOA,GPIO_PIN_13);
    {
        uint32_t i = 0;
        while (i++ < 1000)
            ;
    }
}

/**
*\*\name    COMP_Configuratoin.
*\*\fun     Configures the comp module.
*\*\return  none
**/
void COMP_Configuratoin(void)
{
    COMP_InitType COMP_Initial;

    /*Initial comp*/
    COMP_Initializes_Structure(&COMP_Initial);
    COMP_Initial.InpSel     = COMP_INPSEL_PB0;
    COMP_Initial.InmSel     = COMP_INMSEL_PB1;
	COMP_Initial.SubSel     = COMP_SUB_NO;
	
	COMP_Initial.FilterEn	= ENABLE;
    COMP_Initial.SampWindow = COMPX_FILC_SAMPW_30;
    COMP_Initial.Threshold  = COMPX_FILC_THRESH_16;
	COMP_Initial.En			= DISABLE;
    COMP_Initializes( &COMP_Initial);
	
	/*trig initial as tim1 break*/
    COMP_Output_Trigger_Config( COMP_OUTSEL_TIM1_BKIN);
	/*Enable comp*/
    COMP_ON();
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
	/* Enable GPIOA,COMPE clocks */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_COMP | RCC_APB_PERIPH_COMP_FILT );

    /* Enable GPIOA, GPIOB, AFIO,TIM1 */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA | RCC_APB_PERIPH_IOPB | RCC_APB_PERIPH_AFIO | RCC_APB_PERIPH_TIM1);
}

/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\return  none
**/
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    // INP PB0
    GPIO_InitStructure.Pin            = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_ANALOG;
    GPIO_InitStructure.GPIO_Current   = GPIO_LOW_DREIVE;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    // INM PB1
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);

    // OutSel
    // PA8
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin            = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_COMP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    // GPIO PA12 PA13
    GPIO_InitStructure.Pin            = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Current   = GPIO_LOW_DREIVE;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name    GPIO_TimConfiguration.
*\*\fun     Configure the TIM1 Pins.
*\*\return  none
**/
void GPIO_TimConfiguration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Tim1 GPIOA Configuration: Channel 1 as alternate function push-pull */
    GPIO_InitStructure.Pin        = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current = GPIO_LOW_DREIVE;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    /* GPIOA Configuration: Channel 1N as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name    TIM_Intial.
*\*\fun     Configures timx program.
*\*\param   TIM_Module: timx to be initial.
*\*\return  none
**/
void TIM_Intial(TIM_Module* TIMx)
{
    /* TIMx Configuration ---------------------------------------------------

    The channel 1 and channel 1N duty cycle is set to 50%
    The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
    ----------------------------------------------------------------------- */
    /* Compute the value to be set in AR regiter to generate signal frequency at 960 Khz */
    TimerPeriod = 50;
    /* Compute CCDAT1 value to generate a duty cycle at 50% for channel 1 and 1N */
    Channel1Pulse = (uint16_t)(((uint32_t)5 * (TimerPeriod - 1)) / 10);

	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    /* Time Base configuration */
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = TimerPeriod;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.RepetCnt  = 0;

    TIM_Base_Initialize(TIMx, &TIM_TimeBaseStructure);

    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = Channel1Pulse;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_HIGH;
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_HIGH;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.OcNIdleState = TIM_OCN_IDLE_STATE_RESET;
    TIM_Output_Channel1_Initialize(TIMx, &TIM_OCInitStructure);

    TIM_Break_And_Dead_Time_Struct_Initialize(&TIM_BDTRInitStructure);
    /* TIM1 break */
    TIM_BDTRInitStructure.OssrState       = TIM_OSSR_STATE_ENABLE;
    TIM_BDTRInitStructure.OssiState       = TIM_OSSI_STATE_ENABLE;
    TIM_BDTRInitStructure.LockLevel       = TIM_LOCK_LEVEL_OFF;
    TIM_BDTRInitStructure.DeadTime        = 1;
    TIM_BDTRInitStructure.Break           = TIM_BREAK_IN_ENABLE;
    TIM_BDTRInitStructure.BreakPolarity   = TIM_BREAK_POLARITY_HIGH;
    TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTO_OUTPUT_ENABLE;
    TIM_BDTRInitStructure.IomBreakEn      = false;
    TIM_BDTRInitStructure.LockUpBreakEn   = false;
    TIM_BDTRInitStructure.PvdBreakEn      = false;
    TIM_Break_And_Dead_Time_Set(TIMx, &TIM_BDTRInitStructure);

    /* TIM1 counter enable */
    TIM_On(TIMx);

    /* TIM1 Main Output Enable */
    TIM_PWM_Output_Enable(TIMx);
}


