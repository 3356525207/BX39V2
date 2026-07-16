#ifndef __UART_CAN_H
#define __UART_CAN_H

#include "wk_system.h"
#include "wk_usart.h"

extern volatile uint8_t uart_rx_buf[32];
extern  uint8_t reg_RW;
extern volatile uint8_t g_device_regs[64];
extern  uint8_t data_lens;
extern  uint8_t data_buf[64];
extern volatile uint8_t uart_rx_index;
// 协议相关宏定义
#define FRAME_HEAD      0xAA
#define CMD_READ        0x01
#define CMD_WRITE       0x02

// 地址分配相关宏
#define BROADCAST_ADDR 0x00
#define DEVICE_ADDR_MIN 0x01
#define DEVICE_ADDR_MAX 0xFE
#define ADDR_QUERY_CMD  0xF0

// 串口初始化
void UART_Init(void);

// 寄存器初始化
void init_device_registers(void);

// 发送一个字节
void UART_SendByte(uint8_t byte);

// 接收一个字节
uint8_t UART_ReceiveByte(void);

// 校验计算
uint8_t CalcChecksum(const volatile uint8_t *data, uint8_t len);

// 主机读从机寄存器
uint8_t Master_ReadRegister(uint8_t slave_addr, uint8_t reg_addr);

// 主机写从机寄存器
void Master_WriteRegister(uint8_t slave_addr, uint8_t reg_addr, uint8_t value);

// 自动获取本机地址
uint8_t AutoGetDeviceAddress(void);

// 设置/获取本机设备地址
void SetLocalDeviceAddress(uint8_t addr);
uint8_t GetLocalDeviceAddress(void);

// 快速检查帧（可在主循环中调用）
 uint8_t QuickCheckFrame(void);

void ClearRxBuffer(void);

/* Called from the 5 kHz TMR4 control tick. A valid upper-controller write
 * refreshes the watchdog; silence clears motion commands without changing the
 * normal acceleration/deceleration state machine. */
void UART_CommandWatchdog_Tick(void);

// 帧解析函数
int ParseUartFrame(uint8_t *dev_addr, uint8_t *cmd, uint8_t *reg_addr, 
                   uint8_t *data_len, uint8_t *data);

void SendResponse(unsigned char cmd, unsigned char reg, unsigned char len, const unsigned char *data);

#endif // __UART_CAN_H
