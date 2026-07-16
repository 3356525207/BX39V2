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
*\*\file n32g003.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef __N32G003_H__
#define __N32G003_H__

#ifdef __cplusplus
extern "C" {
#endif


/***  N32G003_Library_Basic ***/

#if !defined USE_STDPERIPH_DRIVER
/** Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will
   be based on direct access to peripherals registers **/
#define USE_STDPERIPH_DRIVER
#endif


#define HSI_VALUE   ((uint32_t)48000000) /* Value of the Internal oscillator in Hz*/
#define HSI_40M     ((uint32_t)40000000)   
#define HSI_STARTUP_TIMEOUT ((uint16_t)0x0500) /* Time out for HSI start up */

/** n32g003 Standard Peripheral Library version number **/
#define __n32g003_STDPERIPH_VERSION_MAIN   (0x01) /* [31:24] main version */
#define __n32g003_STDPERIPH_VERSION_SUB1   (0x00) /* [23:16] sub1 version */
#define __n32g003_STDPERIPH_VERSION_SUB2   (0x00) /* [15:8]  sub2 version */
#define __n32g003_STDPERIPH_VERSION_RC     (0x00) /* [7:0]  release candidate */
#define __n32g003_STDPERIPH_VERSION        ( (__n32g003_STDPERIPH_VERSION_MAIN << 24)\
                                           |(__n32g003_STDPERIPH_VERSION_SUB1 << 16)\
                                           |(__n32g003_STDPERIPH_VERSION_SUB2 << 8)\
                                           |(__n32g003_STDPERIPH_VERSION_RC))

/** Configuration of the Cortex-M0 Processor and Core Peripherals **/
#define __NVIC_PRIO_BITS          2 /* N32G003 uses 2 Bits for the Priority Levels    */
#define __Vendor_SysTickConfig    0 /* Set to 1 if different SysTick Config is used */

/*** N32G003 Interrupt Number Definition ***/

typedef enum IRQn
{
  /** Cortex-M0 Processor Exceptions Numbers **/
  NonMaskableInt_IRQn         = -14,    /* 2 Non Maskable Interrupt                                */
  HardFault_IRQn              = -13,    /* 3 Cortex-M0 Hard Fault Interrupt                        */
  SVC_IRQn                    = -5,     /* 11 Cortex-M0 SV Call Interrupt                          */
  PendSV_IRQn                 = -2,     /* 14 Cortex-M0 Pend SV Interrupt                          */
  SysTick_IRQn                = -1,     /* 15 Cortex-M0 System Tick Interrupt                      */

  /** N32G003 specific Interrupt Numbers  **/
  PVD_IRQn                    = 0,      /* PVD through EXTI Line 18 detect Interrupt               */
  FLASH_IRQn                  = 1,      /* FLASH Interrupt                                         */
  EXTI0_1_IRQn                = 2,      /* EXTI Line 0 and 1 Interrupts                            */
  EXTI2_3_IRQn                = 3,      /* EXTI Line 2 and 3 Interrupts                            */
  EXTI4_17_IRQn               = 4,      /* EXTI Line 4 to 17 Interrupts                            */
  TIM1_BRK_UP_TRG_COM_IRQn    = 5,      /* TIM1 Break, Update, Trigger and Commutation Interrupts  */
  TIM1_CC_IRQn                = 6,      /* TIM1 Capture Compare Interrupt                          */
  TIM3_IRQn                   = 7,      /* TIM3 Interrupt                                          */
  TIM6_IRQn                   = 8,      /* TIM6 through EXTI Line 19 Interrupt                     */
  ADC_IRQn                    = 9,      /* ADC global Interrupt                                    */
  I2C_EV_IRQn                 = 10,     /* I2C event Interrupt                                     */
  I2C_ER_IRQn                 = 11,     /* I2C error Interrupt                                     */
  SPI_IRQn                    = 12,     /* SPI Interrupt                                           */
  UART1_IRQn                  = 13,     /* UART1 Interrupt                                         */
  UART2_IRQn                  = 14,     /* UART2 Interrupt                                         */
  COMP_IRQn                   = 15      /* COMP Interrupt                                          */
} IRQn_Type;

#include "core_cm0.h"
#include "system_n32g003.h"
#include <stdint.h>
#include <stdbool.h>

/* n32g003 Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /* Read Only */
typedef const int16_t sc16;  /* Read Only */
typedef const int8_t sc8;   /* Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /* Read Only */
typedef __I int16_t vsc16;  /* Read Only */
typedef __I int8_t vsc8;   /* Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /* Read Only */
typedef const uint16_t uc16;  /* Read Only */
typedef const uint8_t uc8;   /* Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /* Read Only */
typedef __I uint16_t vuc16;  /* Read Only */
typedef __I uint8_t vuc8;   /* Read Only */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/* n32g003 Standard Peripheral Library definitions (maintained for legacy purpose) */
#define HSI_Value            HSI_VALUE

/*** Analog to Digital Converter ***/
typedef struct
{
    __IO uint32_t STS;
    __IO uint32_t CTRL1;
    __IO uint32_t CTRL2;
    __IO uint32_t CTRL3;
    __IO uint32_t SAMPT;
    __IO uint32_t WDGHIGH;
    __IO uint32_t WDGLOW;
    __IO uint32_t DAT0;
    __IO uint32_t DAT1;
    __IO uint32_t DAT2;
    __IO uint32_t DAT3;
    __IO uint32_t DAT4;

} ADC_Module;


/*** CRC calculation unit ***/
typedef struct
{
    uint32_t      RESERVED0;
    uint32_t      RESERVED1;
    uint32_t      RESERVED2;
    __IO uint8_t  CRC16CTRL;
    uint8_t       RESERVED3;
    uint16_t      RESERVED4;
    __IO uint8_t  CRC16DAT;
    uint8_t       RESERVED5;
    uint16_t      RESERVED6;
    __IO uint16_t CRC16D;
    uint16_t      RESERVED7;
    __IO uint8_t  LRC;
    uint8_t       RESERVED8;
    uint16_t      RESERVED9;
} CRC_Module;



/***  External Interrupt/Event Controller ***/

typedef struct
{
   __IO uint32_t IMASK;   
   __IO uint32_t EMASK;   
   __IO uint32_t RT_CFG;  
   __IO uint32_t FT_CFG;   
   __IO uint32_t SWIE;     
   __IO uint32_t PEND;     
} EXTI_Module;

/*** FLASH Registers ***/

typedef struct
{
  __IO uint32_t AC;
  __IO uint32_t KEY;
  __IO uint32_t OPTKEY;
  __IO uint32_t STS;
  __IO uint32_t CTRL;
  __IO uint32_t ADD;
  __IO uint32_t RESERVED0;
  __IO uint32_t OB;
  __IO uint32_t USER;
  __IO uint32_t VTOR;
} FLASH_Module;

/*** Option Bytes Registers ***/

typedef struct
{
  __IO uint32_t USER_RDP;
  __IO uint32_t Data1_Data0;
  __IO uint32_t USER3_USER2;
  __IO uint32_t USER4_RDP2;
} OB_Module;

/** General Purpose I/O **/

typedef struct
{
    __IO uint32_t PMODE;
    __IO uint32_t POTYPE;
    __IO uint32_t SR;
    __IO uint32_t PUPD;
    __IO uint32_t PID;
    __IO uint32_t POD;
    __IO uint32_t PBSC;
    __IO uint32_t PLOCK;
    __IO uint32_t AFL;
    __IO uint32_t AFH;
    __IO uint32_t PBC;
    __IO uint32_t DS;
} GPIO_Module;

/** Alternate Function I/O **/

typedef struct
{
    __IO uint32_t CFG;
} AFIO_Module;


/***  Inter Integrated Circuit Interface ***/

typedef struct
{
  __IO uint16_t CTRL1;
  uint16_t  RESERVED0;
  __IO uint16_t CTRL2;
  uint16_t  RESERVED1;
  __IO uint16_t OADDR1;
  uint16_t  RESERVED2;
  __IO uint16_t OADDR2;
  uint16_t  RESERVED3;
  __IO uint16_t DAT;
  uint16_t  RESERVED4;
  __IO uint16_t STS1;
  uint16_t  RESERVED5;
  __IO uint16_t STS2;
  uint16_t  RESERVED6;
  __IO uint16_t CLKCTRL;
  uint16_t  RESERVED7;
  __IO uint16_t TMRISE;
  uint16_t  RESERVED8;
  __IO uint16_t GFLTRCTRL;
  uint16_t  RESERVED9;
  __IO uint16_t BYTENUM;
} I2C_Module;


/***  Beeper  ***/

typedef struct
{
  __IO uint32_t CTRL;
} BEEPER_Module;


/*** Independent watchdog ***/

typedef struct
{
  __IO uint32_t KEY;
  __IO uint32_t PREDIV;
  __IO uint32_t RELV;
  __IO uint32_t STS;
  __IO uint32_t FREEZE;
} IWDG_Module;

/*** Power Control ***/
typedef struct
{
    __IO uint32_t CTRL;
    __IO uint32_t CTRLSTS;
    __IO uint32_t CTRL2;
    uint32_t      RESERVED0;
    uint32_t      RESERVED1;
    __IO uint32_t CTRL3;
    uint32_t      RESERVED2;
    uint32_t      RESERVED3;
    __IO uint32_t CTRL4;
    __IO uint32_t CTRL5;
    __IO uint32_t CTRL6;
    uint32_t      RESERVED4;
    __IO uint32_t DBG_CTRL;
} PWR_Module;

/*** Reset and Clock Control ***/

typedef struct
{
  __IO uint32_t HSICTRL;
  __IO uint32_t CFG;
  __IO uint32_t Reserve0;
  __IO uint32_t PRST;
  __IO uint32_t AHBPCLKEN;
  __IO uint32_t APBPCLKEN;
  __IO uint32_t LSICTRL;
  __IO uint32_t CTRLSTS;
  __IO uint32_t CFG2;
  __IO uint32_t EMCCTRL;
} RCC_Module;


/*** compare ***/

typedef struct
{
  __IO uint32_t INTEN;
  __IO uint32_t INTSTS;
  uint32_t  RESERVED0;
  __IO uint32_t LOCK;
  __IO uint32_t CTRL;
  __IO uint32_t FILC;
  __IO uint32_t FILP;
}COMP_Module;



/*** Serial Peripheral Interface ***/

typedef struct
{
    __IO uint16_t CTRL1;
    uint16_t RESERVED0;
    __IO uint16_t CTRL2;
    uint16_t RESERVED1;
    __IO uint16_t STS;
    uint16_t RESERVED2;
    __IO uint16_t DAT;
    uint16_t RESERVED3;
} SPI_Module;


/*** TIM ***/

typedef struct
{
  __IO uint32_t CTRL1;
  __IO uint32_t CTRL2;
  __IO uint16_t SMCTRL;
  uint16_t  RESERVED1;
  __IO uint16_t DINTEN;
  uint16_t  RESERVED2;
  __IO uint32_t STS;
  __IO uint16_t EVTGEN;
  uint16_t  RESERVED3;
  __IO uint16_t CCMOD1;
  uint16_t  RESERVED4;
  __IO uint16_t CCMOD2;
  uint16_t  RESERVED5;
  __IO uint32_t CCEN;
  __IO uint16_t CNT;
  uint16_t  RESERVED6;
  __IO uint16_t PSC;
  uint16_t  RESERVED7;
  __IO uint16_t AR;
  uint16_t  RESERVED8;
  __IO uint16_t REPCNT;
  uint16_t  RESERVED9;
  __IO uint16_t CCDAT1;
  uint16_t  RESERVED10;
  __IO uint16_t CCDAT2;
  uint16_t  RESERVED11;
  __IO uint16_t CCDAT3;
  uint16_t  RESERVED12;
  __IO uint16_t CCDAT4;
  uint16_t  RESERVED13;
  __IO uint16_t BKDT;
  uint16_t  RESERVED14;
  uint32_t  RESERVED15;
  uint32_t  RESERVED16;
  uint32_t  RESERVED17;
  __IO uint16_t CCMOD3;
  uint16_t  RESERVED18;
  __IO uint16_t CCDAT5;
  uint16_t  RESERVED19;
  uint32_t  RESERVED20;
} TIM_Module;



/*** Universal Asynchronous Receiver Transmitter ***/
typedef struct
{
    __IO uint16_t STS;
    uint16_t  RESERVED0;
    __IO uint16_t DAT;
    uint16_t  RESERVED1;
    __IO uint16_t BRCF;
    uint16_t  RESERVED2;
    __IO uint16_t CTRL1;
    uint16_t  RESERVED3;
    __IO uint16_t CTRL2;
    uint16_t  RESERVED4;
    __IO uint16_t CTRL3;
    uint16_t  RESERVED5;
} UART_Module;




/*** Peripheral memory map ***/

#define FLASH_BASE            ((uint32_t)0x08000000) /* FLASH base address in the alias region */
#define SRAM_BASE             ((uint32_t)0x20000000) /* SRAM base address in the alias region */
#define PERIPH_BASE           ((uint32_t)0x40000000) /* Peripheral base address in the alias region */

#define UCID_BASE           ((uint32_t)0x1FFFF4D0) /* UCID Address : 0x1FFF_F7C0 */
#define UCID_LENGTH         ((uint32_t)0x10)       /* UCID Length  : 16 Bytes */
#define UID_BASE            ((uint32_t)0x1FFFF4FC) /* UID Address  : 0x1FFF_F7F0 */
#define UID_LENGTH          ((uint32_t)0x0C)       /* UID Length   : 12 Bytes */
#define DBGMCU_ID_BASE      ((uint32_t)0x1FFFF508) /* DBGMCU_ID Address :0x1FFF_F508 */
#define DBGMCU_ID_LENGTH    ((uint8_t)0x04)				/* DBGMCU_ID Length : 4 Bytes */


/** Peripheral memory map **/
#define APBPERIPH_BASE        (PERIPH_BASE)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

/** APB1 **/
#define TIM3_BASE             (APBPERIPH_BASE + 0x0400)
#define TIM1_BASE             (APBPERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APBPERIPH_BASE + 0x1000)
#define AFIO_BASE             (APBPERIPH_BASE + 0x1400)
#define GPIOA_BASE            (APBPERIPH_BASE + 0x1C00)
#define GPIOB_BASE            (APBPERIPH_BASE + 0x2000)
#define COMP_BASE             (APBPERIPH_BASE + 0x2400)
#define IWDG_BASE             (APBPERIPH_BASE + 0x2C00)
#define EXTI_BASE             (APBPERIPH_BASE + 0x3400)
#define BEEPER_BASE           (APBPERIPH_BASE + 0x3800)
#define UART1_BASE            (APBPERIPH_BASE + 0x4400)
#define UART2_BASE            (APBPERIPH_BASE + 0x4800)
#define I2C_BASE              (APBPERIPH_BASE + 0x5400)
#define SPI_BASE              (APBPERIPH_BASE + 0x5800)
#define PWR_BASE              (APBPERIPH_BASE + 0x7000)


/** AHB **/
#define ADC_BASE              (AHBPERIPH_BASE + 0x0800)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x2000) /* Flash registers base address */
#define OB_BASE               ((uint32_t)0x1FFFF600)    /* Flash Option Bytes base address */
#define CRC_BASE              (AHBPERIPH_BASE + 0x3000)


/** Peripheral_declaration **/

#define BEEPER              ((BEEPER_Module *) BEEPER_BASE)
#define TIM1                ((TIM_Module *) TIM1_BASE)
#define TIM3                ((TIM_Module *) TIM3_BASE)
#define TIM6                ((TIM_Module *) TIM6_BASE)
#define COMP                ((COMP_Module *) COMP_BASE)
#define IWDG                ((IWDG_Module *) IWDG_BASE)
#define SPI                 ((SPI_Module *) SPI_BASE)
#define UART1               ((UART_Module *) UART1_BASE)
#define UART2               ((UART_Module *) UART2_BASE)
#define I2C                 ((I2C_Module *) I2C_BASE)
#define PWR                 ((PWR_Module *) PWR_BASE)

#define AFIO                ((AFIO_Module *) AFIO_BASE)
#define EXTI                ((EXTI_Module *) EXTI_BASE)
#define GPIOA               ((GPIO_Module *) GPIOA_BASE)
#define GPIOB               ((GPIO_Module *) GPIOB_BASE)
#define ADC                 ((ADC_Module *) ADC_BASE)
#define RCC                 ((RCC_Module *) RCC_BASE)
#define FLASH               ((FLASH_Module *) FLASH_R_BASE)
#define OBT                 ((OB_Module *) OB_BASE)
#define CRC                 ((CRC_Module *) CRC_BASE)



/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/

/*** CRC calculation unit ***/
/** Bit definition for CRC_CRC16CTRL register **/
#define CRC16_CTRL_ENDHL    ((uint8_t)0x02)
#define CRC16_CTRL_LITTLE   ((uint8_t)0x02)
#define CRC16_CTRL_BIG      ((uint8_t)0xFD)

#define CRC16_CTRL_CLR      ((uint8_t)0x04)
#define CRC16_CTRL_RESET    ((uint8_t)0x04)
#define CRC16_CTRL_NO_RESET ((uint8_t)0xFB)

/** Bit definition for CRC_CRC16DAT register **/
#define CRC16_CRC16DAT ((uint8_t)0xFF)

/** Bit definition for CRC_CRC16D register **/
#define CRC16_CRC16D ((uint16_t)0xFFFF)

/** Bit definition for CRC_LRC register **/
#define CRC_LRC_LRCDAT ((uint8_t)0xFF)


/*** Power Control ***/
/** Bit definition for PWR_CTRL register **/
#define PWR_CTRL_IWDGRSTEN ((uint16_t)0x0001) /* IWDG reset request causes a system reset */
#define PWR_CTRL_PDSTP     ((uint16_t)0x0002) /* 0: CPU output DEEPSLEEP is 1, and the chip enters STOP mode,*/
												 /*1: CPU output DEEPSLEEP is 1, and the chip enters PD mode */
#define PWR_CTRL_CLRWKUPF  ((uint16_t)0x0004) /* Clear Wakeup Flag */
#define PWR_CTRL_CLRDBGPDF ((uint16_t)0x0008) /* Clear DBGPDF flag */
#define PWR_CTRL_PVDEN     ((uint16_t)0x0010) /* PVD Enables control */

#define PWR_CTRL_PLS       ((uint16_t)0x01E0) /* PLS[3:0] bits (PVD Level Selection) */
#define PWR_CTRL_PLS_0     ((uint16_t)0x0020) /* Bit 0 */
#define PWR_CTRL_PLS_1     ((uint16_t)0x0040) /* Bit 1 */
#define PWR_CTRL_PLS_2     ((uint16_t)0x0080) /* Bit 2 */
#define PWR_CTRL_PLS_3     ((uint16_t)0x0100) /* Bit 3 */

#define PWR_CTRL_PDRS      ((uint16_t)0x0600) /* PDRS[2:0] bits (VDDD PDR trigger level in STOP mode) */
#define PWR_CTRL_PDRS_0    ((uint16_t)0x0200) /* Bit 0 */
#define PWR_CTRL_PDRS_1    ((uint16_t)0x0400) /* Bit 1 */

#define PWR_CTRL_NRSTPOL   ((uint16_t)0x0800) /* NRST polarity selection */

#define PWR_CTRL_PVDCNT    ((uint32_t)0x01FE0000) /* PVD count */
#define PWR_CTRL_PVDFILEN  ((uint32_t)0x02000000) /* Enable PVD filter */
#define PWR_CTRL_PVDITEN   ((uint32_t)0x04000000) /* Enable PVD interrupt */

/** Bit definition for PWR_CTRLSTS register **/
#define PWR_CTRLSTS_WKUPF   ((uint16_t)0x0001) /* Wakeup Flag */
#define PWR_CTRLSTS_DBGPDF  ((uint16_t)0x0002) /* DBGPDF Flag */
#define PWR_CTRLSTS_PVDO    ((uint16_t)0x0004) /* PVD Output */
#define PWR_CTRLSTS_WKUP0EN ((uint16_t)0x0100) /* Enable WKUP pin PA1 */
#define PWR_CTRLSTS_WKUP1EN ((uint16_t)0x0200) /* Enable WKUP pin PA2 */
#define PWR_CTRLSTS_WKUPPOL ((uint16_t)0x0400) /* Polarity of WKUP pin PA1/PA2,0:Falling edge,1:Rising edge */

/** Bit definition for PWR_CTRL2 register **/
#define PWR_CTRL2_LVRCNT   ((uint16_t)0x00FF) /* LVR filter control counts */
#define PWR_CTRL2_LVRFILEN ((uint16_t)0x0100) /* Enable LVR filter */
#define PWR_CTRL2_LVRRSTEN ((uint16_t)0x0200) /* Enable LVR reset */
#define PWR_CTRL2_LVREN    ((uint16_t)0x0400) /* Enable LVR */

#define PWR_CTRL2_LVRLS    ((uint16_t)0x7800) /* LVRLS[3:0] bits (LVR Level Selection) */
#define PWR_CTRL2_LVRLS_0  ((uint16_t)0x0800) /* Bit 0 */
#define PWR_CTRL2_LVRLS_1  ((uint16_t)0x1000) /* Bit 1 */
#define PWR_CTRL2_LVRLS_2  ((uint16_t)0x2000) /* Bit 2 */
#define PWR_CTRL2_LVRLS_3  ((uint16_t)0x4000) /* Bit 3 */

#define PWR_CTRL2_LVRO     ((uint16_t)0x8000) /* LVR Output */

#define PWR_CTRL2_LVRKEY   ((uint32_t)0xFF000000) /* LVR key */

/** Bit definition for PWR_CTRL3 register **/
#define PWR_CTRL3_MRPWR     ((uint16_t)0x0001) /* MR is controlled by PWR */
#define PWR_CTRL3_LPMRPWR   ((uint16_t)0x0002) /* MR low power is controlled by PWR */
#define PWR_CTRL3_SLPFLHPWR ((uint16_t)0x0004) /* Flash sleep is controlled by PWR */
#define PWR_CTRL3_PDFLHPWR  ((uint16_t)0x0008) /* Flash power down is controlled by PWR */
#define PWR_CTRL3_HSIPWR    ((uint16_t)0x0020) /* HSI is controlled by PWR */
#define PWR_CTRL3_HSISEL    ((uint16_t)0x0040) /* HSI frequency selection, 0: 48M, 1: 40M */
#define PWR_CTRL3_LSIEN     ((uint16_t)0x0080) /* Control PWR Enables the LSI */
#define PWR_CTRL3_PDRSEL    ((uint16_t)0x0200) /* VDDD PDR select signal is controlled by PWR */
#define PWR_CTRL3_LSISTPCNT ((uint16_t)0x1C00) /* LSI stop count */

/** Bit definition for PWR_CTRL4 register **/
#define PWR_CTRL4_FLHWKUP ((uint16_t)0x0001) /* Flash quick Wake up */
#define PWR_CTRL4_STBFLH  ((uint16_t)0x0002) /* Flash Deep standby mode */
#define PWR_CTRL4_RUNF    ((uint16_t)0x0004) /* System in RUN mode */

/** Bit definition for PWR_CTRL5 register **/
#define PWR_CTRL5_STPMRSEL   ((uint16_t)0x000C) /* STPMRSEL[1:0] bits (After enter STOP mode, VDDD output voltage select) */
#define PWR_CTRL5_STPMRSEL_0 ((uint16_t)0x0004) /* Bit 0 */
#define PWR_CTRL5_STPMRSEL_1 ((uint16_t)0x0008) /* Bit 1 */

/** Bit definition for PWR_CTRL6 register **/
#define PWR_CTRL6_STPMREN   ((uint16_t)0x000C) /* STPMREN[1:0] bits (After enter STOP mode, Enable VDDD output voltage select) */
#define PWR_CTRL6_STPMREN_0 ((uint16_t)0x0004) /* Bit 0 */
#define PWR_CTRL6_STPMREN_1 ((uint16_t)0x0008) /* Bit 1 */

/** Bit definition for DBG_CTRL register **/
#define DBG_CTRL_STOP    ((uint16_t)0x0002) /* Debug STOP mode control */
#define DBG_CTRL_PD      ((uint16_t)0x0004) /* Debug PD mode control */
#define DBG_CTRL_IWDGSTP ((uint16_t)0x0020) /* Debug LWDG mode control */
#define DBG_CTRL_TIM1STP ((uint16_t)0x0040) /* Debug TIM1 mode control */
#define DBG_CTRL_TIM3STP ((uint16_t)0x0080) /* Debug TIM3 mode control */
#define DBG_CTRL_TIM6STP ((uint16_t)0x0100) /* Debug TIM6 mode control */


/*** Reset and Clock Control ***/

/** Bit definition for RCC_HSICTRL register **/
#define  RCC_HSICTRL_HSI40MRDF                    ((uint32_t)0x00000001)        /* Internal High Speed clock 40M ready flag */
#define  RCC_HSICTRL_HSI48MRDF                    ((uint32_t)0x00000002)        /* Internal High Speed clock 48M ready flag */
#define  RCC_HSICTRL_HSITRIM                      ((uint32_t)0x000000F0)        /* Internal High Speed clock trimming */

/** Bit definition for RCC_CFG register **/

/** AHBPRES configuration */
#define RCC_CFG_AHBPRES       ((uint32_t)0x000000F0) /* AHBPRES[3:0] bits (AHB prescaler) */
#define RCC_CFG_AHBPRES_0     ((uint32_t)0x00000010) /* Bit 0 */
#define RCC_CFG_AHBPRES_1     ((uint32_t)0x00000020) /* Bit 1 */
#define RCC_CFG_AHBPRES_2     ((uint32_t)0x00000040) /* Bit 2 */
#define RCC_CFG_AHBPRES_3     ((uint32_t)0x00000080) /* Bit 3 */

#define RCC_CFG_AHBPRES_DIV1   ((uint32_t)0x00000000) /* SYSCLK not divided */
#define RCC_CFG_AHBPRES_DIV2   ((uint32_t)0x00000080) /* SYSCLK divided by 2 */
#define RCC_CFG_AHBPRES_DIV4   ((uint32_t)0x00000090) /* SYSCLK divided by 4 */
#define RCC_CFG_AHBPRES_DIV8   ((uint32_t)0x000000A0) /* SYSCLK divided by 8 */
#define RCC_CFG_AHBPRES_DIV12  ((uint32_t)0x000000B0) /* SYSCLK divided by 12 */
#define RCC_CFG_AHBPRES_DIV16  ((uint32_t)0x000000C0) /* SYSCLK divided by 16 */
#define RCC_CFG_AHBPRES_DIV24  ((uint32_t)0x000000D0) /* SYSCLK divided by 24 */

/** PPRE configuration **/
/** APBPRES configuration */
#define RCC_CFG_APBPRES       ((uint32_t)0x00000700) /* APB1PRES[2:0] bits (APB1 prescaler) */
#define RCC_CFG_APBPRES_0	    ((uint32_t)0x00000100) /* Bit 0 */
#define RCC_CFG_APBPRES_1     ((uint32_t)0x00000200) /* Bit 1 */
#define RCC_CFG_APBPRES_2     ((uint32_t)0x00000400) /* Bit 2 */

#define RCC_CFG_APBPRES_DIV1  ((uint32_t)0x00000000) /* HCLK not divided */
#define RCC_CFG_APBPRES_DIV2  ((uint32_t)0x00000400) /* HCLK divided by 2 */
#define RCC_CFG_APBPRES_DIV4  ((uint32_t)0x00000500) /* HCLK divided by 4 */
#define RCC_CFG_APBPRES_DIV8  ((uint32_t)0x00000600) /* HCLK divided by 8 */
#define RCC_CFG_APBPRES_DIV16 ((uint32_t)0x00000700) /* HCLK divided by 16 */

/** SYSPRES configuration **/
#define  RCC_CFG_SYSPRES                      ((uint32_t)0x00003000)        /* PRE1[2:0] bits (SYSCLk prescaler) */
#define  RCC_CFG_SYSPRES_0                    ((uint32_t)0x00001000)        /* Bit 0 */
#define  RCC_CFG_SYSPRES_1                    ((uint32_t)0x00002000)        /* Bit 1 */

#define  RCC_CFG_SYSPRES_DIV1                ((uint32_t)0x00000000)        /* HSICLK not divided */
#define  RCC_CFG_SYSPRES_DIV2                ((uint32_t)0x00001000)        /* HSICLK divided by 2 */
#define  RCC_CFG_SYSPRES_DIV5                ((uint32_t)0x00003000)        /* HSICLK divided by 5 */
#define  RCC_CFG_SYSPRES_DIV6                ((uint32_t)0x00003000)        /* HSICLK divided by 6 */


/** MCO configuration **/
#define RCC_CFG_MCO  	   									  ((uint32_t)0x06000000) /* MCO[1:0] bits (Microcontroller Clock Output) */
#define RCC_CFG_MCO_0	  							      ((uint32_t)0x02000000) /* Bit 0 */
#define RCC_CFG_MCO_1	                      ((uint32_t)0x04000000) /* Bit 1 */

#define RCC_CFG_MCO_NOCLK					  	      ((uint32_t)0x00000000) /* No clock */
#define RCC_CFG_MCO_LSI	  							    ((uint32_t)0x02000000) /* LSI clock  selected as MCO source */
#define RCC_CFG_MCO_HSI	                    ((uint32_t)0x04000000) /* HSI clock selected as MCO source */

/** MCOPRE configuration **/
#define  RCC_CFG_MCOPRES                  ((uint32_t)0xF0000000)/* MCOPRE[3:0] bits ( Microcontroller Clock Output prescaler.) */
#define  RCC_CFG_MCOPRES_0                 ((uint32_t)0x10000000) /* Bit 0 */
#define  RCC_CFG_MCOPRES_1                 ((uint32_t)0x20000000) /* Bit 1 */
#define  RCC_CFG_MCOPRES_2                 ((uint32_t)0x40000000) /* Bit 2 */
#define  RCC_CFG_MCOPRES_3                 ((uint32_t)0x80000000) /* Bit 3 */

#define  RCC_CFG_MCOPRES_DIV1             ((uint32_t)0x00000000) /* LSI clock is divided by 1,HSI clock is divided by 2 */
#define  RCC_CFG_MCOPRES_DIV2             ((uint32_t)0x10000000) /* LSI/HSI clock is divided by 2 */
#define  RCC_CFG_MCOPRES_DIV3             ((uint32_t)0x20000000) /* LSI/HSI clock is divided by 3 */
#define  RCC_CFG_MCOPRES_DIV4             ((uint32_t)0x30000000) /* LSI/HSI clock is divided by 4 */
#define  RCC_CFG_MCOPRES_DIV5             ((uint32_t)0x40000000) /* LSI/HSI clock is divided by 5 */
#define  RCC_CFG_MCOPRES_DIV6             ((uint32_t)0x50000000) /* LSI/HSI clock is divided by 6 */
#define  RCC_CFG_MCOPRES_DIV7             ((uint32_t)0x60000000) /* LSI/HSI clock is divided by 7 */
#define  RCC_CFG_MCOPRES_DIV8             ((uint32_t)0x70000000) /* LSI/HSI clock is divided by 8 */
#define  RCC_CFG_MCOPRES_DIV9             ((uint32_t)0x80000000) /* LSI/HSI clock is divided by 9 */
#define  RCC_CFG_MCOPRES_DIV10            ((uint32_t)0x90000000) /* LSI/HSI clock is divided by 10 */
#define  RCC_CFG_MCOPRES_DIV11            ((uint32_t)0xA0000000) /* LSI/HSI clock is divided by 11 */
#define  RCC_CFG_MCOPRES_DIV12            ((uint32_t)0xB0000000) /* LSI/HSI clock is divided by 12 */
#define  RCC_CFG_MCOPRES_DIV13            ((uint32_t)0xC0000000) /* LSI/HSI clock is divided by 13 */
#define  RCC_CFG_MCOPRES_DIV14            ((uint32_t)0xD0000000) /* LSI/HSI clock is divided by 14 */
#define  RCC_CFG_MCOPRES_DIV15            ((uint32_t)0xE0000000) /* LSI/HSI clock is divided by 15 */
#define  RCC_CFG_MCOPRES_DIV32            ((uint32_t)0xF0000000) /* LSI/HSI clock is divided by 32 */

/**  Bit definition for RCC_PRST register  **/
#define RCC_PRST_AFIORST    ((uint32_t)0x00000001) /* Alternate Function I/O reset */
#define RCC_PRST_BEEPRST    ((uint32_t)0x00000002) /* Beeper reset */
#define RCC_PRST_GPIOARST   ((uint32_t)0x00000004) /* GPIOA reset */
#define RCC_PRST_GPIOBRST   ((uint32_t)0x00000008) /* GPIOB reset */
#define RCC_PRST_UART1RST   ((uint32_t)0x00000010) /* UART 1 reset */
#define RCC_PRST_UART2RST   ((uint32_t)0x00000020) /* UART 2 reset */
#define RCC_PRST_I2CRST     ((uint32_t)0x00000040) /* I2C reset */
#define RCC_PRST_PWRRST     ((uint32_t)0x00000080) /* Power interface reset */
#define RCC_PRST_ADCRST     ((uint32_t)0x00000100) /* ADC reset */
#define RCC_PRST_SPIRST     ((uint32_t)0x00000400) /* SPI reset */
#define RCC_PRST_TIM1RST    ((uint32_t)0x00000800) /* Timer 1 reset */
#define RCC_PRST_TIM3RST    ((uint32_t)0x00001000) /* Timer 3 reset */
#define RCC_PRST_TIM6RST    ((uint32_t)0x00002000) /* Timer 6 reset */
#define RCC_PRST_COMPRST    ((uint32_t)0x00004000) /* COMP reset */


/** Bit definition for RCC_AHBPCLKEN register **/
#define RCC_AHBPCLKEN_CRCEN    ((uint32_t)0x00000040) /* CRC clock enable */
#define RCC_AHBPCLKEN_ADCEN    ((uint32_t)0x00001000) /* ADC clock enable */

/** Bit definition for RCC_APBPCLKEN register **/
#define RCC_APBPCLKEN_AFIOEN  			 ((uint32_t)0x00000001) /* Alternate Function I/O enable */
#define RCC_APBPCLKEN_BEEPEREN   		 ((uint32_t)0x00000002) /* Beeper enable */
#define RCC_APBPCLKEN_IOPAEN         ((uint32_t)0x00000004) /* I/O port A clock enable */
#define RCC_APBPCLKEN_IOPBEN         ((uint32_t)0x00000008) /* I/O port B clock enable */
#define RCC_APBPCLKEN_COMPEN         ((uint32_t)0x00000010) /* COMP enable */
#define RCC_APBPCLKEN_COMPFILTEN     ((uint32_t)0x00000020) /* COMPFILT enable */
#define RCC_APBPCLKEN_UART1EN        ((uint32_t)0x00000040) /* UART 1 enable */
#define RCC_APBPCLKEN_UART2EN        ((uint32_t)0x00000080) /* UART 2 enable */
#define RCC_APBPCLKEN_I2CEN          ((uint32_t)0x00000100) /* I2C  enable */
#define RCC_APBPCLKEN_PWREN          ((uint32_t)0x00000200) /* Power enable */
#define RCC_APBPCLKEN_SPIEN          ((uint32_t)0x00000400) /* SPI  enable */
#define RCC_APBPCLKEN_TIM1EN         ((uint32_t)0x00000800) /* Timer 1 enable */
#define RCC_APBPCLKEN_TIM3EN         ((uint32_t)0x00001000) /* Timer 3 enable */
#define RCC_APBPCLKEN_TIM6EN         ((uint32_t)0x00002000) /* Timer 6 enable */
#define RCC_APBPCLKEN_IWDGEN         ((uint32_t)0x00004000) /* Independent Watchdog enable */

/**  Bit definition for RCC_LSICTRL register  **/
#define  RCC_LSICTRL_LSIRDF          ((uint32_t)0x00000002)        /* Internal Low Speed clock ready flag */
#define  RCC_LSICTRL_LSITRIM         ((uint32_t)0x0000007C)        /* Internal Low Speed clock trimming */
#define  RCC_LSICTRL_LSIDETEN        ((uint32_t)0x00000080)        /* Internal Low Speed clock detect enable */
#define  RCC_LSICTRL_LSIFREQ         ((uint32_t)0x00FFFF00)        /* Internal Low Speed clock freq cnt */
#define  RCC_LSICTRL_LSIDETFF        ((uint32_t)0x01000000)        /* Internal Low Speed clock detect finish flag */

/** Bit definition for RCC_CTRLSTS register **/
#define RCC_CTRLSTS_RMRSTF          ((uint32_t)0x00000001) /* Remove reset flag */
#define RCC_CTRLSTS_PINRSTF         ((uint32_t)0x00000008) /* PIN reset flag */
#define RCC_CTRLSTS_PORRSTF         ((uint32_t)0x00000010) /* POR/PDR reset flag */
#define RCC_CTRLSTS_SFTRSTF         ((uint32_t)0x00000020) /* Software Reset flag */
#define RCC_CTRLSTS_IWDGRSTF        ((uint32_t)0x00000040) /* Independent Watchdog reset flag */
#define RCC_CTRLSTS_LVRRSTF         ((uint32_t)0x00000080) /* LVR reset flag */
#define RCC_CTRLSTS_LPWRRSTF        ((uint32_t)0x00000100) /* Low-Power reset flag */
#define RCC_CTRLSTS_EMCRSTF         ((uint32_t)0x00000800) /* EMC reset flag */

/** Bit definition for RCC_CFG2 register **/
/** ADCPRES configuration **/
#define RCC_CFG2_ADCPRES  				 ((uint32_t)0x0000000F) /* ADCHSIPRE[3:0] bits */
#define RCC_CFG2_ADCPRES_0 			   ((uint32_t)0x00000001) /* Bit 0 */
#define RCC_CFG2_ADCPRES_1				 ((uint32_t)0x00000002) /* Bit 1 */
#define RCC_CFG2_ADCPRES_2 			   ((uint32_t)0x00000004) /* Bit 2 */
#define RCC_CFG2_ADCPRES_3				 ((uint32_t)0x00000008) /* Bit 3 */

#define RCC_CFG2_ADCPRES_DIV1      ((uint32_t)0x00000000) /* HCLK clock divided by 1 */
#define RCC_CFG2_ADCPRES_DIV2      ((uint32_t)0x00000001) /* HCLK clock divided by 2 */
#define RCC_CFG2_ADCPRES_DIV3      ((uint32_t)0x00000002) /* HCLK clock divided by 3 */
#define RCC_CFG2_ADCPRES_DIV4      ((uint32_t)0x00000003) /* HCLK clock divided by 4 */
#define RCC_CFG2_ADCPRES_DIV6      ((uint32_t)0x00000004) /* HCLK clock divided by 6 */
#define RCC_CFG2_ADCPRES_DIV8      ((uint32_t)0x00000005) /* HCLK clock divided by 8 */
#define RCC_CFG2_ADCPRES_DIV10     ((uint32_t)0x00000006) /* HCLK clock divided by 10 */
#define RCC_CFG2_ADCPRES_DIV12     ((uint32_t)0x00000007) /* HCLK clock divided by 12 */
#define RCC_CFG2_ADCPRES_DIV24     ((uint32_t)0x00000008) /* HCLK clock divided by 24 */
#define RCC_CFG2_ADCPRES_DIV32     ((uint32_t)0x00000009) /* HCLK clock divided by 32 */
#define RCC_CFG2_ADCPRES_OTHERS    ((uint32_t)0x0000000A) /* HCLK clock divided by 32 */

/** ADC1MSEL configuration **/
#define RCC_CFG2_ADCCLK_SEL     		((uint32_t)0x00000010) /* ADC1M clock source select */
#define RCC_CFG2_ADCCLK_SEL_HSI 		((uint32_t)0x00000000) /* HSI clock selected as ADC1M input clock */
#define RCC_CFG2_ADCCLK_SEL_IOM 		((uint32_t)0x00000010) /* IOM test clock selected as ADC1M input clock */

/** ADC1MPRES configuration **/
#define RCC_CFG2_ADC1MPRES   				((uint32_t)0x00000C00) /* ADC1MPRE[4:0] bits */
#define RCC_CFG2_ADC1MPRES_0 				((uint32_t)0x00000400) /* Bit 0 */
#define RCC_CFG2_ADC1MPRES_1 				((uint32_t)0x00000800) /* Bit 1 */

#define RCC_CFG2_ADC1MPRES_DIV6   		((uint32_t)0x00000000) /* ADC1M source clock is divided by 6 */
#define RCC_CFG2_ADC1MPRES_DIV12 		  ((uint32_t)0x00000400) /* ADC1M source clock is divided by 12 */
#define RCC_CFG2_ADC1MPRES_DIV24  		((uint32_t)0x00000800) /* ADC1M source clock is divided by 24 */
#define RCC_CFG2_ADC1MPRES_DIV40 	    ((uint32_t)0x00000C00) /* ADC1M source clock is divided by 40 */
#define RCC_CFG2_ADC1MPRES_DIV48 	    ((uint32_t)0x00000C00) /* ADC1M source clock is divided by 48 */

/** TIM6CLKSEL  configuration **/
#define RCC_CFG2_TIM6CLKSEL         ((uint32_t)0x40000000) /* Timer6 clock source select */
#define RCC_CFG2_TIM6CLKSEL_PCLK    ((uint32_t)0x00000000) /* Timer6 clock selected as APB input clock */
#define RCC_CFG2_TIM6CLKSEL_LSI     ((uint32_t)0x40000000) /* Timer6 clock selected as LSI clock */

/** TIM1CLKSEL  configuration **/
#define RCC_CFG2_TIM1CLKSEL        	   ((uint32_t)0x80000000) /* Timer1 clock source select */
#define RCC_CFG2_TIM1CLKSEL_PCLK        ((uint32_t)0x00000000) /* Timer1 clock selected as APB input clock */
#define RCC_CFG2_TIM1CLKSEL_SYSCLK      ((uint32_t)0x80000000) /* Timer1 clock selected as System clock */

/**  Bit definition for RCC_EMCCTRL register  **/
#define  RCC_EMCCTRL_EMCDETEN                    ((uint32_t)0x00000001) /* EMC Clamp reset enable */
#define  RCC_EMCCTRL_EMCRSTEN                    ((uint32_t)0x00001000) /* EMC Clamp detect enable */

/*** General Purpose and Alternate Function I/O ***/

/** Bit definition for GPIO_PMODE register **/
#define GPIO_PMODE0_Pos            (0)                                   
#define GPIO_PMODE0_Msk            (0x3 << GPIO_PMODE0_Pos)         /* 0x00000003 */ 
#define GPIO_PMODE0                        GPIO_PMODE0_Msk          /* 0x00000003 */
#define GPIO_PMODE0_0              (0x0 << GPIO_PMODE0_Pos)         /* 0x00000000 */
#define GPIO_PMODE0_1              (0x1 << GPIO_PMODE0_Pos)         /* 0x00000001 */
#define GPIO_PMODE0_2              (0x2 << GPIO_PMODE0_Pos)         /* 0x00000002 */
#define GPIO_PMODE0_3              (0x3 << GPIO_PMODE0_Pos)         /* 0x00000003 */

#define GPIO_PMODE1_Pos            (2)                                   
#define GPIO_PMODE1_Msk            (0x3 << GPIO_PMODE1_Pos)         /* 0x0000000C */
#define GPIO_PMODE1                        GPIO_PMODE1_Msk          /* 0x0000000C */
#define GPIO_PMODE1_0              (0x0 << GPIO_PMODE1_Pos)         /* 0x00000000 */
#define GPIO_PMODE1_1              (0x1 << GPIO_PMODE1_Pos)         /* 0x00000004 */
#define GPIO_PMODE1_2              (0x2 << GPIO_PMODE1_Pos)         /* 0x00000008 */
#define GPIO_PMODE1_3              (0x3 << GPIO_PMODE1_Pos)         /* 0x0000000C */

#define GPIO_PMODE2_Pos            (4)                                   
#define GPIO_PMODE2_Msk            (0x3 << GPIO_PMODE2_Pos)         /* 0x00000030 */               
#define GPIO_PMODE2                        GPIO_PMODE2_Msk          /* 0x00000030 */
#define GPIO_PMODE2_0              (0x0 << GPIO_PMODE2_Pos)         /* 0x00000000 */
#define GPIO_PMODE2_1              (0x1 << GPIO_PMODE2_Pos)         /* 0x00000010 */
#define GPIO_PMODE2_2              (0x2 << GPIO_PMODE2_Pos)         /* 0x00000020 */
#define GPIO_PMODE2_3              (0x3 << GPIO_PMODE2_Pos)         /* 0x00000030 */

#define GPIO_PMODE3_Pos            (6)                                   
#define GPIO_PMODE3_Msk            (0x3 << GPIO_PMODE3_Pos)         /* 0x000000C0 */
#define GPIO_PMODE3                        GPIO_PMODE3_Msk          /* 0x000000C0 */
#define GPIO_PMODE3_0              (0x0 << GPIO_PMODE3_Pos)         /* 0x00000000 */
#define GPIO_PMODE3_1              (0x1 << GPIO_PMODE3_Pos)         /* 0x00000040 */
#define GPIO_PMODE3_2              (0x2 << GPIO_PMODE3_Pos)         /* 0x00000080 */
#define GPIO_PMODE3_3              (0x3 << GPIO_PMODE3_Pos)         /* 0x000000C0 */

#define GPIO_PMODE4_Pos            (8)                                   
#define GPIO_PMODE4_Msk            (0x3 << GPIO_PMODE4_Pos)         /* 0x00000300 */ 
#define GPIO_PMODE4                        GPIO_PMODE4_Msk          /* 0x00000300 */
#define GPIO_PMODE4_0              (0x0 << GPIO_PMODE4_Pos)         /* 0x00000000 */
#define GPIO_PMODE4_1              (0x1 << GPIO_PMODE4_Pos)         /* 0x00000100 */
#define GPIO_PMODE4_2              (0x2 << GPIO_PMODE4_Pos)         /* 0x00000100 */
#define GPIO_PMODE4_3              (0x3 << GPIO_PMODE4_Pos)         /* 0x00000300 */

#define GPIO_PMODE5_Pos            (10)                                   
#define GPIO_PMODE5_Msk            (0x3 << GPIO_PMODE5_Pos)         /* 0x00000C00 */
#define GPIO_PMODE5                        GPIO_PMODE5_Msk          /* 0x00000C00 */
#define GPIO_PMODE5_0              (0x0 << GPIO_PMODE5_Pos)         /* 0x00000000 */
#define GPIO_PMODE5_1              (0x1 << GPIO_PMODE5_Pos)         /* 0x00000400 */
#define GPIO_PMODE5_2              (0x2 << GPIO_PMODE5_Pos)         /* 0x00000800 */
#define GPIO_PMODE5_3              (0x3 << GPIO_PMODE5_Pos)         /* 0x00000C00 */

#define GPIO_PMODE6_Pos            (12)                                   
#define GPIO_PMODE6_Msk            (0x3 << GPIO_PMODE6_Pos)         /* 0x00003000 */      
#define GPIO_PMODE6                        GPIO_PMODE6_Msk          /* 0x00003000 */
#define GPIO_PMODE6_0              (0x0 << GPIO_PMODE6_Pos)         /* 0x00000000 */
#define GPIO_PMODE6_1              (0x1 << GPIO_PMODE6_Pos)         /* 0x00001000 */
#define GPIO_PMODE6_2              (0x2 << GPIO_PMODE6_Pos)         /* 0x00002000 */
#define GPIO_PMODE6_3              (0x3 << GPIO_PMODE6_Pos)         /* 0x00003000 */

#define GPIO_PMODE7_Pos            (14)                                   
#define GPIO_PMODE7_Msk            (0x3 << GPIO_PMODE7_Pos)         /* 0x0000C000 */
#define GPIO_PMODE7                        GPIO_PMODE7_Msk          /* 0x0000C000 */
#define GPIO_PMODE7_0              (0x0 << GPIO_PMODE7_Pos)         /* 0x00000000 */
#define GPIO_PMODE7_1              (0x1 << GPIO_PMODE7_Pos)         /* 0x00004000 */
#define GPIO_PMODE7_2              (0x2 << GPIO_PMODE7_Pos)         /* 0x00008000 */
#define GPIO_PMODE7_3              (0x3 << GPIO_PMODE7_Pos)         /* 0x0000C000 */

#define GPIO_PMODE8_Pos            (16)                                   
#define GPIO_PMODE8_Msk            (0x3 << GPIO_PMODE8_Pos)         /* 0x00030000 */
#define GPIO_PMODE8                        GPIO_PMODE8_Msk          /* 0x00030000 */
#define GPIO_PMODE8_0              (0x0 << GPIO_PMODE8_Pos)         /* 0x00000000 */
#define GPIO_PMODE8_1              (0x1 << GPIO_PMODE8_Pos)         /* 0x00010000 */
#define GPIO_PMODE8_2              (0x2 << GPIO_PMODE8_Pos)         /* 0x00020000 */
#define GPIO_PMODE8_3              (0x3 << GPIO_PMODE8_Pos)         /* 0x00030000 */

#define GPIO_PMODE9_Pos            (18)                                   
#define GPIO_PMODE9_Msk            (0x3 << GPIO_PMODE9_Pos)         /* 0x000C0000 */
#define GPIO_PMODE9                        GPIO_PMODE9_Msk          /* 0x000C0000 */
#define GPIO_PMODE9_0              (0x0 << GPIO_PMODE9_Pos)         /* 0x00000000 */
#define GPIO_PMODE9_1              (0x1 << GPIO_PMODE9_Pos)         /* 0x00040000 */
#define GPIO_PMODE9_2              (0x2 << GPIO_PMODE9_Pos)         /* 0x00080000 */
#define GPIO_PMODE9_3              (0x3 << GPIO_PMODE9_Pos)         /* 0x000C0000 */

#define GPIO_PMODE10_Pos           (20)                                   
#define GPIO_PMODE10_Msk           (0x3 << GPIO_PMODE10_Pos)         /* 0x00300000 */ 
#define GPIO_PMODE10                       GPIO_PMODE10_Msk          /* 0x00300000 */
#define GPIO_PMODE10_0             (0x0 << GPIO_PMODE10_Pos)         /* 0x00000000 */
#define GPIO_PMODE10_1             (0x1 << GPIO_PMODE10_Pos)         /* 0x00100000 */
#define GPIO_PMODE10_2             (0x2 << GPIO_PMODE10_Pos)         /* 0x00200000 */
#define GPIO_PMODE10_3             (0x3 << GPIO_PMODE10_Pos)         /* 0x00300000 */

#define GPIO_PMODE11_Pos           (22)                                   
#define GPIO_PMODE11_Msk           (0x3 << GPIO_PMODE11_Pos)         /* 0x00C00000 */ 
#define GPIO_PMODE11                       GPIO_PMODE11_Msk          /* 0x00C00000 */
#define GPIO_PMODE11_0             (0x0 << GPIO_PMODE11_Pos)         /* 0x00000000 */
#define GPIO_PMODE11_1             (0x1 << GPIO_PMODE11_Pos)         /* 0x00400000 */
#define GPIO_PMODE11_2             (0x2 << GPIO_PMODE11_Pos)         /* 0x00800000 */
#define GPIO_PMODE11_3             (0x3 << GPIO_PMODE11_Pos)         /* 0x00C00000 */

#define GPIO_PMODE12_Pos           (24)                                   
#define GPIO_PMODE12_Msk           (0x3 << GPIO_PMODE12_Pos)         /* 0x03000000 */
#define GPIO_PMODE12                       GPIO_PMODE12_Msk          /* 0x03000000 */
#define GPIO_PMODE12_0             (0x0 << GPIO_PMODE12_Pos)         /* 0x00000000 */
#define GPIO_PMODE12_1             (0x1 << GPIO_PMODE12_Pos)         /* 0x01000000 */
#define GPIO_PMODE12_2             (0x2 << GPIO_PMODE12_Pos)         /* 0x02000000 */
#define GPIO_PMODE12_3             (0x3 << GPIO_PMODE12_Pos)         /* 0x03000000 */

#define GPIO_PMODE13_Pos           (26)                                   
#define GPIO_PMODE13_Msk           (0x3 << GPIO_PMODE13_Pos)         /* 0x0C000000 */
#define GPIO_PMODE13                       GPIO_PMODE13_Msk          /* 0x0C000000 */
#define GPIO_PMODE13_0             (0x0 << GPIO_PMODE13_Pos)         /* 0x00000000 */
#define GPIO_PMODE13_1             (0x1 << GPIO_PMODE13_Pos)         /* 0x04000000 */
#define GPIO_PMODE13_2             (0x2 << GPIO_PMODE13_Pos)         /* 0x08000000 */
#define GPIO_PMODE13_3             (0x3 << GPIO_PMODE13_Pos)         /* 0x0C000000 */

#define GPIO_PMODE14_Pos           (28)                                   
#define GPIO_PMODE14_Msk           (0x3 << GPIO_PMODE14_Pos)         /* 0x30000000 */
#define GPIO_PMODE14                       GPIO_PMODE14_Msk          /* 0x30000000 */
#define GPIO_PMODE14_0             (0x0 << GPIO_PMODE14_Pos)         /* 0x00000000 */
#define GPIO_PMODE14_1             (0x1 << GPIO_PMODE14_Pos)         /* 0x10000000 */
#define GPIO_PMODE14_2             (0x2 << GPIO_PMODE14_Pos)         /* 0x20000000 */
#define GPIO_PMODE14_3             (0x3 << GPIO_PMODE14_Pos)         /* 0x30000000 */

#define GPIO_PMODE15_Pos           (30)                                   
#define GPIO_PMODE15_Msk           (0x3 << GPIO_PMODE15_Pos)         /* 0xC0000000 */
#define GPIO_PMODE15                       GPIO_PMODE15_Msk          /* 0xC0000000 */
#define GPIO_PMODE15_0             (0x0 << GPIO_PMODE15_Pos)         /* 0x00000000 */
#define GPIO_PMODE15_1             (0x1 << GPIO_PMODE15_Pos)         /* 0x40000000 */
#define GPIO_PMODE15_2             (0x2 << GPIO_PMODE15_Pos)         /* 0x80000000 */
#define GPIO_PMODE15_3             (0x3 << GPIO_PMODE15_Pos)         /* 0xC0000000 */

/** Bit definition for GPIO_POTYPE register **/
#define GPIO_POTYPE_POT0           ((uint16_t)0x0001) 
#define GPIO_POTYPE_POT1           ((uint16_t)0x0002) 
#define GPIO_POTYPE_POT2           ((uint16_t)0x0004) 
#define GPIO_POTYPE_POT3           ((uint16_t)0x0008) 
#define GPIO_POTYPE_POT4           ((uint16_t)0x0010) 
#define GPIO_POTYPE_POT5           ((uint16_t)0x0020) 
#define GPIO_POTYPE_POT6           ((uint16_t)0x0040) 
#define GPIO_POTYPE_POT7           ((uint16_t)0x0080) 
#define GPIO_POTYPE_POT8           ((uint16_t)0x0100) 
#define GPIO_POTYPE_POT9           ((uint16_t)0x0200) 
#define GPIO_POTYPE_POT10          ((uint16_t)0x0400) 
#define GPIO_POTYPE_POT11          ((uint16_t)0x0800) 
#define GPIO_POTYPE_POT12          ((uint16_t)0x1000) 
#define GPIO_POTYPE_POT13          ((uint16_t)0x2000) 
#define GPIO_POTYPE_POT14          ((uint16_t)0x4000) 
#define GPIO_POTYPE_POT15          ((uint16_t)0x8000) 

/** Bit definition for GPIO_SR register **/
#define GPIO_SR_SR0                ((uint16_t)0x0001) /* Port x Slew Rate bit 0  */
#define GPIO_SR_SR1                ((uint16_t)0x0002) /* Port x Slew Rate bit 1  */
#define GPIO_SR_SR2                ((uint16_t)0x0004) /* Port x Slew Rate bit 2  */
#define GPIO_SR_SR3                ((uint16_t)0x0008) /* Port x Slew Rate bit 3  */
#define GPIO_SR_SR4                ((uint16_t)0x0010) /* Port x Slew Rate bit 4  */
#define GPIO_SR_SR5                ((uint16_t)0x0020) /* Port x Slew Rate bit 5  */
#define GPIO_SR_SR6                ((uint16_t)0x0040) /* Port x Slew Rate bit 6  */
#define GPIO_SR_SR7                ((uint16_t)0x0080) /* Port x Slew Rate bit 7  */
#define GPIO_SR_SR8                ((uint16_t)0x0100) /* Port x Slew Rate bit 8  */
#define GPIO_SR_SR9                ((uint16_t)0x0200) /* Port x Slew Rate bit 9  */
#define GPIO_SR_SR10               ((uint16_t)0x0400) /* Port x Slew Rate bit 10 */
#define GPIO_SR_SR11               ((uint16_t)0x0800) /* Port x Slew Rate bit 11 */
#define GPIO_SR_SR12               ((uint16_t)0x1000) /* Port x Slew Rate bit 12 */
#define GPIO_SR_SR13               ((uint16_t)0x2000) /* Port x Slew Rate bit 13 */
#define GPIO_SR_SR14               ((uint16_t)0x4000) /* Port x Slew Rate bit 14 */
#define GPIO_SR_SR15               ((uint16_t)0x8000) /* Port x Slew Rate bit 15 */

/** Bit definition for GPIO_PUPD register **/
#define GPIO_PUPD0_Pos             (0)                                   
#define GPIO_PUPD0_Msk             (0x3 << GPIO_PUPD0_Pos)         /* 0x00000003 */
#define GPIO_PUPD0                         GPIO_PUPD0_Msk          /* 0x00000003 */     
#define GPIO_PUPD0_0               (0x0 << GPIO_PUPD0_Pos)         /* 0x00000000 */
#define GPIO_PUPD0_1               (0x1 << GPIO_PUPD0_Pos)         /* 0x00000001 */
#define GPIO_PUPD0_2               (0x2 << GPIO_PUPD0_Pos)         /* 0x00000002 */

#define GPIO_PUPD1_Pos             (2)                                   
#define GPIO_PUPD1_Msk             (0x3 << GPIO_PUPD1_Pos)         /* 0x0000000C */
#define GPIO_PUPD1                         GPIO_PUPD1_Msk          /* 0x0000000C */     
#define GPIO_PUPD1_0               (0x0 << GPIO_PUPD1_Pos)         /* 0x00000000 */
#define GPIO_PUPD1_1               (0x1 << GPIO_PUPD1_Pos)         /* 0x00000004 */
#define GPIO_PUPD1_2               (0x2 << GPIO_PUPD1_Pos)         /* 0x00000008 */

#define GPIO_PUPD2_Pos             (4)                                   
#define GPIO_PUPD2_Msk             (0x3 << GPIO_PUPD2_Pos)         /* 0x00000030 */
#define GPIO_PUPD2                         GPIO_PUPD2_Msk          /* 0x00000030 */       
#define GPIO_PUPD2_0               (0x0 << GPIO_PUPD2_Pos)         /* 0x00000000 */
#define GPIO_PUPD2_1               (0x1 << GPIO_PUPD2_Pos)         /* 0x00000010 */
#define GPIO_PUPD2_2               (0x2 << GPIO_PUPD2_Pos)         /* 0x00000020 */

#define GPIO_PUPD3_Pos             (6)                                   
#define GPIO_PUPD3_Msk             (0x3 << GPIO_PUPD3_Pos)         /* 0x000000C0 */
#define GPIO_PUPD3                         GPIO_PUPD3_Msk          /* 0x000000C0 */        
#define GPIO_PUPD3_0               (0x0 << GPIO_PUPD3_Pos)         /* 0x00000000 */
#define GPIO_PUPD3_1               (0x1 << GPIO_PUPD3_Pos)         /* 0x00000040 */
#define GPIO_PUPD3_2               (0x2 << GPIO_PUPD3_Pos)         /* 0x00000080 */

#define GPIO_PUPD4_Pos             (8)                                   
#define GPIO_PUPD4_Msk             (0x3 << GPIO_PUPD4_Pos)         /* 0x00000300 */
#define GPIO_PUPD4                         GPIO_PUPD4_Msk          /* 0x00000300 */         
#define GPIO_PUPD4_0               (0x0 << GPIO_PUPD4_Pos)         /* 0x00000000 */
#define GPIO_PUPD4_1               (0x1 << GPIO_PUPD4_Pos)         /* 0x00000100 */
#define GPIO_PUPD4_2               (0x2 << GPIO_PUPD4_Pos)         /* 0x00000200 */

#define GPIO_PUPD5_Pos             (10)                                   
#define GPIO_PUPD5_Msk             (0x3 << GPIO_PUPD5_Pos)         /* 0x00000C00 */
#define GPIO_PUPD5                         GPIO_PUPD5_Msk          /* 0x00000C00 */         
#define GPIO_PUPD5_0               (0x0 << GPIO_PUPD5_Pos)         /* 0x00000000 */
#define GPIO_PUPD5_1               (0x1 << GPIO_PUPD5_Pos)         /* 0x00000400 */
#define GPIO_PUPD5_2               (0x2 << GPIO_PUPD5_Pos)         /* 0x00000800 */

#define GPIO_PUPD6_Pos             (12)                                   
#define GPIO_PUPD6_Msk             (0x3 << GPIO_PUPD6_Pos)         /* 0x00003000 */
#define GPIO_PUPD6                         GPIO_PUPD6_Msk          /* 0x00003000 */         
#define GPIO_PUPD6_0               (0x0 << GPIO_PUPD6_Pos)         /* 0x00000000 */
#define GPIO_PUPD6_1               (0x1 << GPIO_PUPD6_Pos)         /* 0x00001000 */
#define GPIO_PUPD6_2               (0x2 << GPIO_PUPD6_Pos)         /* 0x00002000 */

#define GPIO_PUPD7_Pos             (14)                                   
#define GPIO_PUPD7_Msk             (0x3 << GPIO_PUPD7_Pos)         /* 0x0000C000 */
#define GPIO_PUPD7                         GPIO_PUPD7_Msk          /* 0x0000C000 */         
#define GPIO_PUPD7_0               (0x0 << GPIO_PUPD7_Pos)         /* 0x00000000 */
#define GPIO_PUPD7_1               (0x1 << GPIO_PUPD7_Pos)         /* 0x00004000 */
#define GPIO_PUPD7_2               (0x2 << GPIO_PUPD7_Pos)         /* 0x00008000 */

#define GPIO_PUPD8_Pos             (16)                                   
#define GPIO_PUPD8_Msk             (0x3 << GPIO_PUPD8_Pos)         /* 0x00030000 */
#define GPIO_PUPD8                         GPIO_PUPD8_Msk          /* 0x00030000 */         
#define GPIO_PUPD8_0               (0x0 << GPIO_PUPD8_Pos)         /* 0x00000000 */
#define GPIO_PUPD8_1               (0x1 << GPIO_PUPD8_Pos)         /* 0x00010000 */
#define GPIO_PUPD8_2               (0x2 << GPIO_PUPD8_Pos)         /* 0x00020000 */

#define GPIO_PUPD9_Pos             (18)                                   
#define GPIO_PUPD9_Msk             (0x3 << GPIO_PUPD9_Pos)         /* 0x000C0000 */
#define GPIO_PUPD9                         GPIO_PUPD9_Msk          /* 0x000C0000 */         
#define GPIO_PUPD9_0               (0x0 << GPIO_PUPD9_Pos)         /* 0x00000000 */
#define GPIO_PUPD9_1               (0x1 << GPIO_PUPD9_Pos)         /* 0x00040000 */
#define GPIO_PUPD9_2               (0x2 << GPIO_PUPD9_Pos)         /* 0x00080000 */

#define GPIO_PUPD10_Pos            (20)                                   
#define GPIO_PUPD10_Msk            (0x3 << GPIO_PUPD10_Pos)        /* 0x00300000 */
#define GPIO_PUPD10                        GPIO_PUPD10_Msk         /* 0x00300000 */         
#define GPIO_PUPD10_0              (0x0 << GPIO_PUPD10_Pos)        /* 0x00000000 */
#define GPIO_PUPD10_1              (0x1 << GPIO_PUPD10_Pos)        /* 0x00100000 */
#define GPIO_PUPD10_2              (0x2 << GPIO_PUPD10_Pos)        /* 0x00200000 */

#define GPIO_PUPD11_Pos            (22)                                   
#define GPIO_PUPD11_Msk            (0x3 << GPIO_PUPD11_Pos)        /* 0x00C00000 */
#define GPIO_PUPD11                        GPIO_PUPD11_Msk         /* 0x00C00000 */         
#define GPIO_PUPD11_0              (0x0 << GPIO_PUPD11_Pos)        /* 0x00000000 */
#define GPIO_PUPD11_1              (0x1 << GPIO_PUPD11_Pos)        /* 0x00400000 */
#define GPIO_PUPD11_2              (0x2 << GPIO_PUPD11_Pos)        /* 0x00800000 */

#define GPIO_PUPD12_Pos            (24)                                   
#define GPIO_PUPD12_Msk            (0x3 << GPIO_PUPD12_Pos)        /* 0x03000000 */
#define GPIO_PUPD12                        GPIO_PUPD12_Msk         /* 0x03000000 */         
#define GPIO_PUPD12_0              (0x0 << GPIO_PUPD12_Pos)        /* 0x00000000 */
#define GPIO_PUPD12_1              (0x1 << GPIO_PUPD12_Pos)        /* 0x01000000 */
#define GPIO_PUPD12_2              (0x2 << GPIO_PUPD12_Pos)        /* 0x02000000 */

#define GPIO_PUPD13_Pos            (26)                                   
#define GPIO_PUPD13_Msk            (0x3 << GPIO_PUPD13_Pos)         /* 0x0C000000 */
#define GPIO_PUPD13                        GPIO_PUPD13_Msk          /* 0x0C000000 */         
#define GPIO_PUPD13_0              (0x0 << GPIO_PUPD13_Pos)         /* 0x00000000 */
#define GPIO_PUPD13_1              (0x1 << GPIO_PUPD13_Pos)         /* 0x04000000 */
#define GPIO_PUPD13_2              (0x2 << GPIO_PUPD13_Pos)         /* 0x08000000 */

#define GPIO_PUPD14_Pos            (28)                                   
#define GPIO_PUPD14_Msk            (0x3 << GPIO_PUPD14_Pos)         /* 0x30000000 */
#define GPIO_PUPD14                        GPIO_PUPD14_Msk          /* 0x30000000 */ 
#define GPIO_PUPD14_0              (0x0 << GPIO_PUPD14_Pos)         /* 0x00000000 */
#define GPIO_PUPD14_1              (0x1 << GPIO_PUPD14_Pos)         /* 0x10000000 */
#define GPIO_PUPD14_2              (0x2 << GPIO_PUPD14_Pos)         /* 0x30000000 */

#define GPIO_PUPD15_Pos            (30)                                   
#define GPIO_PUPD15_Msk            (0x3 << GPIO_PUPD15_Pos)         /* 0xC0000000 */
#define GPIO_PUPD15                        GPIO_PUPD15_Msk          /* 0xC0000000 */         
#define GPIO_PUPD15_0              (0x0 << GPIO_PUPD15_Pos)         /* 0x00000000 */
#define GPIO_PUPD15_1              (0x1 << GPIO_PUPD15_Pos)         /* 0x40000000 */
#define GPIO_PUPD15_2              (0x2 << GPIO_PUPD15_Pos)         /* 0x80000000 */

/** Bit definition for GPIO_PID register **/
#define GPIO_PID_PID0              ((uint16_t)0x0001) /* Port input data, bit 0  */
#define GPIO_PID_PID1              ((uint16_t)0x0002) /* Port input data, bit 1  */
#define GPIO_PID_PID2              ((uint16_t)0x0004) /* Port input data, bit 2  */
#define GPIO_PID_PID3              ((uint16_t)0x0008) /* Port input data, bit 3  */
#define GPIO_PID_PID4              ((uint16_t)0x0010) /* Port input data, bit 4  */
#define GPIO_PID_PID5              ((uint16_t)0x0020) /* Port input data, bit 5  */
#define GPIO_PID_PID6              ((uint16_t)0x0040) /* Port input data, bit 6  */
#define GPIO_PID_PID7              ((uint16_t)0x0080) /* Port input data, bit 7  */
#define GPIO_PID_PID8              ((uint16_t)0x0100) /* Port input data, bit 8  */
#define GPIO_PID_PID9              ((uint16_t)0x0200) /* Port input data, bit 9  */
#define GPIO_PID_PID10             ((uint16_t)0x0400) /* Port input data, bit 10 */
#define GPIO_PID_PID11             ((uint16_t)0x0800) /* Port input data, bit 11 */
#define GPIO_PID_PID12             ((uint16_t)0x1000) /* Port input data, bit 12 */
#define GPIO_PID_PID13             ((uint16_t)0x2000) /* Port input data, bit 13 */
#define GPIO_PID_PID14             ((uint16_t)0x4000) /* Port input data, bit 14 */
#define GPIO_PID_PID15             ((uint16_t)0x8000) /* Port input data, bit 15 */

/** Bit definition for GPIO_POD register **/
#define GPIO_POD_POD0              ((uint16_t)0x0001) /* Port output data, bit 0  */
#define GPIO_POD_POD1              ((uint16_t)0x0002) /* Port output data, bit 1  */
#define GPIO_POD_POD2              ((uint16_t)0x0004) /* Port output data, bit 2  */
#define GPIO_POD_POD3              ((uint16_t)0x0008) /* Port output data, bit 3  */
#define GPIO_POD_POD4              ((uint16_t)0x0010) /* Port output data, bit 4  */
#define GPIO_POD_POD5              ((uint16_t)0x0020) /* Port output data, bit 5  */
#define GPIO_POD_POD6              ((uint16_t)0x0040) /* Port output data, bit 6  */
#define GPIO_POD_POD7              ((uint16_t)0x0080) /* Port output data, bit 7  */
#define GPIO_POD_POD8              ((uint16_t)0x0100) /* Port output data, bit 8  */
#define GPIO_POD_POD9              ((uint16_t)0x0200) /* Port output data, bit 9  */
#define GPIO_POD_POD10             ((uint16_t)0x0400) /* Port output data, bit 10 */
#define GPIO_POD_POD11             ((uint16_t)0x0800) /* Port output data, bit 11 */
#define GPIO_POD_POD12             ((uint16_t)0x1000) /* Port output data, bit 12 */
#define GPIO_POD_POD13             ((uint16_t)0x2000) /* Port output data, bit 13 */
#define GPIO_POD_POD14             ((uint16_t)0x4000) /* Port output data, bit 14 */
#define GPIO_POD_POD15             ((uint16_t)0x8000) /* Port output data, bit 15 */

/** Bit definition for GPIO_PBSC register **/
#define GPIO_PBSC_PBS0             ((uint32_t)0x00000001) /* Port x Set bit 0  */
#define GPIO_PBSC_PBS1             ((uint32_t)0x00000002) /* Port x Set bit 1  */
#define GPIO_PBSC_PBS2             ((uint32_t)0x00000004) /* Port x Set bit 2  */
#define GPIO_PBSC_PBS3             ((uint32_t)0x00000008) /* Port x Set bit 3  */
#define GPIO_PBSC_PBS4             ((uint32_t)0x00000010) /* Port x Set bit 4  */
#define GPIO_PBSC_PBS5             ((uint32_t)0x00000020) /* Port x Set bit 5  */
#define GPIO_PBSC_PBS6             ((uint32_t)0x00000040) /* Port x Set bit 6  */
#define GPIO_PBSC_PBS7             ((uint32_t)0x00000080) /* Port x Set bit 7  */
#define GPIO_PBSC_PBS8             ((uint32_t)0x00000100) /* Port x Set bit 8  */
#define GPIO_PBSC_PBS9             ((uint32_t)0x00000200) /* Port x Set bit 9  */
#define GPIO_PBSC_PBS10            ((uint32_t)0x00000400) /* Port x Set bit 10 */
#define GPIO_PBSC_PBS11            ((uint32_t)0x00000800) /* Port x Set bit 11 */
#define GPIO_PBSC_PBS12            ((uint32_t)0x00001000) /* Port x Set bit 12 */
#define GPIO_PBSC_PBS13            ((uint32_t)0x00002000) /* Port x Set bit 13 */
#define GPIO_PBSC_PBS14            ((uint32_t)0x00004000) /* Port x Set bit 14 */
#define GPIO_PBSC_PBS15            ((uint32_t)0x00008000) /* Port x Set bit 15 */

#define GPIO_PBSC_PBC0             ((uint32_t)0x00010000) /* Port x Reset bit 0  */
#define GPIO_PBSC_PBC1             ((uint32_t)0x00020000) /* Port x Reset bit 1  */
#define GPIO_PBSC_PBC2             ((uint32_t)0x00040000) /* Port x Reset bit 2  */
#define GPIO_PBSC_PBC3             ((uint32_t)0x00080000) /* Port x Reset bit 3  */
#define GPIO_PBSC_PBC4             ((uint32_t)0x00100000) /* Port x Reset bit 4  */
#define GPIO_PBSC_PBC5             ((uint32_t)0x00200000) /* Port x Reset bit 5  */
#define GPIO_PBSC_PBC6             ((uint32_t)0x00400000) /* Port x Reset bit 6  */
#define GPIO_PBSC_PBC7             ((uint32_t)0x00800000) /* Port x Reset bit 7  */
#define GPIO_PBSC_PBC8             ((uint32_t)0x01000000) /* Port x Reset bit 8  */
#define GPIO_PBSC_PBC9             ((uint32_t)0x02000000) /* Port x Reset bit 9  */
#define GPIO_PBSC_PBC10            ((uint32_t)0x04000000) /* Port x Reset bit 10 */
#define GPIO_PBSC_PBC11            ((uint32_t)0x08000000) /* Port x Reset bit 11 */
#define GPIO_PBSC_PBC12            ((uint32_t)0x10000000) /* Port x Reset bit 12 */
#define GPIO_PBSC_PBC13            ((uint32_t)0x20000000) /* Port x Reset bit 13 */
#define GPIO_PBSC_PBC14            ((uint32_t)0x40000000) /* Port x Reset bit 14 */
#define GPIO_PBSC_PBC15            ((uint32_t)0x80000000) /* Port x Reset bit 15 */

/** Bit definition for GPIO_PBC register **/
#define GPIO_PBC_PBC0              ((uint16_t)0x0001) /* Port x Reset bit 0  */
#define GPIO_PBC_PBC1              ((uint16_t)0x0002) /* Port x Reset bit 1  */
#define GPIO_PBC_PBC2              ((uint16_t)0x0004) /* Port x Reset bit 2  */
#define GPIO_PBC_PBC3              ((uint16_t)0x0008) /* Port x Reset bit 3  */
#define GPIO_PBC_PBC4              ((uint16_t)0x0010) /* Port x Reset bit 4  */
#define GPIO_PBC_PBC5              ((uint16_t)0x0020) /* Port x Reset bit 5  */
#define GPIO_PBC_PBC6              ((uint16_t)0x0040) /* Port x Reset bit 6  */
#define GPIO_PBC_PBC7              ((uint16_t)0x0080) /* Port x Reset bit 7  */
#define GPIO_PBC_PBC8              ((uint16_t)0x0100) /* Port x Reset bit 8  */
#define GPIO_PBC_PBC9              ((uint16_t)0x0200) /* Port x Reset bit 9  */
#define GPIO_PBC_PBC10             ((uint16_t)0x0400) /* Port x Reset bit 10 */
#define GPIO_PBC_PBC11             ((uint16_t)0x0800) /* Port x Reset bit 11 */
#define GPIO_PBC_PBC12             ((uint16_t)0x1000) /* Port x Reset bit 12 */
#define GPIO_PBC_PBC13             ((uint16_t)0x2000) /* Port x Reset bit 13 */
#define GPIO_PBC_PBC14             ((uint16_t)0x4000) /* Port x Reset bit 14 */
#define GPIO_PBC_PBC15             ((uint16_t)0x8000) /* Port x Reset bit 15 */

/**  Bit definition for GPIO_PLOCK register **/
#define GPIO_PLOCK_PLOCK0          ((uint32_t)0x00000001) /* Port x Lock bit 0  */
#define GPIO_PLOCK_PLOCK1          ((uint32_t)0x00000002) /* Port x Lock bit 1  */
#define GPIO_PLOCK_PLOCK2          ((uint32_t)0x00000004) /* Port x Lock bit 2  */
#define GPIO_PLOCK_PLOCK3          ((uint32_t)0x00000008) /* Port x Lock bit 3  */
#define GPIO_PLOCK_PLOCK4          ((uint32_t)0x00000010) /* Port x Lock bit 4  */
#define GPIO_PLOCK_PLOCK5          ((uint32_t)0x00000020) /* Port x Lock bit 5  */
#define GPIO_PLOCK_PLOCK6          ((uint32_t)0x00000040) /* Port x Lock bit 6  */
#define GPIO_PLOCK_PLOCK7          ((uint32_t)0x00000080) /* Port x Lock bit 7  */
#define GPIO_PLOCK_PLOCK8          ((uint32_t)0x00000100) /* Port x Lock bit 8  */
#define GPIO_PLOCK_PLOCK9          ((uint32_t)0x00000200) /* Port x Lock bit 9  */
#define GPIO_PLOCK_PLOCK10         ((uint32_t)0x00000400) /* Port x Lock bit 10 */
#define GPIO_PLOCK_PLOCK11         ((uint32_t)0x00000800) /* Port x Lock bit 11 */
#define GPIO_PLOCK_PLOCK12         ((uint32_t)0x00001000) /* Port x Lock bit 12 */
#define GPIO_PLOCK_PLOCK13         ((uint32_t)0x00002000) /* Port x Lock bit 13 */
#define GPIO_PLOCK_PLOCK14         ((uint32_t)0x00004000) /* Port x Lock bit 14 */
#define GPIO_PLOCK_PLOCK15         ((uint32_t)0x00008000) /* Port x Lock bit 15 */
#define GPIO_PLOCK_PLOCKK          ((uint32_t)0x00010000) /* Lock key */

/** Bit definition for GPIO_AFL register **/
#define GPIO_AFL_AFSEL0            ((uint32_t)0x0000000F) /* Port x AFL bit (0..3) */
#define GPIO_AFL_AFSEL1            ((uint32_t)0x000000F0) /* Port x AFL bit (4..7) */
#define GPIO_AFL_AFSEL2            ((uint32_t)0x00000F00) /* Port x AFL bit (8..11) */
#define GPIO_AFL_AFSEL3            ((uint32_t)0x0000F000) /* Port x AFL bit (12..15) */
#define GPIO_AFL_AFSEL4            ((uint32_t)0x000F0000) /* Port x AFL bit (16..19) */
#define GPIO_AFL_AFSEL5            ((uint32_t)0x00F00000) /* Port x AFL bit (20..23) */
#define GPIO_AFL_AFSEL6            ((uint32_t)0x0F000000) /* Port x AFL bit (24..27) */
#define GPIO_AFL_AFSEL7            ((uint32_t)0xF0000000) /* Port x AFL bit (27..31) */

/** Bit definition for GPIO_AFH register **/
#define GPIO_AFH_AFSEL8            ((uint32_t)0x0000000F) /* Port x AFH bit (0..3) */
#define GPIO_AFH_AFSEL9            ((uint32_t)0x000000F0) /* Port x AFH bit (4..7) */
#define GPIO_AFH_AFSEL10           ((uint32_t)0x00000F00) /* Port x AFH bit (8..11) */
#define GPIO_AFH_AFSEL11           ((uint32_t)0x0000F000) /* Port x AFH bit (12..15) */
#define GPIO_AFH_AFSEL12           ((uint32_t)0x000F0000) /* Port x AFH bit (16..19) */
#define GPIO_AFH_AFSEL13           ((uint32_t)0x00F00000) /* Port x AFH bit (20..23) */
#define GPIO_AFH_AFSEL14           ((uint32_t)0x0F000000) /* Port x AFH bit (24..27) */
#define GPIO_AFH_AFSEL15           ((uint32_t)0xF0000000) /* Port x AFH bit (27..31) */

/** Bit definition for GPIO_DS register **/
#define GPIO_DS_DS0                ((uint16_t)0x0001) /* Port x Reset bit 0  */
#define GPIO_DS_DS1                ((uint16_t)0x0002) /* Port x Reset bit 1  */
#define GPIO_DS_DS2                ((uint16_t)0x0004) /* Port x Reset bit 2  */
#define GPIO_DS_DS3                ((uint16_t)0x0008) /* Port x Reset bit 3  */
#define GPIO_DS_DS4                ((uint16_t)0x0010) /* Port x Reset bit 4  */
#define GPIO_DS_DS5                ((uint16_t)0x0020) /* Port x Reset bit 5  */
#define GPIO_DS_DS6                ((uint16_t)0x0040) /* Port x Reset bit 6  */
#define GPIO_DS_DS7                ((uint16_t)0x0080) /* Port x Reset bit 7  */
#define GPIO_DS_DS8                ((uint16_t)0x0100) /* Port x Reset bit 8  */
#define GPIO_DS_DS9                ((uint16_t)0x0200) /* Port x Reset bit 9  */
#define GPIO_DS_DS10               ((uint16_t)0x0400) /* Port x Reset bit 10 */
#define GPIO_DS_DS11               ((uint16_t)0x0800) /* Port x Reset bit 11 */
#define GPIO_DS_DS12               ((uint16_t)0x1000) /* Port x Reset bit 12 */
#define GPIO_DS_DS13               ((uint16_t)0x2000) /* Port x Reset bit 13 */
#define GPIO_DS_DS14               ((uint16_t)0x4000) /* Port x Reset bit 14 */
#define GPIO_DS_DS15               ((uint16_t)0x8000) /* Port x Reset bit 15 */

/** Bit definition for AFIO_CFG register **/
#define AFIO_CFG_MASK              ((uint16_t)0x001F) /* EXTI_ETRR bit mask */
#define AFIO_CFG0                  ((uint16_t)0x0001) /* AFIO bit 0        */
#define AFIO_CFG1                  ((uint16_t)0x0002) /* AFIO bit 1        */
#define AFIO_CFG2                  ((uint16_t)0x0004) /* AFIO bit 2        */
#define AFIO_CFG3                  ((uint16_t)0x0008) /* AFIO bit 3        */
#define AFIO_CFG4                  ((uint16_t)0x0010) /* AFIO bit 4        */
#define AFIO_CFG_TIM3CH2_MAP       ((uint16_t)0x0020) /* AFIO bit 5        */
#define AFIO_CFG_SPI_NSS           ((uint16_t)0x0040) /* AFIO bit 6        */

/*** SystemTick ***/

/** Bit definition for SysTick_CTRL register **/
#define SysTick_CTRL_ENABLE    ((uint32_t)0x00000001) /* Counter enable */
#define SysTick_CTRL_TICKINT   ((uint32_t)0x00000002) /* Counting down to 0 pends the SysTick handler */
#define SysTick_CTRL_CLKSOURCE ((uint32_t)0x00000004) /* Clock source */
#define SysTick_CTRL_COUNTFLAG ((uint32_t)0x00010000) /* Count Flag */

/** Bit definition for SysTick_LOAD register **/
#define SysTick_LOAD_RELOAD ((uint32_t)0x00FFFFFF) /* Value to load into the SysTick Current Value Register when the counter reaches 0 */
/** Bit definition for SysTick_VAL register **/
#define SysTick_VAL_CURRENT ((uint32_t)0x00FFFFFF) /* Current value at the time the register is accessed */

/** Bit definition for SysTick_CALIB register **/
#define SysTick_CALIB_TENMS ((uint32_t)0x00FFFFFF) /* Reload value to use for 10ms timing */
#define SysTick_CALIB_SKEW  ((uint32_t)0x40000000) /* Calibration value is not exactly 10 ms */
#define SysTick_CALIB_NOREF ((uint32_t)0x80000000) /* The reference clock is not provided */


/*** Nested Vectored Interrupt Controller ***/

/** Bit definition for NVIC_ISER register **/
#define NVIC_ISER_SETENA    ((uint32_t)0xFFFFFFFF) /* Interrupt set enable bits */
#define NVIC_ISER_SETENA_0  ((uint32_t)0x00000001) /* bit 0 */
#define NVIC_ISER_SETENA_1  ((uint32_t)0x00000002) /* bit 1 */
#define NVIC_ISER_SETENA_2  ((uint32_t)0x00000004) /* bit 2 */
#define NVIC_ISER_SETENA_3  ((uint32_t)0x00000008) /* bit 3 */
#define NVIC_ISER_SETENA_4  ((uint32_t)0x00000010) /* bit 4 */
#define NVIC_ISER_SETENA_5  ((uint32_t)0x00000020) /* bit 5 */
#define NVIC_ISER_SETENA_6  ((uint32_t)0x00000040) /* bit 6 */
#define NVIC_ISER_SETENA_7  ((uint32_t)0x00000080) /* bit 7 */
#define NVIC_ISER_SETENA_8  ((uint32_t)0x00000100) /* bit 8 */
#define NVIC_ISER_SETENA_9  ((uint32_t)0x00000200) /* bit 9 */
#define NVIC_ISER_SETENA_10 ((uint32_t)0x00000400) /* bit 10 */
#define NVIC_ISER_SETENA_11 ((uint32_t)0x00000800) /* bit 11 */
#define NVIC_ISER_SETENA_12 ((uint32_t)0x00001000) /* bit 12 */
#define NVIC_ISER_SETENA_13 ((uint32_t)0x00002000) /* bit 13 */
#define NVIC_ISER_SETENA_14 ((uint32_t)0x00004000) /* bit 14 */
#define NVIC_ISER_SETENA_15 ((uint32_t)0x00008000) /* bit 15 */
#define NVIC_ISER_SETENA_16 ((uint32_t)0x00010000) /* bit 16 */
#define NVIC_ISER_SETENA_17 ((uint32_t)0x00020000) /* bit 17 */
#define NVIC_ISER_SETENA_18 ((uint32_t)0x00040000) /* bit 18 */
#define NVIC_ISER_SETENA_19 ((uint32_t)0x00080000) /* bit 19 */
#define NVIC_ISER_SETENA_20 ((uint32_t)0x00100000) /* bit 20 */
#define NVIC_ISER_SETENA_21 ((uint32_t)0x00200000) /* bit 21 */
#define NVIC_ISER_SETENA_22 ((uint32_t)0x00400000) /* bit 22 */
#define NVIC_ISER_SETENA_23 ((uint32_t)0x00800000) /* bit 23 */
#define NVIC_ISER_SETENA_24 ((uint32_t)0x01000000) /* bit 24 */
#define NVIC_ISER_SETENA_25 ((uint32_t)0x02000000) /* bit 25 */
#define NVIC_ISER_SETENA_26 ((uint32_t)0x04000000) /* bit 26 */
#define NVIC_ISER_SETENA_27 ((uint32_t)0x08000000) /* bit 27 */
#define NVIC_ISER_SETENA_28 ((uint32_t)0x10000000) /* bit 28 */
#define NVIC_ISER_SETENA_29 ((uint32_t)0x20000000) /* bit 29 */
#define NVIC_ISER_SETENA_30 ((uint32_t)0x40000000) /* bit 30 */
#define NVIC_ISER_SETENA_31 ((uint32_t)0x80000000) /* bit 31 */

/** Bit definition for NVIC_ICER register ***/
#define NVIC_ICER_CLRENA    ((uint32_t)0xFFFFFFFF) /* Interrupt clear-enable bits */
#define NVIC_ICER_CLRENA_0  ((uint32_t)0x00000001) /* bit 0 */
#define NVIC_ICER_CLRENA_1  ((uint32_t)0x00000002) /* bit 1 */
#define NVIC_ICER_CLRENA_2  ((uint32_t)0x00000004) /* bit 2 */
#define NVIC_ICER_CLRENA_3  ((uint32_t)0x00000008) /* bit 3 */
#define NVIC_ICER_CLRENA_4  ((uint32_t)0x00000010) /* bit 4 */
#define NVIC_ICER_CLRENA_5  ((uint32_t)0x00000020) /* bit 5 */
#define NVIC_ICER_CLRENA_6  ((uint32_t)0x00000040) /* bit 6 */
#define NVIC_ICER_CLRENA_7  ((uint32_t)0x00000080) /* bit 7 */
#define NVIC_ICER_CLRENA_8  ((uint32_t)0x00000100) /* bit 8 */
#define NVIC_ICER_CLRENA_9  ((uint32_t)0x00000200) /* bit 9 */
#define NVIC_ICER_CLRENA_10 ((uint32_t)0x00000400) /* bit 10 */
#define NVIC_ICER_CLRENA_11 ((uint32_t)0x00000800) /* bit 11 */
#define NVIC_ICER_CLRENA_12 ((uint32_t)0x00001000) /* bit 12 */
#define NVIC_ICER_CLRENA_13 ((uint32_t)0x00002000) /* bit 13 */
#define NVIC_ICER_CLRENA_14 ((uint32_t)0x00004000) /* bit 14 */
#define NVIC_ICER_CLRENA_15 ((uint32_t)0x00008000) /* bit 15 */
#define NVIC_ICER_CLRENA_16 ((uint32_t)0x00010000) /* bit 16 */
#define NVIC_ICER_CLRENA_17 ((uint32_t)0x00020000) /* bit 17 */
#define NVIC_ICER_CLRENA_18 ((uint32_t)0x00040000) /* bit 18 */
#define NVIC_ICER_CLRENA_19 ((uint32_t)0x00080000) /* bit 19 */
#define NVIC_ICER_CLRENA_20 ((uint32_t)0x00100000) /* bit 20 */
#define NVIC_ICER_CLRENA_21 ((uint32_t)0x00200000) /* bit 21 */
#define NVIC_ICER_CLRENA_22 ((uint32_t)0x00400000) /* bit 22 */
#define NVIC_ICER_CLRENA_23 ((uint32_t)0x00800000) /* bit 23 */
#define NVIC_ICER_CLRENA_24 ((uint32_t)0x01000000) /* bit 24 */
#define NVIC_ICER_CLRENA_25 ((uint32_t)0x02000000) /* bit 25 */
#define NVIC_ICER_CLRENA_26 ((uint32_t)0x04000000) /* bit 26 */
#define NVIC_ICER_CLRENA_27 ((uint32_t)0x08000000) /* bit 27 */
#define NVIC_ICER_CLRENA_28 ((uint32_t)0x10000000) /* bit 28 */
#define NVIC_ICER_CLRENA_29 ((uint32_t)0x20000000) /* bit 29 */
#define NVIC_ICER_CLRENA_30 ((uint32_t)0x40000000) /* bit 30 */
#define NVIC_ICER_CLRENA_31 ((uint32_t)0x80000000) /* bit 31 */

/** Bit definition for NVIC_ISPR register **/
#define NVIC_ISPR_SETPEND    ((uint32_t)0xFFFFFFFF) /* Interrupt set-pending bits */
#define NVIC_ISPR_SETPEND_0  ((uint32_t)0x00000001) /* bit 0 */
#define NVIC_ISPR_SETPEND_1  ((uint32_t)0x00000002) /* bit 1 */
#define NVIC_ISPR_SETPEND_2  ((uint32_t)0x00000004) /* bit 2 */
#define NVIC_ISPR_SETPEND_3  ((uint32_t)0x00000008) /* bit 3 */
#define NVIC_ISPR_SETPEND_4  ((uint32_t)0x00000010) /* bit 4 */
#define NVIC_ISPR_SETPEND_5  ((uint32_t)0x00000020) /* bit 5 */
#define NVIC_ISPR_SETPEND_6  ((uint32_t)0x00000040) /* bit 6 */
#define NVIC_ISPR_SETPEND_7  ((uint32_t)0x00000080) /* bit 7 */
#define NVIC_ISPR_SETPEND_8  ((uint32_t)0x00000100) /* bit 8 */
#define NVIC_ISPR_SETPEND_9  ((uint32_t)0x00000200) /* bit 9 */
#define NVIC_ISPR_SETPEND_10 ((uint32_t)0x00000400) /* bit 10 */
#define NVIC_ISPR_SETPEND_11 ((uint32_t)0x00000800) /* bit 11 */
#define NVIC_ISPR_SETPEND_12 ((uint32_t)0x00001000) /* bit 12 */
#define NVIC_ISPR_SETPEND_13 ((uint32_t)0x00002000) /* bit 13 */
#define NVIC_ISPR_SETPEND_14 ((uint32_t)0x00004000) /* bit 14 */
#define NVIC_ISPR_SETPEND_15 ((uint32_t)0x00008000) /* bit 15 */
#define NVIC_ISPR_SETPEND_16 ((uint32_t)0x00010000) /* bit 16 */
#define NVIC_ISPR_SETPEND_17 ((uint32_t)0x00020000) /* bit 17 */
#define NVIC_ISPR_SETPEND_18 ((uint32_t)0x00040000) /* bit 18 */
#define NVIC_ISPR_SETPEND_19 ((uint32_t)0x00080000) /* bit 19 */
#define NVIC_ISPR_SETPEND_20 ((uint32_t)0x00100000) /* bit 20 */
#define NVIC_ISPR_SETPEND_21 ((uint32_t)0x00200000) /* bit 21 */
#define NVIC_ISPR_SETPEND_22 ((uint32_t)0x00400000) /* bit 22 */
#define NVIC_ISPR_SETPEND_23 ((uint32_t)0x00800000) /* bit 23 */
#define NVIC_ISPR_SETPEND_24 ((uint32_t)0x01000000) /* bit 24 */
#define NVIC_ISPR_SETPEND_25 ((uint32_t)0x02000000) /* bit 25 */
#define NVIC_ISPR_SETPEND_26 ((uint32_t)0x04000000) /* bit 26 */
#define NVIC_ISPR_SETPEND_27 ((uint32_t)0x08000000) /* bit 27 */
#define NVIC_ISPR_SETPEND_28 ((uint32_t)0x10000000) /* bit 28 */
#define NVIC_ISPR_SETPEND_29 ((uint32_t)0x20000000) /* bit 29 */
#define NVIC_ISPR_SETPEND_30 ((uint32_t)0x40000000) /* bit 30 */
#define NVIC_ISPR_SETPEND_31 ((uint32_t)0x80000000) /* bit 31 */

/** Bit definition for NVIC_ICPR register **/
#define NVIC_ICPR_CLRPEND    ((uint32_t)0xFFFFFFFF) /* Interrupt clear-pending bits */
#define NVIC_ICPR_CLRPEND_0  ((uint32_t)0x00000001) /* bit 0 */
#define NVIC_ICPR_CLRPEND_1  ((uint32_t)0x00000002) /* bit 1 */
#define NVIC_ICPR_CLRPEND_2  ((uint32_t)0x00000004) /* bit 2 */
#define NVIC_ICPR_CLRPEND_3  ((uint32_t)0x00000008) /* bit 3 */
#define NVIC_ICPR_CLRPEND_4  ((uint32_t)0x00000010) /* bit 4 */
#define NVIC_ICPR_CLRPEND_5  ((uint32_t)0x00000020) /* bit 5 */
#define NVIC_ICPR_CLRPEND_6  ((uint32_t)0x00000040) /* bit 6 */
#define NVIC_ICPR_CLRPEND_7  ((uint32_t)0x00000080) /* bit 7 */
#define NVIC_ICPR_CLRPEND_8  ((uint32_t)0x00000100) /* bit 8 */
#define NVIC_ICPR_CLRPEND_9  ((uint32_t)0x00000200) /* bit 9 */
#define NVIC_ICPR_CLRPEND_10 ((uint32_t)0x00000400) /* bit 10 */
#define NVIC_ICPR_CLRPEND_11 ((uint32_t)0x00000800) /* bit 11 */
#define NVIC_ICPR_CLRPEND_12 ((uint32_t)0x00001000) /* bit 12 */
#define NVIC_ICPR_CLRPEND_13 ((uint32_t)0x00002000) /* bit 13 */
#define NVIC_ICPR_CLRPEND_14 ((uint32_t)0x00004000) /* bit 14 */
#define NVIC_ICPR_CLRPEND_15 ((uint32_t)0x00008000) /* bit 15 */
#define NVIC_ICPR_CLRPEND_16 ((uint32_t)0x00010000) /* bit 16 */
#define NVIC_ICPR_CLRPEND_17 ((uint32_t)0x00020000) /* bit 17 */
#define NVIC_ICPR_CLRPEND_18 ((uint32_t)0x00040000) /* bit 18 */
#define NVIC_ICPR_CLRPEND_19 ((uint32_t)0x00080000) /* bit 19 */
#define NVIC_ICPR_CLRPEND_20 ((uint32_t)0x00100000) /* bit 20 */
#define NVIC_ICPR_CLRPEND_21 ((uint32_t)0x00200000) /* bit 21 */
#define NVIC_ICPR_CLRPEND_22 ((uint32_t)0x00400000) /* bit 22 */
#define NVIC_ICPR_CLRPEND_23 ((uint32_t)0x00800000) /* bit 23 */
#define NVIC_ICPR_CLRPEND_24 ((uint32_t)0x01000000) /* bit 24 */
#define NVIC_ICPR_CLRPEND_25 ((uint32_t)0x02000000) /* bit 25 */
#define NVIC_ICPR_CLRPEND_26 ((uint32_t)0x04000000) /* bit 26 */
#define NVIC_ICPR_CLRPEND_27 ((uint32_t)0x08000000) /* bit 27 */
#define NVIC_ICPR_CLRPEND_28 ((uint32_t)0x10000000) /* bit 28 */
#define NVIC_ICPR_CLRPEND_29 ((uint32_t)0x20000000) /* bit 29 */
#define NVIC_ICPR_CLRPEND_30 ((uint32_t)0x40000000) /* bit 30 */
#define NVIC_ICPR_CLRPEND_31 ((uint32_t)0x80000000) /* bit 31 */

/** Bit definition for NVIC_PRI0 register **/
#define NVIC_IPR0_PRI_0 ((uint32_t)0x000000FF) /* Priority of interrupt 0 */
#define NVIC_IPR0_PRI_1 ((uint32_t)0x0000FF00) /* Priority of interrupt 1 */
#define NVIC_IPR0_PRI_2 ((uint32_t)0x00FF0000) /* Priority of interrupt 2 */
#define NVIC_IPR0_PRI_3 ((uint32_t)0xFF000000) /* Priority of interrupt 3 */

/** Bit definition for NVIC_PRI1 register **/
#define NVIC_IPR1_PRI_4 ((uint32_t)0x000000FF) /* Priority of interrupt 4 */
#define NVIC_IPR1_PRI_5 ((uint32_t)0x0000FF00) /* Priority of interrupt 5 */
#define NVIC_IPR1_PRI_6 ((uint32_t)0x00FF0000) /* Priority of interrupt 6 */
#define NVIC_IPR1_PRI_7 ((uint32_t)0xFF000000) /* Priority of interrupt 7 */

/** Bit definition for NVIC_PRI2 register **/
#define NVIC_IPR2_PRI_8  ((uint32_t)0x000000FF) /* Priority of interrupt 8 */
#define NVIC_IPR2_PRI_9  ((uint32_t)0x0000FF00) /* Priority of interrupt 9 */
#define NVIC_IPR2_PRI_10 ((uint32_t)0x00FF0000) /* Priority of interrupt 10 */
#define NVIC_IPR2_PRI_11 ((uint32_t)0xFF000000) /* Priority of interrupt 11 */

/** Bit definition for NVIC_PRI3 register **/
#define NVIC_IPR3_PRI_12 ((uint32_t)0x000000FF) /* Priority of interrupt 12 */
#define NVIC_IPR3_PRI_13 ((uint32_t)0x0000FF00) /* Priority of interrupt 13 */
#define NVIC_IPR3_PRI_14 ((uint32_t)0x00FF0000) /* Priority of interrupt 14 */
#define NVIC_IPR3_PRI_15 ((uint32_t)0xFF000000) /* Priority of interrupt 15 */

/** Bit definition for NVIC_PRI4 register **/
#define NVIC_IPR4_PRI_16 ((uint32_t)0x000000FF) /* Priority of interrupt 16 */
#define NVIC_IPR4_PRI_17 ((uint32_t)0x0000FF00) /* Priority of interrupt 17 */
#define NVIC_IPR4_PRI_18 ((uint32_t)0x00FF0000) /* Priority of interrupt 18 */
#define NVIC_IPR4_PRI_19 ((uint32_t)0xFF000000) /* Priority of interrupt 19 */

/** Bit definition for NVIC_PRI5 register **/
#define NVIC_IPR5_PRI_20 ((uint32_t)0x000000FF) /* Priority of interrupt 20 */
#define NVIC_IPR5_PRI_21 ((uint32_t)0x0000FF00) /* Priority of interrupt 21 */
#define NVIC_IPR5_PRI_22 ((uint32_t)0x00FF0000) /* Priority of interrupt 22 */
#define NVIC_IPR5_PRI_23 ((uint32_t)0xFF000000) /* Priority of interrupt 23 */

/** Bit definition for NVIC_PRI6 register **/
#define NVIC_IPR6_PRI_24 ((uint32_t)0x000000FF) /* Priority of interrupt 24 */
#define NVIC_IPR6_PRI_25 ((uint32_t)0x0000FF00) /* Priority of interrupt 25 */
#define NVIC_IPR6_PRI_26 ((uint32_t)0x00FF0000) /* Priority of interrupt 26 */
#define NVIC_IPR6_PRI_27 ((uint32_t)0xFF000000) /* Priority of interrupt 27 */

/** Bit definition for NVIC_PRI7 register **/
#define NVIC_IPR7_PRI_28 ((uint32_t)0x000000FF) /* Priority of interrupt 28 */
#define NVIC_IPR7_PRI_29 ((uint32_t)0x0000FF00) /* Priority of interrupt 29 */
#define NVIC_IPR7_PRI_30 ((uint32_t)0x00FF0000) /* Priority of interrupt 30 */
#define NVIC_IPR7_PRI_31 ((uint32_t)0xFF000000) /* Priority of interrupt 31 */

/** Bit definition for SCB_CPUID register **/
#define SCB_CPUID_REVISION    ((uint32_t)0x0000000F) /* Implementation defined revision number */
#define SCB_CPUID_PARTNO      ((uint32_t)0x0000FFF0) /* Number of processor within family */
#define SCB_CPUID_Constant    ((uint32_t)0x000F0000) /* Reads as 0x0F */
#define SCB_CPUID_VARIANT     ((uint32_t)0x00F00000) /* Implementation defined variant number */
#define SCB_CPUID_IMPLEMENTER ((uint32_t)0xFF000000) /* Implementer code. ARM is 0x41 */

/** Bit definition for SCB_ICSR register **/
#define SCB_ICSR_VECTACTIVE  ((uint32_t)0x000001FF) /* Active INTSTS number field */
#define SCB_ICSR_RETTOBASE   ((uint32_t)0x00000800) /* All active exceptions minus the IPSR_current_exception yields the empty set */
#define SCB_ICSR_VECTPENDING ((uint32_t)0x003FF000) /* Pending INTSTS number field */
#define SCB_ICSR_ISRPENDING  ((uint32_t)0x00400000) /* Interrupt pending flag */
#define SCB_ICSR_ISRPREEMPT  ((uint32_t)0x00800000) /* It indicates that a pending interrupt becomes active in the next running cycle */
#define SCB_ICSR_PENDSTCLR   ((uint32_t)0x02000000) /* Clear pending SysTick bit */
#define SCB_ICSR_PENDSTSET   ((uint32_t)0x04000000) /* Set pending SysTick bit */
#define SCB_ICSR_PENDSVCLR   ((uint32_t)0x08000000) /* Clear pending pendSV bit */
#define SCB_ICSR_PENDSVSET   ((uint32_t)0x10000000) /* Set pending pendSV bit */
#define SCB_ICSR_NMIPENDSET  ((uint32_t)0x80000000) /* Set pending NMI bit */

/** Bit definition for SCB_AIRCR register **/
#define SCB_AIRCR_VECTCLRACTIVE ((uint32_t)0x00000002) /* Clear active vector bit */
#define SCB_AIRCR_SYSRESETREQ   ((uint32_t)0x00000004) /* Requests chip control logic to generate a reset */
#define SCB_AIRCR_ENDIANESS     ((uint32_t)0x00008000) /* Data endianness bit */
#define SCB_AIRCR_VECTKEYSTAT   ((uint32_t)0xFFFF0000) /* Reads as 0xFA05 (VECTKEYSTAT) */
#define SCB_AIRCR_VECTKEY       ((uint32_t)0x05FA0000) /* Register access key */

/** Bit definition for SCB_SCR register **/
#define SCB_SCR_SLEEPONEXIT ((uint8_t)0x02) /* Sleep on exit bit */
#define SCB_SCR_SLEEPDEEP   ((uint8_t)0x04) /* Sleep deep bit */
#define SCB_SCR_SEVONPEND   ((uint8_t)0x10) /* Wake up from WFE */

/** Bit definition for SCB_CCR register **/
#define SCB_CCR_UNALIGN_TRP ((uint16_t)0x0008) /* Trap for unaligned access */
#define SCB_CCR_STKALIGN    ((uint16_t)0x0200) /* On exception entry, the SP used prior to the exception is adjusted to be 8-byte aligned */

/** Bit definition for SCB_SHPR register **/
#define SCB_SHPR_PRI_N    ((uint32_t)0x000000FF) /* Priority of system handler 4,8, and 12. Mem Manage, reserved and Debug Monitor */
#define SCB_SHPR_PRI_N1   ((uint32_t)0x0000FF00) /* Priority of system handler 5,9, and 13. Bus Fault, reserved and reserved */
#define SCB_SHPR_PRI_N2   ((uint32_t)0x00FF0000) /* Priority of system handler 6,10, and 14. Usage Fault, reserved and PendSV */
#define SCB_SHPR_PRI_N3   ((uint32_t)0xFF000000) /* Priority of system handler 7,11, and 15. Reserved, SVCall and SysTick */

/** Bit definition for SCB_SHCSR register **/
#define SCB_SHCSR_SVCALLPENDED   ((uint32_t)0x00008000) /* SVCall is pended */

/** Bit definition for SCB_DFSR register **/
#define SCB_DFSR_HALTED   ((uint8_t)0x01) /* Halt request flag */
#define SCB_DFSR_BKPT     ((uint8_t)0x02) /* BKPT flag */
#define SCB_DFSR_DWTTRAP  ((uint8_t)0x04) /* Data Watchpoint and Trace (DWT) flag */
#define SCB_DFSR_VCATCH   ((uint8_t)0x08) /* Vector catch flag */
#define SCB_DFSR_EXTERNAL ((uint8_t)0x10) /* External debug request flag */

/** Bit definition for SCB_afsr register **/
#define SCB_AFSR_IMPDEF ((uint32_t)0xFFFFFFFF) /* Implementation defined */



/*** External Interrupt/Event Controller ***/

/** Bit definition for EXTI_IMASK register **/
#define EXTI_IMASK_IMASK0        ((uint32_t)0x00000001) /* Interrupt Mask on line 0 */
#define EXTI_IMASK_IMASK1        ((uint32_t)0x00000002) /* Interrupt Mask on line 1 */
#define EXTI_IMASK_IMASK2        ((uint32_t)0x00000004) /* Interrupt Mask on line 2 */
#define EXTI_IMASK_IMASK3        ((uint32_t)0x00000008) /* Interrupt Mask on line 3 */
#define EXTI_IMASK_IMASK4        ((uint32_t)0x00000010) /* Interrupt Mask on line 4 */
#define EXTI_IMASK_IMASK5        ((uint32_t)0x00000020) /* Interrupt Mask on line 5 */
#define EXTI_IMASK_IMASK6        ((uint32_t)0x00000040) /* Interrupt Mask on line 6 */
#define EXTI_IMASK_IMASK7        ((uint32_t)0x00000080) /* Interrupt Mask on line 7 */
#define EXTI_IMASK_IMASK8        ((uint32_t)0x00000100) /* Interrupt Mask on line 8 */
#define EXTI_IMASK_IMASK9        ((uint32_t)0x00000200) /* Interrupt Mask on line 9 */
#define EXTI_IMASK_IMASK10       ((uint32_t)0x00000400) /* Interrupt Mask on line 10 */
#define EXTI_IMASK_IMASK11       ((uint32_t)0x00000800) /* Interrupt Mask on line 11 */
#define EXTI_IMASK_IMASK12       ((uint32_t)0x00001000) /* Interrupt Mask on line 12 */
#define EXTI_IMASK_IMASK13       ((uint32_t)0x00002000) /* Interrupt Mask on line 13 */
#define EXTI_IMASK_IMASK14       ((uint32_t)0x00004000) /* Interrupt Mask on line 14 */
#define EXTI_IMASK_IMASK15       ((uint32_t)0x00008000) /* Interrupt Mask on line 15 */
#define EXTI_IMASK_IMASK16       ((uint32_t)0x00010000) /* Interrupt Mask on line 16 */
#define EXTI_IMASK_IMASK17       ((uint32_t)0x00020000) /* Interrupt Mask on line 17 */
#define EXTI_IMASK_IMASK18       ((uint32_t)0x00040000) /* Interrupt Mask on line 18 */
#define EXTI_IMASK_IMASK19       ((uint32_t)0x00080000) /* Interrupt Mask on line 19 */

/** Bit definition for EXTI_EMASK register **/
#define EXTI_EMASK_EMASK0        ((uint32_t)0x00000001) /* Event Mask on line 0 */
#define EXTI_EMASK_EMASK1        ((uint32_t)0x00000002) /* Event Mask on line 1 */
#define EXTI_EMASK_EMASK2        ((uint32_t)0x00000004) /* Event Mask on line 2 */
#define EXTI_EMASK_EMASK3        ((uint32_t)0x00000008) /* Event Mask on line 3 */
#define EXTI_EMASK_EMASK4        ((uint32_t)0x00000010) /* Event Mask on line 4 */
#define EXTI_EMASK_EMASK5        ((uint32_t)0x00000020) /* Event Mask on line 5 */
#define EXTI_EMASK_EMASK6        ((uint32_t)0x00000040) /* Event Mask on line 6 */
#define EXTI_EMASK_EMASK7        ((uint32_t)0x00000080) /* Event Mask on line 7 */
#define EXTI_EMASK_EMASK8        ((uint32_t)0x00000100) /* Event Mask on line 8 */
#define EXTI_EMASK_EMASK9        ((uint32_t)0x00000200) /* Event Mask on line 9 */
#define EXTI_EMASK_EMASK10       ((uint32_t)0x00000400) /* Event Mask on line 10 */
#define EXTI_EMASK_EMASK11       ((uint32_t)0x00000800) /* Event Mask on line 11 */
#define EXTI_EMASK_EMASK12       ((uint32_t)0x00001000) /* Event Mask on line 12 */
#define EXTI_EMASK_EMASK13       ((uint32_t)0x00002000) /* Event Mask on line 13 */
#define EXTI_EMASK_EMASK14       ((uint32_t)0x00004000) /* Event Mask on line 14 */
#define EXTI_EMASK_EMASK15       ((uint32_t)0x00008000) /* Event Mask on line 15 */
#define EXTI_EMASK_EMASK16       ((uint32_t)0x00010000) /* Event Mask on line 16 */
#define EXTI_EMASK_EMASK17       ((uint32_t)0x00020000) /* Event Mask on line 17 */
#define EXTI_EMASK_EMASK18       ((uint32_t)0x00040000) /* Event Mask on line 18 */
#define EXTI_EMASK_EMASK19       ((uint32_t)0x00080000) /* Event Mask on line 19 */

/** Bit definition for EXTI_RT_CFG register **/
#define EXTI_RT_CFG_RT_CFG0        ((uint32_t)0x00000001) /* Rising trigger event configuration bit of line 0 */
#define EXTI_RT_CFG_RT_CFG1        ((uint32_t)0x00000002) /* Rising trigger event configuration bit of line 1 */
#define EXTI_RT_CFG_RT_CFG2        ((uint32_t)0x00000004) /* Rising trigger event configuration bit of line 2 */
#define EXTI_RT_CFG_RT_CFG3        ((uint32_t)0x00000008) /* Rising trigger event configuration bit of line 3 */
#define EXTI_RT_CFG_RT_CFG4        ((uint32_t)0x00000010) /* Rising trigger event configuration bit of line 4 */
#define EXTI_RT_CFG_RT_CFG5        ((uint32_t)0x00000020) /* Rising trigger event configuration bit of line 5 */
#define EXTI_RT_CFG_RT_CFG6        ((uint32_t)0x00000040) /* Rising trigger event configuration bit of line 6 */
#define EXTI_RT_CFG_RT_CFG7        ((uint32_t)0x00000080) /* Rising trigger event configuration bit of line 7 */
#define EXTI_RT_CFG_RT_CFG8        ((uint32_t)0x00000100) /* Rising trigger event configuration bit of line 8 */
#define EXTI_RT_CFG_RT_CFG9        ((uint32_t)0x00000200) /* Rising trigger event configuration bit of line 9 */
#define EXTI_RT_CFG_RT_CFG10       ((uint32_t)0x00000400) /* Rising trigger event configuration bit of line 10 */
#define EXTI_RT_CFG_RT_CFG11       ((uint32_t)0x00000800) /* Rising trigger event configuration bit of line 11 */
#define EXTI_RT_CFG_RT_CFG12       ((uint32_t)0x00001000) /* Rising trigger event configuration bit of line 12 */
#define EXTI_RT_CFG_RT_CFG13       ((uint32_t)0x00002000) /* Rising trigger event configuration bit of line 13 */
#define EXTI_RT_CFG_RT_CFG14       ((uint32_t)0x00004000) /* Rising trigger event configuration bit of line 14 */
#define EXTI_RT_CFG_RT_CFG15       ((uint32_t)0x00008000) /* Rising trigger event configuration bit of line 15 */
#define EXTI_RT_CFG_RT_CFG16       ((uint32_t)0x00010000) /* Rising trigger event configuration bit of line 16 */
#define EXTI_RT_CFG_RT_CFG17       ((uint32_t)0x00020000) /* Rising trigger event configuration bit of line 17 */
#define EXTI_RT_CFG_RT_CFG18       ((uint32_t)0x00040000) /* Rising trigger event configuration bit of line 18 */
#define EXTI_RT_CFG_RT_CFG19       ((uint32_t)0x00080000) /* Rising trigger event configuration bit of line 19 */

/** Bit definition for EXTI_FT_CFG register **/
#define EXTI_FT_CFG_FT_CFG0        ((uint32_t)0x00000001) /* Falling trigger event configuration bit of line 0 */
#define EXTI_FT_CFG_FT_CFG1        ((uint32_t)0x00000002) /* Falling trigger event configuration bit of line 1 */
#define EXTI_FT_CFG_FT_CFG2        ((uint32_t)0x00000004) /* Falling trigger event configuration bit of line 2 */
#define EXTI_FT_CFG_FT_CFG3        ((uint32_t)0x00000008) /* Falling trigger event configuration bit of line 3 */
#define EXTI_FT_CFG_FT_CFG4        ((uint32_t)0x00000010) /* Falling trigger event configuration bit of line 4 */
#define EXTI_FT_CFG_FT_CFG5        ((uint32_t)0x00000020) /* Falling trigger event configuration bit of line 5 */
#define EXTI_FT_CFG_FT_CFG6        ((uint32_t)0x00000040) /* Falling trigger event configuration bit of line 6 */
#define EXTI_FT_CFG_FT_CFG7        ((uint32_t)0x00000080) /* Falling trigger event configuration bit of line 7 */
#define EXTI_FT_CFG_FT_CFG8        ((uint32_t)0x00000100) /* Falling trigger event configuration bit of line 8 */
#define EXTI_FT_CFG_FT_CFG9        ((uint32_t)0x00000200) /* Falling trigger event configuration bit of line 9 */
#define EXTI_FT_CFG_FT_CFG10       ((uint32_t)0x00000400) /* Falling trigger event configuration bit of line 10 */
#define EXTI_FT_CFG_FT_CFG11       ((uint32_t)0x00000800) /* Falling trigger event configuration bit of line 11 */
#define EXTI_FT_CFG_FT_CFG12       ((uint32_t)0x00001000) /* Falling trigger event configuration bit of line 12 */
#define EXTI_FT_CFG_FT_CFG13       ((uint32_t)0x00002000) /* Falling trigger event configuration bit of line 13 */
#define EXTI_FT_CFG_FT_CFG14       ((uint32_t)0x00004000) /* Falling trigger event configuration bit of line 14 */
#define EXTI_FT_CFG_FT_CFG15       ((uint32_t)0x00008000) /* Falling trigger event configuration bit of line 15 */
#define EXTI_FT_CFG_FT_CFG16       ((uint32_t)0x00010000) /* Falling trigger event configuration bit of line 16 */
#define EXTI_FT_CFG_FT_CFG17       ((uint32_t)0x00020000) /* Falling trigger event configuration bit of line 17 */
#define EXTI_FT_CFG_FT_CFG18       ((uint32_t)0x00040000) /* Falling trigger event configuration bit of line 18 */
#define EXTI_FT_CFG_FT_CFG19       ((uint32_t)0x00080000) /* Falling trigger event configuration bit of line 19 */

/** Bit definition for EXTI_SWIE register **/
#define EXTI_SWIE_SWIE0        ((uint32_t)0x00000001) /* Software Interrupt on line 0 */
#define EXTI_SWIE_SWIE1        ((uint32_t)0x00000002) /* Software Interrupt on line 1 */
#define EXTI_SWIE_SWIE2        ((uint32_t)0x00000004) /* Software Interrupt on line 2 */
#define EXTI_SWIE_SWIE3        ((uint32_t)0x00000008) /* Software Interrupt on line 3 */
#define EXTI_SWIE_SWIE4        ((uint32_t)0x00000010) /* Software Interrupt on line 4 */
#define EXTI_SWIE_SWIE5        ((uint32_t)0x00000020) /* Software Interrupt on line 5 */
#define EXTI_SWIE_SWIE6        ((uint32_t)0x00000040) /* Software Interrupt on line 6 */
#define EXTI_SWIE_SWIE7        ((uint32_t)0x00000080) /* Software Interrupt on line 7 */
#define EXTI_SWIE_SWIE8        ((uint32_t)0x00000100) /* Software Interrupt on line 8 */
#define EXTI_SWIE_SWIE9        ((uint32_t)0x00000200) /* Software Interrupt on line 9 */
#define EXTI_SWIE_SWIE10       ((uint32_t)0x00000400) /* Software Interrupt on line 10 */
#define EXTI_SWIE_SWIE11       ((uint32_t)0x00000800) /* Software Interrupt on line 11 */
#define EXTI_SWIE_SWIE12       ((uint32_t)0x00001000) /* Software Interrupt on line 12 */
#define EXTI_SWIE_SWIE13       ((uint32_t)0x00002000) /* Software Interrupt on line 13 */
#define EXTI_SWIE_SWIE14       ((uint32_t)0x00004000) /* Software Interrupt on line 14 */
#define EXTI_SWIE_SWIE15       ((uint32_t)0x00008000) /* Software Interrupt on line 15 */
#define EXTI_SWIE_SWIE16       ((uint32_t)0x00010000) /* Software Interrupt on line 16 */
#define EXTI_SWIE_SWIE17       ((uint32_t)0x00020000) /* Software Interrupt on line 17 */
#define EXTI_SWIE_SWIE18       ((uint32_t)0x00040000) /* Software Interrupt on line 18 */
#define EXTI_SWIE_SWIE19       ((uint32_t)0x00080000) /* Software Interrupt on line 19 */

/** Bit definition for EXTI_PEND register **/
#define EXTI_PEND_PEND0        ((uint32_t)0x00000001) /* Pending bit for line 0 */
#define EXTI_PEND_PEND1        ((uint32_t)0x00000002) /* Pending bit for line 1 */
#define EXTI_PEND_PEND2        ((uint32_t)0x00000004) /* Pending bit for line 2 */
#define EXTI_PEND_PEND3        ((uint32_t)0x00000008) /* Pending bit for line 3 */
#define EXTI_PEND_PEND4        ((uint32_t)0x00000010) /* Pending bit for line 4 */
#define EXTI_PEND_PEND5        ((uint32_t)0x00000020) /* Pending bit for line 5 */
#define EXTI_PEND_PEND6        ((uint32_t)0x00000040) /* Pending bit for line 6 */
#define EXTI_PEND_PEND7        ((uint32_t)0x00000080) /* Pending bit for line 7 */
#define EXTI_PEND_PEND8        ((uint32_t)0x00000100) /* Pending bit for line 8 */
#define EXTI_PEND_PEND9        ((uint32_t)0x00000200) /* Pending bit for line 9 */
#define EXTI_PEND_PEND10       ((uint32_t)0x00000400) /* Pending bit for line 10 */
#define EXTI_PEND_PEND11       ((uint32_t)0x00000800) /* Pending bit for line 11 */
#define EXTI_PEND_PEND12       ((uint32_t)0x00001000) /* Pending bit for line 12 */
#define EXTI_PEND_PEND13       ((uint32_t)0x00002000) /* Pending bit for line 13 */
#define EXTI_PEND_PEND14       ((uint32_t)0x00004000) /* Pending bit for line 14 */
#define EXTI_PEND_PEND15       ((uint32_t)0x00008000) /* Pending bit for line 15 */
#define EXTI_PEND_PEND16       ((uint32_t)0x00010000) /* Pending bit for line 16 */
#define EXTI_PEND_PEND17       ((uint32_t)0x00020000) /* Pending bit for line 17 */
#define EXTI_PEND_PEND18       ((uint32_t)0x00040000) /* Pending bit for line 18 */
#define EXTI_PEND_PEND19       ((uint32_t)0x00080000) /* Pending bit for line 19 */



//*** Analog to Digital Converter ***/

/** Bit definition for ADC_STS register **/
#define ADC_STS_AWDG     					((uint8_t)0x01) /* Analog watchdog flag */
#define ADC_STS_ENDC     					((uint8_t)0x02) /* End of conversion */
#define ADC_STS_STR      					((uint8_t)0x04) /* Regular channel Start flag */
#define ADC_STS_ENDCA    					((uint8_t)0x08) /* Regular channel any end flag */

/** Bit definition for ADC_CTRL1 register **/
#define ADC_CTRL1_AWDGCH					((uint32_t)0x0000001F) /* AWDG_CH[3:0] bits (Analog watchdog channel select bits) */
#define ADC_CTRL1_AWDGCH_0 					((uint32_t)0x00000001) /* Bit 0 */
#define ADC_CTRL1_AWDGCH_1 					((uint32_t)0x00000002) /* Bit 1 */
#define ADC_CTRL1_AWDGCH_2 					((uint32_t)0x00000004) /* Bit 2 */
#define ADC_CTRL1_AWDGCH_3 					((uint32_t)0x00000008) /* Bit 3 */

#define ADC_CTRL1_ENDCIEN   				((uint32_t)0x00000010) /* Interrupt enable for EOC */
#define ADC_CTRL1_AWDGIEN   				((uint32_t)0x00000020) /* Analog Watchdog interrupt enable */
#define ADC_CTRL1_AWDGSGLEN 				((uint32_t)0x00000040) /* Enable the watchdog on a single channel in scan mode */
#define ADC_CTRL1_AWDGERCH 					((uint32_t)0x00000080) /* Analog watchdog enable on regular channels */

/** Bit definition for ADC_CTRL2 register **/
#define ADC_CTRL2_ON    					((uint32_t)0x00000001) /* A/D Converter ON / OFF */
#define ADC_CTRL2_CTU   					((uint32_t)0x00000002) /* Continuous Conversion */
#define ADC_CTRL2_ALIG  					((uint32_t)0x00000040) /* Data Alignment */

#define ADC_CTRL2_EXTRSEL   				((uint32_t)0x00000078) /* EXTSEL[3:0] bits (External Event Select for regular group) */
#define ADC_CTRL2_EXTRSEL_0 				((uint32_t)0x00000080) /* Bit 0 */
#define ADC_CTRL2_EXTRSEL_1 				((uint32_t)0x00000010) /* Bit 1 */
#define ADC_CTRL2_EXTRSEL_2 				((uint32_t)0x00080020) /* Bit 2 */
#define ADC_CTRL2_EXTRSEL_3 				((uint32_t)0x00080040) /* Bit 3 */

#define ADC_CTRL2_EXTRTRIG 					((uint32_t)0x00000080) /* External Trigger Conversion mode for regular channels */
#define ADC_CTRL2_SWSTRRCH 					((uint32_t)0x00000100) /* Start Conversion of regular channels */

#define ADC_CTRL2_LEN 						((uint32_t)0x00000E00) /* LEN[2:0] bits (Regular Sequence length) */
#define ADC_CTRL2_LEN_0 					((uint32_t)0x00000200) /* Bit 0 */
#define ADC_CTRL2_LEN_1 					((uint32_t)0x00000400) /* Bit 1 */
#define ADC_CTRL2_LEN_2 					((uint32_t)0x00000800) /* Bit 2 */

#define ADC_CTRL2_SCANMD    				((uint32_t)0x00001000) /* Scan mode */

/** Bit definition for ADC_CTRL3 register **/
#define ADC_CTRL3_VREFEN     				((uint16_t)0x0002) /* ADC VREFINT_EN */
#define ADC_CTRL3_VREFRDY    				((uint16_t)0x0004) /* ADC VREFINT_READY */
#define ADC_CTRL3_RDY       				((uint16_t)0x0008) /* ADC Ready */
#define ADC_CTRL3_PDRDY      				((uint16_t)0x0010) /* ADC Power_down Ready */
#define ADC_CTRL3_ENDCAIEN   				((uint16_t)0x0020) /* Interrupt enable for any regular channels */

/** Bit definition for ADC_SAMPT register **/
#define  ADC_SAMPT_SAMP                     ((uint32_t)0x00000001F) /* SMP10[4:0] bits (Channel Sample time selection) */
#define  ADC_SAMPT_SAMP_0                   ((uint32_t)0x00000001)  /* Bit 0 */
#define  ADC_SAMPT_SAMP_1                   ((uint32_t)0x00000002)  /* Bit 1 */
#define  ADC_SAMPT_SAMP_2                   ((uint32_t)0x00000004)  /* Bit 2 */
#define  ADC_SAMPT_SAMP_3                   ((uint32_t)0x00000008)  /* Bit 3 */
#define  ADC_SAMPT_SAMP_4                   ((uint32_t)0x00000010)  /* Bit 4 */

/** Bit definition for ADC_WDGHIGH register **/
#define ADC_WDGHIGH_HTH 					((uint16_t)0x0FFF)  /* Analog watchdog high threshold */

/** Bit definition for ADC_WDGLOW register **/
#define ADC_WDGLOW_LTH 						((uint16_t)0x0FFF)  /* Analog watchdog low threshold */

/** Bit definition for ADC_DATA0 register **/
#define ADC_DAT0_DAT     					((uint32_t)0x0000FFFF) /* Regular data */
#define ADC_DAT0_SEQ0     					((uint32_t)0x000F0000) /* channel seqence of conversion */
#define ADC_DAT0_SEQ0_0     				((uint32_t)0x00010000) /* Bit 0 */
#define ADC_DAT0_SEQ0_1     				((uint32_t)0x00020000) /* Bit 1 */
#define ADC_DAT0_SEQ0_2     				((uint32_t)0x00040000) /* Bit 2 */
#define ADC_DAT0_SEQ0_3     				((uint32_t)0x00080000) /* Bit 3 */

/** Bit definition for ADC_DATA1 register **/
#define ADC_DAT1_DAT     					((uint32_t)0x0000FFFF) /* Regular data */
#define ADC_DAT1_SEQ1     					((uint32_t)0x000F0000) /* channel seqence of conversion */
#define ADC_DAT1_SEQ1_0     				((uint32_t)0x00010000) /* Bit 0 */
#define ADC_DAT1_SEQ1_1     				((uint32_t)0x00020000) /* Bit 1 */
#define ADC_DAT1_SEQ1_2     				((uint32_t)0x00040000) /* Bit 2 */
#define ADC_DAT1_SEQ1_3     				((uint32_t)0x00080000) /* Bit 3 */

/** Bit definition for ADC_DATA2 register **/
#define ADC_DAT2_DAT     					((uint32_t)0x0000FFFF) /* Regular data */
#define ADC_DAT2_SEQ2     					((uint32_t)0x000F0000) /* channel seqence of conversion */
#define ADC_DAT2_SEQ2_0     				((uint32_t)0x00010000) /* Bit 0 */
#define ADC_DAT2_SEQ2_1     				((uint32_t)0x00020000) /* Bit 1 */
#define ADC_DAT2_SEQ2_2     				((uint32_t)0x00040000) /* Bit 2 */
#define ADC_DAT2_SEQ2_3     				((uint32_t)0x00080000) /* Bit 3 */

/** Bit definition for ADC_DATA3 register **/
#define ADC_DAT3_DAT     					((uint32_t)0x0000FFFF) /* Regular data */
#define ADC_DAT3_SEQ3     					((uint32_t)0x000F0000) /* channel seqence of conversion */
#define ADC_DAT3_SEQ3_0     				((uint32_t)0x00010000) /* Bit 0 */
#define ADC_DAT3_SEQ3_1     				((uint32_t)0x00020000) /* Bit 1 */
#define ADC_DAT3_SEQ3_2     				((uint32_t)0x00040000) /* Bit 2 */
#define ADC_DAT3_SEQ3_3     				((uint32_t)0x00080000) /* Bit 3 */

/** Bit definition for ADC_DATA4 register **/
#define ADC_DAT4_DAT     					((uint32_t)0x0000FFFF) /* Regular data */
#define ADC_DAT4_SEQ4     					((uint32_t)0x000F0000) /* channel seqence of conversion */
#define ADC_DAT4_SEQ4_0     				((uint32_t)0x00010000) /* Bit 0 */
#define ADC_DAT4_SEQ4_1     				((uint32_t)0x00020000) /* Bit 1 */
#define ADC_DAT4_SEQ4_2     				((uint32_t)0x00040000) /* Bit 2 */
#define ADC_DAT4_SEQ4_3     				((uint32_t)0x00080000) /* Bit 3 */



/***  Comparators Peripheral Interface ***/

/** Bit definition for COMP_INTEN register **/
#define COMP_INTEN_CMPIEN 					((uint32_t)0x00000001) /* COMP Interrupt Enable */

/** Bit definition for COMP_INTSTS register **/
#define COMP_INTSTS_CMPIS 					((uint32_t)0x00000001) /* COMP Interrupt Status */

/** Bit definition for COMP_LOCK register **/
#define COMP_LOCK_CMPLK 					((uint32_t)0x00000001) /* COMP Control Register Lock Enable */

/** Bit definition for COMP_CTRL register **/
#define COMP_CTRL_EN     					((uint32_t)0x00000001) /* COMP Enable */
#define COMP_CTRL_INMSEL 					((uint32_t)0x00000002) /* COMP Inverting Input Selection */
#define COMP_CTRL_INPSEL 					((uint32_t)0x00000004) /* COMP Non Inverting Input Selection */

#define COMP_CTRL_CMPVOS 					((uint32_t)0x00000018) /* COMP Non Inverting Subtraction Selection */
#define COMP_CTRL_CMPVOS_0     				((uint32_t)0x00000008) /* Bit 0 */
#define COMP_CTRL_CMPVOS_1     				((uint32_t)0x00000010) /* Bit 1 */

#define COMP_CTRL_OUTTRG 					((uint32_t)0x00000380) /* COMP Output trigger */
#define COMP_CTRL_OUTTRG_0     				((uint32_t)0x00000080) /* Bit 0 */
#define COMP_CTRL_OUTTRG_1     				((uint32_t)0x00000100) /* Bit 1 */
#define COMP_CTRL_OUTTRG_2     				((uint32_t)0x00000200) /* Bit 2 */

#define COMP_CTRL_POL    					((uint32_t)0x00000800) /* COMP Output invert */

#define COMP_CTRL_HYST   					((uint32_t)0x00003000) /* COMP Output Hysteresis Level */
#define COMP_CTRL_HYST_0     				((uint32_t)0x00001000) /* Bit 0 */
#define COMP_CTRL_HYST_1     				((uint32_t)0x00002000) /* Bit 1 */

#define COMP_CTRL_BLKING 					((uint32_t)0x00004000) /* COMP Output Blanking */
#define COMP_CTRL_OUT    					((uint32_t)0x00020000) /* COMP Output Status */

/** Bit definition for COMP_FILC register **/
#define COMP_FILC_FILEN   					((uint32_t)0x00000001) /* COMP Filter Enable */
#define COMP_FILC_THRESH  					((uint32_t)0x0000003E) /* COMP Low Filter Majority Voting Threshold */
#define COMP_FILC_SAMPW 					((uint32_t)0x000007C0) /* COMP Low Filter Sample Window Size */

/** Bit definition for COMP_FILP register **/
#define COMP_FILP_CLKPSC  					((uint32_t)0x0000FFFF) /* COMP Low Filter Sample Clock Prescale */



/*** TIM ***/

/** Bit definition for TIM_CTRL1 register  **/

#define TIM_CTRL1_CNTEN     ((uint32_t)0x00000001) /* Counter enable */
#define TIM_CTRL1_UPDIS     ((uint32_t)0x00000002) /* Update disable */
#define TIM_CTRL1_UPRS      ((uint32_t)0x00000004) /* Update request source */
#define TIM_CTRL1_ONEPM     ((uint32_t)0x00000008) /* One pulse mode */
#define TIM_CTRL1_DIR       ((uint32_t)0x00000010) /* Direction */

#define TIM_CTRL1_CAMSEL    ((uint32_t)0x00000060) /* CMS[1:0] bits (Center-aligned mode selection) */
#define TIM_CTRL1_CAMSEL_0  ((uint32_t)0x00000020) /* Bit 0 */
#define TIM_CTRL1_CAMSEL_1  ((uint32_t)0x00000040) /* Bit 1 */

#define TIM_CTRL1_ARPEN     ((uint32_t)0x00000080) /* Auto-reload preload enable */

#define TIM_CTRL1_CLKD      ((uint32_t)0x00000300) /* CKD[1:0] bits (clock division) */
#define TIM_CTRL1_CLKD_0    ((uint32_t)0x00000100) /* Bit 0 */
#define TIM_CTRL1_CLKD_1    ((uint32_t)0x00000200) /* Bit 1 */

#define TIM_CTRL1_IOMBKPEN  ((uint32_t)0x00000400) /* Break_in selection from IOM/COMP */
#define TIM_CTRL1_C1SEL     ((uint32_t)0x00000800) /* Channel 1 selection from IOM/COMP */
#define TIM_CTRL1_CLRSEL    ((uint32_t)0x00008000) /* OCxRef selection from ETR/COMP */

#define TIM_CTRL1_LBKPEN    ((uint32_t)0x00010000) /* LOCKUP as bkp Enable*/
#define TIM_CTRL1_PBKPEN    ((uint32_t)0x00020000) /* PVD as bkp Enable */

/** Bit definition for TIM_CTRL2 register **/

#define TIM_CTRL2_CCPCTL    ((uint32_t)0x00000001) /* Capture/Compare Preloaded Control */
#define TIM_CTRL2_CCUSEL    ((uint32_t)0x00000004) /* Capture/Compare Control Update Selection */

#define TIM_CTRL2_MMSEL     ((uint32_t)0x00000070) /* MMS[3:0] bits (Master Mode Selection) */
#define TIM_CTRL2_MMSEL_0   ((uint32_t)0x00000010) /* Bit 0 */
#define TIM_CTRL2_MMSEL_1   ((uint32_t)0x00000020) /* Bit 1 */
#define TIM_CTRL2_MMSEL_2   ((uint32_t)0x00000040) /* Bit 2 */


#define TIM_CTRL2_TI1SEL    ((uint32_t)0x00000080) /* TI1 Selection */

#define TIM_CTRL2_OI1       ((uint32_t)0x00000100) /* Output Idle state 1 (OC1 output) */
#define TIM_CTRL2_OI1N      ((uint32_t)0x00000200) /* Output Idle state 1 (OC1N output) */
#define TIM_CTRL2_OI2       ((uint32_t)0x00000400) /* Output Idle state 2 (OC2 output) */
#define TIM_CTRL2_OI2N      ((uint32_t)0x00000800) /* Output Idle state 2 (OC2N output) */
#define TIM_CTRL2_OI3       ((uint32_t)0x00001000) /* Output Idle state 3 (OC3 output) */
#define TIM_CTRL2_OI3N      ((uint32_t)0x00002000) /* Output Idle state 3 (OC3N output) */
#define TIM_CTRL2_OI4       ((uint32_t)0x00004000) /* Output Idle state 4 (OC4 output) */
#define TIM_CTRL2_OI5       ((uint32_t)0x00010000) /* Output Idle state 5 (OC5 output) */


/** Bit definition for TIM_SMCTRL register **/

#define TIM_SMCTRL_SMSEL    ((uint16_t)0x0007) /* SMS[2:0] bits (Slave mode selection) */
#define TIM_SMCTRL_SMSEL_0  ((uint16_t)0x0001) /* Bit 0 */
#define TIM_SMCTRL_SMSEL_1  ((uint16_t)0x0002) /* Bit 1 */
#define TIM_SMCTRL_SMSEL_2  ((uint16_t)0x0004) /* Bit 2 */

#define TIM_SMCTRL_TSEL     ((uint16_t)0x0070) /* TS[2:0] bits (Trigger selection) */
#define TIM_SMCTRL_TSEL_0   ((uint16_t)0x0010) /* Bit 0 */
#define TIM_SMCTRL_TSEL_1   ((uint16_t)0x0020) /* Bit 1 */
#define TIM_SMCTRL_TSEL_2   ((uint16_t)0x0040) /* Bit 2 */

#define TIM_SMCTRL_MSMD     ((uint16_t)0x0080) /* Master/slave mode */

#define TIM_SMCTRL_EXTF     ((uint16_t)0x0F00) /* ETF[3:0] bits (External trigger filter) */
#define TIM_SMCTRL_EXTF_0   ((uint16_t)0x0100) /* Bit 0 */
#define TIM_SMCTRL_EXTF_1   ((uint16_t)0x0200) /* Bit 1 */
#define TIM_SMCTRL_EXTF_2   ((uint16_t)0x0400) /* Bit 2 */
#define TIM_SMCTRL_EXTF_3   ((uint16_t)0x0800) /* Bit 3 */

#define TIM_SMCTRL_EXTPS    ((uint16_t)0x3000) /* ETPS[1:0] bits (External trigger prescaler) */
#define TIM_SMCTRL_EXTPS_0  ((uint16_t)0x1000) /* Bit 0 */
#define TIM_SMCTRL_EXTPS_1  ((uint16_t)0x2000) /* Bit 1 */

#define TIM_SMCTRL_EXCEN    ((uint16_t)0x4000) /* External clock enable */
#define TIM_SMCTRL_EXTP     ((uint16_t)0x8000) /* External trigger polarity */

/** Bit definition for TIM_DINTEN register **/

#define TIM_DINTEN_UIEN     ((uint16_t)0x0001) /* Update interrupt enable */
#define TIM_DINTEN_CC1IEN   ((uint16_t)0x0002) /* Capture/Compare 1 interrupt enable */
#define TIM_DINTEN_CC2IEN   ((uint16_t)0x0004) /* Capture/Compare 2 interrupt enable */
#define TIM_DINTEN_CC3IEN   ((uint16_t)0x0008) /* Capture/Compare 3 interrupt enable */
#define TIM_DINTEN_CC4IEN   ((uint16_t)0x0010) /* Capture/Compare 4 interrupt enable */
#define TIM_DINTEN_COMIEN   ((uint16_t)0x0020) /* COM interrupt enable */
#define TIM_DINTEN_TIEN     ((uint16_t)0x0040) /* Trigger interrupt enable */
#define TIM_DINTEN_BIEN     ((uint16_t)0x0080) /* Break interrupt enable */

/** Bit definition for TIM_STS register **/

#define TIM_STS_UDITF       ((uint32_t)0x00000001) /* Update interrupt Flag */
#define TIM_STS_CC1ITF      ((uint32_t)0x00000002) /* Capture/Compare 1 interrupt Flag */
#define TIM_STS_CC2ITF      ((uint32_t)0x00000004) /* Capture/Compare 2 interrupt Flag */
#define TIM_STS_CC3ITF      ((uint32_t)0x00000008) /* Capture/Compare 3 interrupt Flag */
#define TIM_STS_CC4ITF      ((uint32_t)0x00000010) /* Capture/Compare 4 interrupt Flag */
#define TIM_STS_COMITF      ((uint32_t)0x00000020) /* COM interrupt Flag */
#define TIM_STS_TITF        ((uint32_t)0x00000040) /* Trigger interrupt Flag */
#define TIM_STS_BITF        ((uint32_t)0x00000080) /* Break interrupt Flag */
#define TIM_STS_CC1OCF      ((uint32_t)0x00000200) /* Capture/Compare 1 Overcapture Flag */
#define TIM_STS_CC2OCF      ((uint32_t)0x00000400) /* Capture/Compare 2 Overcapture Flag */
#define TIM_STS_CC3OCF      ((uint32_t)0x00000800) /* Capture/Compare 3 Overcapture Flag */
#define TIM_STS_CC4OCF      ((uint32_t)0x00001000) /* Capture/Compare 4 Overcapture Flag */
#define TIM_STS_CC5ITF      ((uint32_t)0x00010000) /* Capture/Compare 5 interrupt Flag */


/** Bit definition for TIM_EVTGEN register **/

#define TIM_EVTGEN_UDGN     ((uint8_t)0x01) /* Update Generation */
#define TIM_EVTGEN_CC1GN    ((uint8_t)0x02) /* Capture/Compare 1 Generation */
#define TIM_EVTGEN_CC2GN    ((uint8_t)0x04) /* Capture/Compare 2 Generation */
#define TIM_EVTGEN_CC3GN    ((uint8_t)0x08) /* Capture/Compare 3 Generation */
#define TIM_EVTGEN_CC4GN    ((uint8_t)0x10) /* Capture/Compare 4 Generation */
#define TIM_EVTGEN_CCUDGN   ((uint8_t)0x20) /* Capture/Compare Control Update Generation */
#define TIM_EVTGEN_TGN      ((uint8_t)0x40) /* Trigger Generation */
#define TIM_EVTGEN_BGN      ((uint8_t)0x80) /* Break Generation */

/** Bit definition for TIM_CCMOD1 register **/

#define TIM_CCMOD1_CC1SEL       ((uint16_t)0x0003) /* CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define TIM_CCMOD1_CC1SEL_0     ((uint16_t)0x0001) /* Bit 0 */
#define TIM_CCMOD1_CC1SEL_1     ((uint16_t)0x0002) /* Bit 1 */

#define TIM_CCMOD1_OC1FEN       ((uint16_t)0x0004) /* Output Compare 1 Fast enable */
#define TIM_CCMOD1_OC1PEN       ((uint16_t)0x0008) /* Output Compare 1 Preload enable */

#define TIM_CCMOD1_OC1MD         ((uint16_t)0x0070) /* OC1M[2:0] bits (Output Compare 1 Mode) */
#define TIM_CCMOD1_OC1MD_0       ((uint16_t)0x0010) /* Bit 0 */
#define TIM_CCMOD1_OC1MD_1       ((uint16_t)0x0020) /* Bit 1 */
#define TIM_CCMOD1_OC1MD_2       ((uint16_t)0x0040) /* Bit 2 */

#define TIM_CCMOD1_OC1CEN       ((uint16_t)0x0080) /* Output Compare 1Clear Enable */

#define TIM_CCMOD1_CC2SEL       ((uint16_t)0x0300) /* CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define TIM_CCMOD1_CC2SEL_0     ((uint16_t)0x0100) /* Bit 0 */
#define TIM_CCMOD1_CC2SEL_1     ((uint16_t)0x0200) /* Bit 1 */

#define TIM_CCMOD1_OC2FEN       ((uint16_t)0x0400) /* Output Compare 2 Fast enable */
#define TIM_CCMOD1_OC2PEN       ((uint16_t)0x0800) /* Output Compare 2 Preload enable */

#define TIM_CCMOD1_OC2MD         ((uint16_t)0x7000) /* OC2M[2:0] bits (Output Compare 2 Mode) */
#define TIM_CCMOD1_OC2MD_0       ((uint16_t)0x1000) /* Bit 0 */
#define TIM_CCMOD1_OC2MD_1       ((uint16_t)0x2000) /* Bit 1 */
#define TIM_CCMOD1_OC2MD_2       ((uint16_t)0x4000) /* Bit 2 */

#define TIM_CCMOD1_OC2CEN       ((uint16_t)0x8000) /* Output Compare 2 Clear Enable */

#define TIM_CCMOD1_IC1PSC       ((uint16_t)0x000C) /* IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define TIM_CCMOD1_IC1PSC_0     ((uint16_t)0x0004) /* Bit 0 */
#define TIM_CCMOD1_IC1PSC_1     ((uint16_t)0x0008) /* Bit 1 */

#define TIM_CCMOD1_IC1F         ((uint16_t)0x00F0) /* IC1F[3:0] bits (Input Capture 1 Filter) */
#define TIM_CCMOD1_IC1F_0       ((uint16_t)0x0010) /* Bit 0 */
#define TIM_CCMOD1_IC1F_1       ((uint16_t)0x0020) /* Bit 1 */
#define TIM_CCMOD1_IC1F_2       ((uint16_t)0x0040) /* Bit 2 */
#define TIM_CCMOD1_IC1F_3       ((uint16_t)0x0080) /* Bit 3 */

#define TIM_CCMOD1_IC2PSC       ((uint16_t)0x0C00) /* IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define TIM_CCMOD1_IC2PSC_0     ((uint16_t)0x0400) /* Bit 0 */
#define TIM_CCMOD1_IC2PSC_1     ((uint16_t)0x0800) /* Bit 1 */

#define TIM_CCMOD1_IC2F         ((uint16_t)0xF000) /* IC2F[3:0] bits (Input Capture 2 Filter) */
#define TIM_CCMOD1_IC2F_0       ((uint16_t)0x1000) /* Bit 0 */
#define TIM_CCMOD1_IC2F_1       ((uint16_t)0x2000) /* Bit 1 */
#define TIM_CCMOD1_IC2F_2       ((uint16_t)0x4000) /* Bit 2 */
#define TIM_CCMOD1_IC2F_3       ((uint16_t)0x8000) /* Bit 3 */

/** Bit definition for TIM_CCMOD2 register **/

#define TIM_CCMOD2_CC3SEL       ((uint32_t)0x00000003) /* CC3S[1:0] bits (Capture/Compare 3 Selection) */
#define TIM_CCMOD2_CC3SEL_0     ((uint32_t)0x00000001) /* Bit 0 */
#define TIM_CCMOD2_CC3SEL_1     ((uint32_t)0x00000002) /* Bit 1 */

#define TIM_CCMOD2_OC3FEN       ((uint32_t)0x00000004) /* Output Compare 3 Fast enable */
#define TIM_CCMOD2_OC3PEN       ((uint32_t)0x00000008) /* Output Compare 3 Preload enable */

#define TIM_CCMOD2_OC3MD        ((uint32_t)0x00000070) /* OC3M[2:0] bits (Output Compare 3 Mode) */
#define TIM_CCMOD2_OC3MD_0      ((uint32_t)0x00000010) /* Bit 0 */
#define TIM_CCMOD2_OC3MD_1      ((uint32_t)0x00000020) /* Bit 1 */
#define TIM_CCMOD2_OC3MD_2      ((uint32_t)0x00000040) /* Bit 2 */

#define TIM_CCMOD2_OC3CEN       ((uint32_t)0x00000080) /* Output Compare 3 Clear Enable */

#define TIM_CCMOD2_CC4SEL       ((uint32_t)0x00000300) /* CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define TIM_CCMOD2_CC4SEL_0     ((uint32_t)0x00000100) /* Bit 0 */
#define TIM_CCMOD2_CC4SEL_1     ((uint32_t)0x00000200) /* Bit 1 */

#define TIM_CCMOD2_OC4FEN       ((uint32_t)0x00000400) /* Output Compare 4 Fast enable */
#define TIM_CCMOD2_OC4PEN       ((uint32_t)0x00000800) /* Output Compare 4 Preload enable */

#define TIM_CCMOD2_OC4MD        ((uint32_t)0x00007000) /* OC4M[2:0] bits (Output Compare 4 Mode) */
#define TIM_CCMOD2_OC4MD_0      ((uint32_t)0x00001000) /* Bit 0 */
#define TIM_CCMOD2_OC4MD_1      ((uint32_t)0x00002000) /* Bit 1 */
#define TIM_CCMOD2_OC4MD_2      ((uint32_t)0x00004000) /* Bit 2 */

#define TIM_CCMOD2_OC4CEN       ((uint32_t)0x00008000) /* Output Compare 4 Clear Enable */


#define TIM_CCMOD2_IC3PSC       ((uint32_t)0x0000000C) /* IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define TIM_CCMOD2_IC3PSC_0     ((uint32_t)0x00000004) /* Bit 0 */
#define TIM_CCMOD2_IC3PSC_1     ((uint32_t)0x00000008) /* Bit 1 */

#define TIM_CCMOD2_IC3F         ((uint32_t)0x000000F0) /* IC3F[3:0] bits (Input Capture 3 Filter) */
#define TIM_CCMOD2_IC3F_0       ((uint32_t)0x00000010) /* Bit 0 */
#define TIM_CCMOD2_IC3F_1       ((uint32_t)0x00000020) /* Bit 1 */
#define TIM_CCMOD2_IC3F_2       ((uint32_t)0x00000040) /* Bit 2 */
#define TIM_CCMOD2_IC3F_3       ((uint32_t)0x00000080) /* Bit 3 */

#define TIM_CCMOD2_IC4PSC       ((uint32_t)0x00000C00) /* IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define TIM_CCMOD2_IC4PSC_0     ((uint32_t)0x00000400) /* Bit 0 */
#define TIM_CCMOD2_IC4PSC_1     ((uint32_t)0x00000800) /* Bit 1 */

#define TIM_CCMOD2_IC4F         ((uint32_t)0x0000F000) /* IC4F[3:0] bits (Input Capture 4 Filter) */
#define TIM_CCMOD2_IC4F_0       ((uint32_t)0x00001000) /* Bit 0 */
#define TIM_CCMOD2_IC4F_1       ((uint32_t)0x00002000) /* Bit 1 */
#define TIM_CCMOD2_IC4F_2       ((uint32_t)0x00004000) /* Bit 2 */
#define TIM_CCMOD2_IC4F_3       ((uint32_t)0x00008000) /* Bit 3 */

/** Bit definition for TIM_CCMOD3 register **/

#define TIM_CCMOD3_OC5FEN       ((uint16_t)0x0004) /* Output Compare 5 Fast enable */
#define TIM_CCMOD3_OC5PEN       ((uint16_t)0x0008) /* Output Compare 5 Preload enable */

#define TIM_CCMOD3_OC5MD        ((uint16_t)0x0070) /* OC5M[2:0] bits (Output Compare 5 Mode) */
#define TIM_CCMOD3_OC5MD_0      ((uint16_t)0x0010) /* Bit 0 */
#define TIM_CCMOD3_OC5MD_1      ((uint16_t)0x0020) /* Bit 1 */
#define TIM_CCMOD3_OC5MD_2      ((uint16_t)0x0040) /* Bit 2 */

#define TIM_CCMOD3_OC5CEN       ((uint16_t)0x0080) /* Output Compare 5Clear Enable */



/** Bit definition for TIM_CCEN register **/

#define TIM_CCEN_CC1EN          ((uint32_t)0x00000001) /* Capture/Compare 1 output enable */
#define TIM_CCEN_CC1P           ((uint32_t)0x00000002) /* Capture/Compare 1 output Polarity */
#define TIM_CCEN_CC1NEN         ((uint32_t)0x00000004) /* Capture/Compare 1 Complementary output enable */
#define TIM_CCEN_CC1NP          ((uint32_t)0x00000008) /* Capture/Compare 1 Complementary output Polarity */
#define TIM_CCEN_CC2EN          ((uint32_t)0x00000010) /* Capture/Compare 2 output enable */
#define TIM_CCEN_CC2P           ((uint32_t)0x00000020) /* Capture/Compare 2 output Polarity */
#define TIM_CCEN_CC2NEN         ((uint32_t)0x00000040) /* Capture/Compare 2 Complementary output enable */
#define TIM_CCEN_CC2NP          ((uint32_t)0x00000080) /* Capture/Compare 2 Complementary output Polarity */
#define TIM_CCEN_CC3EN          ((uint32_t)0x00000100) /* Capture/Compare 3 output enable */
#define TIM_CCEN_CC3P           ((uint32_t)0x00000200) /* Capture/Compare 3 output Polarity */
#define TIM_CCEN_CC3NEN         ((uint32_t)0x00000400) /* Capture/Compare 3 Complementary output enable */
#define TIM_CCEN_CC3NP          ((uint32_t)0x00000800) /* Capture/Compare 3 Complementary output Polarity */
#define TIM_CCEN_CC4EN          ((uint32_t)0x00001000) /* Capture/Compare 4 output enable */
#define TIM_CCEN_CC4P           ((uint32_t)0x00002000) /* Capture/Compare 4 output Polarity */
#define TIM_CCEN_CC5EN          ((uint32_t)0x00010000) /* Capture/Compare 5 output enable */
#define TIM_CCEN_CC5P           ((uint32_t)0x00020000) /* Capture/Compare 5 output Polarity */


/** Bit definition for TIM_CNT register **/
#define TIM_CNT_CNT             ((uint16_t)0xFFFF) /* Counter Value */

/** Bit definition for TIM_PSC register **/
#define TIM_PSC_PSC             ((uint16_t)0xFFFF) /* Prescaler Value */

/** Bit definition for TIM_AR register **/
#define TIM_AR_AR               ((uint16_t)0xFFFF) /* actual auto-reload Value */

/** Bit definition for TIM_REPCNT register **/
#define TIM_REPCNT_REPCNT       ((uint8_t)0xFF) /* Repetition Counter Value */

/** Bit definition for TIM_CCDAT1 register **/
#define TIM_CCDAT1_CCDAT1       ((uint16_t)0xFFFF)  /* Capture/Compare 1 Value */

/** Bit definition for TIM_CCDAT2 register **/
#define TIM_CCDAT2_CCDAT2       ((uint16_t)0xFFFF)  /* Capture/Compare 2 Value */

/** Bit definition for TIM_CCDAT3 register **/
#define TIM_CCDAT3_CCDAT3       ((uint16_t)0xFFFF)  /* Capture/Compare 3 Value */

/** Bit definition for TIM_CCDAT4 register **/
#define TIM_CCDAT4_CCDAT4       ((uint16_t)0xFFFF)  /* Capture/Compare 4 Value */

/** Bit definition for TIM_CCDAT5 register **/
#define TIM_CCDAT5_CCDAT5       ((uint16_t)0xFFFF) /* Capture/Compare 5 Value */


/** Bit definition for TIM_BKDT register **/

#define TIM_BKDT_DTGN           ((uint16_t)0x00FF) /* DTG[0:7] bits (Dead-Time Generator set-up) */
#define TIM_BKDT_DTGN_0         ((uint16_t)0x0001) /* Bit 0 */
#define TIM_BKDT_DTGN_1         ((uint16_t)0x0002) /* Bit 1 */
#define TIM_BKDT_DTGN_2         ((uint16_t)0x0004) /* Bit 2 */
#define TIM_BKDT_DTGN_3         ((uint16_t)0x0008) /* Bit 3 */
#define TIM_BKDT_DTGN_4         ((uint16_t)0x0010) /* Bit 4 */
#define TIM_BKDT_DTGN_5         ((uint16_t)0x0020) /* Bit 5 */
#define TIM_BKDT_DTGN_6         ((uint16_t)0x0040) /* Bit 6 */
#define TIM_BKDT_DTGN_7         ((uint16_t)0x0080) /* Bit 7 */

#define TIM_BKDT_LCKCFG         ((uint16_t)0x0300) /* LOCK[1:0] bits (Lock Configuration) */
#define TIM_BKDT_LCKCFG_0       ((uint16_t)0x0100) /* Bit 0 */
#define TIM_BKDT_LCKCFG_1       ((uint16_t)0x0200) /* Bit 1 */

#define TIM_BKDT_OSSI           ((uint16_t)0x0400) /* Off-State Selection for Idle mode */
#define TIM_BKDT_OSSR           ((uint16_t)0x0800) /* Off-State Selection for Run mode */
#define TIM_BKDT_BKEN           ((uint16_t)0x1000) /* Break enable */
#define TIM_BKDT_BKP            ((uint16_t)0x2000) /* Break Polarity */
#define TIM_BKDT_AOEN           ((uint16_t)0x4000) /* Automatic Output enable */
#define TIM_BKDT_MOEN           ((uint16_t)0x8000) /* Main Output enable */






/*** Independent watchdog ***/

/** Bit definition for IWDG_KEY register **/
#define  IWDG_KEY_KEYV                         ((uint16_t)0xFFFF) /* Key value (write only, read 0000h) */

/**  Bit definition for IWDG_PREDIV register  **/
#define  IWDG_PREDIV_PD                       ((uint8_t)0x07)     /* PD[2:0] (Prescaler divider) */
#define  IWDG_PREDIV_PD0                      ((uint8_t)0x01)     /* Bit 0 */
#define  IWDG_PREDIV_PD1                      ((uint8_t)0x02)     /* Bit 1 */
#define  IWDG_PREDIV_PD2                      ((uint8_t)0x04)     /* Bit 2 */

/**  Bit definition for IWDG_RELV register  **/
#define  IWDG_RELV_REL                         ((uint16_t)0x0FFF)  /* Watchdog counter reload value */

/**  Bit definition for IWDG_STS register  **/
#define  IWDG_STS_PVU                          ((uint8_t)0x01)     /* Watchdog prescaler value update */
#define  IWDG_STS_CRVU                         ((uint8_t)0x02)     /* Watchdog counter reload value update */

/**  Bit definition for IWDG_FREEZE register  **/
#define  IWDG_FREEZE_FREEZE                    ((uint8_t)0x01)     /* Watchdog freeze status value */

/*** Beeper ***/

/** Bit definition for BEEPER_CTRL register **/
#define BEEPER_CTRL_EN            ((uint32_t)0x00000001)  /* Bit[0] */
#define BEEPER_CTRL_INV_EN        ((uint32_t)0x00000002)  /* Bit[1] */

#define BEEPER_CTRL_FREQ_SEL       ((uint32_t)0x000000FC)  /* Beeper output frequency selection */
#define BEEPER_CTRL_FREQ_SEL_0     ((uint32_t)0x00000004)  /* Bit[0] */
#define BEEPER_CTRL_FREQ_SEL_1     ((uint32_t)0x00000008)  /* Bit[1] */
#define BEEPER_CTRL_FREQ_SEL_2     ((uint32_t)0x00000010)  /* Bit[2] */
#define BEEPER_CTRL_FREQ_SEL_3     ((uint32_t)0x00000020)  /* Bit[3] */
#define BEEPER_CTRL_FREQ_SEL_4     ((uint32_t)0x00000040)  /* Bit[4] */
#define BEEPER_CTRL_FREQ_SEL_5     ((uint32_t)0x00000080)  /* Bit[5] */

#define BEEPER_CTRL_FREQ_SEL_8K     ((uint32_t)0x00000004)  /* Beeper output frequency is 8kHz */
#define BEEPER_CTRL_FREQ_SEL_4K     ((uint32_t)0x0000000C)  /* Beeper output frequency is 8kHz */
#define BEEPER_CTRL_FREQ_SEL_2K     ((uint32_t)0x0000001C)  /* Beeper output frequency is 8kHz */
#define BEEPER_CTRL_FREQ_SEL_1K     ((uint32_t)0x0000003C)  /* Beeper output frequency is 8kHz */




/*** Serial Peripheral Interface ***/

/** Bit definition for SPI_CTRL1 register **/

#define SPI_CTRL1_CLKPHA    ((uint16_t)0x0001)      /* Clock Phase */
#define SPI_CTRL1_CLKPOL    ((uint16_t)0x0002)      /* Clock Polarity */
#define SPI_CTRL1_MSEL      ((uint16_t)0x0004)      /* Master Selection */

#define SPI_CTRL1_BR        ((uint16_t)0x0038)      /* BR[2:0] bits (Baud Rate Control) */
#define SPI_CTRL1_BR0       ((uint16_t)0x0008)      /* Bit 0 */
#define SPI_CTRL1_BR1       ((uint16_t)0x0010)      /* Bit 1 */
#define SPI_CTRL1_BR2       ((uint16_t)0x0020)      /* Bit 2 */

#define SPI_CTRL1_SPIEN     ((uint16_t)0x0040)      /* SPI Enable */
#define SPI_CTRL1_LSBFF     ((uint16_t)0x0080)      /* Frame Format */
#define SPI_CTRL1_SSEL      ((uint16_t)0x0100)      /* Internal slave select */
#define SPI_CTRL1_SSMEN     ((uint16_t)0x0200)      /* Software slave management */
#define SPI_CTRL1_RONLY     ((uint16_t)0x0400)      /* Receive only */
#define SPI_CTRL1_DATFF     ((uint16_t)0x0800)      /* Data Frame Format */
#define SPI_CTRL1_BIDIROEN  ((uint16_t)0x4000)      /* Output enable in bidirectional mode */
#define SPI_CTRL1_BIDIRMODE ((uint16_t)0x8000)      /* Bidirectional data mode enable */

/** Bit definition for SPI_CTRL2 register **/ 
#define SPI_CTRL2_SSOEN     ((uint8_t)0x04)         /* SS Output Enable */
#define SPI_CTRL2_ERRINTEN  ((uint8_t)0x20)         /* Error Interrupt Enable */
#define SPI_CTRL2_RNEINTEN  ((uint8_t)0x40)         /* RX buffer Not Empty Interrupt Enable */
#define SPI_CTRL2_TEINTEN   ((uint8_t)0x80)         /* Tx buffer Empty Interrupt Enable */

/** Bit definition for SPI_STS register **/   
#define SPI_STS_RNE         ((uint8_t)0x01)         /* Receive buffer Not Empty */
#define SPI_STS_TE          ((uint8_t)0x02)         /* Transmit buffer Empty */
#define SPI_STS_MODERR      ((uint8_t)0x20)         /* Mode fault */
#define SPI_STS_OVER        ((uint8_t)0x40)         /* Overrun flag */
#define SPI_STS_BUSY        ((uint8_t)0x80)         /* Busy flag */

/** Bit definition for SPI_DAT register **/
#define SPI_DAT_DAT         ((uint16_t)0xFFFF)      /* Data Register */


/*** Inter-integrated Circuit Interface ***/

/** Bit definition for I2C_CTRL1 register **/
#define I2C_CTRL1_EN       ((uint16_t)0x0001) /* Peripheral Enable */
#define I2C_CTRL1_PECEN    ((uint16_t)0x0020) /* PEC Enable */
#define I2C_CTRL1_GCEN     ((uint16_t)0x0040) /* General Call Enable */
#define I2C_CTRL1_NOEXTEND ((uint16_t)0x0080) /* Clock Stretching Disable (Slave mode) */
#define I2C_CTRL1_STARTGEN ((uint16_t)0x0100) /* Start Generation */
#define I2C_CTRL1_STOPGEN  ((uint16_t)0x0200) /* Stop Generation */
#define I2C_CTRL1_ACKEN    ((uint16_t)0x0400) /* Acknowledge Enable */
#define I2C_CTRL1_ACKPOS   ((uint16_t)0x0800) /* Acknowledge/PEC Position (for data reception) */
#define I2C_CTRL1_PEC      ((uint16_t)0x1000) /* Packet Error Checking */
#define I2C_CTRL1_SWRESET  ((uint16_t)0x8000) /* Software Reset */

/** Bit definition for I2C_CTRL2 register **/
#define I2C_CTRL2_CLKFREQ   ((uint16_t)0x003F) /* FREQ[5:0] bits (Peripheral Clock Frequency) */
#define I2C_CTRL2_CLKFREQ_0 ((uint16_t)0x0001) /* Bit 0 */
#define I2C_CTRL2_CLKFREQ_1 ((uint16_t)0x0002) /* Bit 1 */
#define I2C_CTRL2_CLKFREQ_2 ((uint16_t)0x0004) /* Bit 2 */
#define I2C_CTRL2_CLKFREQ_3 ((uint16_t)0x0008) /* Bit 3 */
#define I2C_CTRL2_CLKFREQ_4 ((uint16_t)0x0010) /* Bit 4 */
#define I2C_CTRL2_CLKFREQ_5 ((uint16_t)0x0020) /* Bit 5 */

#define I2C_CTRL2_ERRINTEN ((uint16_t)0x0100) /* Error Interrupt Enable */
#define I2C_CTRL2_EVTINTEN ((uint16_t)0x0200) /* Event Interrupt Enable */
#define I2C_CTRL2_BUFINTEN ((uint16_t)0x0400) /* Buffer Interrupt Enable */

/** Bit definition for I2C_OADDR1 register **/
#define I2C_OADDR1_ADDR1_7 ((uint16_t)0x00FE) /* Interface Address */
#define I2C_OADDR1_ADDR8_9 ((uint16_t)0x0300) /* Interface Address */

#define I2C_OADDR1_ADDR0 ((uint16_t)0x0001) /* Bit 0 */
#define I2C_OADDR1_ADDR1 ((uint16_t)0x0002) /* Bit 1 */
#define I2C_OADDR1_ADDR2 ((uint16_t)0x0004) /* Bit 2 */
#define I2C_OADDR1_ADDR3 ((uint16_t)0x0008) /* Bit 3 */
#define I2C_OADDR1_ADDR4 ((uint16_t)0x0010) /* Bit 4 */
#define I2C_OADDR1_ADDR5 ((uint16_t)0x0020) /* Bit 5 */
#define I2C_OADDR1_ADDR6 ((uint16_t)0x0040) /* Bit 6 */
#define I2C_OADDR1_ADDR7 ((uint16_t)0x0080) /* Bit 7 */
#define I2C_OADDR1_ADDR8 ((uint16_t)0x0100) /* Bit 8 */
#define I2C_OADDR1_ADDR9 ((uint16_t)0x0200) /* Bit 9 */

#define I2C_OADDR1_ADDRMODE ((uint16_t)0x8000) /* Addressing Mode (Slave mode) */

/** Bit definition for I2C_OADDR2 register **/
#define I2C_OADDR2_DUALEN ((uint8_t)0x01) /* Dual addressing mode enable */
#define I2C_OADDR2_ADDR2  ((uint8_t)0xFE) /* Interface address */

/** Bit definition for I2C_DAT register **/
#define I2C_DAT_DATA ((uint8_t)0xFF) /* 8-bit Data Register */

/** Bit definition for I2C_STS1 register **/
#define I2C_STS1_STARTBF  ((uint16_t)0x0001) /* Start Bit (Master mode) */
#define I2C_STS1_ADDRF    ((uint16_t)0x0002) /* Address sent (master mode)/matched (slave mode) */
#define I2C_STS1_BSF      ((uint16_t)0x0004) /* Byte Transfer Finished */
#define I2C_STS1_ADDR10F  ((uint16_t)0x0008) /* 10-bit header sent (Master mode) */
#define I2C_STS1_STOPF    ((uint16_t)0x0010) /* Stop detection (Slave mode) */
#define I2C_STS1_RXDATNE  ((uint16_t)0x0040) /* Data Register not Empty (receivers) */
#define I2C_STS1_TXDATE   ((uint16_t)0x0080) /* Data Register Empty (transmitters) */
#define I2C_STS1_BUSERR   ((uint16_t)0x0100) /* Bus Error */
#define I2C_STS1_ARLOST   ((uint16_t)0x0200) /* Arbitration Lost (master mode) */
#define I2C_STS1_ACKFAIL  ((uint16_t)0x0400) /* Acknowledge Failure */
#define I2C_STS1_OVERRUN  ((uint16_t)0x0800) /* Overrun/Underrun */
#define I2C_STS1_PECERR   ((uint16_t)0x1000) /* PEC Error in reception */

/** Bit definition for I2C_STS2 register **/
#define I2C_STS2_MSMODE    ((uint16_t)0x0001) /* Master/Slave */
#define I2C_STS2_BUSY      ((uint16_t)0x0002) /* Bus Busy */
#define I2C_STS2_TRF       ((uint16_t)0x0004) /* Transmitter/Receiver */
#define I2C_STS2_GCALLADDR ((uint16_t)0x0010) /* General Call Address (Slave mode) */
#define I2C_STS2_DUALFLAG  ((uint16_t)0x0080) /* Dual Flag (Slave mode) */
#define I2C_STS2_PECVAL    ((uint16_t)0xFF00) /* Packet Error Checking Register */

/** Bit definition for I2C_CLKCTRL register **/
#define I2C_CLKCTRL_CLKCTRL ((uint16_t)0x0FFF) /* Clock Control Register in Fast/Standard mode (Master mode) */
#define I2C_CLKCTRL_DUTY    ((uint16_t)0x4000) /* Fast Mode Duty Cycle */
#define I2C_CLKCTRL_FSMODE  ((uint16_t)0x8000) /* I2C Master Mode Selection */

/******************  Bit definition for I2C_TMRISE register  *******************/
#define  I2C_TMRISE_TRISE   ((uint8_t)0x3F)               /* Maximum Rise Time in Fast/Standard mode (Master mode) */

/******************  Bit definition for I2C_GFLTRCTRL register  *******************/
#define  I2C_GFLTRCTRL_SDADFW   ((uint16_t)0x000F) /* SDA digital gfilter width selection */
#define  I2C_GFLTRCTRL_SCLDFW   ((uint16_t)0x00F0) /* SCL digital gfilter width selection */

#define  I2C_GFLTRCTRL_SDAAFW   ((uint16_t)0x0300) /* SDA analog gfilter width selection */
#define  I2C_GFLTRCTRL_SDAAFW_0 ((uint16_t)0x0100) /* Bit 0 */
#define  I2C_GFLTRCTRL_SDAAFW_1 ((uint16_t)0x0200) /* Bit 1 */

#define  I2C_GFLTRCTRL_SDAAFENN ((uint16_t)0x0400) /* SDA analog gfilter enable */

#define  I2C_GFLTRCTRL_SCLAFW   ((uint16_t)0x1800) /* SCL analog gfilter width selection */
#define  I2C_GFLTRCTRL_SCLAFW_0 ((uint16_t)0x0800) /* Bit 0 */
#define  I2C_GFLTRCTRL_SCLAFW_1 ((uint16_t)0x1000) /* Bit 1 */

#define  I2C_GFLTRCTRL_SCLAFENN ((uint16_t)0x2000) /* SCL analog gfilter enable */

/******************  Bit definition for I2C_BYTENUM register  *******************/
#define  I2C_BYTENUM_BYTENUM       ((uint16_t)0x3FFF)               /* Master receives bytes configuration */
#define  I2C_BYTENUM_RXFSEL        ((uint16_t)0x4000)               /* Receive end send condition selection */
#define  I2C_BYTENUM_BYTENUMEN     ((uint16_t)0x8000)               /* Master receive byte control enable */


/*** Universal Asynchronous Receiver Transmitter ***/
/** Bit definition for UART_STS register **/
#define UART_STS_PEF    ((uint16_t)0x0001) /* Parity Error */
#define UART_STS_FEF    ((uint16_t)0x0002) /* Framing Error */
#define UART_STS_NEF    ((uint16_t)0x0004) /* Noise Error Flag */
#define UART_STS_OREF   ((uint16_t)0x0008) /* OverRun Error */
#define UART_STS_IDLEF  ((uint16_t)0x0010) /* IDLE line detected */
#define UART_STS_RXDNE  ((uint16_t)0x0020) /* Read Data Register Not Empty */
#define UART_STS_TXC    ((uint16_t)0x0040) /* Transmission Complete */
#define UART_STS_TXDE   ((uint16_t)0x0080) /* Transmit Data Register Empty */

/** Bit definition for UART_DAT register **/
#define UART_DAT_DATV   ((uint16_t)0x01FF) /* Data value */

/** Bit definition for UART_BRCF register **/
#define UART_BRCF_DIV_Decimal ((uint16_t)0x000F) /* Fraction of UARTDIV */
#define UART_BRCF_DIV_Integer ((uint16_t)0xFFF0) /* Mantissa of UARTDIV */

/** Bit definition for UART_CTRL1 register **/
#define UART_CTRL1_SDBRK    ((uint16_t)0x0001) /* Send Break */
#define UART_CTRL1_RCVWU    ((uint16_t)0x0002) /* Receiver wakeup */
#define UART_CTRL1_RXEN     ((uint16_t)0x0004) /* Receiver Enable */
#define UART_CTRL1_TXEN     ((uint16_t)0x0008) /* Transmitter Enable */
#define UART_CTRL1_IDLEIEN  ((uint16_t)0x0010) /* IDLE Interrupt Enable */
#define UART_CTRL1_RXDNEIEN ((uint16_t)0x0020) /* RXNE Interrupt Enable */
#define UART_CTRL1_TXCIEN   ((uint16_t)0x0040) /* Transmission Complete Interrupt Enable */
#define UART_CTRL1_TXDEIEN  ((uint16_t)0x0080) /* PE Interrupt Enable */
#define UART_CTRL1_PEIEN    ((uint16_t)0x0100) /* PE Interrupt Enable */
#define UART_CTRL1_PSEL     ((uint16_t)0x0200) /* Parity Selection */
#define UART_CTRL1_PCEN     ((uint16_t)0x0400) /* Parity Control Enable */
#define UART_CTRL1_WUM      ((uint16_t)0x0800) /* Wakeup method */
#define UART_CTRL1_WL       ((uint16_t)0x1000) /* Word length */
#define UART_CTRL1_UEN      ((uint16_t)0x2000) /* UART Enable */

/** Bit definition for UART_CTRL2 register **/
#define UART_CTRL2_ADDR   ((uint16_t)0x000F) /* Address of the UART node */
#define UART_CTRL2_STPB   ((uint16_t)0x3000) /* STOP[1:0] bits (STOP bits) */
#define UART_CTRL2_STPB_0 ((uint16_t)0x1000) /* Bit 0 */
#define UART_CTRL2_STPB_1 ((uint16_t)0x2000) /* Bit 1 */

/** Bit definition for UART_CTRL3 register **/
#define UART_CTRL3_HDMEN  ((uint16_t)0x0008) /* Half-Duplex Selection */


/*** FLASH Registers ***/

/**  Bit definition for FLASH_AC register  **/
#define  FLASH_AC_LATENCY                   ((uint8_t)0x07)               /* LATENCY[2:0] bits (Latency) */
#define  FLASH_AC_LATENCY_0                 ((uint8_t)0x00)               /* Bit 0 = 0 */
#define  FLASH_AC_LATENCY_1                 ((uint8_t)0x01)               /* Bit 0 = 1 */
#define  FLASH_AC_LATENCY_2                 ((uint8_t)0x02)               /* Bit 0 = 0; Bit 1 = 1 */
#define  FLASH_AC_LATENCY_3                 ((uint8_t)0x03)               /* Bit 0 = 1; Bit 1 = 1 */

#define  FLASH_AC_PRFTBFEN                  ((uint8_t)0x10)               /* Prefetch Buffer Enable */
#define  FLASH_AC_PRFTBFSTS                 ((uint8_t)0x20)               /* Prefetch Buffer Status */

/**  Bit definition for FLASH_KEY register  **/
#define  FLASH_KEY_FKEY                    ((uint32_t)0xFFFFFFFF)        /* FPEC Key */

/**  Bit definition for FLASH_OPTKEY register  **/
#define  FLASH_OPTKEY_OPTKEY              ((uint32_t)0xFFFFFFFF)        /* Option Byte Key */

/**  Bit definition for FLASH_STS register  **/
#define  FLASH_STS_BUSY                       ((uint16_t)0x0001)               /* Busy */
#define  FLASH_STS_PGERR                      ((uint16_t)0x0004)               /* Programming Error */
#define  FLASH_STS_WRPERR                     ((uint16_t)0x0010)               /* Write Protection Error */
#define  FLASH_STS_EOP                        ((uint16_t)0x0020)               /* End of operation */
#define  FLASH_STS_SPRERR                     ((uint16_t)0x0100)               /* Spccial Pattern read ERROR */
#define  FLASH_STS_CFGERR                     ((uint16_t)0x0200)               /* Flash initial configuration error */

/**  Bit definition for FLASH_CTRL register  **/
#define  FLASH_CTRL_PG                         ((uint16_t)0x0001)            /* Programming */
#define  FLASH_CTRL_PER                        ((uint16_t)0x0002)            /* Page Erase */
#define  FLASH_CTRL_MER                        ((uint16_t)0x0004)            /* Mass Erase */
#define  FLASH_CTRL_OPTPG                      ((uint16_t)0x0010)            /* Option Byte Programming */
#define  FLASH_CTRL_OPTER                      ((uint16_t)0x0020)            /* Option Byte Erase */
#define  FLASH_CTRL_START                      ((uint16_t)0x0040)            /* Start */
#define  FLASH_CTRL_LOCK                       ((uint16_t)0x0080)            /* Lock */
#define  FLASH_CTRL_MMER                       ((uint16_t)0x0100)            /* Mass Erase flash and optionbytes*/
#define  FLASH_CTRL_OPTWRE                     ((uint16_t)0x0200)            /* Option Bytes Write Enable */
#define  FLASH_CTRL_ERRITE                     ((uint16_t)0x0400)            /* Error Interrupt Enable */
#define  FLASH_CTRL_EOPITE                     ((uint16_t)0x1000)            /* End of operation interrupt enable */

/**  Bit definition for FLASH_ADD register  **/
#define  FLASH_ADD_FADD                        ((uint32_t)0xFFFFFFFF)        /* Flash Address */

/**  Bit definition for FLASH_OB register  **/
#define  FLASH_OB_OBERR                      ((uint16_t)0x0001)            /* Option Byte Error */
#define  FLASH_OB_RDPRT1                     ((uint16_t)0x0002)            /* Read protection */

#define  FLASH_OB_USER                      ((uint16_t)0x007C)            /* User Option Bytes */
#define  FLASH_OB_WDG_SW                    ((uint16_t)0x0004)            /* WDG_SW */
#define  FLASH_OB_nRST_STOP                 ((uint16_t)0x0008)            /* nRST_STOP */
#define  FLASH_OB_nRST_PD                   ((uint16_t)0x0010)            /* nRST_STDBY */
#define  FLASH_OB_BOOT_LOCK                 ((uint16_t)0x0020)            /* BOOT_LOCK */
#define  FLASH_OB_NRST_PA0                  ((uint16_t)0x0040)            /* NRST_PA0 */

#define  FLASH_OB_DATA0_MSK                 ((uint32_t)0x0003FC00)        /* Data0 mask */
#define  FLASH_OB_DATA1_MSK                 ((uint32_t)0x03FC0000)        /* Data1 mask */
#define  FLASH_OB_RDPRT2                    ((uint32_t)0x80000000)        /* Read protection level 2 */

/**  Bit definition for FLASH_USER register  **/
#define  FLASH_USER_LVRCNT                  ((uint32_t)0x000000FF)       /* LVR filter control count value */
#define  FLASH_USER_LVRLS                   ((uint32_t)0x00000F00)       /* LVR gear selection */
#define  FLASH_USER_LVREN                   ((uint32_t)0x00001000)       /* LVR enable */
#define  FLASH_USER_LVRFILEN                ((uint32_t)0x00002000)       /* LVR filter enable */
#define  FLASH_USER_LVRRSTEN                ((uint32_t)0x00001000)       /* LVR reset enable */
#define  FLASH_USER_POR_DELAY               ((uint32_t)0x00FF0000)       /* Delay time of CPU reset after POR is triggered */

/**  Bit definition for FLASH_VTOR register  **/
#define  FLASH_VTOR_EN                      ((uint32_t)0x80000000)       /* VTOR enalbe */
#define  FLASH_VTOR_VALUE                   ((uint32_t)0x7FFFFFFF)       /* VTOR address value */



/*** Option Bytes register ***/

/**  Bit definition for FLASH_RDP register  **/
#define  FLASH_RDP_RDP1                      ((uint32_t)0x000000FF)        /* Read protection option byte */
#define  FLASH_RDP_nRDP1                     ((uint32_t)0x0000FF00)        /* Read protection complemented option byte */

/**  Bit definition for FLASH_USER register  **/
#define  FLASH_USER_USER                     ((uint32_t)0x00FF0000)        /* User option byte */
#define  FLASH_USER_nUSER                    ((uint32_t)0xFF000000)        /* User complemented option byte */

/**  Bit definition for FLASH_Data0 register  **/
#define  FLASH_Data0_Data0                   ((uint32_t)0x000000FF)        /* User data storage option byte */
#define  FLASH_Data0_nData0                  ((uint32_t)0x0000FF00)        /* User data storage complemented option byte */

/**  Bit definition for FLASH_Data1 register  **/
#define  FLASH_Data1_Data1                   ((uint32_t)0x00FF0000)        /* User data storage option byte */
#define  FLASH_Data1_nData1                  ((uint32_t)0xFF000000)        /* User data storage complemented option byte */

/**  Bit definition for FLASH_WRP0 register  **/
#define  FLASH_WRP0_USER2                     ((uint32_t)0x000000FF)        /* User2 option byte */
#define  FLASH_WRP0_nUSER2                    ((uint32_t)0x0000FF00)        /* User2 complemented option byte */

/**  Bit definition for FLASH_WRP1 register  **/
#define  FLASH_WRP1_USER3                     ((uint32_t)0x00FF0000)        /* User3 option byte */
#define  FLASH_WRP1_nUSER3                    ((uint32_t)0xFF000000)        /* User3 complemented option byte */

/**  Bit definition for FLASH_WRP3 register  **/
#define  FLASH_WRP3_USER4                     ((uint32_t)0x00FF0000)        /* User4 option byte */
#define  FLASH_WRP3_nUSER4                    ((uint32_t)0xFF000000)        /* User4 complemented option byte */

/**  Bit definition for FLASH_RDP2 register  **/
#define  FLASH_RDP_RDP2                      ((uint32_t)0x000000FF)        /* Read protection level 2 option byte */
#define  FLASH_RDP_nRDP2                     ((uint32_t)0x0000FF00)        /* Read protection level 2 complemented option byte */



/** Bit Offset register **/
#define REG_BIT1_OFFSET    ((uint32_t)0x00000001)
#define REG_BIT2_OFFSET    ((uint32_t)0x00000002)
#define REG_BIT3_OFFSET    ((uint32_t)0x00000003)
#define REG_BIT4_OFFSET    ((uint32_t)0x00000004)
#define REG_BIT5_OFFSET    ((uint32_t)0x00000005)
#define REG_BIT6_OFFSET    ((uint32_t)0x00000006)
#define REG_BIT7_OFFSET    ((uint32_t)0x00000007)
#define REG_BIT8_OFFSET    ((uint32_t)0x00000008)
#define REG_BIT9_OFFSET    ((uint32_t)0x00000009)
#define REG_BIT10_OFFSET   ((uint32_t)0x0000000A)
#define REG_BIT11_OFFSET   ((uint32_t)0x0000000B)
#define REG_BIT12_OFFSET   ((uint32_t)0x0000000C)
#define REG_BIT13_OFFSET   ((uint32_t)0x0000000D)
#define REG_BIT14_OFFSET   ((uint32_t)0x0000000E)
#define REG_BIT15_OFFSET   ((uint32_t)0x0000000F)
#define REG_BIT16_OFFSET   ((uint32_t)0x00000010)
#define REG_BIT17_OFFSET   ((uint32_t)0x00000011)
#define REG_BIT18_OFFSET   ((uint32_t)0x00000012)
#define REG_BIT19_OFFSET   ((uint32_t)0x00000013)
#define REG_BIT20_OFFSET   ((uint32_t)0x00000014)
#define REG_BIT21_OFFSET   ((uint32_t)0x00000015)
#define REG_BIT22_OFFSET   ((uint32_t)0x00000016)
#define REG_BIT23_OFFSET   ((uint32_t)0x00000017)
#define REG_BIT24_OFFSET   ((uint32_t)0x00000018)
#define REG_BIT25_OFFSET   ((uint32_t)0x00000019)
#define REG_BIT26_OFFSET   ((uint32_t)0x0000001A)
#define REG_BIT27_OFFSET   ((uint32_t)0x0000001B)
#define REG_BIT28_OFFSET   ((uint32_t)0x0000001C)
#define REG_BIT29_OFFSET   ((uint32_t)0x0000001D)
#define REG_BIT30_OFFSET   ((uint32_t)0x0000001E)
#define REG_BIT31_OFFSET   ((uint32_t)0x0000001F)
#define REG_BIT32_OFFSET   ((uint32_t)0x00000020)



#ifdef USE_STDPERIPH_DRIVER
  #include "n32g003_conf.h"
#endif

/*** Exported_macro ***/

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))



#ifdef __cplusplus
}
#endif

#endif /* __n32g003_H */




