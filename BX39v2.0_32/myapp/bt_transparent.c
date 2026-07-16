#include "bt_transparent.h"
#include "bsp_delay.h"
#include "SYS_RUN.h"
#include "RCV315.h"
#include "uart_comm.h"
#include "pairing_store.h"
#include "sn_builder.h"
#include <string.h>
#include <stdio.h>

extern volatile uint32_t g_ms_tick;

/* 16 字节协议寄存器组 */
uint8_t g_bt_regs[BT_REGS_SIZE];

/* 配对确认提交标志：UART2 接收中断命中 reg=0x03/data=0x01 时置 1，
 * 主循环检测后调用 Pairing_MarkDone() 擦写 Flash 并清零（方案 A：
 * 中断仅置标志，不在中断上下文擦写）。 */
volatile uint8_t g_pair_commit_req = 0;

/*==============================================================================
 * FW-1  非阻塞 ack 待发请求 (中断置位, 主循环排空)
 * FW-8  设备主动事件标志 (检测置位, 主循环发送)
 *============================================================================*/
volatile BT_PendingAck_t g_pending_ack = { 0, ACKOF_NONE, ACK_STATUS_OK, ACKERR_NONE };

volatile uint8_t g_evt_overheat_downspeed = 0;
volatile uint8_t g_evt_overheat_stop      = 0;
volatile uint8_t g_evt_emergency_stop     = 0;
volatile uint8_t g_evt_system_fault       = 0;

/* FW-1：统一单调递增序号，遥测/心跳/ack/事件共用 (重启后从 0 起，见 §2)。
 * 仅在主循环上下文的上行构建器中调用 → 无并发。 */
static uint32_t s_bt_seq = 0;

uint32_t BT_NextSeq(void)
{
    return ++s_bt_seq;
}

/* 帧解析状态 */
typedef enum {
    BT_WAIT_HEAD1,   /* 等待 0xAA */
    BT_WAIT_HEAD2,   /* 等待 0x55 */
    BT_WAIT_CMD,     /* 等待命令 (0x02 写 / 0x03 读) */
    BT_WAIT_REG,     /* 等待寄存器起始地址 */
    BT_WAIT_LEN,     /* 等待数据长度 */
    BT_DATA,         /* 接收数据 */
    BT_WAIT_TAIL     /* 等待帧尾 0x55 */
} BT_RxState_t;

static BT_RxState_t bt_state = BT_WAIT_HEAD1;
static uint8_t bt_cmd;
static uint8_t bt_reg;
static uint8_t bt_len;
static uint8_t bt_idx;
static uint8_t bt_buf[BT_REGS_SIZE];

/* MX-01P AT response capture.
 * The module prints ASCII responses such as OK/ERROR/+READY/+UUIDS.
 * Keep a small rolling buffer so BT_Init can follow the vendor command set:
 * wait +READY, wait OK for every write command, then reboot and wait +READY.
 */
#define BT_AT_RX_BUF_SIZE 128
static volatile char s_bt_at_rx[BT_AT_RX_BUF_SIZE];
static volatile uint8_t s_bt_at_rx_len = 0;

void BT_Regs_Init(void)
{
    memset(g_bt_regs, 0, sizeof(g_bt_regs));
}

static void BT_AT_ClearRx(void)
{
    s_bt_at_rx_len = 0;
    s_bt_at_rx[0] = '\0';
}

static void BT_AT_CaptureByte(uint8_t byte)
{
    if (byte == '\r' || byte == '\n' || (byte >= 0x20 && byte <= 0x7E)) {
        if (s_bt_at_rx_len >= (BT_AT_RX_BUF_SIZE - 1)) {
            s_bt_at_rx_len = 0;
        }
        s_bt_at_rx[s_bt_at_rx_len++] = (char)byte;
        s_bt_at_rx[s_bt_at_rx_len] = '\0';
    }
}

static uint8_t BT_AT_Contains(const char *needle)
{
    return (strstr((const char *)s_bt_at_rx, needle) != NULL) ? 1 : 0;
}

static uint8_t BT_AT_WaitFor(const char *expect, uint32_t timeout_ms)
{
    uint32_t start = g_ms_tick;

    while ((uint32_t)(g_ms_tick - start) < timeout_ms) {
        if (BT_AT_Contains(expect)) return 1;
        if (BT_AT_Contains("ERROR")) return 0;
    }
    return 0;
}

static uint8_t BT_AT_SendAndWait(const char *cmd, const char *expect, uint32_t timeout_ms)
{
    BT_AT_ClearRx();
    UART2_SendString((char *)cmd);
    return BT_AT_WaitFor(expect, timeout_ms);
}

static uint8_t BT_AT_SendAndWaitOk(const char *cmd, uint32_t timeout_ms)
{
    uint8_t i;

    for (i = 0; i < 3; i++) {
        if (BT_AT_SendAndWait(cmd, "OK", timeout_ms)) return 1;
        SysTick_Delay_Ms(120);
    }
    return 0;
}

static uint8_t BT_AT_QueryContains(const char *cmd, const char *expect, uint32_t timeout_ms)
{
    BT_AT_ClearRx();
    UART2_SendString((char *)cmd);
    return BT_AT_WaitFor(expect, timeout_ms);
}

static void BT_SendFrame(uint8_t cmd, uint8_t reg, uint8_t len, uint8_t *data)
{
    uint8_t i;
    uint8_t ch;

    ch = 0xAA; UART2_SendData(&ch, 1);
    ch = 0x55; UART2_SendData(&ch, 1);
    UART2_SendData(&cmd, 1);
    UART2_SendData(&reg, 1);
    UART2_SendData(&len, 1);
    for (i = 0; i < len; i++) {
        UART2_SendData(&data[i], 1);
    }
    ch = 0x55; UART2_SendData(&ch, 1);
}

/*==============================================================================
 * FW-1  ack 请求 (中断上下文调用，绝不发送，只置标志)
 *   单指令在途 (SAFE-3)：若上一条 ack 尚未排空，最后一条有效帧覆盖之，安全。
 *   pending 最后置位，主循环永不读到半更新的结构体。
 *============================================================================*/
static void BT_RequestAck(BT_AckOf_t ackOf, BT_AckStatus_t status, BT_AckErr_t err)
{
    g_pending_ack.ackOf   = ackOf;
    g_pending_ack.status  = status;
    g_pending_ack.err     = err;
    g_pending_ack.pending = 1;   /* 最后置位 */
}

static BT_AckOf_t BT_CmdToAckOf(uint8_t cmd)
{
    switch (cmd) {
    case 0x00: return ACKOF_STOP;
    case 0x01: return ACKOF_START;
    case 0x02: return ACKOF_PAUSE;
    case 0x03: return ACKOF_RESUME;
    case 0x04: return ACKOF_MODE1;
    case 0x05: return ACKOF_MODE2;
    case 0x06: return ACKOF_MODE3;
    case 0x07: return ACKOF_MODE4;
    default:   return ACKOF_NONE;
    }
}

static const char *BT_AckOfStr(BT_AckOf_t a)
{
    switch (a) {
    case ACKOF_START:          return "start";
    case ACKOF_STOP:           return "stop";
    case ACKOF_PAUSE:          return "pause";
    case ACKOF_RESUME:         return "resume";
    case ACKOF_SETSPEED:       return "setSpeed";
    case ACKOF_MODE1:          return "mode1";
    case ACKOF_MODE2:          return "mode2";
    case ACKOF_MODE3:          return "mode3";
    case ACKOF_MODE4:          return "mode4";
    case ACKOF_CONFIRM_PAIRING:return "confirmPairing";
    default:                   return "";
    }
}

/*==============================================================================
 * FW-5  状态×指令矩阵 (§6.2) + 冷却门 (SAFE-5)
 *============================================================================*/
typedef enum { MX_ACCEPT = 0, MX_IDEMPOTENT, MX_BUSY, MX_REJECT } BT_MatrixAction_t;

/* setSpeed 不是 reg0 控制码，用独立哨兵值参与矩阵查询 */
#define BT_CMD_STOP     0x00
#define BT_CMD_START    0x01
#define BT_CMD_PAUSE    0x02
#define BT_CMD_RESUME   0x03
#define BT_CMD_SETSPEED 0x80

/* 冷却门：temperatureState != normal 时，start/resume/setSpeed 一律 COMMAND_REJECTED。
 * 中断上下文只读温度缓存与状态，不阻塞。 */
static uint8_t BT_ThermalNotNormal(void)
{
    if (sys_state == SYS_STATE_ESTOP) return 1;   /* 热急停强制 overheat */
    if (g_sys_err_code == UART_ERR_E7_IPM_OVER_TEMP) return 1;
    if (UART1_GetIPMTemp() >= UART_IPM_DERATE_LEVEL1_C) return 1;
    return 0;
}

static BT_MatrixAction_t BT_Matrix(uint8_t state, uint8_t cmd)
{
    switch (state) {
    case SYS_STATE_STANDBY:            /* idle */
    case SYS_STATE_STOPPED:            /* stopped */
        switch (cmd) {
        case BT_CMD_START: return MX_ACCEPT;      /* 新会话 */
        case BT_CMD_STOP:  return MX_IDEMPOTENT;
        default:           return MX_REJECT;      /* pause/resume/setSpeed */
        }

    case SYS_STATE_COUNTDOWN_START:    /* starting */
        switch (cmd) {
        case BT_CMD_START:    return MX_IDEMPOTENT;
        case BT_CMD_STOP:     return MX_ACCEPT;   /* →stopping */
        case BT_CMD_PAUSE:    return MX_ACCEPT;   /* →paused   */
        case BT_CMD_RESUME:   return MX_IDEMPOTENT;
        case BT_CMD_SETSPEED: return MX_ACCEPT;   /* 改 targetSpeed */
        default:              return MX_REJECT;
        }

    case SYS_STATE_RUNNING:            /* running */
        switch (cmd) {
        case BT_CMD_START:    return MX_IDEMPOTENT;
        case BT_CMD_STOP:     return MX_ACCEPT;
        case BT_CMD_PAUSE:    return MX_ACCEPT;
        case BT_CMD_RESUME:   return MX_IDEMPOTENT;
        case BT_CMD_SETSPEED: return MX_ACCEPT;
        default:              return MX_REJECT;
        }

    case SYS_STATE_PAUSED:             /* paused */
        switch (cmd) {
        case BT_CMD_START:    return MX_REJECT;   /* 用 resume */
        case BT_CMD_STOP:     return MX_ACCEPT;
        case BT_CMD_PAUSE:    return MX_IDEMPOTENT;
        case BT_CMD_RESUME:   return MX_ACCEPT;
        case BT_CMD_SETSPEED: return MX_ACCEPT;   /* 改目标 */
        default:              return MX_REJECT;
        }

    case SYS_STATE_COUNTDOWN_RESUME:   /* resuming */
        switch (cmd) {
        case BT_CMD_START:    return MX_IDEMPOTENT;
        case BT_CMD_STOP:     return MX_ACCEPT;
        case BT_CMD_PAUSE:    return MX_ACCEPT;
        case BT_CMD_RESUME:   return MX_IDEMPOTENT;
        case BT_CMD_SETSPEED: return MX_ACCEPT;
        default:              return MX_REJECT;
        }

    case SYS_STATE_STOPPING:           /* stopping */
        switch (cmd) {
        case BT_CMD_STOP:     return MX_IDEMPOTENT;
        default:              return MX_BUSY;     /* start/pause/resume/setSpeed */
        }

    case SYS_STATE_VIBRATION:          /* 律动运行 (telemetry 报 running) */
        switch (cmd) {
        case BT_CMD_STOP:     return MX_ACCEPT;   /* →idle */
        default:              return MX_REJECT;   /* 律动无 start/pause/resume/setSpeed */
        }

    case SYS_STATE_ESTOP:              /* error (热急停) */
        switch (cmd) {
        case BT_CMD_STOP:     return MX_ACCEPT;   /* 退出急停回待机(保留恢复路径) */
        default:              return MX_REJECT;
        }

    case SYS_STATE_COMM_ERR:           /* error (安全扣/通信) — 无法经蓝牙清除 */
        switch (cmd) {
        case BT_CMD_STOP:     return MX_IDEMPOTENT;
        default:              return MX_REJECT;
        }

    default:
        return MX_REJECT;
    }
}

/* 接受的 setSpeed：设定目标速度并（仅运行态）应用到跑带。 */
static void BT_ApplySetSpeed(uint8_t speed_val)
{
    float s = speed_val / 10.0f;
    if (s < 1.0f) s = 1.0f;          /* 钳位 1.0~3.8 (§6.4) */
    if (s > 3.8f) s = 3.8f;
    g_target_speed = s;
    /* 仅运行态改变实际跑带速度；starting/paused/resuming 仅记忆目标，
     * 运行态由 SYS_RUN_EnterState(RUNNING) 应用 g_target_speed。 */
    if (sys_state == SYS_STATE_RUNNING)
        RCV315_SetSpeed(s);
    g_bt_regs[1] = (uint8_t)(s * 10.0f + 0.5f);   /* 供 0x03 读回 */
}

/* reg 0x01 setSpeed 分派 (FW-5) */
static void BT_DispatchSetSpeed(uint8_t speed_val)
{
    BT_MatrixAction_t act;

    /* 冷却门：非 normal 温度拒绝 setSpeed (SAFE-5) */
    if (BT_ThermalNotNormal()) {
        BT_RequestAck(ACKOF_SETSPEED, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
        return;
    }

    act = BT_Matrix(sys_state, BT_CMD_SETSPEED);
    switch (act) {
    case MX_ACCEPT:
        BT_ApplySetSpeed(speed_val);
        BT_RequestAck(ACKOF_SETSPEED, ACK_STATUS_OK, ACKERR_NONE);
        break;
    case MX_IDEMPOTENT:
        BT_RequestAck(ACKOF_SETSPEED, ACK_STATUS_OK, ACKERR_NONE);
        break;
    case MX_BUSY:
        BT_RequestAck(ACKOF_SETSPEED, ACK_STATUS_WARNING, ACKERR_DEVICE_BUSY);
        break;
    case MX_REJECT:
    default:
        BT_RequestAck(ACKOF_SETSPEED, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
        break;
    }
}

/* reg 0x00 运行控制 + 律动模式分派 (FW-5)。每条有效帧恰好一条 ack。 */
static void BT_DispatchControl(uint8_t cmd)
{
    BT_AckOf_t        ackOf = BT_CmdToAckOf(cmd);
    BT_MatrixAction_t act;

    /* 律动模式 0x04~0x07：仅在 idle/stopped/vibration 接受，否则 REJ。
     * 接受时 SYS_RUN_HandleBTCtrl 只切一次档 → App 收到一条 ack 不再重发
     * (修复 3 声蜂鸣 / 反复下发 bug #4)。 */
    if (cmd >= 0x04 && cmd <= 0x07) {
        if (sys_state == SYS_STATE_STANDBY ||
            sys_state == SYS_STATE_STOPPED ||
            sys_state == SYS_STATE_VIBRATION) {
            SYS_RUN_HandleBTCtrl(cmd);
            BT_RequestAck(ackOf, ACK_STATUS_OK, ACKERR_NONE);
        } else {
            BT_RequestAck(ackOf, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
        }
        return;
    }

    /* 冷却门 (SAFE-5)：非 normal 温度时 start/resume 强制 COMMAND_REJECTED */
    if ((cmd == BT_CMD_START || cmd == BT_CMD_RESUME) && BT_ThermalNotNormal()) {
        BT_RequestAck(ackOf, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
        return;
    }

    act = BT_Matrix(sys_state, cmd);
    switch (act) {
    case MX_ACCEPT:
        SYS_RUN_HandleBTCtrl(cmd);
        BT_RequestAck(ackOf, ACK_STATUS_OK, ACKERR_NONE);
        break;
    case MX_IDEMPOTENT:
        BT_RequestAck(ackOf, ACK_STATUS_OK, ACKERR_NONE);
        break;
    case MX_BUSY:
        BT_RequestAck(ackOf, ACK_STATUS_WARNING, ACKERR_DEVICE_BUSY);
        break;
    case MX_REJECT:
    default:
        BT_RequestAck(ackOf, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
        break;
    }
}

static void BT_ProcessFrame(void)
{
    if (bt_cmd == 0x02) {
        /* —— 配对确认寄存器 0x03（需求 5.1/5.4/7.2）——
         * 命中条件：本帧数据覆盖到寄存器 3。对标准帧
         * AA 55 02 03 01 01 55 成立（reg=0x03, len=0x01, data=0x01）。 */
        if (bt_reg <= 3 && (bt_reg + bt_len) > 3) {
            uint8_t pair_val = bt_buf[3 - bt_reg];
            if (pair_val == 0x01) {
                /* 方案 A：中断仅置易失标志，由主循环检测后擦写 Flash。 */
                g_pair_commit_req = 1;
                /* FW-1：同时回 ack ok，App 的 AckMatcher 得以匹配 confirmPairing。 */
                BT_RequestAck(ACKOF_CONFIRM_PAIRING, ACK_STATUS_OK, ACKERR_NONE);
            }
            /* pair_val != 0x01 → 不置标志/不回 ack（视为无效数据）。 */
            return;
        }

        /* 寄存器 0x01：速度 (setSpeed)，值/10=速度 */
        if (bt_reg <= 1 && (bt_reg + bt_len) > 1) {
            uint8_t speed_val = bt_buf[1 - bt_reg];
            /* §4.5：越界 (<0x0A 或 >0x26) → COMMAND_REJECTED（不改速度） */
            if (speed_val < 0x0A || speed_val > 0x26) {
                BT_RequestAck(ACKOF_SETSPEED, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
                return;
            }
            BT_DispatchSetSpeed(speed_val);
            return;
        }

        /* 寄存器 0x00：运行控制 / 律动模式 */
        if (bt_reg == 0 && bt_len > 0) {
            BT_DispatchControl(bt_buf[0]);
            return;
        }

        /* §4.5：未知寄存器 → COMMAND_REJECTED（ackOf 为空串） */
        BT_RequestAck(ACKOF_NONE, ACK_STATUS_WARNING, ACKERR_COMMAND_REJECTED);
        return;
    } else if (bt_cmd == 0x03) {
        BT_SendFrame(0x03, bt_reg, bt_len, &g_bt_regs[bt_reg]);
    }
}

void BT_ParseByte(uint8_t byte)
{
    BT_AT_CaptureByte(byte);

    switch (bt_state) {
    case BT_WAIT_HEAD1:
        if (byte == 0xAA) bt_state = BT_WAIT_HEAD2;
        break;

    case BT_WAIT_HEAD2:
        bt_state = (byte == 0x55) ? BT_WAIT_CMD : BT_WAIT_HEAD1;
        break;

    case BT_WAIT_CMD:
        if (byte == 0x02 || byte == 0x03) {
            bt_cmd = byte;
            bt_state = BT_WAIT_REG;
        } else {
            bt_state = BT_WAIT_HEAD1;
        }
        break;

    case BT_WAIT_REG:
        bt_reg = byte;
        bt_state = BT_WAIT_LEN;
        break;

    case BT_WAIT_LEN:
        bt_len = byte;
        if (bt_len == 0) {
            bt_state = BT_WAIT_TAIL;
        } else if (bt_len <= BT_REGS_SIZE) {
            bt_idx = 0;
            bt_state = BT_DATA;
        } else {
            bt_state = BT_WAIT_HEAD1;
        }
        break;

    case BT_DATA:
        bt_buf[bt_idx++] = byte;
        if (bt_idx >= bt_len) bt_state = BT_WAIT_TAIL;
        break;

    case BT_WAIT_TAIL:
        if (byte == 0x55) BT_ProcessFrame();
        bt_state = BT_WAIT_HEAD1;
        break;
    }
}

/**
 * UART2 硬件初始化 (RX=PA1, TX=PA2, 115200-8-N-1)
 */
void BT_UART_Init(void)
{
            UART_InitType UART_InitStruct;
    GPIO_InitType GPIO_InitStructure;
        NVIC_InitType NVIC_InitStructure;
    /* Initialize GPIO_InitStructure */
    GPIO_Structure_Initialize(&GPIO_InitStructure);



    /* 使能 GPIOA 和 UART2 时钟 */
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART2);



            /* Configure UARTz Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = UARTz_TxPin;
        GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = UARTz_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(UARTz_GPIO, &GPIO_InitStructure);

    /* Configure UARTz Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = UARTz_RxPin;
            GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate = UARTz_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(UARTz_GPIO, &GPIO_InitStructure);



    UART_Structure_Initializes(&UART_InitStruct);

    /* UART2 配置: 115200-8-N-1 */
    UART_Structure_Initializes(&UART_InitStruct);
    UART_InitStruct.BaudRate   = 115200;
    UART_InitStruct.WordLength = UART_WL_8B;
    UART_InitStruct.StopBits   = UART_STPB_1;
    UART_InitStruct.Parity     = UART_PE_NO;
    UART_InitStruct.Mode       = UART_MODE_TX | UART_MODE_RX;

    UART_Initializes(UART2, &UART_InitStruct);
        UART_Interrput_Enable(UARTz, UART_INT_RXDNE);
    /* TXDE 中断只在有数据要发送时才开，否则初始化后立即触发死循环 */
    UART_Enable(UART2);




//    /* Enable the UARTy Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel         = UARTy_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
//    NVIC_Initializes(&NVIC_InitStructure);

        /* Enable the UARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel         = UARTz_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

uint16_t BT_GenRand4Digits(void)
{
    uint32_t seed;
    seed = TIM_Base_Count_Get(TIM1) ^ g_ms_tick;
    return (uint16_t)(seed % 10000);
}

/*----------------------------------------------------------------------------
 * BLE UUID（MX-01P 透传模块出厂默认 16-bit UUID）
 *
 *   Service      FFF0
 *   上行 Read/Notify FFF1  设备→App，JSON
 *   下行 Write       FFF2  App→设备，hex(WWR)
 *
 * UUID 由 MX-01P 持久化保存。N32 侧在启动时通过 UART2 主动写回这组默认值，
 * 以覆盖可能已保存的旧自定义 UUID；配置后需 AT+REBOOT=1 重启生效。
 *--------------------------------------------------------------------------*/
#define BT_UUID_SERVICE   "FFF0"
#define BT_UUID_NOTIFY    "FFF1"
#define BT_UUID_WRITE     "FFF2"

/*
 * 向透传模块（MX-01P）写回默认 16-bit UUID 配置。
 *
 * MX-01P AT 指令集：
 *   AT+UUIDS=FFF0  设置 Service UUID
 *   AT+UUIDN=FFF1  设置 Read/Notify（上行）UUID
 *   AT+UUIDW=FFF2  设置 Write（下行）UUID
 * 模块返回 OK 后需 AT+REBOOT=1 才生效。
 */
static uint8_t BT_ConfigUuids(void)
{
    char cmd[80];
    uint8_t ok = 1;

    sprintf(cmd, "AT+UUIDS=%s\r\n", BT_UUID_SERVICE);
    ok &= BT_AT_SendAndWaitOk(cmd, 800);

    sprintf(cmd, "AT+UUIDN=%s\r\n", BT_UUID_NOTIFY);
    ok &= BT_AT_SendAndWaitOk(cmd, 800);

    sprintf(cmd, "AT+UUIDW=%s\r\n", BT_UUID_WRITE);
    ok &= BT_AT_SendAndWaitOk(cmd, 800);

    return ok;
}

static uint8_t BT_VerifyUuids(void)
{
    uint8_t ok = 1;

    ok &= BT_AT_QueryContains("AT+UUIDS?\r\n", BT_UUID_SERVICE, 800);
    ok &= BT_AT_QueryContains("AT+UUIDN?\r\n", BT_UUID_NOTIFY, 800);
    ok &= BT_AT_QueryContains("AT+UUIDW?\r\n", BT_UUID_WRITE, 800);

    return ok;
}

void BT_Init(void)
{
    char cmd[64];
    char device_name[12] = "TW-04<0000>"; /* "TW-04<" + SN last 4 + ">" + NUL */
    const char *sn = SN_Builder_Get();
    uint8_t sn_len = 0;

    /* Vendor command set: after power-on/reboot, wait +READY before AT/data.
     * If +READY was emitted before MCU enabled UART2, this times out and the
     * following OK-checked commands still provide a second readiness check.
     */
    BT_AT_ClearRx();
    (void)BT_AT_WaitFor("+READY", 1500);
    /* SN_Builder_Init runs before BT_Init. Bound the scan to its fixed buffer;
     * an empty, short, NULL, or unterminated value safely keeps suffix "0000".
     * device_name is exactly 11 chars and this AT command needs at most 22 bytes
     * including CRLF and NUL, well within cmd[64].
     */
    if (sn != NULL) {
        while (sn_len < SN_BUF_SIZE && sn[sn_len] != '\0') {
            sn_len++;
        }
        if (sn_len >= 4U && sn_len < SN_BUF_SIZE) {
            memcpy(&device_name[6], &sn[sn_len - 4U], 4U);
        }
    }
    sprintf(cmd, "AT+NAME=%s\r\n", device_name);
    (void)BT_AT_SendAndWaitOk(cmd, 800);

    /* Stop advertising while changing UUIDs, then restart it after reboot so
     * the module refreshes the advertising payload from the current GATT setup.
     */
    (void)BT_AT_SendAndWaitOk("AT+ADV=0\r\n", 800);

    /* 写回 FFF0/FFF1/FFF2；在 REBOOT 使其生效之前下发。 */
    (void)BT_ConfigUuids();
    (void)BT_VerifyUuids();

    /* UUID changes take effect only after module reboot. Wait for +READY, then
     * reopen advertising and query UUIDs once more to catch failed persistence.
     */
    (void)BT_AT_SendAndWait("AT+REBOOT=1\r\n", "+READY", 4000);
    (void)BT_AT_SendAndWaitOk("AT+ADV=1\r\n", 800);
    (void)BT_VerifyUuids();
}

void UART2_SendData(uint8_t *pData, uint16_t len)
{
    while (len--)
    {
        while (UART_Flag_Status_Get(UART2, UART_FLAG_TXDE) == RESET);
        UART_Data_Send(UART2, *pData++);
    }
    while (UART_Flag_Status_Get(UART2, UART_FLAG_TXC) == RESET);
}

void UART2_SendString(char *str)
{
    UART2_SendData((uint8_t *)str, strlen(str));
}

/*==============================================================================
 * BT_SendTelemetry
 *   构建 JSON 遥测报文并通过蓝牙 UART2 上行发送
 *============================================================================*/
#define TELEM_BUF_SIZE  512
#define TELEM_MODEL     "TW-04"
#define TELEM_PROD_DATE "20260315"

void BT_SendTelemetry(void)
{
    char             buf[TELEM_BUF_SIZE];
    const char      *dev_state;
    const char      *temp_state;
    const char      *running_str;
    uint8_t          motor_temp;
    uint8_t          safety_rhythm;
    float            telem_speed;
    float            telem_target;
    uint16_t         speed_tenths;
    uint16_t         target_tenths;
    uint32_t         distance_hundredths;

    /* FW-2：映射系统状态 → 规范 deviceState 字符串
     * (idle/starting/running/paused/resuming/stopping/stopped/error；律动报 running) */
    switch (sys_state)
    {
    case SYS_STATE_STANDBY:          dev_state = "idle";     break;
    case SYS_STATE_COUNTDOWN_START:  dev_state = "starting"; break;
    case SYS_STATE_RUNNING:          dev_state = "running";  break;
    case SYS_STATE_PAUSED:           dev_state = "paused";   break;
    case SYS_STATE_COUNTDOWN_RESUME: dev_state = "resuming"; break;
    case SYS_STATE_STOPPING:         dev_state = "stopping"; break;
    case SYS_STATE_STOPPED:          dev_state = "stopped";  break;
    case SYS_STATE_VIBRATION:        dev_state = "running";  break;  /* 律动上报 running */
    case SYS_STATE_ESTOP:            dev_state = "error";    break;
    case SYS_STATE_COMM_ERR:         dev_state = "error";    break;
    default:                         dev_state = "idle";     break;
    }

    /* FW-3：映射电机温度 → temperatureState，仅 normal/hot/overheat */
    motor_temp = UART1_GetIPMTemp();
    if      (sys_state == SYS_STATE_ESTOP ||
             g_sys_err_code == UART_ERR_E7_IPM_OVER_TEMP)
                                               temp_state = "overheat";
    else if (motor_temp >= UART_IPM_ERROR_TRIP_C)
                                               temp_state = "overheat";
    else if (motor_temp >= UART_IPM_DERATE_LEVEL1_C)
                                               temp_state = "hot";
    else                                       temp_state = "normal";

    safety_rhythm = (sys_state == SYS_STATE_COMM_ERR &&
                     g_safety_err &&
                     SYS_RUN_SafetySnapshotIsVibration());

    /* —— 律动(振动)模式遥测：BX39 组合机在 VIBRATION 态按 §3.2 上报
     *    deviceType:"rhythm" + mode(1~4)，不带 speed/targetSpeed/distance。
     *    使 App 能从遥测（含重连后首帧）恢复当前律动档位，而非仅靠 ack。
     *    安全卡扣中断期间保留 rhythm 形态、档位和被冻结的剩余倒计时，避免
     *    App 误判为走步机或律动自然结束；重新插回后从该倒计时继续。 */
    if (sys_state == SYS_STATE_VIBRATION || safety_rhythm)
    {
        sprintf(buf,
            "{\"type\":\"telemetry\",\"seq\":%lu,\"ts\":%lu,\"status\":\"ok\"," 
            "\"data\":{\"deviceType\":\"rhythm\",\"deviceState\":\"%s\"," 
            "\"sessionId\":%u,\"running\":%s,\"mode\":%u,\"calories\":%u," 
            "\"duration\":%u,\"countdown\":%u,\"temperatureState\":\"%s\"," 
            "\"manufacturerData\":{\"model\":\"%s\",\"sn\":\"%s\","
            "\"productionDate\":\"%s\",\"firstPairing\":%s}},"
            "\"error\":null}\r\n",
            (unsigned long)BT_NextSeq(),
            (unsigned long)g_ms_tick,
            safety_rhythm ? "error" : "running",
            g_session_id,
            safety_rhythm ? "false" : "true",
            (unsigned)(safety_rhythm
                ? SYS_RUN_GetSafetyVibrationLevel()
                : RCV315_GetVibLevel()),
            g_calories,
            sys_run_time,
            safety_rhythm ? SYS_RUN_GetSafetyCountdown() : g_countdown,
            temp_state,
            TELEM_MODEL,
            SN_Builder_Get(),
            TELEM_PROD_DATE,
            Pairing_IsFirst() ? "true" : "false"
        );
        UART2_SendString(buf);
        return;
    }

    /* 运行状态判断：running/vibration(律动) 均为 true */
    running_str = (sys_state == SYS_STATE_RUNNING ||
                   sys_state == SYS_STATE_VIBRATION) ? "true" : "false";

    /* 跑带停止的状态下速度上报 0 (idle/stopped/error)，其余上报实际速度 */
    switch (sys_state)
    {
    case SYS_STATE_STANDBY:
    case SYS_STATE_STOPPED:
    case SYS_STATE_ESTOP:
    case SYS_STATE_COMM_ERR:
        telem_speed = 0.0f;
        break;
    default:
        telem_speed = RCV315_GetSpeed();
        break;
    }

    telem_target = (sys_state == SYS_STATE_COMM_ERR &&
                    g_safety_err &&
                    SYS_RUN_IsSafetyInterrupted())
        ? SYS_RUN_GetSafetyTargetSpeed()
        : g_target_speed;

    /*
     * 仅在序列化边界把浮点数转成定点数。避免 sprintf("%f") 将整套
     * double/float printf 库拉入 32 KB MCU，数值精度仍严格保持 1/2 位小数。
     */
    speed_tenths = (uint16_t)(telem_speed * 10.0f + 0.5f);
    target_tenths = (uint16_t)(telem_target * 10.0f + 0.5f);
    distance_hundredths = (uint32_t)(g_distance_miles * 100.0f + 0.5f);

    /* 构建 JSON 遥测报文：speed=实际速度, targetSpeed=g_target_speed (FW-6 独立) */
    sprintf(buf,
        "{\"type\":\"telemetry\",\"seq\":%lu,\"ts\":%lu,\"status\":\"ok\","
        "\"data\":{\"deviceType\":\"treadmill\",\"deviceState\":\"%s\","
        "\"sessionId\":%u,\"running\":%s,\"speed\":%u.%u,\"targetSpeed\":%u.%u,"
        "\"distance\":%lu.%02lu,\"calories\":%u,\"duration\":%u,\"countdown\":%u,"
        "\"temperatureState\":\"%s\","
        "\"manufacturerData\":{\"model\":\"%s\",\"sn\":\"%s\","
        "\"productionDate\":\"%s\",\"firstPairing\":%s}},"
        "\"error\":null}\r\n",
        (unsigned long)BT_NextSeq(),            /* FW-1：统一 seq */
        (unsigned long)g_ms_tick,
        dev_state,
        g_session_id,
        running_str,
        (unsigned)(speed_tenths / 10u),
        (unsigned)(speed_tenths % 10u),
        (unsigned)(target_tenths / 10u),         /* 安全中断时仍报告中断前目标 */
        (unsigned)(target_tenths % 10u),
        (unsigned long)(distance_hundredths / 100u),
        (unsigned long)(distance_hundredths % 100u),
        g_calories,
        sys_run_time,
        g_countdown,
        temp_state,
        TELEM_MODEL,
        SN_Builder_Get(),                       /* sn ← 动态 UID（需求 3.1） */
        TELEM_PROD_DATE,
        Pairing_IsFirst() ? "true" : "false"    /* firstPairing ← Flash（需求 4.4） */
    );

    UART2_SendString(buf);
}

/*==============================================================================
 * FW-1  ack 构建/发送 (只能在主循环上下文调用；内部阻塞式 TX)
 *============================================================================*/
void BT_SendAck(BT_AckOf_t ackOf, BT_AckStatus_t status, BT_AckErr_t err)
{
    char buf[256];
    if (status == ACK_STATUS_OK) {
        sprintf(buf,
          "{\"type\":\"ack\",\"seq\":%lu,\"ts\":%lu,\"status\":\"ok\","
          "\"data\":{\"ackOf\":\"%s\"},\"error\":null}\r\n",
          (unsigned long)BT_NextSeq(), (unsigned long)g_ms_tick, BT_AckOfStr(ackOf));
    } else {
        const char *code = (err == ACKERR_DEVICE_BUSY) ? "DEVICE_BUSY" : "COMMAND_REJECTED";
        sprintf(buf,
          "{\"type\":\"ack\",\"seq\":%lu,\"ts\":%lu,\"status\":\"warning\","
          "\"data\":{\"ackOf\":\"%s\"},"
          "\"error\":{\"code\":\"%s\",\"level\":\"warning\","
          "\"message\":\"Rejected in current state\",\"recoverable\":true}}\r\n",
          (unsigned long)BT_NextSeq(), (unsigned long)g_ms_tick, BT_AckOfStr(ackOf), code);
    }
    UART2_SendString(buf);
}

/*==============================================================================
 * FW-7  心跳 (每 2s，主循环发送)
 *============================================================================*/
void BT_SendHeartbeat(void)
{
    char buf[128];
    sprintf(buf,
        "{\"type\":\"heartbeat\",\"seq\":%lu,\"ts\":%lu,"
        "\"status\":\"ok\",\"data\":{},\"error\":null}\r\n",
        (unsigned long)BT_NextSeq(), (unsigned long)g_ms_tick);
    UART2_SendString(buf);
}

/*==============================================================================
 * FW-8  设备主动事件 (主循环发送)
 *============================================================================*/
/*
 * 走步机故障帧的公共构建器。普通故障与安全卡扣中断使用同一帧结构，
 * 避免在小容量 Flash 中保留两份几乎相同的 JSON 格式串。
 */
static void BT_SendTreadmillError(const char *code,
                                  const char *message,
                                  const char *tempState,
                                  const char *recoverable,
                                  float targetSpeed,
                                  uint16_t countdown)
{
    char buf[512];
    uint16_t target_tenths = (uint16_t)(targetSpeed * 10.0f + 0.5f);
    uint32_t distance_hundredths = (uint32_t)(g_distance_miles * 100.0f + 0.5f);
    sprintf(buf,
        "{\"type\":\"device_error\",\"seq\":%lu,\"ts\":%lu,\"status\":\"error\","
        "\"data\":{\"deviceType\":\"treadmill\",\"deviceState\":\"error\",\"sessionId\":%u,"
        "\"running\":false,\"speed\":0,\"targetSpeed\":%u.%u,\"distance\":%lu.%02lu,\"calories\":%u,"
        "\"duration\":%u,\"countdown\":%u,\"temperatureState\":\"%s\"},"
        "\"error\":{\"code\":\"%s\",\"level\":\"critical\",\"message\":\"%s\",\"recoverable\":%s}}\r\n",
        (unsigned long)BT_NextSeq(), (unsigned long)g_ms_tick, g_session_id,
        (unsigned)(target_tenths / 10u), (unsigned)(target_tenths % 10u),
        (unsigned long)(distance_hundredths / 100u),
        (unsigned long)(distance_hundredths % 100u),
        g_calories, sys_run_time, countdown,
        tempState, code, message, recoverable);
    UART2_SendString(buf);
}

/* 严重故障标准回传 (§7.1/§7.3)：deviceState:error, running:false, speed:0, targetSpeed:0 */
void BT_SendDeviceError(const char *code, const char *message, const char *tempState)
{
    const char *recoverable =
        (strcmp(code, "MOTOR_FAULT") == 0 || strcmp(code, "SENSOR_FAULT") == 0)
        ? "false" : "true";

    BT_SendTreadmillError(code, message, tempState, recoverable, 0.0f, 0u);
}

/* 安全卡扣是可恢复的会话中断：错误帧保留中断前设备形态与关键会话数据。 */
void BT_SendSafetyKeyError(void)
{
    if (!SYS_RUN_SafetySnapshotIsVibration())
    {
        BT_SendTreadmillError("EMERGENCY_STOP", "Safety key removed", "normal", "true",
                              SYS_RUN_GetSafetyTargetSpeed(), SYS_RUN_GetSafetyCountdown());
        return;
    }

    {
        char buf[512];
        sprintf(buf,
            "{\"type\":\"device_error\",\"seq\":%lu,\"ts\":%lu,\"status\":\"error\"," 
            "\"data\":{\"deviceType\":\"rhythm\",\"deviceState\":\"error\",\"sessionId\":%u," 
            "\"running\":false,\"mode\":%u,\"calories\":%u,\"duration\":%u,\"countdown\":%u," 
            "\"temperatureState\":\"normal\"},"
            "\"error\":{\"code\":\"EMERGENCY_STOP\",\"level\":\"critical\"," 
            "\"message\":\"Safety key removed\",\"recoverable\":true}}\r\n",
            (unsigned long)BT_NextSeq(), (unsigned long)g_ms_tick, g_session_id,
            (unsigned)SYS_RUN_GetSafetyVibrationLevel(), g_calories, sys_run_time,
            SYS_RUN_GetSafetyCountdown());
        UART2_SendString(buf);
    }
}

/* 过热降速预警 (§7.1)：speed/targetSpeed=降速后安全值, temperatureState:hot */
void BT_SendOverheatDownspeed(void)
{
    char  buf[384];
    float sp = RCV315_GetSpeed();
    uint16_t speed_tenths = (uint16_t)(sp * 10.0f + 0.5f);
    uint16_t target_tenths = (uint16_t)(g_target_speed * 10.0f + 0.5f);
    sprintf(buf,
        "{\"type\":\"warning\",\"seq\":%lu,\"ts\":%lu,\"status\":\"warning\","
        "\"data\":{\"deviceType\":\"treadmill\",\"speed\":%u.%u,\"targetSpeed\":%u.%u,"
        "\"temperatureState\":\"hot\"},"
        "\"error\":{\"code\":\"OVERHEAT_DOWNSPEED\",\"level\":\"warning\","
        "\"message\":\"Overheat downspeed\",\"recoverable\":true}}\r\n",
        (unsigned long)BT_NextSeq(), (unsigned long)g_ms_tick,
        (unsigned)(speed_tenths / 10u), (unsigned)(speed_tenths % 10u),
        (unsigned)(target_tenths / 10u), (unsigned)(target_tenths % 10u));
    UART2_SendString(buf);
}




void UARTz_IRQHandler(void)
{
    if (UART_Interrupt_Status_Get(UARTz, UART_INT_RXDNE) != RESET)
    {
                    uint8_t byte = (uint8_t)UART_Data_Receive(UARTz);
        BT_ParseByte(byte);
   

    }

    if (UART_Interrupt_Status_Get(UARTz, UART_INT_TXDE) != RESET)
    {
        /* Write one byte to the transmit data register */

    }

    /* Determine if an error flag still exists and clear the error flag */
    if ((UART_Flag_Status_Get(UARTz, UART_FLAG_OREF) != RESET) || \
        (UART_Flag_Status_Get(UARTz, UART_FLAG_NEF) != RESET) ||  \
        (UART_Flag_Status_Get(UARTz, UART_FLAG_FEF) != RESET) ||  \
        (UART_Flag_Status_Get(UARTz, UART_FLAG_PEF) != RESET))
    {
        /* Read the STS register first, and the read the DAT register to clear the all error flag */
        (void) UARTz->STS;
        (void) UARTz->DAT;
        /* Under normal circumstances, all error flags will be cleared when the upper data is read and will not be executed here;
        users can add their own processing according to the actual scenario. */
    }
}
