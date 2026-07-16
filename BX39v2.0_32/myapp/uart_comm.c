#include "uart_comm.h"
#include <string.h>

/*==============================================================================
 * UART1 引脚映射: PB0=TX, PB1=RX, AF2
 *============================================================================*/
#define UART1_GPIO_PORT     GPIOB
#define UART1_TX_PIN        GPIO_PIN_0
#define UART1_RX_PIN        GPIO_PIN_1
#define UART1_GPIO_AF       GPIO_AF2_UART1
#define UART1_GPIO_CLK      RCC_APB_PERIPH_IOPB
#define UART1_CLK           RCC_APB_PERIPH_UART1

/*==============================================================================
 * 模块静态变量
 *============================================================================*/
static uint8_t  s_dev_addr;
static uint8_t  s_tx_buf[UART_TX_BUF_SIZE];

static volatile uint8_t  g_uart_regs[UART_REGS_SIZE];

static volatile UART_RxFrame_t  s_rx_frame;
static volatile FrameRxState_t  s_rx_state     = FRAME_STATE_START1;
static volatile uint8_t         s_rx_data_idx  = 0;
static volatile uint8_t         s_frame_ready  = 0;

static uint8_t UART1_CalcChecksum(const uint8_t *data, uint8_t len)
{
    uint8_t checksum = 0;
    uint8_t i;

    for (i = 0; i < len; i++)
        checksum ^= data[i];

    return checksum;
}

/*==============================================================================
 * UART1_Comm_Init
 *   初始化 UART1 (PB0=TX, PB1=RX)，使能接收中断。
 *   baudrate: 波特率 (如 115200)
 *   dev_addr: 本机设备地址
 *============================================================================*/
void UART1_Comm_Init(uint32_t baudrate, uint8_t dev_addr)
{
    UART_InitType UART_InitStruct;
    GPIO_InitType GPIO_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    s_dev_addr = dev_addr;

    /* 使能 GPIOB 和 UART1 时钟 */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART1);

    GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* TX: PB0, AF push-pull */
    GPIO_InitStructure.Pin            = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_UART1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);

    /* RX: PB1, input */
    GPIO_InitStructure.Pin            = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_UART1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);

    /* UART1 配置: 8-N-1 */
    UART_Structure_Initializes(&UART_InitStruct);
    UART_InitStruct.BaudRate   = baudrate;
    UART_InitStruct.WordLength = UART_WL_8B;
    UART_InitStruct.StopBits   = UART_STPB_1;
    UART_InitStruct.Parity     = UART_PE_NO;
    UART_InitStruct.Mode       = UART_MODE_TX | UART_MODE_RX;
    UART_Initializes(UART1, &UART_InitStruct);

    /* 使能 UART1 接收中断 (RXDNE) */
    UART_Interrput_Enable(UART1, UART_INT_RXDNE);

    /* NVIC 配置: 使能 UART1 中断通道 */
    NVIC_InitStructure.NVIC_IRQChannel         = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_0;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    UART_Enable(UART1);
}

/*==============================================================================
 * UART1_SendBytes
 *   发送原始字节流（阻塞轮询方式）。
 *============================================================================*/
void UART1_SendBytes(uint8_t *pData, uint16_t len)
{
    while (len--)
    {
        while (UART_Flag_Status_Get(UART1, UART_FLAG_TXDE) == RESET);
        UART_Data_Send(UART1, *pData++);
    }
    while (UART_Flag_Status_Get(UART1, UART_FLAG_TXC) == RESET);
}

/*==============================================================================
 * UART1_SendResponse
 *   发送响应帧: 0xAA 0x55 + DevAddr + Cmd + RegAddr + Len + Data + XOR校验
 *   设备地址使用初始化时设定的本机地址。
 *============================================================================*/
void UART1_SendResponse(uint8_t cmd, uint8_t reg_addr, uint8_t *pData, uint8_t len)
{
    uint8_t idx = 0;

    if (len > (UART_TX_BUF_SIZE - 7U))
        return;

    s_tx_buf[idx++] = UART_FRAME_START1;    /* 起始 0xAA */
    s_tx_buf[idx++] = UART_FRAME_START2;    /* 起始 0x55 */
    s_tx_buf[idx++] = s_dev_addr;           /* 设备地址 */
    s_tx_buf[idx++] = cmd;                  /* 命令 */
    s_tx_buf[idx++] = reg_addr;             /* 寄存器地址 */
    s_tx_buf[idx++] = len;                  /* 数据长度 */

    if (pData != 0 && len > 0)
    {
        memcpy(&s_tx_buf[idx], pData, len);
        idx += len;
    }

    s_tx_buf[idx] = UART1_CalcChecksum(&s_tx_buf[2], (uint8_t)(idx - 2U));
    idx++;

    UART1_SendBytes(s_tx_buf, idx);
}

/*==============================================================================
 * UART1_SendSpeed
 *   发送速度数据到寄存器 0x0A (写命令 0x02, 设备地址 0x01)。
 *   数据 = speed * 1300, uint16_t 大端格式, 2 字节。
 *============================================================================*/
void UART1_SendSpeed(float speed)
{
    uint16_t val = speed * 1300;
    uint8_t data[2];
    uint8_t idx = 0;

    data[0] = (uint8_t)(val >> 8);
    data[1] = (uint8_t)(val & 0xFF);
    s_tx_buf[idx++] = UART_FRAME_START1;
    s_tx_buf[idx++] = UART_FRAME_START2;
    s_tx_buf[idx++] = 0x01;
    s_tx_buf[idx++] = UART_CMD_WRITE;
    s_tx_buf[idx++] = UART_REG_SPEED;
    s_tx_buf[idx++] = 2;
    s_tx_buf[idx++] = data[0];
    s_tx_buf[idx++] = data[1];
    s_tx_buf[idx] = UART1_CalcChecksum(&s_tx_buf[2], (uint8_t)(idx - 2U));
    idx++;

    UART1_SendBytes(s_tx_buf, idx);
}

/*==============================================================================
 * UART1_SendVibration
 *============================================================================*/
void UART1_SendVibration(uint8_t level)
{
    uint8_t idx = 0;

    s_tx_buf[idx++] = UART_FRAME_START1;
    s_tx_buf[idx++] = UART_FRAME_START2;
    s_tx_buf[idx++] = 0x01;
    s_tx_buf[idx++] = UART_CMD_WRITE;
    s_tx_buf[idx++] = UART_REG_VIBRATION;
    s_tx_buf[idx++] = 1;
    s_tx_buf[idx++] = level;
    s_tx_buf[idx] = UART1_CalcChecksum(&s_tx_buf[2], (uint8_t)(idx - 2U));
    idx++;

    UART1_SendBytes(s_tx_buf, idx);
}

/*==============================================================================
 * UART1_SendEmergencyStop
 *   发送紧急停机指令 0xAA55 到寄存器 0x10
 *============================================================================*/
void UART1_SendEmergencyStop(void)
{
    uint8_t idx = 0;

    s_tx_buf[idx++] = UART_FRAME_START1;
    s_tx_buf[idx++] = UART_FRAME_START2;
    s_tx_buf[idx++] = 0x01;
    s_tx_buf[idx++] = UART_CMD_WRITE;
    s_tx_buf[idx++] = UART_REG_EMERGENCY;
    s_tx_buf[idx++] = 2;
    s_tx_buf[idx++] = 0xAA;
    s_tx_buf[idx++] = 0x55;
    s_tx_buf[idx] = UART1_CalcChecksum(&s_tx_buf[2], (uint8_t)(idx - 2U));
    idx++;

    UART1_SendBytes(s_tx_buf, idx);
}

/*==============================================================================
 * UART1_SendEmergencyRelease
 *   发送紧急停机解除指令 0x5555 到寄存器 0x10
 *============================================================================*/
void UART1_SendEmergencyRelease(void)
{
    uint8_t idx = 0;

    s_tx_buf[idx++] = UART_FRAME_START1;
    s_tx_buf[idx++] = UART_FRAME_START2;
    s_tx_buf[idx++] = 0x01;
    s_tx_buf[idx++] = UART_CMD_WRITE;
    s_tx_buf[idx++] = UART_REG_EMERGENCY;
    s_tx_buf[idx++] = 2;
    s_tx_buf[idx++] = 0x55;
    s_tx_buf[idx++] = 0x55;
    s_tx_buf[idx] = UART1_CalcChecksum(&s_tx_buf[2], (uint8_t)(idx - 2U));
    idx++;

    UART1_SendBytes(s_tx_buf, idx);
}

/*==============================================================================
 * UART1_IsFrameReady
 *   检查是否收到完整帧。返回非零表示有新帧待读取。
 *============================================================================*/
uint8_t UART1_IsFrameReady(void)
{
    return s_frame_ready;
}

/*==============================================================================
 * UART1_GetFrame
 *   读取接收到的帧数据。调用后应调用 UART1_ClearFrame 清除标志。
 *============================================================================*/
void UART1_GetFrame(UART_RxFrame_t *pFrame)
{
    if (pFrame != 0)
    {
        *pFrame = s_rx_frame;
    }
}

/*==============================================================================
 * UART1_GetSteps
 *   从寄存器 0x02/0x03 解码步数 (16位大端格式，0x02高字节)。
 *   返回 0~65535 的步数值。
 *============================================================================*/
uint16_t UART1_GetSteps(void)
{
    return ((uint16_t)g_uart_regs[0x02] << 8) | g_uart_regs[0x03];
}

/*==============================================================================
 * UART1_GetTemp
 *   从寄存器 0x01 读取电机温度 (uint8)。
 *============================================================================*/
uint8_t UART1_GetTemp(void)
{
    return g_uart_regs[0x01];
}

/* IPM 温度位于下控状态帧寄存器 0x04，区别于 0x01 的电机温度。 */
uint8_t UART1_GetIPMTemp(void)
{
    return g_uart_regs[0x04];
}

/*==============================================================================
 * UART1_GetSystemError
 *   从寄存器 0x05 读取下位机系统错误代码。
 *   0x05 = 错误E5: 电机堵转保护
 *   0x06 = 错误E6: 电机编码器校准失败
 *   0x07 = 错误E7: IPM 过温保护
 *   返回非零错误代码，调用方应按紧急停机处理。
 *============================================================================*/
uint8_t UART1_GetSystemError(void)
{
    /* 错误由下控锁存；上控不得读后清零，否则无法可靠确认人工恢复结果。 */
    return g_uart_regs[0x05];
}

/*==============================================================================
 * UART1_CheckAndClearAck
 *   检查寄存器 0x00 是否有 ACK 应答 (0x55)，有则清除并返回 1。
 *============================================================================*/
uint8_t UART1_CheckAndClearAck(void)
{
    if (g_uart_regs[0x00] == 0x55)
    {
        g_uart_regs[0x00] = 0;
        return 1;
    }
    return 0;
}

/*==============================================================================
 * UART1_ClearFrame
 *   清除帧就绪标志，准备接收下一帧。
 *============================================================================*/
void UART1_ClearFrame(void)
{
    s_frame_ready = 0;
}

/*==============================================================================
 * UART1_IRQHandler
 *   UART1 接收中断服务例程，以状态机解析帧格式。
 *============================================================================*/
void UART1_IRQHandler(void)
{
    if (UART_Interrupt_Status_Get(UART1, UART_INT_RXDNE) != RESET)
    {
        uint8_t byte = (uint8_t)UART_Data_Receive(UART1);

        switch (s_rx_state)
        {
        case FRAME_STATE_START1:
            if (byte == UART_FRAME_START1)
                s_rx_state = FRAME_STATE_START2;
            break;

        case FRAME_STATE_START2:
            if (byte == UART_FRAME_START2)
                s_rx_state = FRAME_STATE_DEV_ADDR;
            else
                s_rx_state = FRAME_STATE_START1;
            break;

        case FRAME_STATE_DEV_ADDR:
            s_rx_frame.dev_addr = byte;
            s_rx_state = FRAME_STATE_CMD;
            break;

        case FRAME_STATE_CMD:
            s_rx_frame.cmd = byte;
            s_rx_state = FRAME_STATE_REG_ADDR;
            break;

        case FRAME_STATE_REG_ADDR:
            s_rx_frame.reg_addr = byte;
            s_rx_state = FRAME_STATE_LENGTH;
            break;

        case FRAME_STATE_LENGTH:
            s_rx_frame.length = byte;
            if (byte > UART_RX_BUF_SIZE)
            {
                s_rx_data_idx = 0;
                s_rx_state = FRAME_STATE_START1;
            }
            else if (byte > 0)
            {
                s_rx_data_idx = 0;
                s_rx_state = FRAME_STATE_DATA;
            }
            else
            {
                s_rx_state = FRAME_STATE_RESERVED;
            }
            break;

        case FRAME_STATE_DATA:
            s_rx_frame.data[s_rx_data_idx++] = byte;
            if (s_rx_data_idx >= s_rx_frame.length)
                s_rx_state = FRAME_STATE_RESERVED;
            break;

        case FRAME_STATE_RESERVED:
            s_rx_frame.reserved = byte;
            {
                uint8_t checksum = s_rx_frame.dev_addr ^ s_rx_frame.cmd ^
                                   s_rx_frame.reg_addr ^ s_rx_frame.length;
                uint8_t checksum_idx;

                for (checksum_idx = 0; checksum_idx < s_rx_frame.length; checksum_idx++)
                    checksum ^= s_rx_frame.data[checksum_idx];

            if (byte == checksum &&
                (s_rx_frame.dev_addr == s_dev_addr || s_rx_frame.dev_addr == 0x00))
            {
                /* 以寄存器地址为起始写入数据到寄存器数组 */
                uint8_t i;
                for (i = 0; i < s_rx_frame.length; i++)
                {
                    uint8_t reg_idx = s_rx_frame.reg_addr + i;
                    if (reg_idx < UART_REGS_SIZE)
                        g_uart_regs[reg_idx] = s_rx_frame.data[i];
                }
                g_uart_regs[0x00] = 0x55;  /* ACK 应答标志 */
                s_frame_ready = 1;
            }
            }
            s_rx_state = FRAME_STATE_START1;
            break;

        default:
            s_rx_state = FRAME_STATE_START1;
            break;
        }
    }

    /* 清除错误标志 */
    if ((UART_Flag_Status_Get(UART1, UART_FLAG_OREF) != RESET) ||
        (UART_Flag_Status_Get(UART1, UART_FLAG_NEF) != RESET) ||
        (UART_Flag_Status_Get(UART1, UART_FLAG_FEF) != RESET) ||
        (UART_Flag_Status_Get(UART1, UART_FLAG_PEF) != RESET))
    {
        (void)UART1->STS;
        (void)UART1->DAT;
    }
}
