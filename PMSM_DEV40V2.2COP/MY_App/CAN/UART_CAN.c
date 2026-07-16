#include "wk_system.h"
#include "wk_usart.h"
#include "UART_CAN.h"
#include "at32m412_416_int.h"
#include "vofa.h"
#define UARTx_INSTANCE USART3
#define RX_BUF_SIZE  32
#define BROADCAST_ADDR 0x00
#define DEVICE_ADDR_MIN 0x01
#define DEVICE_ADDR_MAX 0xFE
#define ADDR_QUERY_CMD 0xF0
#define ADDR_QUERY_TIMEOUT 50 // ms

#define COMMAND_WATCHDOG_HZ          5000UL
#define COMMAND_TIMEOUT_MS           2000UL
#define COMMAND_TIMEOUT_TICKS        ((COMMAND_WATCHDOG_HZ * COMMAND_TIMEOUT_MS) / 1000UL)

#define FRAME_HEAD1 0xAA
#define FRAME_HEAD2 0x55
#define REG_COUNT 64  // 设备寄存器数量
#define REG_WEIGHT_START 0x02 // 可写寄存器起始地址
#define REG_WEIGHT_END   0x3F // 可写寄存器结束地址
#define REG_WRITE 0x02//写命令
#define REG_READ 0x01//读命令
#define REG_return 0x03//返回命令

 uint8_t reg_RW = 0; // 寄存器命令
 uint8_t data_lens;// 数据长度
 uint8_t data_buf[64]; // 数据缓冲区

// 添加本机地址变量
volatile uint8_t g_local_device_addr = 0x01;

// 设备寄存器数组：0x00=addr, 0x01=device_type, 0x02-0x3F=data
volatile uint8_t g_device_regs[REG_COUNT];

static volatile uint8_t s_valid_command_seen = 0;
static uint32_t s_command_silence_ticks = 0;
static uint8_t s_command_timed_out = 0;



//UART_HandleTypeDef huart1;
volatile uint8_t uart_rx_buf[RX_BUF_SIZE];
volatile uint8_t uart_rx_index = 0;
volatile uint8_t uart_rx_flag = 0;



//寄存器初始化
void init_device_registers(void) {
    g_device_regs[0x00] = g_local_device_addr; // 设备地址
    g_device_regs[0x01] = 0x10; // 设备类型示例
    // 其他寄存器初始化为0
    for (int i = 2; i < REG_COUNT; i++) {
        g_device_regs[i] = 0;
    }
}

void UART_CommandWatchdog_Tick(void)
{
    if (s_valid_command_seen)
    {
        s_valid_command_seen = 0;
        s_command_silence_ticks = 0;
        s_command_timed_out = 0;
        return;
    }

    if (s_command_silence_ticks < COMMAND_TIMEOUT_TICKS)
        s_command_silence_ticks++;

    if (!s_command_timed_out && s_command_silence_ticks >= COMMAND_TIMEOUT_TICKS)
    {
        /* Communication silence is a local fail-safe only. Treadmill mode
         * follows the existing target-to-zero ramp; vibration exits when
         * g_Master consumes the cleared command. */
        g_device_regs[10] = 0;
        g_device_regs[11] = 0;
        g_device_regs[14] = 0;
        g_device_regs[15] = 0;
        s_command_timed_out = 1;
    }
}


// 发送一个字节（HAL库方式）
void UART_SendByte(uint8_t byte) {
//    HAL_UART_Transmit(&huart3, &byte, 1, HAL_MAX_DELAY);//WDP
}


void USART1_IRQHandler(void)
{
  /* add user code begin USART1_IRQ 0 */

  /* add user code end USART1_IRQ 0 */

  uint8_t rx_byte;

  if(usart_interrupt_flag_get(USART1, USART_RDBF_FLAG) != RESET)
  {
    /* add user code begin USART1_USART_RDBF_FLAG */
    /* handle data received and clear flag */
		

	
		
		
		

        uart_rx_flag = 1;
  

        if (uart_rx_index >= RX_BUF_SIZE) uart_rx_index = 0;
//        HAL_UART_Receive_IT(&huart3, &uart_rx_buf[uart_rx_index], 1);
			

		rx_byte = (uint8_t)usart_data_receive(USART1);
		uart_rx_buf[uart_rx_index] = rx_byte;
		uart_rx_index++;
		
    /* add user code end USART1_USART_RDBF_FLAG */ 
  }

  /* add user code begin USART1_IRQ 1 */

  /* add user code end USART1_IRQ 1 */
}






// 中断接收回调
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	

//    if (huart->Instance == UARTx_INSTANCE) {
//        uart_rx_flag = 1;
//        uart_rx_index++;

//        if (uart_rx_index >= RX_BUF_SIZE) uart_rx_index = 0;
//        HAL_UART_Receive_IT(&huart3, &uart_rx_buf[uart_rx_index], 1);
//			

//    }
//}





// 读取接收缓冲区的一个字节（阻塞直到有数据）
uint8_t UART_ReceiveByte(void) {
    static uint8_t last_rx_index = 0;
    while (!uart_rx_flag);
    uart_rx_flag = 0;
    uint8_t data = uart_rx_buf[last_rx_index];
    last_rx_index++;
    if (last_rx_index >= RX_BUF_SIZE) last_rx_index = 0;
    return data;
}

// 计算校验（简单异或校验）
uint8_t CalcChecksum(const volatile uint8_t *data, uint8_t len) {
    uint8_t sum = 0;  // 初始值为0
    for (uint8_t i = 0; i < len; i++) {
        sum ^= data[i];  // 逐字节异或
    }
    return sum;  // 返回校验值
}

// 帧格式示例：
// 0xAA 0x55 | 0x01 | 0x03 | 0x10 | 0x02 | 0xAA 0xBB | 校验
//   帧头    | 地址 | 命令 | 寄存器| 长度|  数据   | XOR
//
// 校验计算范围：从地址到数据结束
// sum = 0x01 ^ 0x03 ^ 0x10 ^ 0x02 ^ 0xAA ^ 0xBB

// 主机读从机寄存器
uint8_t Master_ReadRegister(uint8_t slave_addr, uint8_t reg_addr) {
    uint8_t frame[5];
    frame[0] = FRAME_HEAD;
    frame[1] = slave_addr;
    frame[2] = CMD_READ;
    frame[3] = reg_addr;
    frame[4] = CalcChecksum(&frame[1], 3);

    for (int i = 0; i < 5; i++) {
        UART_SendByte(frame[i]);
    }

    uint8_t rx[5];
    for (int i = 0; i < 5; i++) {
        rx[i] = UART_ReceiveByte();
    }
    if (rx[0] != FRAME_HEAD) return 0xFF;
    if (rx[1] != 0x00) return 0xFF;
    if (rx[2] != reg_addr) return 0xFF;
    if (rx[4] != CalcChecksum(&rx[1], 3)) return 0xFF;

    return rx[3];
}

// 主机写从机寄存器
void Master_WriteRegister(uint8_t slave_addr, uint8_t reg_addr, uint8_t value) {
    uint8_t frame[6];
    frame[0] = FRAME_HEAD;
    frame[1] = slave_addr;
    frame[2] = CMD_WRITE;
    frame[3] = reg_addr;
    frame[4] = value;
    frame[5] = CalcChecksum(&frame[1], 4);

    for (int i = 0; i < 6; i++) {
        UART_SendByte(frame[i]);
    }
    // 可选：等待从机返回确认帧
}

// 查询地址是否被占用，返回1=被占用，0=空闲
static uint8_t QueryAddressOccupied(uint8_t addr) {
    uint8_t frame[5];
    frame[0] = FRAME_HEAD;
    frame[1] = addr;
    frame[2] = ADDR_QUERY_CMD;
    frame[3] = CalcChecksum(&frame[1], 3);
  
    for (int i = 0; i < 5; i++) {
        UART_SendByte(frame[i]);
    }

    uint32_t tickstart =0;//WDP
    while ((0- tickstart) > ADDR_QUERY_TIMEOUT) {
        // 解析应答帧：0xAA | 设备地址 | 0xF0 | 查询地址 | 校验
        uint8_t resp[5];
        int valid = 1;
        for (int i = 0; i < 5; i++) {
            resp[i] = UART_ReceiveByte();
        }
        // 校验帧格式
        if (resp[0] != FRAME_HEAD) valid = 0;
        if (resp[2] != ADDR_QUERY_CMD) valid = 0;
        if (resp[3] != addr) valid = 0;
        if (resp[4] != CalcChecksum(&resp[1], 3)) valid = 0;
        if (valid) {
            return 1; // 有设备应答，地址被占用
        }
        // 若无效，继续等待直到超时
    }
    return 0; // 超时无应答，地址空闲
}

// 自动获取本机地址
uint8_t AutoGetDeviceAddress(void) {
    for (uint8_t addr = DEVICE_ADDR_MIN; addr <= DEVICE_ADDR_MAX; addr++) {
        if (!QueryAddressOccupied(addr)) {
            return addr;
        }
    }
    return 0xFF;
}

// 设置本机设备地址
void SetLocalDeviceAddress(uint8_t addr) {
    g_local_device_addr = addr;
}

// 获取本机设备地址
uint8_t GetLocalDeviceAddress(void) {
    return g_local_device_addr;
}

// 清空接收缓冲区
 void ClearRxBuffer(void) {
//    HAL_UART_AbortReceive_IT(&huart3); // 先停止当前接收，避免继续写入旧索引WDP
    uart_rx_index = 0;
    uart_rx_flag = 0;
    for (uint8_t i = 0; i < RX_BUF_SIZE; i++) {
        uart_rx_buf[i] = 0;
    }
//    HAL_UART_Receive_IT(&huart3, &uart_rx_buf[uart_rx_index], 1); // 从0重新开始接收WDP
}

// 快速检查帧头和地址（在中断中调用，减少无效帧处理）
uint8_t QuickCheckFrame(void) {
    if (uart_rx_index < 3) return 0;
    
    // 查找帧头
    if (uart_rx_buf[0] == FRAME_HEAD1 && uart_rx_buf[1] == FRAME_HEAD2) {
        uint8_t addr = uart_rx_buf[2];
        // 只处理广播地址或本机地址
        if (addr == BROADCAST_ADDR || addr == g_local_device_addr) {
            return 1; // 是目标帧
        } else {
            ClearRxBuffer(); // 非本机地址，立即清空
            return 0;
        }
    }
    
    // 帧头不匹配，查找下一个可能的帧头
    if (uart_rx_index >= 2 && uart_rx_buf[uart_rx_index-1] == FRAME_HEAD1) {
        return 0; // 可能是新帧开始
    }
    
    // 无有效帧头，清空缓冲区
    if (uart_rx_index > 2) {
        ClearRxBuffer();
    }
    return 0;
}

// 寄存器写入：返回1成功，0失败
static uint8_t WriteDeviceRegisters(uint8_t reg_addr, uint8_t len, const volatile uint8_t *data) {
    if (len == 0 || data == NULL) return 0;
    if (reg_addr < REG_WEIGHT_START || reg_addr > REG_WEIGHT_END) return 0;
    if ((uint16_t)reg_addr + len > REG_COUNT) return 0;

    for (uint8_t i = 0; i < len; i++) {
        g_device_regs[reg_addr + i] = data[i];
    }
    return 1;
}

// 寄存器读取：返回1成功，0失败
static uint8_t ReadDeviceRegisters(uint8_t reg_addr, uint8_t len, uint8_t *out) {
    if (len == 0 || out == NULL) return 0;
    if ((uint16_t)reg_addr + len > REG_COUNT) return 0;

    for (uint8_t i = 0; i < len; i++) {
        out[i] = g_device_regs[reg_addr + i];
    }
    return 1;
}

// 帧解析函数 - 直接解析uart_rx_buf
// 返回值：0=无有效帧，1=解析成功
int ParseUartFrame(uint8_t *dev_addr, uint8_t *cmd, uint8_t *reg_addr, 
                   uint8_t *data_len, uint8_t *data)
{
	        data_lens=uart_rx_index; //更新指针位置
	
    uint8_t buf_len = uart_rx_index;
    
    // 最小帧长检查
    if (buf_len < 7) {
        return 2; // 数据不足，不清空，等待更多数据
    }
    
    // 验证帧头
    if (uart_rx_buf[0] != FRAME_HEAD1 || uart_rx_buf[1] != FRAME_HEAD2) {
        ClearRxBuffer(); // 帧头错误，清空
        return 3;
    }
    
    // 快速地址过滤
    uint8_t addr = uart_rx_buf[2];
    if (addr != BROADCAST_ADDR && addr != g_local_device_addr) {
        ClearRxBuffer(); // 非本机地址，清空
        return 4;
    }
    
    uint8_t command = uart_rx_buf[3];
    uint8_t reg = uart_rx_buf[4];
    uint8_t len = uart_rx_buf[5];

    if (len > (RX_BUF_SIZE - 7U) || len > sizeof(data_buf)) {
        ClearRxBuffer();
        return 6;
    }
    
    // 检查完整帧长度
    uint8_t frame_len = 7 + len; // 帧头2 + 地址1 + 命令1 + 寄存器1 + 长度1 + 数据len + 校验1
    if (buf_len < frame_len) {
        return 5; // 数据未接收完整，等待
    }

    if (uart_rx_buf[6U + len] != CalcChecksum(&uart_rx_buf[2], (uint8_t)(4U + len))) {
        ClearRxBuffer();
        return 0;
    }
    
    // // 校验和验证
    // uint8_t checksum = uart_rx_buf[6+len];
    // uint8_t sum = 0;
    // for (uint8_t j = 0; j < (4 + len); j++) {
    //     sum ^= uart_rx_buf[2+j];
    // }
    
    // if (checksum != sum) {
    //     ClearRxBuffer(); // 校验失败，清空
    //     return 0;
    // }
    
    // 解析成功，提取数据
    if (dev_addr) *dev_addr = addr;
    if (cmd) *cmd = command;
    if (reg_addr) *reg_addr = reg;
    if (data_len) *data_len = len;
    if (data && len > 0) {
        for (uint8_t k = 0; k < len; k++) {
            data[k] = uart_rx_buf[6+k];
        }
    } 

    // 根据命令写入寄存器
    if (command == REG_WRITE) {
        if (WriteDeviceRegisters(reg, len, &uart_rx_buf[6])) {
            s_valid_command_seen = 1;
        }
    }

    ClearRxBuffer(); // 解析成功，清空缓冲区
    return 1;
}


/**
 * @function    SendResponse
 * @brief       发送响应帧，格式: 0xAA 0x55 + DevAddr + Cmd + RegAddr + Len + Data + XOR校验
 * @param[in]   cmd   命令类型
 * @param[in]   reg   寄存器地址
 * @param[in]   len   数据长度
 * @param[in]   data  数据缓冲区指针
 * @param[out]  无
 * @return      无
 */
void SendResponse(unsigned char cmd, unsigned char reg, unsigned char len, const unsigned char *data)
{
    unsigned char frame_len;
    unsigned char frame[RX_BUF_SIZE];

    if (len > (RX_BUF_SIZE - 7U))
        return;

    frame_len = (unsigned char)(7U + len);

    frame[0] = FRAME_HEAD1;                     // 0xAA
    frame[1] = FRAME_HEAD2;                     // 0x55
    frame[2] = g_local_device_addr;             // DevAddr
    frame[3] = cmd;                             // Cmd
    frame[4] = reg;                             // RegAddr
    frame[5] = len;                             // Len
    for (unsigned char i = 0; i < len; i++) {
        frame[6 + i] = data[i];                 // Data
    }
    frame[6 + len] = CalcChecksum(&frame[2], (uint8_t)(4U + len));

    send_array(frame, frame_len);
}  




