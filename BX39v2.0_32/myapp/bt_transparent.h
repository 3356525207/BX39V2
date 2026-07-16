#ifndef __BT_TRANSPARENT_H__
#define __BT_TRANSPARENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* N32G003F5Q7 使用 UART2 作为蓝牙透传模块的通信接口, RX=PA1, TX=PA2 */

/* 协议寄存器 (16 字节) */
#define BT_REGS_SIZE 16
extern uint8_t g_bt_regs[BT_REGS_SIZE];

/* 配对确认提交标志：BT_ProcessFrame 命中 reg=0x03/data=0x01 时置 1，
 * 主循环检测后调用 Pairing_MarkDone() 并清零（方案 A）。 */
extern volatile uint8_t g_pair_commit_req;

/*==============================================================================
 * FW-1  非阻塞 ack 路径 (SAFE-4)
 *   BT_ProcessFrame (UART2 RX 中断上下文) 只置 pending 请求，绝不发送；
 *   主循环 (src/main.c) 排空 g_pending_ack 并构建/发送 JSON。
 *============================================================================*/
typedef enum { ACK_STATUS_OK = 0, ACK_STATUS_WARNING = 1 } BT_AckStatus_t;

typedef enum {
    ACKOF_NONE = 0, ACKOF_START, ACKOF_STOP, ACKOF_PAUSE, ACKOF_RESUME,
    ACKOF_SETSPEED, ACKOF_MODE1, ACKOF_MODE2, ACKOF_MODE3, ACKOF_MODE4,
    ACKOF_CONFIRM_PAIRING
} BT_AckOf_t;

typedef enum {
    ACKERR_NONE = 0, ACKERR_COMMAND_REJECTED, ACKERR_DEVICE_BUSY
} BT_AckErr_t;

typedef struct {
    volatile uint8_t   pending;   /* 1 = 有一条 ack 待发送 */
    BT_AckOf_t         ackOf;
    BT_AckStatus_t     status;
    BT_AckErr_t        err;       /* 仅 status==WARNING 时有意义 */
} BT_PendingAck_t;

extern volatile BT_PendingAck_t g_pending_ack;

/*==============================================================================
 * FW-8  设备自发事件标志 (检测置位 / 主循环发送)
 *   置位点：SYS_RUN_Process(过热) / CTRL_TX_Process(安全扣) —— 均非发送上下文。
 *============================================================================*/
extern volatile uint8_t g_evt_overheat_downspeed;  /* warning  OVERHEAT_DOWNSPEED */
extern volatile uint8_t g_evt_overheat_stop;       /* device_error OVERHEAT_STOP  */
extern volatile uint8_t g_evt_emergency_stop;      /* safety key: EMERGENCY_STOP   */
extern volatile uint8_t g_evt_system_fault;        /* lower-controller fault event */

void BT_UART_Init(void);
void BT_Init(void);
void BT_Regs_Init(void);
void BT_ParseByte(uint8_t byte);
void UART2_SendData(uint8_t *pData, uint16_t len);
void UART2_SendString(char *str);
void BT_SendTelemetry(void);

/* FW-1/FW-7/FW-8 上行构建器 (只能在主循环上下文调用，内部会阻塞式 TX) */
uint32_t BT_NextSeq(void);
void BT_SendAck(BT_AckOf_t ackOf, BT_AckStatus_t status, BT_AckErr_t err);
void BT_SendHeartbeat(void);
void BT_SendDeviceError(const char *code, const char *message, const char *tempState);
void BT_SendSafetyKeyError(void);
void BT_SendOverheatDownspeed(void);



#define UARTy            UART1
#define UARTy_CLK        RCC_APB_PERIPH_UART1
#define UARTy_GPIO       GPIOA
#define UARTy_GPIO_CLK   RCC_APB_PERIPH_IOPA
#define UARTy_RxPin      GPIO_PIN_12
#define UARTy_TxPin      GPIO_PIN_14
#define UARTy_Rx_GPIO_AF GPIO_AF2_UART1
#define UARTy_Tx_GPIO_AF GPIO_AF2_UART1
#define UARTy_APBxClkCmd RCC_APB_Peripheral_Clock_Enable
#define UARTy_IRQn       UART1_IRQn
#define UARTy_IRQHandler UART1_IRQHandler

#define UARTz            UART2
#define UARTz_GPIO       GPIOA
#define UARTz_CLK        RCC_APB_PERIPH_UART2
#define UARTz_GPIO_CLK   RCC_APB_PERIPH_IOPA
#define UARTz_RxPin      GPIO_PIN_1
#define UARTz_TxPin      GPIO_PIN_2
#define UARTz_Rx_GPIO_AF GPIO_AF1_UART2
#define UARTz_Tx_GPIO_AF GPIO_AF1_UART2
#define UARTz_APBxClkCmd RCC_APB_Peripheral_Clock_Enable
#define UARTz_IRQn       UART2_IRQn
#define UARTz_IRQHandler UART2_IRQHandler


#ifdef __cplusplus
}
#endif

#endif /* __BT_TRANSPARENT_H__ */
