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
*\*\file n32g003_gpio.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32G003_GPIO_H__
#define __N32G003_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g003.h"


#define GPIOA_POSITION1                 (8U)
#define GPIOA_POSITION2                 (9U)

#define GPIO_AF_MODE                    ((uint32_t)GPIO_PMODE0_2)
#define GPIO_ANALOG_MODE                ((uint32_t)GPIO_PMODE0_Msk)
#define GPIO_PU_MODE                    ((uint32_t)GPIO_PUPD0_1)
#define GPIO_PD_MODE                    ((uint32_t)GPIO_PUPD0_2)
#define GPIO_SR_SLOW                    (GPIO_SR_SR0)
#define GPIO_LOW_DRIVER                 (GPIO_DS_DS0)

#define GPIO_OUTPUT_TYPE_MASK           ((uint32_t)0x00000010)
#define GPIO_SET_PMODE_MASK             ((uint32_t)0x00000003)
#define GPIO_AF_MASK                    ((uint32_t)0x0000000F)
#define GPIO_PMODE_MASK                 ((uint32_t)GPIO_PMODE0_Msk)
#define GPIO_PUPD_MASK                  ((uint32_t)GPIO_PUPD0_Msk)
#define GPIO_POTYPE_MASK                (GPIO_POTYPE_POT0)
#define GPIO_SR_MASK                    (GPIO_SR_SR0)
#define GPIO_DS_MASK                    (GPIO_DS_DS0)
#define GPIO_PLOCKK_MASK                (GPIO_PLOCK_PLOCKK)
#define AF_SELECTION_MASK               (0x08)
#define AF_WRITE_POSITION_MASK          (0x07U)
#define IO_POSITION_MASK                (1U)

#define MULTIPLIER_FACTOR_2             (2U)
#define MULTIPLIER_FACTOR_4             (4U)

#define SHIFT_POTYPE                    (REG_BIT4_OFFSET)
#define SHIFT_PBSC_HIGH16               (REG_BIT16_OFFSET)

#define GPIO_GET_PERIPH(INDEX)          (((INDEX)==((uint8_t)0x00))? GPIOA : GPIOB )

/**
 *\*\name   GPIO_ModeType. 
 *\*\fun    GPIO mode definition.
 *\*\       Values convention: 0x00YZ
 *\*\          - Y  : Output type (Push Pull or Open Drain)
 *\*\          - Z  : IO Direction mode (Input, Output, Alternate or Analog)      
 */
#define GPIO_MODE_INPUT     ((uint32_t)0x00000000U)  /* Input Floating Mode */
#define GPIO_MODE_OUT_PP    ((uint32_t)0x00000001U)  /* Output Push Pull Mode */
#define GPIO_MODE_OUT_OD    ((uint32_t)0x00000011U)  /* Output Open Drain Mode */
#define GPIO_MODE_AF_PP     ((uint32_t)0x00000002U)  /* Alternate Function Push Pull Mode  */
#define GPIO_MODE_AF_OD     ((uint32_t)0x00000012U)  /* Alternate Function Open Drain Mode */
#define GPIO_MODE_ANALOG    ((uint32_t)0x00000003U)  /* Analog Mode */

/** GPIO Pull-Up or Pull-Down Activation **/
#define GPIO_NO_PULL         ((uint32_t)GPIO_PUPD0_0) /* No Pull-up or Pull-down activation */
#define GPIO_PULL_UP         ((uint32_t)GPIO_PUPD0_1) /* Pull-up activation */
#define GPIO_PULL_DOWN       ((uint32_t)GPIO_PUPD0_2) /* Pull-down activation */

/** slew rate config **/
#define GPIO_SLEW_RATE_FAST  ((uint32_t)0x00000000U)
#define GPIO_SLEW_RATE_SLOW  ((uint32_t)0x00000001U)

/** driver strength config **/
#define GPIO_HIGH_DREIVE     ((uint32_t)0x00000000U)
#define GPIO_LOW_DREIVE      ((uint32_t)0x00000001U)


/** GPIO Init Structure Definition **/
typedef struct
{
    uint32_t         Pin;            /* Specifies the GPIO pins to be configured. */

    uint32_t         GPIO_Mode;      /* Specifies the operating mode for the selected pins. */

    uint32_t         GPIO_Pull;      /* Specifies the Pull-up or Pull-Down activation for the selected pins. */

    uint32_t         GPIO_Slew_Rate; /* Specify the reverse speed for the selected pins. */

    uint32_t         GPIO_Current;   /* Driving drive capability of the select pins. */

    uint32_t         GPIO_Alternate; /* Peripheral to be connected to the selected pins. */ 
}GPIO_InitType;

/** Bit_SET and Bit_RESET enumeration **/
typedef enum
{
    PIN_RESET = 0,
    PIN_SET
}Bit_CommandType;

/** GPIO_pins_define **/
#define GPIO_PIN_0           ((uint16_t)0x0001U) /* Pin 0 selected    */
#define GPIO_PIN_1           ((uint16_t)0x0002U) /* Pin 1 selected    */
#define GPIO_PIN_2           ((uint16_t)0x0004U) /* Pin 2 selected    */
#define GPIO_PIN_3           ((uint16_t)0x0008U) /* Pin 3 selected    */
#define GPIO_PIN_4           ((uint16_t)0x0010U) /* Pin 4 selected    */
#define GPIO_PIN_5           ((uint16_t)0x0020U) /* Pin 5 selected    */
#define GPIO_PIN_6           ((uint16_t)0x0040U) /* Pin 6 selected    */
#define GPIO_PIN_7           ((uint16_t)0x0080U) /* Pin 7 selected    */
#define GPIO_PIN_8           ((uint16_t)0x0100U) /* Pin 8 selected    */
#define GPIO_PIN_9           ((uint16_t)0x0200U) /* Pin 9 selected    */
#define GPIO_PIN_10          ((uint16_t)0x0400U) /* Pin 10 selected   */
#define GPIO_PIN_11          ((uint16_t)0x0800U) /* Pin 11 selected   */
#define GPIO_PIN_12          ((uint16_t)0x1000U) /* Pin 12 selected   */
#define GPIO_PIN_13          ((uint16_t)0x2000U) /* Pin 13 selected   */
#define GPIO_PIN_14          ((uint16_t)0x4000U) /* Pin 14 selected   */
#define GPIO_PIN_15          ((uint16_t)0x8000U) /* Pin 15 selected   */
#define GPIO_PIN_ALL         ((uint16_t)0xFFFFU) /* All pins selected */

/** GPIO Port Sources **/
#define GPIOA_PORT_SOURCE    ((uint8_t)0x00)
#define GPIOB_PORT_SOURCE    ((uint8_t)0x01)

/** GPIO Pin Sources **/
#define GPIO_PIN_SOURCE0     ((uint8_t)0x00)
#define GPIO_PIN_SOURCE1     ((uint8_t)0x01)
#define GPIO_PIN_SOURCE2     ((uint8_t)0x02)
#define GPIO_PIN_SOURCE3     ((uint8_t)0x03)
#define GPIO_PIN_SOURCE4     ((uint8_t)0x04)
#define GPIO_PIN_SOURCE5     ((uint8_t)0x05)
#define GPIO_PIN_SOURCE6     ((uint8_t)0x06)
#define GPIO_PIN_SOURCE7     ((uint8_t)0x07)
#define GPIO_PIN_SOURCE8     ((uint8_t)0x08)
#define GPIO_PIN_SOURCE9     ((uint8_t)0x09)
#define GPIO_PIN_SOURCE10    ((uint8_t)0x0A)
#define GPIO_PIN_SOURCE11    ((uint8_t)0x0B)
#define GPIO_PIN_SOURCE12    ((uint8_t)0x0C)
#define GPIO_PIN_SOURCE13    ((uint8_t)0x0D)
#define GPIO_PIN_SOURCE14    ((uint8_t)0x0E)
#define GPIO_PIN_SOURCE15    ((uint8_t)0x0F)


/** EXTI line and GPIO sources **/
#define AFIO_ADC_TRIG_PA0   ((uint8_t)0x00) /* PA0  */
#define AFIO_ADC_TRIG_PA1   ((uint8_t)0x01) /* PA1  */
#define AFIO_ADC_TRIG_PA2   ((uint8_t)0x02) /* PA2  */
#define AFIO_ADC_TRIG_PA3   ((uint8_t)0x03) /* PA3  */
#define AFIO_ADC_TRIG_PA4   ((uint8_t)0x04) /* PA4  */
#define AFIO_ADC_TRIG_PA5   ((uint8_t)0x05) /* PA5  */
#define AFIO_ADC_TRIG_PA6   ((uint8_t)0x06) /* PA6  */
#define AFIO_ADC_TRIG_PA7   ((uint8_t)0x07) /* PA7  */
#define AFIO_ADC_TRIG_PA8   ((uint8_t)0x08) /* PA8  */
#define AFIO_ADC_TRIG_PA9   ((uint8_t)0x09) /* PA9  */
#define AFIO_ADC_TRIG_PA10  ((uint8_t)0x0A) /* PA10 */
#define AFIO_ADC_TRIG_PA11  ((uint8_t)0x0B) /* PA11 */
#define AFIO_ADC_TRIG_PA12  ((uint8_t)0x0C) /* PA12 */
#define AFIO_ADC_TRIG_PA13  ((uint8_t)0x0D) /* PA13 */
#define AFIO_ADC_TRIG_PA14  ((uint8_t)0x0E) /* PA14 */
#define AFIO_ADC_TRIG_PA15  ((uint8_t)0x0F) /* PA15 */
#define AFIO_ADC_TRIG_PB0   ((uint8_t)0x10) /* PB0  */
#define AFIO_ADC_TRIG_PB1   ((uint8_t)0x11) /* PB1  */


/** GPIOx_Alternate_function_selection Alternate function selection **/

/** Alternate function AF0 **/
#define ALTERNATE_FUN_0      ((uint8_t)0x00U)
#define GPIO_AF0_SWDIO       (ALTERNATE_FUN_0) /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK       (ALTERNATE_FUN_0) /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_TIM1        (ALTERNATE_FUN_0) /* TIM1 Alternate Function mapping     */
#define GPIO_AF0_TIM3        (ALTERNATE_FUN_0) /* TIM3 Alternate Function mapping     */
#define GPIO_AF0_SPI         (ALTERNATE_FUN_0) /* SPI Alternate Function mapping      */

/** Alternate function AF1 **/
#define ALTERNATE_FUN_1      ((uint8_t)0x01U)
#define GPIO_AF1_UART2       (ALTERNATE_FUN_1) /* UART2 Alternate Function mapping    */
#define GPIO_AF1_TIM3        (ALTERNATE_FUN_1) /* TIM3 Alternate Function mapping     */

/** Alternate function AF2 **/
#define ALTERNATE_FUN_2      ((uint8_t)0x02U)
#define GPIO_AF2_UART1       (ALTERNATE_FUN_2) /* UART1 Alternate Function mapping    */
#define GPIO_AF2_TIM1        (ALTERNATE_FUN_2) /* TIM1 Alternate Function mapping     */
#define GPIO_AF2_TIM3        (ALTERNATE_FUN_2) /* TIM3 Alternate Function mapping     */


/** Alternate function AF3 **/
#define ALTERNATE_FUN_3      ((uint8_t)0x03U)
#define GPIO_AF3_EVENTOUT    (ALTERNATE_FUN_3) /* EVENTOUT Alternate Function mapping */
#define GPIO_AF3_TIM1        (ALTERNATE_FUN_3) /* TIM1 Alternate Function mapping     */
#define GPIO_AF3_COMP        (ALTERNATE_FUN_3) /* COMP Alternate Function mapping     */

/** Alternate function AF4 **/
#define ALTERNATE_FUN_4      ((uint8_t)0x04U)
#define GPIO_AF4_TIM1        (ALTERNATE_FUN_4) /* TIM1 Alternate Function mapping     */

/** Alternate function AF5 **/
#define ALTERNATE_FUN_5      ((uint8_t)0x05U)
#define GPIO_AF5_UART1       (ALTERNATE_FUN_5) /* UART1 Alternate Function mapping    */
#define GPIO_AF5_TIM1        (ALTERNATE_FUN_5) /* TIM1 Alternate Function mapping     */
#define GPIO_AF5_SPI         (ALTERNATE_FUN_5) /* SPI Alternate Function mapping      */

/** Alternate function AF6 **/
#define ALTERNATE_FUN_6      ((uint8_t)0x06U)
#define GPIO_AF6_SPI         (ALTERNATE_FUN_6) /* SPI Alternate Function mapping      */
#define GPIO_AF6_I2C         (ALTERNATE_FUN_6) /* I2C Alternate Function mapping      */
#define GPIO_AF6_TIM3        (ALTERNATE_FUN_6) /* TIM3 Alternate Function mapping     */
#define GPIO_AF6_UART2       (ALTERNATE_FUN_6) /* UART2 Alternate Function mapping    */
#define GPIO_AF6_MCO         (ALTERNATE_FUN_6) /* MCO Alternate Function mapping      */

/** Alternate function AF7 **/
#define ALTERNATE_FUN_7      ((uint8_t)0x07U)
#define GPIO_AF7_BEEPER      (ALTERNATE_FUN_7) /* BEEPER Alternate Function mapping   */

/** Alternate function AF8 **/
#define ALTERNATE_FUN_8      ((uint8_t)0x08U)
#define GPIO_AF8_UART2       (ALTERNATE_FUN_8) /* UART2 Alternate Function mapping    */

/** Alternate function AF15 **/
#define ALTERNATE_FUN_15     ((uint8_t)0x0FU)  /* NON Alternate Function mapping      */

#define GPIO_NO_AF           (ALTERNATE_FUN_15)

/** SPI mode definition in AFIO register  **/
#define AFIO_SPI_NSS         ((uint32_t)AFIO_CFG_SPI_NSS)

#define AFIO_SPI_NSS_HIGH_IMPEDANCE  (0x0UL)
#define AFIO_SPI_NSS_High_LEVEL      (0x1UL)

/** TIM3_CH2 internal remap definition in AFIO register  **/
#define AFIO_TIM3CH2_MAP     ((uint32_t)AFIO_CFG_TIM3CH2_MAP)

#define TIM3CH2_TO_PORT      (0x0UL)
#define TIM3CH2_TO_LSI       (0x1UL)



/** GPIO_Exported_Functions **/

void GPIO_Reset(GPIO_Module* GPIOx);
void GPIO_Alternate_Function_Reset(void);
void GPIOA_Pin_Reset(uint16_t pin);
void GPIOB_Pin_Reset(uint16_t pin);

void GPIO_Alternate_Set(GPIO_Module* GPIOx, uint32_t alternate, uint32_t position);
void GPIO_Mode_Set(GPIO_Module* GPIOx, uint32_t mode, uint32_t position);
void GPIO_Pull_Set(GPIO_Module* GPIOx, uint32_t pull, uint32_t position);
void GPIO_SlewRate_Set(GPIO_Module* GPIOx, uint32_t slew_rate, uint32_t position);
void GPIO_Driver_Set(GPIO_Module* GPIOx, uint32_t current, uint32_t position);
void GPIO_Peripheral_Initialize(GPIO_Module* GPIOx, GPIO_InitType* GPIO_InitStructure);
void GPIO_Structure_Initialize(GPIO_InitType* GPIO_InitStruct);

uint8_t GPIO_Input_Pin_Data_Get(GPIO_Module* GPIOx, uint16_t pin);
uint16_t GPIO_Input_Data_Get(GPIO_Module* GPIOx);
uint8_t GPIO_Output_Pin_Data_Get(GPIO_Module* GPIOx, uint16_t pin);
uint16_t GPIO_Output_Data_Get(GPIO_Module* GPIOx);
void GPIO_Pins_Set(GPIO_Module* GPIOx, uint16_t pin);
void GPIO_Pins_Reset(GPIO_Module* GPIOx, uint16_t pin);
void GPIO_PBSC_Pins_Reset(GPIO_Module* GPIOx, uint16_t pin);
void GPIO_PBC_Pins_Reset(GPIO_Module* GPIOx, uint16_t pin);
void GPIO_Write(GPIO_Module* GPIOx, uint16_t data_value);
void GPIO_Pin_Toggle(GPIO_Module* GPIOx, uint16_t pin);
void GPIO_Pin_Lock_Set(GPIO_Module* GPIOx, uint16_t pin);

void GPIO_Pin_Remap_Set(uint8_t port_source, uint8_t pin_source, uint32_t alternate_function);
void AFIO_ADC_External_Trigger_Set(uint8_t ADC_trigger);
void AFIO_SPI_NSS_Mode_Set(uint32_t SPI_NSS_mode);
void AFIO_TIM3CH2_Remap(uint32_t TIM3CH2_remap);


#ifdef __cplusplus
}
#endif

#endif /* __N32G003_GPIO_H__ */

