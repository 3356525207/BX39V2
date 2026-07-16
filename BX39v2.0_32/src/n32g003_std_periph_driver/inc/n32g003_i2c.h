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
*\*\file n32g003_i2c.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32G003_I2C_H_
#define __N32G003_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "n32g003.h"

/**  N32G003_StdPeriph_Driver
 *
 */

/** I2C Init structure definition **/

typedef struct
{
    uint32_t ClkSpeed; /* Specifies the clock frequency. */

    uint16_t DutyCycle; /* Specifies the I2C duty cycle. */

    uint16_t OwnAddr1; /* Specifies the first device own address. */

    uint16_t AckEnable; /* Enables or disables the acknowledgement. */

    uint16_t AddrMode; /* Specifies if 7-bit or 10-bit address is acknowledged. */
} I2C_InitType;



#define APB_FREQ_MAX_VALUE    (48) /*48M*/
#define CLK_SPEED_100K        (100000)
#define CLK_SPEED_400K        (400000)
#define CLK_SPEED_1M          (1000000)
#define SM_CLKCTRL_LOW_LIMIT  (0x04)
#define FM_CLKCTRL_LOW_LIMIT  (0x01)
#define SM_TRISE_100K         (1000) /*1000ns*/
#define FM_TRISE_400K         (300)  /*300ns*/
#define FM_TRISE_1M           (120)  /*120ns*/

/** Register shift macro definition **/
#define  RCC_FLAG_STS2_OFFSET     (REG_BIT16_OFFSET)
#define  RCC_FLAG_GET_OFFSET      (REG_BIT28_OFFSET)

#define I2C_REG_BIT_MASK      ((uint16_t)0x0000)

/* I2C EN mask */
#define I2C_EN_SET      (I2C_CTRL1_EN)
#define I2C_EN_RESET    (~I2C_CTRL1_EN)

/* I2C START mask */
#define I2C_START_SET   (I2C_CTRL1_STARTGEN)
#define I2C_START_RESET (~I2C_CTRL1_STARTGEN)

/* I2C STOP mask */
#define I2C_STOP_SET    (I2C_CTRL1_STOPGEN)
#define I2C_STOP_RESET  (~I2C_CTRL1_STOPGEN)

/* I2C ACK mask */
#define I2C_ACK_SET     (I2C_CTRL1_ACKEN)
#define I2C_ACK_RESET   (~I2C_CTRL1_ACKEN)

/* I2C ENGC mask */
#define I2C_GCEN_SET    (I2C_CTRL1_GCEN)
#define I2C_GCEN_RESET  (~I2C_CTRL1_GCEN)

/* I2C SWRST mask */
#define I2C_SWRESET_SET   (I2C_CTRL1_SWRESET)
#define I2C_SWRESET_RESET (~I2C_CTRL1_SWRESET)

/* I2C PEC mask */
#define I2C_PEC_SET        (I2C_CTRL1_PEC)
#define I2C_PEC_RESET      (~I2C_CTRL1_PEC)

/* I2C ENPEC mask */
#define I2C_PECEN_SET      (I2C_CTRL1_PECEN)
#define I2C_PECEN_RESET    (~I2C_CTRL1_PECEN)

/* I2C NOSTRETCH mask */
#define I2C_NOEXTEND_SET   (I2C_CTRL1_NOEXTEND)
#define I2C_NOEXTEND_RESET (~I2C_CTRL1_NOEXTEND)

/* I2C registers Masks */
#define I2C_ACKEN_CLR_MASK    (~(I2C_CTRL1_ACKEN))
#define I2C_ADDRMODE_CLR_MASK (~(I2C_OADDR1_ADDRMODE))
#define I2C_OADDR_CLR_MASK    (~(I2C_OADDR1_ADDR0 | I2C_OADDR1_ADDR1_7 | I2C_OADDR1_ADDR8_9))

/* I2C FREQ mask */
#define I2C_CLKFREQ_RESET (~I2C_CTRL2_CLKFREQ)

/* I2C ADD0 mask */
#define I2C_ADDR0_SET    (I2C_OADDR1_ADDR0)
#define I2C_ADDR0_RESET  (~I2C_OADDR1_ADDR0)


/* I2C ENDUAL mask */
#define I2C_DUALEN_SET   (((uint16_t)I2C_OADDR2_DUALEN))
#define I2C_DUALEN_RESET (~((uint16_t)I2C_OADDR2_DUALEN))

/* I2C ADD2 mask */
#define I2C_ADDR2_SET    ((uint16_t)I2C_OADDR2_ADDR2)
#define I2C_ADDR2_RESET  (~((uint16_t)I2C_OADDR2_ADDR2))

/* I2C F/S mask */
#define I2C_FSMODE_SET  (I2C_CLKCTRL_FSMODE)

/* I2C CHCFG mask */
#define I2C_CLKCTRL_SET (I2C_CLKCTRL_CLKCTRL)

/* I2C TMRISE mask */
#define I2C_SDADFW_MASK  (~I2C_GFLTRCTRL_SDADFW)
#define I2C_SCLDFW_MASK  (~I2C_GFLTRCTRL_SCLDFW)
#define I2C_SDAAFW_MASK  (~I2C_GFLTRCTRL_SDAAFW)
#define I2C_SCLAFW_MASK  (~I2C_GFLTRCTRL_SCLAFW)

#define I2C_SDAAFENN_SET  (I2C_GFLTRCTRL_SDAAFENN)
#define I2C_SDAAFENN_RESET  (~I2C_GFLTRCTRL_SDAAFENN)

#define I2C_SCLAFENN_SET  (I2C_GFLTRCTRL_SCLAFENN)
#define I2C_SCLAFENN_RESET  (~I2C_GFLTRCTRL_SCLAFENN)

#define I2C_TMRISE_MASK  (I2C_TMRISE_TRISE)


/* I2C FLAG mask */
#define I2C_FLAG_Mask   ((uint32_t)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define I2C_INTEN_MASK  ((uint32_t)0x07000000)


/**  I2C_duty_cycle **/
#define I2C_SMDUTYCYCLE_1       (I2C_REG_BIT_MASK) /* I2C standard mode Tlow/Thigh = 1/1 */
#define I2C_FMDUTYCYCLE_16_9    (I2C_CLKCTRL_DUTY) /* I2C fast mode Tlow/Thigh = 16/9 */
#define I2C_FMDUTYCYCLE_2       (~I2C_CLKCTRL_DUTY) /* I2C fast mode Tlow/Thigh = 2 */

/**  I2C_acknowledgement **/
#define I2C_ACKEN               (I2C_CTRL1_ACKEN)
#define I2C_ACKDIS              (I2C_REG_BIT_MASK)

/**  I2C_transfer_direction **/
#define I2C_DIRECTION_SEND          ((uint8_t)0x00)
#define I2C_DIRECTION_RECV          ((uint8_t)0x01)

/**  I2C_acknowledged_address **/
/* Bit 14 Should be kept at 1 by software*/
#define I2C_ADDR_MODE_7BIT    (((uint16_t)0x4000) | I2C_REG_BIT_MASK)
#define I2C_ADDR_MODE_10BIT   (((uint16_t)0x4000) | I2C_OADDR1_ADDRMODE)


/**  I2C_registers **/

#define I2C_REG_CTRL1      ((uint8_t)0x00)
#define I2C_REG_CTRL2      ((uint8_t)0x04)
#define I2C_REG_OADDR1     ((uint8_t)0x08)
#define I2C_REG_OADDR2     ((uint8_t)0x0C)
#define I2C_REG_DAT        ((uint8_t)0x10)
#define I2C_REG_STS1       ((uint8_t)0x14)
#define I2C_REG_STS2       ((uint8_t)0x18)
#define I2C_REG_CLKCTRL    ((uint8_t)0x1C)
#define I2C_REG_TMRISE     ((uint8_t)0x20)
#define I2C_REG_GFLTRCTRL  ((uint8_t)0x24)
#define I2C_REG_BYTENUM    ((uint8_t)0x28)


/**  I2C_PEC_position **/
#define I2C_PEC_POS_NEXT         (I2C_CTRL1_ACKPOS)
#define I2C_PEC_POS_CURRENT      (~I2C_CTRL1_ACKPOS)

/**  I2C_NCAK_position **/
#define I2C_NACK_POS_NEXT         (I2C_CTRL1_ACKPOS)
#define I2C_NACK_POS_CURRENT      (~I2C_CTRL1_ACKPOS)

/**  I2C_Analog_Filter_Width **/
#define I2C_ANALOG_FILTER_WIDTH_5NS   ((uint16_t)0x0000)
#define I2C_ANALOG_FILTER_WIDTH_15NS  (I2C_GFLTRCTRL_SDAAFW_0)
#define I2C_ANALOG_FILTER_WIDTH_25NS  (I2C_GFLTRCTRL_SDAAFW_1)
#define I2C_ANALOG_FILTER_WIDTH_35NS  (I2C_GFLTRCTRL_SDAAFW_0|I2C_GFLTRCTRL_SDAAFW_1)

/**  I2C_interrupts_definition **/
#define I2C_INT_BUF        (I2C_CTRL2_BUFINTEN)
#define I2C_INT_EVENT      (I2C_CTRL2_EVTINTEN)
#define I2C_INT_ERR        (I2C_CTRL2_ERRINTEN)

/* I2C BYTE_NUM_EN mask */
#define I2C_BYTENUMEN_SET              (I2C_BYTENUM_BYTENUMEN)
#define I2C_BYTENUMEN_RESET            (~I2C_BYTENUM_BYTENUMEN)

/* I2C BYTE_NUM byte number mask */
#define I2C_BYTENUM_MASK              (~I2C_BYTENUM_BYTENUM)

/* I2C master sended status after finishing receiving data bytes */
#define I2C_BYTENUM_LAST_STOP    (~I2C_BYTENUM_RXFSEL)
#define I2C_BYTENUM_LAST_START   (I2C_BYTENUM_RXFSEL)


/**  I2C_interrupts_definition **/
#define I2C_INT_PECERR   (((uint32_t)0x01000000) | ((uint32_t)I2C_STS1_PECERR))
#define I2C_INT_OVERRUN  (((uint32_t)0x01000000) | ((uint32_t)I2C_STS1_OVERRUN))
#define I2C_INT_ACKFAIL  (((uint32_t)0x01000000) | ((uint32_t)I2C_STS1_ACKFAIL))
#define I2C_INT_ARLOST   (((uint32_t)0x01000000) | ((uint32_t)I2C_STS1_ARLOST))
#define I2C_INT_BUSERR   (((uint32_t)0x01000000) | ((uint32_t)I2C_STS1_BUSERR))
#define I2C_INT_TXDATE   (((uint32_t)0x06000000) | ((uint32_t)I2C_STS1_TXDATE))
#define I2C_INT_RXDATNE  (((uint32_t)0x06000000) | ((uint32_t)I2C_STS1_RXDATNE))
#define I2C_INT_STOPF    (((uint32_t)0x02000000) | ((uint32_t)I2C_STS1_STOPF))
#define I2C_INT_ADDR10F  (((uint32_t)0x02000000) | ((uint32_t)I2C_STS1_ADDR10F))
#define I2C_INT_BYTEF    (((uint32_t)0x02000000) | ((uint32_t)I2C_STS1_BSF))
#define I2C_INT_ADDRF    (((uint32_t)0x02000000) | ((uint32_t)I2C_STS1_ADDRF))
#define I2C_INT_STARTBF  (((uint32_t)0x02000000) | ((uint32_t)I2C_STS1_STARTBF))

/**  I2C_flags_definition **/

/** STS2 register flags **/

#define I2C_FLAG_DUALFLAG  (((uint32_t)I2C_STS2_DUALFLAG)<<16)
#define I2C_FLAG_GCALLADDR (((uint32_t)I2C_STS2_GCALLADDR)<<16)
#define I2C_FLAG_TRF       (((uint32_t)I2C_STS2_TRF)<<16)
#define I2C_FLAG_BUSY      (((uint32_t)I2C_STS2_BUSY)<<16)
#define I2C_FLAG_MSMODE    (((uint32_t)I2C_STS2_MSMODE)<<16)

/** STS1 register flags **/

#define I2C_FLAG_PECERR   (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_PECERR))
#define I2C_FLAG_OVERRUN  (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_OVERRUN))
#define I2C_FLAG_ACKFAIL  (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_ACKFAIL))
#define I2C_FLAG_ARLOST   (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_ARLOST))
#define I2C_FLAG_BUSERR   (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_BUSERR))
#define I2C_FLAG_TXDATE   (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_TXDATE))
#define I2C_FLAG_RXDATNE  (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_RXDATNE))
#define I2C_FLAG_STOPF    (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_STOPF))
#define I2C_FLAG_ADDR10F  (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_ADDR10F))
#define I2C_FLAG_BYTEF    (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_BSF))
#define I2C_FLAG_ADDRF    (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_ADDRF))
#define I2C_FLAG_STARTBF  (((uint32_t)0x10000000) | ((uint32_t)I2C_STS1_STARTBF))


/**  I2C_Events **/

/** I2C Master Events (Events grouped in order of communication) **/
/**
*\*\brief  Communication start
*\*\After sending the START condition (I2C_Generate_Start_Enable() function) the master
*\*\has to wait for this event. It means that the Start condition has been correctly
*\*\released on the I2C bus (the bus is free, no other devices is communicating).
**/

/* MSMODE */ 
#define I2C_ROLE_MASTER (((uint32_t)I2C_STS2_MSMODE)<<16) 
/* EV5 */
/* BUSY, MSMODE and STARTBF flag*/
#define I2C_EVT_MASTER_MODE_FLAG (((uint32_t)I2C_STS1_STARTBF)\
                                |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE))<<16))
 

/**
*\*\brief  Address Acknowledge
*\*\After checking on EV5 (start condition correctly released on the bus), the
*\*\master sends the address of the slave(s) with which it will communicate
*\*\(I2C_7bit_Addr_Send() function, it also determines the direction of the communication:
*\*\Master transmitter or Receiver). Then the master has to wait that a slave acknowledges
*\*\his address. If an acknowledge is sent on the bus, one of the following events will
*\*\be set:
*\*\ 1) In case of Master Receiver (7-bit addressing): the I2C_EVT_MASTER_RXMODE_FLAG
*\*\    event is set.
*\*\ 2) In case of Master Transmitter (7-bit addressing): the I2C_EVT_MASTER_TXMODE_FLAG
*\*\    is set
*\*\ 3) In case of 10-Bit addressing mode, the master (just after generating the START
*\*\ and checking on EV5) has to send the header of 10-bit addressing mode (I2C_Data_Send()
*\*\ function). Then master should wait on EV9. It means that the 10-bit addressing
*\*\ header has been correctly sent on the bus. Then master should send the second part of
*\*\ the 10-bit address (LSB) using the function I2C_7bit_Addr_Send(). Then master
*\*\ should wait for event EV6.
 **/

/* --EV6 */
/* BUSY, MSMODE, ADDRF, TXDATE and TRF flags */
#define I2C_EVT_MASTER_TXMODE_FLAG (((uint32_t)I2C_STS1_ADDRF)|((uint32_t)I2C_STS1_TXDATE)\
                                  |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE)|((uint32_t)I2C_STS2_TRF))<<16)) 
/* BUSY, MSMODE and ADDRF flags */
#define I2C_EVT_MASTER_RXMODE_FLAG (((uint32_t)I2C_STS1_ADDRF)\
                                  |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE))<<16)) 
/* --EV9 */
/* BUSY, MSMODE and ADD10RF flags */
#define I2C_EVT_MASTER_MODE_ADDRESS10_FLAG (((uint32_t)I2C_STS1_ADDR10F)\
                                          |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE))<<16))  

/**
*\*\brief Communication events
*\*\If a communication is established (START condition generated and slave address
*\*\acknowledged) then the master has to check on one of the following events for
*\*\communication procedures:
*\*\1) Master Receiver mode: The master has to wait on the event EV7 then to read
*\*\   the data received from the slave (I2C_Data_Recv() function).
*\*\2) Master Transmitter mode: The master has to send data (I2C_Data_Send()
*\*\   function) then to wait on event EV8 or EV8_2.
*\*\   These two events are similar:
*\*\    - EV8 means that the data has been written in the data register and is
*\*\      being shifted out.
*\*\    - EV8_2 means that the data has been physically shifted out and output
*\*\      on the bus.
*\*\    In most cases, using EV8 is sufficient for the application.
*\*\    Using EV8_2 leads to a slower communication but ensure more reliable test.
*\*\    EV8_2 is also more suitable than EV8 for testing on the last data transmission
*\*\    (before Stop condition generation).
*\*\note In case the  user software does not guarantee that this event EV7 is
*\*\ managed before the current byte end of transfer, then user may check on EV7
*\*\ and BSF flag at the same time (ie. (I2C_EVT_MASTER_DATA_RECVD_FLAG | I2C_FLAG_BYTEF)).
*\*\ In this case the communication may be slower.
 **/

/* Master RECEIVER mode -----------------------------*/
/* --EV7 */
/* BUSY, MSMODE and RXDATNE flags */
#define I2C_EVT_MASTER_DATA_RECVD_FLAG (((uint32_t)I2C_STS1_RXDATNE)\
                                       |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE))<<16)) 
                                       
/* EV7x shifter register full */
/* BUSY, MSMODE, BSF and RXDATNE flags */
#define I2C_EVT_MASTER_DATA_RECVD_BSF_FLAG (((uint32_t)I2C_STS1_RXDATNE)|((uint32_t)I2C_STS1_BSF)\
                                          |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE))<<16))

/* Master TRANSMITTER mode --------------------------*/
/* --EV8 */
/* TRF, BUSY, MSMODE, TXDATE flags */
#define I2C_EVT_MASTER_DATA_SENDING (((uint32_t)I2C_STS1_TXDATE)\
                  |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE)|((uint32_t)I2C_STS2_TRF))<<16)) 
/* --EV8_2 */
/* TRF, BUSY, MSMODE, TXDATE and BSF flags */
#define I2C_EVT_MASTER_DATA_SENDED (((uint32_t)I2C_STS1_BSF)|((uint32_t)I2C_STS1_TXDATE)\
                  |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_MSMODE)|((uint32_t)I2C_STS2_TRF))<<16)) 

/*========================================

                     I2C Slave Events (Events grouped in order of communication)
                                                        ==========================================*/

/**
*\*\brief  Communication start events
*\*\Wait on one of these events at the start of the communication. It means that
*\*\the I2C peripheral detected a Start condition on the bus (generated by master
*\*\device) followed by the peripheral address. The peripheral generates an ACK
*\*\condition on the bus (if the acknowledge feature is enabled through function
*\*\I2C_ConfigAck()) and the events listed above are set :
*\*\1) In normal case (only one address managed by the slave), when the address
*\*\  sent by the master matches the own address of the peripheral (configured by
*\*\  OwnAddr1 field) the I2C_EVENT_SLAVE_XXX_ADDRESS_MATCHED event is set
*\*\  (where XXX could be TRANSMITTER or RECEIVER).
*\*\2) In case the address sent by the master matches the second address of the
*\*\  peripheral (configured by the function I2C_ConfigOwnAddr2() and enabled
*\*\  by the function I2C_EnableDualAddr()) the events I2C_EVENT_SLAVE_XXX_SECONDADDRESS_MATCHED
*\*\  (where XXX could be TRANSMITTER or RECEIVER) are set.
*\*\3) In case the address sent by the master is General Call (address 0x00) and
*\*\  if the General Call is enabled for the peripheral (using function I2C_EnableGeneralCall())
*\*\  the following event is set I2C_EVT_SLAVE_GCALLADDR_MATCHED.
 **/

/* --EV1  (all the events below are variants of EV1) */
/* 1) Case of One Single Address managed by the slave */
 /* BUSY and ADDRF flags */
#define I2C_EVT_SLAVE_RECV_ADDR_MATCHED (((uint32_t)I2C_STS1_ADDRF)\
                                       |((((uint32_t)I2C_STS2_BUSY))<<16)) 
 /* TRF, BUSY, TXDATE and ADDRF flags */
#define I2C_EVT_SLAVE_SEND_ADDR_MATCHED (((uint32_t)I2C_STS1_TXDATE)|((uint32_t)I2C_STS1_ADDRF)\
                                       |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_TRF))<<16))

/* 2) Case of Dual address managed by the slave */
 /* DUALF and BUSY flags */
#define I2C_EVT_SLAVE_RECV_ADDR2_MATCHED ((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_DUALFLAG))<<16) 
 /* DUALF, TRF, BUSY and TXDATE flags */
#define I2C_EVT_SLAVE_SEND_ADDR2_MATCHED (((uint32_t)I2C_STS1_TXDATE)\
               |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_DUALFLAG)|((uint32_t)I2C_STS2_TRF))<<16))

/* 3) Case of General Call enabled for the slave */
 /* GENCALL and BUSY flags */
#define I2C_EVT_SLAVE_GCALLADDR_MATCHED ((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_GCALLADDR))<<16) 

/**
*\*\brief  Communication events
*\*\Wait on one of these events when EV1 has already been checked and:
*\*\- Slave RECEIVER mode:
*\*\    - EV2: When the application is expecting a data byte to be received.
*\*\    - EV4: When the application is expecting the end of the communication: master
*\*\      sends a stop condition and data transmission is stopped.
*\*\- Slave Transmitter mode:
*\*\   - EV3: When a byte has been transmitted by the slave and the application is expecting
*\*\     the end of the byte transmission. The two events I2C_EVT_SLAVE_DATA_SENDED and
*\*\     I2C_EVT_SLAVE_DATA_SENDING are similar. The second one can optionally be
*\*\     used when the user software doesn't guarantee the EV3 is managed before the
*\*\     current byte end of transfer.
*\*\   - EV3_2: When the master sends a NACK in order to tell slave that data transmission
*\*\     shall end (before sending the STOP condition). In this case slave has to stop sending
*\*\     data bytes and expect a Stop condition on the bus.
*\*\note In case the  user software does not guarantee that the event EV2 is
*\*\ managed before the current byte end of transfer, then user may check on EV2
*\*\ and BSF flag at the same time (ie. (I2C_EVT_SLAVE_DATA_RECVD | I2C_FLAG_BYTEF)).
*\*\In this case the communication may be slower.
 **/

/* Slave RECEIVER mode --------------------------*/
/* --EV2 */
 /* BUSY and RXDATNE flags */
#define I2C_EVT_SLAVE_DATA_RECVD (((uint32_t)I2C_STS1_RXDATNE)\
                                |((((uint32_t)I2C_STS2_BUSY))<<16))  
/* --EV2x */
 /* no BUSY and RXDATNE flags */
#define I2C_EVT_SLAVE_DATA_RECVD_NOBUSY ((uint32_t)I2C_STS1_RXDATNE)
/* --EV4  */
 /* STOPF flag */
#define I2C_EVT_SLAVE_STOP_RECVD ((uint32_t)I2C_STS1_STOPF)

/* Slave TRANSMITTER mode -----------------------*/
/* --EV3 */
 /* TRF, BUSY, TXDATE and BSF flags */
#define I2C_EVT_SLAVE_DATA_SENDED  (((uint32_t)I2C_STS1_BSF)|((uint32_t)I2C_STS1_TXDATE)\
                                  |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_TRF))<<16)) 
 /* TRF, BUSY and TXDATE flags */
#define I2C_EVT_SLAVE_DATA_SENDING (((uint32_t)I2C_STS1_TXDATE)\
                                  |((((uint32_t)I2C_STS2_BUSY)|((uint32_t)I2C_STS2_TRF))<<16)) 
/* --EV3_2 */
 /* AF flag */
#define I2C_EVT_SLAVE_ACK_MISS ((uint32_t)I2C_STS1_ACKFAIL)

/*===========================      End of Events Description           ==========================================*/

void I2C_Reset(I2C_Module* I2Cx);
void I2C_Initializes(I2C_Module* I2Cx, I2C_InitType* I2C_InitParam);
void I2C_Clock_Speed_Config(I2C_Module* I2Cx, uint32_t clk_speed, uint16_t duty_cycle);
void I2C_Acknowledgement_Config(I2C_Module* I2Cx, uint16_t ack);
void I2C_Addressing_Mode_Config(I2C_Module* I2Cx, uint16_t mode);
void I2C_Own_Addr1_Config(I2C_Module* I2Cx, uint16_t addr1);
void I2C_Initializes_Structure(I2C_InitType* I2C_InitStruct);
void I2C_ON(I2C_Module* I2Cx);
void I2C_OFF(I2C_Module* I2Cx);
void I2C_Generate_Start_Enable(I2C_Module* I2Cx);
void I2C_Generate_Start_Disable(I2C_Module* I2Cx);
void I2C_Generate_Stop_Enable(I2C_Module* I2Cx);
void I2C_Generate_Stop_Disable(I2C_Module* I2Cx);
void I2C_Acknowledg_Enable(I2C_Module* I2Cx);
void I2C_Acknowledg_Disable(I2C_Module* I2Cx);
void I2C_Own_Addr2_Set(I2C_Module* I2Cx, uint8_t address);
void I2C_Dual_Addr_Enable(I2C_Module* I2Cx);
void I2C_Dual_Addr_Disable(I2C_Module* I2Cx);
void I2C_General_Call_Enable(I2C_Module* I2Cx);
void I2C_General_Call_Disable(I2C_Module* I2Cx);
void I2C_Interrupts_Enable(I2C_Module* I2Cx, uint16_t I2C_IT);
void I2C_Interrupts_Disable(I2C_Module* I2Cx, uint16_t I2C_IT);
void I2C_Data_Send(I2C_Module* I2Cx, uint8_t data);
uint8_t I2C_Data_Recv(I2C_Module* I2Cx);
void I2C_7bit_Addr_Send(I2C_Module* I2Cx, uint8_t address, uint8_t mode);
uint16_t I2C_Register_Value_Get(I2C_Module* I2Cx, uint8_t I2C_Register);
void I2C_Software_Reset_Enable(I2C_Module* I2Cx);
void I2C_Software_Reset_Disable(I2C_Module* I2Cx);
void I2C_NACK_Position_Set(I2C_Module* I2Cx, uint16_t position);
void I2C_PEC_Send_Enable(I2C_Module* I2Cx);
void I2C_PEC_Send_Disable(I2C_Module* I2Cx);
void I2C_PEC_Position_Set(I2C_Module* I2Cx, uint16_t position);
void I2C_PEC_Compute_Enable(I2C_Module* I2Cx);
void I2C_PEC_Compute_Disable(I2C_Module* I2Cx);
uint8_t I2C_PEC_Get(I2C_Module* I2Cx);
void I2C_Extend_Clock_Enable(I2C_Module* I2Cx);
void I2C_Extend_Clock_Disable(I2C_Module* I2Cx);
void I2C_Fast_Mode_Duty_Cycle_Set(I2C_Module* I2Cx, uint16_t duty_cycle);
void I2C_SDA_Digital_Filter_Width_Set(I2C_Module* I2Cx, uint8_t width);
void I2C_SCL_Digital_Filter_Width_Set(I2C_Module* I2Cx, uint8_t width);
void I2C_SDA_Analog_Filter_Width_Set(I2C_Module* I2Cx, uint16_t width);
void I2C_SDA_Analog_Filter_Enable(I2C_Module* I2Cx);
void I2C_SDA_Analog_Filter_Disable(I2C_Module* I2Cx);
void I2C_SCL_Analog_Filter_Width_Set(I2C_Module* I2Cx, uint16_t width);
void I2C_SCL_Analog_Filter_Enable(I2C_Module* I2Cx);
void I2C_SCL_Analog_Filter_Disable(I2C_Module* I2Cx);
ErrorStatus I2C_Event_Check( I2C_Module* I2Cx, uint32_t I2C_event );
uint32_t I2C_Last_Event_Get( I2C_Module* I2Cx );
FlagStatus I2C_Flag_Status_Get( I2C_Module* I2Cx, uint32_t I2C_flag );
void I2C_Flag_Status_Clear( I2C_Module* I2Cx, uint32_t I2C_flag );
ITStatus I2C_Interrupt_Status_Get( I2C_Module* I2Cx, uint32_t Interrupt );
void I2C_Interrupt_Statu_Clear(I2C_Module* I2Cx, uint32_t Interrupt);
void I2C_ByteNum_enable(I2C_Module* I2Cx);
void I2C_ByteNum_Disable(I2C_Module* I2Cx);
void I2C_Master_Received_DataBytesNum_Set(I2C_Module* I2Cx, uint16_t Number_Of_bytes);
void I2C_ByteNum_Last_StartStop_Set(I2C_Module* I2Cx, uint16_t LastStatus);

#ifdef __cplusplus
}
#endif

#endif /*__N32G003_I2C_H */
/**
 *
 */
