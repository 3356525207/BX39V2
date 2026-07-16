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
#include "n32g003.h"
#include "n32g003_i2c.h"
#include "main.h"
#include "log.h"

/** I2C_Slave **/

#define TEST_BUFFER_SIZE  100
#define I2CT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
#define I2C_SLAVE_ADDR    0x10

#define I2C_TEST
#define I2Cx I2C
#define I2Cx_SCL_PIN GPIO_PIN_4
#define I2Cx_SDA_PIN GPIO_PIN_5
#define GPIOx        GPIOA
#define GPIO_AF_I2C  GPIO_AF6_I2C

uint8_t data_buf[TEST_BUFFER_SIZE] = {0};
volatile Status test_status        = FAILED;

Status Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

static __IO uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;

void CommTimeOut_CallBack(ErrCode_t errcode);

/**
*\*\name    i2c_slave_init.
*\*\fun     slave gpio/rcc/i2c initializes.
*\*\param   none
*\*\return  result 
**/
int i2c_slave_init(void)
{
    I2C_InitType i2c_slave;
    GPIO_InitType i2c_gpio;
    /* enable clk */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_I2C);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA|RCC_APB_PERIPH_AFIO);
    GPIOx->POD |= (I2Cx_SCL_PIN | I2Cx_SDA_PIN);//pull up pin

    GPIO_Structure_Initialize(&i2c_gpio);
    /*PA4 -- SCL; PA5 -- SDA*/
    i2c_gpio.Pin        = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    i2c_gpio.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST; 
    i2c_gpio.GPIO_Mode      = GPIO_MODE_AF_OD;
    i2c_gpio.GPIO_Alternate = GPIO_AF_I2C;
    GPIO_Peripheral_Initialize(GPIOx, &i2c_gpio);

    I2C_Reset(I2Cx);
    I2C_Initializes_Structure(&i2c_slave);
    i2c_slave.DutyCycle   = I2C_SMDUTYCYCLE_1;  
    i2c_slave.OwnAddr1    = I2C_SLAVE_ADDR;
    i2c_slave.AckEnable   = I2C_ACKEN;
    i2c_slave.AddrMode    = I2C_ADDR_MODE_7BIT;
    i2c_slave.ClkSpeed    = 100000; /* 100k */

    I2C_Initializes(I2Cx, &i2c_slave);

    I2C_ON(I2Cx);
    return 0;
}

/**
*\*\name    i2c_slave_send.
*\*\fun     slave send data.
*\*\param   data-data to send
*\*\param   len-length of data to send
*\*\return  send result 
**/
int i2c_slave_send(uint8_t* data, int len)
{
    uint32_t lastevent;
    uint8_t* sendBufferPtr = data;
    I2CTimeout = I2CT_LONG_TIMEOUT*10;
    
    while (!I2C_Event_Check(I2C, I2C_EVT_SLAVE_SEND_ADDR_MATCHED))/* send addr matched */ 
   {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(SLAVE_MODE);
            return 1;
        }
    }
    
    I2CTimeout = I2CT_LONG_TIMEOUT;

    I2C_ON(I2C);
    I2C_Data_Send(I2C, *sendBufferPtr++);
    while (1)
    {
        lastevent = I2C_Last_Event_Get(I2C);
        if (lastevent == I2C_EVT_SLAVE_DATA_SENDED)
        {
            I2C_Data_Send(I2C, *sendBufferPtr++);
            I2CTimeout = I2CT_LONG_TIMEOUT;
        }
        else if ((lastevent == I2C_EVT_SLAVE_ACK_MISS) || (lastevent == I2C_EVT_SLAVE_STOP_RECVD))
        {
            I2C_Flag_Status_Clear(I2C, I2C_FLAG_ACKFAIL);
            I2C_ON(I2C);
            break;
        }
        else if ((lastevent & 0x00000100) == 0x00000100)
        {
            CommTimeOut_CallBack(SLAVE_BUSERR);
            return 1;
        }
        else
        {
            if ((I2CTimeout--) == 0)
            {
                CommTimeOut_CallBack(SLAVE_UNKNOW);
                return 1;
            }
        }
    }
    return 0;
}

/**
*\*\name    i2c_slave_recv.
*\*\fun     slave receive data.
*\*\param   data-data to receive
*\*\param   rcv_len-length of data to receive
*\*\return  receive result
**/
int i2c_slave_recv(uint8_t* data, uint32_t rcv_len)
{
    uint32_t lastevent;
    
    I2CTimeout = I2CT_LONG_TIMEOUT*10;
    while (!I2C_Event_Check(I2C, I2C_EVT_SLAVE_RECV_ADDR_MATCHED))/* receive addr matched */    
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(SLAVE_MODE);
            return 1;
        }
    }
    I2CTimeout = I2CT_LONG_TIMEOUT;

    I2C_ON(I2C);
    while (1)
    {
        lastevent = I2C_Last_Event_Get(I2C);
        if (lastevent == I2C_EVT_SLAVE_DATA_RECVD)
        {
            *data++ = I2C_Data_Recv(I2C);
            I2CTimeout = I2CT_LONG_TIMEOUT;
        }
        else if (lastevent == I2C_EVT_SLAVE_STOP_RECVD)
        {
            I2C_ON(I2C);
            break;
        }
        else if ((lastevent & 0x00000100) == 0x00000100)
        {
            CommTimeOut_CallBack(SLAVE_BUSERR);
            return 1;
        }
        else
        {
            if ((I2CTimeout--) == 0)
            {
                CommTimeOut_CallBack(SLAVE_UNKNOW);
                return 1;
            }
        }
    }
    
    return 0;
}

/**
*\*\name    main.
*\*\fun     main function.
*\*\param   none
*\*\return  none 
**/
int main(void)
{
    uint16_t i = 0;

    log_init();
    log_info("this is a i2c slave test demo\r\n");
    /* Initialize the I2C EEPROM driver ----------------------------------------*/
    i2c_slave_init();

    while (1)
    {
        for(; ;)
        {
            /* Read data */
            if(i2c_slave_recv(data_buf, TEST_BUFFER_SIZE))
            {
                break;  /* receive fail, reaccept*/
            }
            
            /* Write data*/
            if(i2c_slave_send(data_buf, TEST_BUFFER_SIZE))
            {
                break;  /* send fail, reaccept*/
            }
            for (i = 0; i < TEST_BUFFER_SIZE; i++)
            {
                data_buf[i] = 0;
            }
        }
    }
}
/**
*\*\name    SystemNVICReset.
*\*\fun     System software reset.
*\*\param   none
*\*\return  none 
**/    
void SystemNVICReset(void)
{
    
    __disable_irq();
    log_info("***** NVIC system reset! *****\r\n");
    NVIC_SystemReset();
}

/**
*\*\name    IIC_RCCReset.
*\*\fun     RCC clock reset.
*\*\param   none
*\*\return  none 
**/
void IIC_RCCReset(void)
{
    
    RCC_Peripheral_Reset(RCC_RST_I2CRST);
    
    log_info("***** IIC module by RCC reset! *****\r\n");
    i2c_slave_init();
    
}

/**
*\*\name    IIC_SWReset.
*\*\fun     I2c software reset.
*\*\param   none
*\*\return  none 
**/
void IIC_SWReset(void)
{
    GPIO_InitType i2cx_gpio;
    
    GPIO_Structure_Initialize(&i2cx_gpio);
    i2cx_gpio.Pin            = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    i2cx_gpio.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    i2cx_gpio.GPIO_Mode      = GPIO_MODE_INPUT;
    GPIO_Peripheral_Initialize(GPIOx, &i2cx_gpio);
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    for (;;)
    {
        if ((I2Cx_SCL_PIN | I2Cx_SDA_PIN) == (GPIOx->PID & (I2Cx_SCL_PIN | I2Cx_SDA_PIN)))
        {
            I2Cx->CTRL1 |= I2C_CTRL1_SWRESET;
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            I2Cx->CTRL1 &= ~I2C_CTRL1_SWRESET;
            
            log_info("***** IIC module self reset! *****\r\n");
            break;
        }
        else
        {
            if ((I2CTimeout--) == 0)
            {
                IIC_RCCReset();
            }
        }
    }
}

/**
*\*\name    CommTimeOut_CallBack.
*\*\fun     Callback function.
*\*\param   none
*\*\return  none 
**/
void CommTimeOut_CallBack(ErrCode_t errcode)
{
    log_info("...ErrCode:%d\r\n", errcode);
    
#if (COMM_RECOVER_MODE == MODULE_SELF_RESET)
    IIC_SWReset();
#elif (COMM_RECOVER_MODE == MODULE_RCC_RESET)
    IIC_RCCReset();
#elif (COMM_RECOVER_MODE == SYSTEM_NVIC_RESET)
    SystemNVICReset();
#endif
}


