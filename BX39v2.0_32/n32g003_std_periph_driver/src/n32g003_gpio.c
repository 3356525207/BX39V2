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
*\*\file n32g003_gpio.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "n32g003_gpio.h"


/**
 *\*\name   GPIO_Reset.
 *\*\fun    Reset the GPIOx peripheral registers to their default reset values.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\return none
 */
void GPIO_Reset(GPIO_Module* GPIOx)
{
    if (GPIOx == GPIOA)
    {
        RCC_Peripheral_Reset(RCC_RST_GPIOARST);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_Peripheral_Reset(RCC_RST_GPIOBRST);
    }
    else
    {
        return;
    }
}

/**
 *\*\name   GPIO_Alternate_Function_Reset.
 *\*\fun    Reset the Alternate Function registers to their default reset values.
 *\*\param  none
 *\*\return none
 */
void GPIO_Alternate_Function_Reset(void)
{
    RCC_Peripheral_Reset(RCC_RST_AFIORST);
}


/**
 *\*\name   GPIOA_Pin_Reset.
 *\*\fun    Reset the GPIOA peripheral registers to it's default reset values.
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\          - GPIO_PIN_ALL
 *\*\return none
 */
void GPIOA_Pin_Reset(uint16_t pin)
{
    uint32_t position = 0x00U;
    uint32_t current_pin = 0x00U;

    while((pin >> position) != 0)
    {
        /* Get the IO position */
        current_pin = (pin) & (IO_POSITION_MASK << position);

        if(current_pin)
        {
            if(position == GPIOA_POSITION1 || position == GPIOA_POSITION2)
            {
                /* Configure IO Direction in alternate Mode */
                GPIOA->PMODE |= (GPIO_AF_MODE << (position * MULTIPLIER_FACTOR_2));
            }
            else
            {
                /* Configure IO Direction in analog Mode */
                GPIOA->PMODE |= (GPIO_ANALOG_MODE << (position * MULTIPLIER_FACTOR_2));
            }

            /* Configure the default Alternate Function in current IO */
            if(position & AF_SELECTION_MASK)
            {
				if(position == GPIOA_POSITION1 || position == GPIOA_POSITION2)
				{
					GPIOA->AFH &= ~(GPIO_AF_MASK << ((position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
				}
				else
				{
					GPIOA->AFH |= (GPIO_AF_MASK << ((position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
				}
            }
            else
            {
                GPIOA->AFL |= (GPIO_AF_MASK << ((position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
            }

            /* Reset GPIO output type to push-pull output */
            GPIOA->POTYPE &= ~(GPIO_POTYPE_MASK << position);

            
            if(position == GPIOA_POSITION1)
            {
                /* Reset the GPIO pull mode to pull-up */
                GPIOA->PUPD |= (GPIO_PU_MODE << (position * MULTIPLIER_FACTOR_2));  
            }
            else if(position == GPIOA_POSITION2)
            {
                /* Reset the GPIO pull mode to pull-down */
                GPIOA->PUPD |= (GPIO_PD_MODE << (position * MULTIPLIER_FACTOR_2));  
            }
            else
            {
                /* Reset the GPIO pull mode to no-pull */
                GPIOA->PUPD &= ~(GPIO_PUPD_MASK << (position * MULTIPLIER_FACTOR_2));  
            }

            /* Reset the GPIO slew rate to slow */
            GPIOA->SR |= (GPIO_SR_SLOW << position);  

            /* Reset the GPIO high driver capacity */
            GPIOA->DS &= ~(GPIO_LOW_DRIVER << position);  
        }
        position++;
    }
}

/**
 *\*\name   GPIOB_Pin_Reset.
 *\*\fun    Reset the GPIOB peripheral registers to it's default reset values.
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\return none
 */
void GPIOB_Pin_Reset(uint16_t pin)
{
    uint32_t position = 0x00U;
    uint32_t current_pin = 0x00U;

    while((pin >> position) != 0)
    {
        /* Get the IO position */
        current_pin = (pin) & (IO_POSITION_MASK << position);

        if(current_pin)
        {
            /** Reset GPIO pin mode **/

            /* Configure IO Direction in analog Mode */
            GPIOB->PMODE |= (GPIO_ANALOG_MODE << (position * MULTIPLIER_FACTOR_2));

            /** Configure the default Alternate Function in current IO **/
            GPIOB->AFL |= (GPIO_AF_MASK << ((position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
            
            /* Reset GPIO output type to push-pull output */
            GPIOB->POTYPE &= ~(GPIO_POTYPE_MASK << position);

            /* Reset the GPIO pull mode to no-pull */
            GPIOB->PUPD &= ~(GPIO_PUPD_MASK << (position * MULTIPLIER_FACTOR_2));  

            /* Reset the GPIO slew rate to slow */
            GPIOB->SR |= (GPIO_SR_SLOW << position);  

            /* Reset the GPIO high driver capacity */
			GPIOB->DS &= ~(GPIO_LOW_DRIVER << position);  
        }
        position++;
    }
}


/**
 *\*\name   GPIO_Alternate_Set.
 *\*\fun    Configure GPIO alternate function.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  alternate
 *\*\          - Refer to the "alternate_function" parameter 
 *\*\            list of the GPIO_Pin_Remap_Set() function
 *\*\param  position
 *\*\          - 0x00 ~ 0x0F    0x00 represents GPIO_PIN_0, 
 *\*\                           0x01 represents GPIO_PIN_1
 *\*\                           ...
 *\*\                           0x0F represents GPIO_PIN_15
 *\*\return none
 */
void GPIO_Alternate_Set(GPIO_Module* GPIOx, uint32_t alternate, uint32_t position)
{
    uint32_t temp_value = 0x00U;
    
    /* Configure Alternate function mapped with the current IO */ 
    if(position & AF_SELECTION_MASK)
    {
        temp_value = GPIOx->AFH;
        temp_value &= ~(GPIO_AF_MASK << ((uint32_t)(position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
        temp_value |= ((uint32_t)alternate << ((uint32_t)(position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
        GPIOx->AFH = temp_value;
    }
    else
    {
        temp_value = GPIOx->AFL;
        temp_value &= ~(GPIO_AF_MASK << ((uint32_t)(position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
        temp_value |= ((uint32_t)alternate << ((uint32_t)(position & (uint32_t)AF_WRITE_POSITION_MASK) * MULTIPLIER_FACTOR_4));
        GPIOx->AFL = temp_value;
    }
}


/**
 *\*\name   GPIO_Mode_Set.
 *\*\fun    Configure GPIO mode.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  mode 
 *\*\          - GPIO_MODE_INPUT
 *\*\          - GPIO_MODE_OUT_PP
 *\*\          - GPIO_MODE_OUT_OD
 *\*\          - GPIO_MODE_AF_PP
 *\*\          - GPIO_MODE_AF_OD
 *\*\          - GPIO_MODE_ANALOG
 *\*\param  position
 *\*\          - 0x00 ~ 0x0F    0x00 represents GPIO_PIN_0, 
 *\*\                           0x01 represents GPIO_PIN_1
 *\*\                           ...
 *\*\                           0x0F represents GPIO_PIN_15
 *\*\return none
 */
void GPIO_Mode_Set(GPIO_Module* GPIOx, uint32_t mode, uint32_t position)
{
    uint32_t temp_value = 0x00U;
    uint32_t temp = 0x00U;

    /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
    temp_value = GPIOx->PMODE;
    temp = (mode & GPIO_SET_PMODE_MASK);
    temp_value &= ~(GPIO_PMODE_MASK << (position * MULTIPLIER_FACTOR_2));
    temp_value |= (temp << (position * MULTIPLIER_FACTOR_2));
    GPIOx->PMODE = temp_value;

    /* In case of Output or Alternate function mode selection */
    if((mode == GPIO_MODE_OUT_PP) || (mode == GPIO_MODE_OUT_OD) ||
       (mode == GPIO_MODE_AF_PP) || (mode == GPIO_MODE_AF_OD))
    {
        /* Configure the IO Output Type */
        temp_value = GPIOx->POTYPE;
        temp_value &= ~(GPIO_POTYPE_MASK << position);
        temp_value |= (((mode & GPIO_OUTPUT_TYPE_MASK) >> SHIFT_POTYPE) << position);
        GPIOx->POTYPE = temp_value;
    }
}


/**
 *\*\name   GPIO_Pull_Set.
 *\*\fun    Configure pull mode.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pull 
 *\*\          - GPIO_NO_PULL
 *\*\          - GPIO_PULL_UP
 *\*\          - GPIO_PULL_DOWN
 *\*\param  position
 *\*\          - 0x00 ~ 0x0F    0x00 represents GPIO_PIN_0, 
 *\*\                           0x01 represents GPIO_PIN_1
 *\*\                           ...
 *\*\                           0x0F represents GPIO_PIN_15
 *\*\return none
 */
void GPIO_Pull_Set(GPIO_Module* GPIOx, uint32_t pull, uint32_t position)
{
    uint32_t temp_value = 0x00U;

    /* Configure pull-down mode */
    temp_value = GPIOx->PUPD;
    temp_value &= ~(GPIO_PUPD_MASK << (position * MULTIPLIER_FACTOR_2));
    temp_value |= (pull << (position * MULTIPLIER_FACTOR_2));
    GPIOx->PUPD = temp_value;
}


/**
 *\*\name   GPIO_SlewRate_Set.
 *\*\fun    GPIO slew rate configuration.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  slew_rate
 *\*\          - GPIO_SLEW_RATE_FAST
 *\*\          - GPIO_SLEW_RATE_SLOW
 *\*\param  position
 *\*\          - 0x00 ~ 0x0F    0x00 represents GPIO_PIN_0, 
 *\*\                           0x01 represents GPIO_PIN_1
 *\*\                           ...
 *\*\                           0x0F represents GPIO_PIN_15
 *\*\return none
 */
void GPIO_SlewRate_Set(GPIO_Module* GPIOx, uint32_t slew_rate, uint32_t position)
{
    uint32_t temp_value = 0x00U;

    /* Configure slew rate */
    temp_value = GPIOx->SR;
    temp_value &= ~(GPIO_SR_MASK << position);
    temp_value |= (slew_rate << position);
    GPIOx->SR = temp_value;
}


/**
 *\*\name   GPIO_Driver_Set.
 *\*\fun    GPIO driver configuration.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  current 
 *\*\          - GPIO_HIGH_DREIVE
 *\*\          - GPIO_LOW_DREIVE
 *\*\param  position
 *\*\          - 0x00 ~ 0x0F    0x00 represents GPIO_PIN_0, 
 *\*\                           0x01 represents GPIO_PIN_1
 *\*\                           ...
 *\*\                           0x0F represents GPIO_PIN_15
 *\*\return none
 */ 
void GPIO_Driver_Set(GPIO_Module* GPIOx, uint32_t current, uint32_t position)
{
    uint32_t temp_value = 0x00U;
    
    temp_value = GPIOx->DS;
    temp_value &= ~(GPIO_DS_MASK << position);
    temp_value |= (current << position);
    GPIOx->DS = temp_value;
}


/**
 *\*\name   GPIO_Peripheral_Initialization.
 *\*\fun    Initialize the GPIOx peripheral with the value of the GPIO_InitStruct structure.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\param  GPIO_InitStructure :
 *\*\           pointer to GPIO_InitType structure.
 *\*\return none
 */
void GPIO_Peripheral_Initialize(GPIO_Module* GPIOx, GPIO_InitType* GPIO_InitStructure)
{
    uint32_t position = 0x00U;
    uint32_t current_pin = 0x00U;

    while(((GPIO_InitStructure->Pin) >> position) != 0)
    {
        /* Get the IO position */
        current_pin = (GPIO_InitStructure->Pin) & (IO_POSITION_MASK << position);
        if(current_pin)
        {
            /* Configure GPIO alternate function */
            GPIO_Alternate_Set(GPIOx, GPIO_InitStructure->GPIO_Alternate, position);

            /* Configure GPIO mode */
            GPIO_Mode_Set(GPIOx, GPIO_InitStructure->GPIO_Mode, position);

            /* Configure pull-down or pull-up mode */
            GPIO_Pull_Set(GPIOx, GPIO_InitStructure->GPIO_Pull, position);

            /* Configure slew rate */
            GPIO_SlewRate_Set(GPIOx, GPIO_InitStructure->GPIO_Slew_Rate, position);

            /* Configuration drive capability */
            GPIO_Driver_Set(GPIOx, GPIO_InitStructure->GPIO_Current, position);
        }
        position++;
    }
}


/**
 *\*\name   GPIO_Struct_Initialization.
 *\*\fun    Assign default values to each GPIO_InitStruct member.
 *\*\param  GPIO_InitStructure :
 *\*\           pointer to GPIO_InitType structure.
 *\*\return none
 */
void GPIO_Structure_Initialize(GPIO_InitType* GPIO_InitStruct)
{
    /* Reset GPIO structure member */
    GPIO_InitStruct->Pin            = GPIO_PIN_ALL;
    GPIO_InitStruct->GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStruct->GPIO_Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct->GPIO_Alternate = GPIO_NO_AF;
    GPIO_InitStruct->GPIO_Pull      = GPIO_NO_PULL;
    GPIO_InitStruct->GPIO_Current   = GPIO_HIGH_DREIVE;
}


/**
 *\*\name   GPIO_Input_Pin_Data_Get.
 *\*\fun    Get the pin status on the specified input port.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return the pin state on the input port.
 */
uint8_t GPIO_Input_Pin_Data_Get(GPIO_Module* GPIOx, uint16_t pin)
{
    if ((GPIOx->PID & pin) != (uint32_t)PIN_RESET)
    {
        return (uint8_t)PIN_SET;
    }
    else
    {
        return (uint8_t)PIN_RESET;
    }
}


/**
 *\*\name   GPIO_Input_Data_Get.
 *\*\fun    Get the input data on the designated GPIO port.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only bit0 and bit1 are valid
 *\*\return the data value on the GPIO input port.
 */
uint16_t GPIO_Input_Data_Get(GPIO_Module* GPIOx)
{
    return ((uint16_t)GPIOx->PID);
}


/**
 *\*\name   GPIO_Output_Pin_Data_Get.
 *\*\fun    Get the pin status on the specified output port.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return the pin state on the output port.
 */
uint8_t GPIO_Output_Pin_Data_Get(GPIO_Module* GPIOx, uint16_t pin)
{
    if ((GPIOx->POD & pin) != (uint32_t)PIN_RESET)
    {
        return (uint8_t)PIN_SET;
    }
    else
    {
        return (uint8_t)PIN_RESET;
    }
}


/**
 *\*\name   GPIO_Output_Data_Get.
 *\*\fun    Get the output data on the designated GPIO port.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only bit0 and bit1 are valid
 *\*\return the data value on the GPIO output port.
 */
uint16_t GPIO_Output_Data_Get(GPIO_Module* GPIOx)
{
    return ((uint16_t)GPIOx->POD);
}


/**
 *\*\name   GPIO_Pins_Set.
 *\*\fun    Sets the selected data port bits.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return none
 */
void GPIO_Pins_Set(GPIO_Module* GPIOx, uint16_t pin)
{
    GPIOx->PBSC = pin;
}

/**
 *\*\name   GPIO_Pins_Reset.
 *\*\fun    Reset the selected data port bits.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return none
 */
void GPIO_Pins_Reset(GPIO_Module* GPIOx, uint16_t pin)
{
    GPIOx->PBC = pin;
}

/**
 *\*\name   GPIO_PBSC_Pins_Reset.
 *\*\fun    Sets the selected data port bits.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return none
 */
void GPIO_PBSC_Pins_Reset(GPIO_Module* GPIOx, uint16_t pin)
{
    GPIOx->PBSC = ((uint32_t)pin << SHIFT_PBSC_HIGH16);
}


/**
 *\*\name   GPIO_PBC_Pins_Reset.
 *\*\fun    Reset the selected data port bits.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return none
 */
void GPIO_PBC_Pins_Reset(GPIO_Module* GPIOx, uint16_t pin)
{
    GPIOx->PBC = pin;
}


/**
 *\*\name   GPIO_Write_Data.
 *\*\fun    Write data on the designated GPIO data port.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
  *\*\       Note: When selects the GPIOB, Only bit0 and bit1 are valid
 *\*\param  data_value :
 *\*\          the value to be written to the port output data register.
 *\*\return none
 */
void GPIO_Write(GPIO_Module* GPIOx, uint16_t data_value)
{
    GPIOx->POD = data_value;
}


/**
 *\*\name   GPIO_Pin_Toggle.
 *\*\fun    Toggle the specified port pin level.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return none
 */
void GPIO_Pin_Toggle(GPIO_Module* GPIOx, uint16_t pin)
{
    GPIOx->POD ^= pin;
}


/**
 *\*\name   GPIO_Pin_Lock_Set.
 *\*\fun    GPIO port lock register configuration.
 *\*\param  GPIOx :
 *\*\          - GPIOA
 *\*\          - GPIOB
 *\*\       Note: When selects the GPIOB, Only GPIO_PIN_0 and GPIO_PIN_1 are valid
 *\*\param  pin :
 *\*\          - GPIO_PIN_0
 *\*\          - GPIO_PIN_1
 *\*\          - GPIO_PIN_2
 *\*\          - GPIO_PIN_3
 *\*\          - GPIO_PIN_4
 *\*\          - GPIO_PIN_5
 *\*\          - GPIO_PIN_6
 *\*\          - GPIO_PIN_7
 *\*\          - GPIO_PIN_8
 *\*\          - GPIO_PIN_9
 *\*\          - GPIO_PIN_10
 *\*\          - GPIO_PIN_11
 *\*\          - GPIO_PIN_12
 *\*\          - GPIO_PIN_13
 *\*\          - GPIO_PIN_14
 *\*\          - GPIO_PIN_15
 *\*\return none
 */
void GPIO_Pin_Lock_Set(GPIO_Module* GPIOx, uint16_t pin)
{
    uint32_t temp_value = 0x00U;
	
	temp_value = (GPIO_PLOCKK_MASK | pin);

    /* PLOCKK and PLOCKx write 1 */
    GPIOx->PLOCK = temp_value;
    
    /* PLOCKK write 0 */
    GPIOx->PLOCK = pin; 
    
    /* PLOCKK and PLOCKx write 1 */
    GPIOx->PLOCK = temp_value; 

    /* PLOCKK read 0 */
    temp_value = GPIOx->PLOCK;
    
    /* PLOCKK read 1 */
    temp_value = GPIOx->PLOCK;
}


/**
 *\*\name   GPIO_Pin_Remap_Set.
 *\*\fun    Pin remapping configuration.
 *\*\param  port_source :
 *\*\          - GPIOA_PORT_SOURCE
 *\*\          - GPIOB_PORT_SOURCE
  *\*\       Note: When selects the GPIOB, Only GPIO_PIN_SOURCE0 and GPIO_PIN_SOURCE1 are valid
 *\*\param  pin_source :
 *\*\          - GPIO_PIN_SOURCE0
 *\*\          - GPIO_PIN_SOURCE1
 *\*\          - GPIO_PIN_SOURCE2
 *\*\          - GPIO_PIN_SOURCE3
 *\*\          - GPIO_PIN_SOURCE4
 *\*\          - GPIO_PIN_SOURCE5
 *\*\          - GPIO_PIN_SOURCE6
 *\*\          - GPIO_PIN_SOURCE7
 *\*\          - GPIO_PIN_SOURCE8
 *\*\          - GPIO_PIN_SOURCE9
 *\*\          - GPIO_PIN_SOURCE10
 *\*\          - GPIO_PIN_SOURCE11
 *\*\          - GPIO_PIN_SOURCE12
 *\*\          - GPIO_PIN_SOURCE13
 *\*\          - GPIO_PIN_SOURCE14
 *\*\          - GPIO_PIN_SOURCE15
 *\*\param  alternate_function :
 *\*\          - GPIO_AF0_SWDIO        SWDIO Alternate Function mapping   
 *\*\          - GPIO_AF0_SWCLK        SWCLK Alternate Function mapping   
 *\*\          - GPIO_AF0_TIM1         TIM1  Alternate Function mapping   
 *\*\          - GPIO_AF0_TIM3         TIM3  Alternate Function mapping   
 *\*\          - GPIO_AF0_SPI          SPI   Alternate Function mapping   
 *\*\
 *\*\          - GPIO_AF1_UART2        UART2 Alternate Function mapping    
 *\*\          - GPIO_AF1_TIM3         TIM3  Alternate Function mapping    
 *\*\
 *\*\          - GPIO_AF2_UART1        UART1 Alternate Function mapping    
 *\*\          - GPIO_AF2_TIM1         TIM1 Alternate Function mapping    
 *\*\          - GPIO_AF2_TIM3         TIM3 Alternate Function mapping    
 *\*\
 *\*\          - GPIO_AF3_TIM1         TIM1 Alternate Function mapping    
 *\*\          - GPIO_AF3_COMP         COMP Alternate Function mapping    
 *\*\          - GPIO_AF3_EVENTOUT     EVENTOUT Alternate Function mapping     
 *\*\
 *\*\          - GPIO_AF4_TIM1         TIM1 Alternate Function mapping
 *\*\
 *\*\          - GPIO_AF5_UART1        UART1 Alternate Function mapping  
 *\*\          - GPIO_AF5_TIM1         TIM1 Alternate Function mapping  
 *\*\          - GPIO_AF5_SPI          SPI Alternate Function mapping      
 *\*\
 *\*\          - GPIO_AF6_SPI          SPI Alternate Function mapping    
 *\*\          - GPIO_AF6_I2C          I2C Alternate Function mapping    
 *\*\          - GPIO_AF6_TIM3         TIM3 Alternate Function mapping    
 *\*\          - GPIO_AF6_UART2        UART2 Alternate Function mapping    
 *\*\          - GPIO_AF6_MCO          MCO Alternate Function mapping        
 *\*\
 *\*\          - GPIO_AF7_BEEPER       BEEPER Alternate Function mapping         
 *\*\return none
 */
void GPIO_Pin_Remap_Set(uint8_t port_source, uint8_t pin_source, uint32_t alternate_function)
{
    uint32_t position = 0x00, temp_value = 0x00;
    GPIO_Module *GPIOx;

    /* Get Peripheral index */
    GPIOx = GPIO_GET_PERIPH(port_source);

    if(pin_source & (uint8_t)AF_SELECTION_MASK)
    {
        position = (uint32_t)(pin_source & (uint8_t)AF_WRITE_POSITION_MASK);

        /* Read GPIO_AFH register */
        temp_value = GPIOx->AFH;

        /* Reset corresponding bits */
        temp_value &= ~(GPIO_AF_MASK << (position * MULTIPLIER_FACTOR_4));

        /* Set corresponding bits */
        temp_value |= (alternate_function << (position * MULTIPLIER_FACTOR_4));

        /* Write to the GPIO_AFH register */
        GPIOx->AFH = temp_value;
    }
    else
    {
        position = (uint32_t)(pin_source & (uint8_t)AF_WRITE_POSITION_MASK);

        /* Read GPIO_AFL register */
        temp_value  = GPIOx->AFL;

        /* Reset corresponding bits */
        temp_value &= ~(GPIO_AF_MASK << (position * MULTIPLIER_FACTOR_4)); 

        /* Set corresponding bits */
        temp_value |= (alternate_function << (position * MULTIPLIER_FACTOR_4));

        /* Write to the GPIO_AFL register */
        GPIOx->AFL = temp_value;
    }
}

/**
 *\*\name   AFIO_ADC_External_Trigger_Set
 *\*\fun    Selects the GPIO pin used as EXTI Line.
 *\*\param  line_source:
 *\*\          - AFIO_ADC_TRIG_PA0
 *\*\          - AFIO_ADC_TRIG_PA1
 *\*\          - AFIO_ADC_TRIG_PA2
 *\*\          - AFIO_ADC_TRIG_PA3
 *\*\          - AFIO_ADC_TRIG_PA4
 *\*\          - AFIO_ADC_TRIG_PA5
 *\*\          - AFIO_ADC_TRIG_PA6
 *\*\          - AFIO_ADC_TRIG_PA7
 *\*\          - AFIO_ADC_TRIG_PA8
 *\*\          - AFIO_ADC_TRIG_PA9
 *\*\          - AFIO_ADC_TRIG_PA10
 *\*\          - AFIO_ADC_TRIG_PA11
 *\*\          - AFIO_ADC_TRIG_PA12
 *\*\          - AFIO_ADC_TRIG_PA13
 *\*\          - AFIO_ADC_TRIG_PA14
 *\*\          - AFIO_ADC_TRIG_PA15
 *\*\          - AFIO_ADC_TRIG_PB0
 *\*\          - AFIO_ADC_TRIG_PB1
 *\*\          Note: Each EXTI line corresponds to a separate pin in this series.
 *\*\return none
 */
void AFIO_ADC_External_Trigger_Set(uint8_t ADC_trigger)
{
	uint32_t temp_value = 0x00;
	
	temp_value = AFIO->CFG;
	temp_value &= ~AFIO_CFG_MASK;
	temp_value |= ADC_trigger;
	AFIO->CFG = temp_value;
}


/**
 *\*\name   AFIO_SPI_NSS_Mode_Set.
 *\*\fun    Selects NSS mode of SPI when idle.
 *\*\param  SPI_nss_mode : 
 *\*\          specifies the SPI_NSS mode to be configured.
 *\*\          - AFIO_SPI_NSS_HIGH_IMPEDANCE
 *\*\          - AFIO_SPI_NSS_High_LEVEL
 *\*\return none
 */
void AFIO_SPI_NSS_Mode_Set(uint32_t SPI_NSS_mode)
{
    uint32_t temp_value = 0x00U;

    temp_value = (AFIO->CFG & (~AFIO_CFG_SPI_NSS));

    if(SPI_NSS_mode != AFIO_SPI_NSS_HIGH_IMPEDANCE)
    {
        temp_value |= AFIO_SPI_NSS;
    }
    else
    {
        temp_value &= ~AFIO_SPI_NSS;
    }

    AFIO->CFG = temp_value;
}


/**
 *\*\name   AFIO_TIM3CH2_Remap.
 *\*\fun    Controls the TIM3_CH2 internal mapping.
 *\*\param  TIM3CH2_remap : 
 *\*\          specifies the TIM3_CH2 internal mapping.
 *\*\          - TIM3CH2_TO_PORT
 *\*\          - TIM3CH2_TO_LSI
 *\*\return none
 */
void AFIO_TIM3CH2_Remap(uint32_t TIM3CH2_remap)
{
    uint32_t temp_value = 0x00U;

    temp_value = (AFIO->CFG & (~AFIO_CFG_TIM3CH2_MAP));

    if(TIM3CH2_remap != TIM3CH2_TO_PORT)
    {
        temp_value |= AFIO_TIM3CH2_MAP;
    }
    else
    {
        temp_value &= ~AFIO_TIM3CH2_MAP;
    }

    AFIO->CFG = temp_value;
}





