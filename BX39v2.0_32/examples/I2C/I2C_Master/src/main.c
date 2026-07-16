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



/** I2C_Master **/

#define I2C_MASTER_SPEED  100000 /* 100K */
#define TEST_BUFFER_SIZE  100
#define I2CT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
#define I2C_MASTER_ADDR   0x30
#define I2C_SLAVE_ADDR    0x10

#ifdef NON_REENTRANT /* Avoid function reentrant */
static uint32_t Mutex_Flag = 0;
#endif

/* Enable master receive bytenumber control mode */
//#define MASTER_RECEIVE_BYTENUM_MODE  

#define I2C_TEST
#define I2C_REMAP
#define I2Cx I2C
#define I2Cx_SCL_PIN GPIO_PIN_4
#define I2Cx_SDA_PIN GPIO_PIN_5
#define GPIOx        GPIOA
#define GPIO_AF_I2C  GPIO_AF6_I2C

static uint8_t tx_buf[TEST_BUFFER_SIZE] = {0};
static uint8_t rx_buf[TEST_BUFFER_SIZE] = {0};
volatile Status test_status      = FAILED;
static __IO uint32_t I2CTimeout;
static CommCtrl_t Comm_Flag = C_READY;  /* Avoid star flag and stop flag set 1 at the same time */
Status Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void CommTimeOut_CallBack(ErrCode_t errcode);

/**
*\*\name    Delay.
*\*\fun     system ms delay function.
*\*\param   nCount
*\*\return  none 
**/
void Delay(uint32_t nCount)
{
    uint32_t tcnt;
    while (nCount--)
    {
        tcnt = 48000 / 5;
        while (tcnt--){;}
    }
}

/**
*\*\name    Delay_us.
*\*\fun     system us delay function.
*\*\param   nCount
*\*\return  none 
**/
void Delay_us(uint32_t nCount)
{
    uint32_t tcnt;
    while (nCount--)
    {
        tcnt = 48 / 5;
        while (tcnt--){;}
    }
}

/**
*\*\name    i2c_master_init.
*\*\fun     master gpio/rcc/i2c initializes.
*\*\param   none
*\*\return  result 
**/
int i2c_master_init(void)
{
    I2C_InitType i2c_master;
    GPIO_InitType i2c_gpio;
    
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_I2C);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA|RCC_APB_PERIPH_AFIO);
    GPIOx->POD |= (I2Cx_SCL_PIN | I2Cx_SDA_PIN);//pull up pin

    GPIO_Structure_Initialize(&i2c_gpio);
    /*PA4 -- SCL; PA5 -- SDA*/
    i2c_gpio.Pin            = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    i2c_gpio.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST; 
    i2c_gpio.GPIO_Mode      = GPIO_MODE_AF_OD;
    i2c_gpio.GPIO_Alternate = GPIO_AF_I2C;
    i2c_gpio.GPIO_Pull      = GPIO_PULL_UP; 
    GPIO_Peripheral_Initialize(GPIOx, &i2c_gpio);

    I2C_Reset(I2Cx);
    I2C_Initializes_Structure(&i2c_master);
    i2c_master.DutyCycle   = I2C_SMDUTYCYCLE_1; 
    i2c_master.OwnAddr1    = I2C_MASTER_ADDR;
    i2c_master.AckEnable   = I2C_ACKEN;
    i2c_master.AddrMode    = I2C_ADDR_MODE_7BIT;
    i2c_master.ClkSpeed    = I2C_MASTER_SPEED; 

    I2C_Initializes(I2Cx, &i2c_master);
    
    I2C_ON(I2Cx);
    return 0;
}

/**
*\*\name    i2c_master_send.
*\*\fun     master send data.
*\*\param   data-data to send
*\*\param   len-length of data to send
*\*\return  send result 
**/
int i2c_master_send(uint8_t* data, int len)
{
    uint8_t* sendBufferPtr = data;
    
#ifdef NON_REENTRANT
    if (Mutex_Flag)
        return -1;
    else
        Mutex_Flag = 1; /* Enter function,Mutex_Flag = 1 */
#endif
    
    I2CTimeout             = I2CT_LONG_TIMEOUT;
    while (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
            return 1;
        }
    }
    
    if (Comm_Flag == C_READY)
    {
        Comm_Flag = C_START_BIT;
        I2C_Generate_Start_Enable(I2C);
    }
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_MODE_FLAG)) /* EV5 */
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_MODE);
            return 1;
        }
    }
    
    I2C_7bit_Addr_Send(I2C, I2C_SLAVE_ADDR, I2C_DIRECTION_SEND);   
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_TXMODE_FLAG)) /* EV6 */
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_TXMODE);
            return 1;
        }
    }
    Comm_Flag = C_READY;
    
    /* send data */
    while (len-- > 0)
    {
        I2C_Data_Send(I2C, *sendBufferPtr++);
        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_DATA_SENDING)) /* EV8 */
        {
            if ((I2CTimeout--) == 0)
            {
                CommTimeOut_CallBack(MASTER_SENDING);
                return 1;
            }
        }
    }

    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_DATA_SENDED)) /* EV8-2 */
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_SENDED);
            return 1;
        }
    }
    
    if (Comm_Flag == C_READY)
    {
        Comm_Flag = C_STOP_BIT;
        I2C_Generate_Stop_Enable(I2C);
    }
    
    while (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
            return 1;
        }
    }
    Comm_Flag = C_READY;

#ifdef NON_REENTRANT
    if (Mutex_Flag)
        Mutex_Flag = 0; /* Exit function,Mutex_Flag = 0 */
    else
        return -2;
#endif
    
    return 0;
}

/**
*\*\name    i2c_master_recv.
*\*\fun     master receive data.
*\*\param   data-data to receive
*\*\param   len-length of data to receive
*\*\return  receive result 
**/
int i2c_master_recv(uint8_t* data, int len)
{
    uint8_t* recvBufferPtr = data;

#ifdef NON_REENTRANT
    if (Mutex_Flag)
        return -1;
    else
        Mutex_Flag = 1; /* Enter function,Mutex_Flag = 1 */
#endif
 
    
    I2CTimeout             = I2CT_LONG_TIMEOUT;
    while (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
            return 1;
        }
    }
#ifdef MASTER_RECEIVE_BYTENUM_MODE
    I2C_Master_Received_DataBytesNum_Set(I2Cx,len);
    I2C_ByteNum_Last_StartStop_Set(I2Cx,I2C_BYTENUM_LAST_STOP);
    I2C_ByteNum_enable(I2Cx);
#endif
    I2C_Acknowledg_Enable(I2C);

    /* send start */
    if (Comm_Flag == C_READY)
    {
        Comm_Flag = C_START_BIT;
        I2C_Generate_Start_Enable(I2C);
    }
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_MODE_FLAG)) /* EV5 */
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_MODE);
            return 1;
        }
    }
    /* send addr */
    I2C_7bit_Addr_Send(I2C, I2C_SLAVE_ADDR, I2C_DIRECTION_RECV);
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_RXMODE_FLAG)) /* EV6 */
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_RXMODE);
            return 1;
        }
    }
    Comm_Flag = C_READY;
    
#ifdef MASTER_RECEIVE_BYTENUM_MODE
    while (len)
    {
           
        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_DATA_RECVD_FLAG)) /* EV7 */
        {
            if ((I2CTimeout--) == 0)
            {
                CommTimeOut_CallBack(MASTER_RECVD);
                return 1;
            }
        }
        *recvBufferPtr++ = I2C_Data_Recv(I2C);
        len--;
    }
    Comm_Flag = C_STOP_BIT;
  
#else    
    if (len == 1)
    {
        I2C_Acknowledg_Disable(I2C);
        (void)(I2C->STS1); /*/ clear ADDR */
        (void)(I2C->STS2);
        if (Comm_Flag == C_READY)
        {
            Comm_Flag = C_STOP_BIT;
            I2C_Generate_Stop_Enable(I2C);
        }
        
        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (!I2C_Flag_Status_Get(I2C, I2C_FLAG_RXDATNE))
        {
            if ((I2CTimeout--) == 0)
            {
                CommTimeOut_CallBack(MASTER_RECVD);
                return 1;
            }
        }
        *recvBufferPtr++ = I2C_Data_Recv(I2C);
        len--;
    }
    else if (len == 2)
    {
        I2C_NACK_Position_Set(I2C, I2C_NACK_POS_NEXT); /*/ set ACKPOS */
        (void)(I2C->STS1);
        (void)(I2C->STS2);
        I2C_Acknowledg_Disable(I2C);
        
        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (!I2C_Flag_Status_Get(I2C, I2C_FLAG_BYTEF))
        {
            if ((I2CTimeout--) == 0)
            {
                CommTimeOut_CallBack(MASTER_BYTEF);
                return 1;
            }
        }
        
        if (Comm_Flag == C_READY)
        {
            Comm_Flag = C_STOP_BIT;
            I2C_Generate_Stop_Enable(I2C);
        }
        
        *recvBufferPtr++ = I2C_Data_Recv(I2C);
        len--;
        *recvBufferPtr++ = I2C_Data_Recv(I2C);
        len--;
        I2C_NACK_Position_Set(I2C, I2C_NACK_POS_CURRENT); /*/ clear ACKPOS */
    }
    else
    {
        I2C_Acknowledg_Enable(I2C);
        (void)(I2C->STS1);
        (void)(I2C->STS2);
        
        while (len)
        {
            if (len == 3)
            {
                I2CTimeout = I2CT_LONG_TIMEOUT;
                while (!I2C_Flag_Status_Get(I2C, I2C_FLAG_BYTEF))
                {
                    if ((I2CTimeout--) == 0)
                    {
                        CommTimeOut_CallBack(MASTER_BYTEF);
                        return 1;
                    }
                }
                I2C_Acknowledg_Disable(I2C);
                *recvBufferPtr++ = I2C_Data_Recv(I2C);
                len--;
                
                I2CTimeout = I2CT_LONG_TIMEOUT;
                while (!I2C_Flag_Status_Get(I2C, I2C_FLAG_BYTEF))
                {
                    if ((I2CTimeout--) == 0)
                    {
                        CommTimeOut_CallBack(MASTER_BYTEF);
                        return 1;
                    }
                }
                
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_Generate_Stop_Enable(I2C);
                }
        
                *recvBufferPtr++ = I2C_Data_Recv(I2C);
                len--;
                *recvBufferPtr++ = I2C_Data_Recv(I2C);
                len--;
                
                break;
            }
            
            I2CTimeout = I2CT_LONG_TIMEOUT;
            while (!I2C_Event_Check(I2C, I2C_EVT_MASTER_DATA_RECVD_FLAG)) /* EV7 */
            {
                if ((I2CTimeout--) == 0)
                {
                    CommTimeOut_CallBack(MASTER_RECVD);
                    return 1;
                }
            }
            *recvBufferPtr++ = I2C_Data_Recv(I2C);
            len--;
        }
    }
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
            return 1;
        }
    }
    Comm_Flag = C_READY;
#endif  


    
#ifdef NON_REENTRANT
    if (Mutex_Flag)
        Mutex_Flag = 0; /* Exit function,Mutex_Flag = 0 */
    else
        return -2;
#endif
    
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
    log_info(" this is a i2c master demo\r\n");
    /* Initialize the I2C EEPROM driver ----------------------------------------*/
    i2c_master_init();

    /* Fill the buffer to send */
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        tx_buf[i] = i;
    }
    while (1)
    {
         for(; ;)
        {
            /* First write in the memory followed by a read of the written data --------*/
            /* Write data*/
            if(i2c_master_send(tx_buf, TEST_BUFFER_SIZE))
            {
                break;  /* send fail, resend*/
            }

            /* Read data */
            if(i2c_master_recv(rx_buf, TEST_BUFFER_SIZE))
            {
                break; /* receive fail, resend*/
            }

            /* Check if the data written to the memory is read correctly */
            test_status = Buffercmp(tx_buf, rx_buf, TEST_BUFFER_SIZE);
            if (test_status == PASSED)
            {
                log_info("write and read data are same, i2c master test pass\r\n");
            }
            else
            {
                log_info("write and read data are different, i2c master test fail\r\n");
            }
            for (i = 0; i < TEST_BUFFER_SIZE; i++)
            {
                rx_buf[i] = 0;
            }
            Delay_us(50000);
        }
    }
}

/**
*\*\name   Buffercmp.
*\*\fun    Compares two buffers.
*\*\param  pBuffer, pBuffer1: buffers to be compared.
*\*\param  BufferLength: buffer's length
*\*\return PASSED: pBuffer identical to pBuffer1
*\*\       FAILED: pBuffer differs from pBuffer1
**/
Status Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer != *pBuffer1)
        {
            return FAILED;
        }

        pBuffer++;
        pBuffer1++;
    }

    return PASSED;
}

/**
*\*\name    IIC_RestoreSlaveByClock.
*\*\fun     Emulate 9 clock recovery slave by GPIO.
*\*\param   none
*\*\return  none 
**/
void IIC_RestoreSlaveByClock(void)
{
    uint8_t i;
    GPIO_InitType i2cx_gpio;
    
    GPIOx->POD |= (I2Cx_SCL_PIN | I2Cx_SDA_PIN);//pull up pin
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
    GPIOx->POD |= (I2Cx_SCL_PIN | I2Cx_SDA_PIN);//pull up pin
    
    GPIO_Structure_Initialize(&i2cx_gpio);
    i2cx_gpio.Pin            = I2Cx_SCL_PIN;
    i2cx_gpio.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    i2cx_gpio.GPIO_Mode      = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOx, &i2cx_gpio);
    
    for (i = 0; i < 9; i++)
    {
        GPIO_Pins_Set(GPIOx, I2Cx_SCL_PIN);
        Delay_us(5);
        GPIO_PBC_Pins_Reset(GPIOx, I2Cx_SCL_PIN);
        Delay_us(5);
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
    
    IIC_RestoreSlaveByClock();
    
    log_info("***** IIC module by RCC reset! *****\r\n");
    i2c_master_init();    
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
    Mutex_Flag = 0;
    Comm_Flag = C_READY;
    
#if (COMM_RECOVER_MODE == MODULE_SELF_RESET)
    IIC_SWReset();
#elif (COMM_RECOVER_MODE == MODULE_RCC_RESET)
    IIC_RCCReset();
#elif (COMM_RECOVER_MODE == SYSTEM_NVIC_RESET)
    SystemNVICReset();
#endif
}


