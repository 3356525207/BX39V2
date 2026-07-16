#ifndef __UART_COMM_H__
#define __UART_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/*==============================================================================
 * 帧格式: 起始(0xAA 0x55) + 设备地址 + 命令 + 寄存器地址 + 长度 + 数据 + 备用
 *============================================================================*/

#define UART_FRAME_START1   0xAA
#define UART_FRAME_START2   0x55

#define UART_RX_BUF_SIZE    64
#define UART_TX_BUF_SIZE    64
#define UART_REGS_SIZE      16

/* 帧接收状态机 */
typedef enum {
    FRAME_STATE_START1 = 0,
    FRAME_STATE_START2,
    FRAME_STATE_DEV_ADDR,
    FRAME_STATE_CMD,
    FRAME_STATE_REG_ADDR,
    FRAME_STATE_LENGTH,
    FRAME_STATE_DATA,
    FRAME_STATE_RESERVED
} FrameRxState_t;

/* 接收帧结构 */
typedef struct {
    uint8_t dev_addr;
    uint8_t cmd;
    uint8_t reg_addr;
    uint8_t length;
    uint8_t data[UART_RX_BUF_SIZE];
    uint8_t reserved;
} UART_RxFrame_t;

/*==============================================================================
 * 寄存器数组 (16字节, 以寄存器地址为索引)
 *============================================================================*/
extern volatile uint8_t g_uart_regs[UART_REGS_SIZE];

/*==============================================================================
 * API 函数
 *============================================================================*/

#define UART_CMD_WRITE          0x02
#define UART_REG_SPEED          0x0A
#define UART_REG_SYS_ERROR      0x05
#define UART_REG_VIBRATION      0x0E
#define UART_REG_EMERGENCY      0x10

/* 下位机系统错误代码 */
#define UART_ERR_E5_MOTOR_STALL     0x05    /* 错误代码E5: 电机堵转保护 */
#define UART_ERR_E6_ENC_CAL_FAIL    0x06    /* 错误代码E6: 电机编码器校准失败 */
#define UART_ERR_E7_IPM_OVER_TEMP   0x07    /* 错误代码E7: IPM 过温保护 */

#define UART_IPM_DERATE_LEVEL1_C    98u
#define UART_IPM_DERATE_LEVEL2_C    100u
#define UART_IPM_DERATE_LEVEL3_C    102u
#define UART_IPM_RECOVER_C          100u
#define UART_IPM_ERROR_TRIP_C       105u
#define UART_MOTOR_RECOVER_C        125u

void UART1_Comm_Init(uint32_t baudrate, uint8_t dev_addr);
void UART1_SendResponse(uint8_t cmd, uint8_t reg_addr, uint8_t *pData, uint8_t len);
void UART1_SendBytes(uint8_t *pData, uint16_t len);
void UART1_SendSpeed(float speed);
void UART1_SendVibration(uint8_t level);
void UART1_SendEmergencyStop(void);
void UART1_SendEmergencyRelease(void);
uint8_t UART1_IsFrameReady(void);
void UART1_GetFrame(UART_RxFrame_t *pFrame);
void UART1_ClearFrame(void);
uint16_t UART1_GetSteps(void);
uint8_t  UART1_GetTemp(void);
uint8_t  UART1_GetIPMTemp(void);
uint8_t  UART1_CheckAndClearAck(void);
uint8_t  UART1_GetSystemError(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_COMM_H__ */
