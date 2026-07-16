#include "ctrl_tx.h"
#include "SYS_RUN.h"
#include "RCV315.h"
#include "uart_comm.h"
#include "beep.h"
#include "bt_transparent.h"   /* FW-8: g_evt_emergency_stop 事件标志 */

/* Bench-testing bypass for the E0 (UART1 ACK timeout) path.
 * DEFINE for bench/test builds only. Leave undefined for production.
 * See .kiro/specs/e0-bypass-bt-testing.
 * 量产状态：保持注释 —— E0（上下控通信超时）报错正常启用。 */
/* Production default: E0 timeout protection is enabled. Define only for an
 * isolated bench build where the downstream controller is intentionally absent. */

#define CTRL_PERIOD_MS 200

extern uint8_t  sys_state;
extern volatile uint32_t g_ms_tick;

uint8_t g_safety_err   = 0;
uint8_t g_sys_err_code = 0;
static uint8_t s_fault_recovery_pending = 0;

void CTRL_TX_Init(void)
{
}

uint8_t CTRL_TX_RequestFaultRecovery(void)
{
    if (g_safety_err)
        return 0;

    if (g_sys_err_code == UART_ERR_E5_MOTOR_STALL)
    {
        s_fault_recovery_pending = 1;
        return 1;
    }

    if (g_sys_err_code == UART_ERR_E7_IPM_OVER_TEMP &&
        UART1_GetIPMTemp() <= UART_IPM_RECOVER_C &&
        UART1_GetTemp() <= UART_MOTOR_RECOVER_C)
    {
        s_fault_recovery_pending = 1;
        return 1;
    }

    return 0;
}

void CTRL_TX_Process(void)
{
    uint32_t now = g_ms_tick;
    static uint32_t last_call       = 0;
    static uint8_t  last_state      = 0xFF;
    static uint32_t last_ack        = 0;
    static uint8_t  ack_inited      = 0;
    static uint8_t  safety_releasing = 0;
    static uint8_t  safety_was_high  = 0;
    static uint8_t  vib_off_pending  = 0;
    static uint8_t  estop_release_pending = 0;
    static uint8_t  e0_active = 0;
    static uint8_t  e0_release_pending = 0;
    uint8_t safety_high;
    uint8_t ack_received;

    if (now - last_call < CTRL_PERIOD_MS) return;
    last_call = now;

    if (!ack_inited) {
        last_ack   = now;
        ack_inited = 1;
    }

    safety_high = (GPIO_Input_Pin_Data_Get(GPIOA, GPIO_PIN_4) == SET);

    /* 安全卡扣上升沿：保存会话并进入安全停止；任何原状态都必须上报一次。 */
    if (safety_high) {
        g_safety_err       = 1;
        s_fault_recovery_pending = 0;
        e0_active = 0;
        e0_release_pending = 0;
        safety_releasing = 0;
        if (!safety_was_high) {
            SYS_RUN_HandleSafetyRemoved();
            Beep_Tem = BEEP_SHORT;
            /* 安全卡扣脱落 = 急停，向 App 上报一次 EMERGENCY_STOP。 */
            g_evt_emergency_stop = 1;
        } else if (sys_state != SYS_STATE_COMM_ERR) {
            /* 卡扣仍脱落时不允许按键/蓝牙命令提前退出安全态。 */
            SYS_RUN_HandleSafetyRemoved();
        }
    }
    safety_was_high = safety_high;

    /* ACK 看门狗检查 */
    ack_received = UART1_CheckAndClearAck();
    if (ack_received) {
        last_ack = now;
        vib_off_pending = 0;
        estop_release_pending = 0;
        if (sys_state == SYS_STATE_COMM_ERR) {
            /* 仅当非安全扣/非系统错误触发、或安全扣已发解除且收到确认 ACK 时才退出 */
            if (safety_releasing && !safety_high) {
                g_safety_err     = 0;
                safety_releasing = 0;
                e0_active        = 0;
                e0_release_pending = 0;
                Beep_Tem         = 800;
                /* 卡扣已恢复但下控仍有真实故障时继续保持错误态，禁止恢复运动。 */
                if (!g_sys_err_code)
                    SYS_RUN_HandleSafetyRestored();
                else
                    SYS_RUN_DiscardSafetySnapshot();
            } else if (!g_safety_err && !g_sys_err_code) {
                if (e0_active && !e0_release_pending) {
                    /* ACK confirms communication is back. Keep the error state
                     * for one more exchange so the lower controller explicitly
                     * consumes CMD_EMERGENCY_OFF before normal commands resume. */
                    e0_release_pending = 1;
                } else {
                    e0_active = 0;
                    e0_release_pending = 0;
                    SYS_RUN_HandleSystemFaultRecovered();
                }
            }
        }
    }

    /* 通信超时 → COMM_ERR (2s 无应答) */
#ifndef TEST_BYPASS_COMM_ERR
    if (!g_safety_err && !e0_active && (now - last_ack > 2000)) {
        e0_active = 1;
        e0_release_pending = 0;
        s_fault_recovery_pending = 0;
        /* A lower-controller fault code is valid only while fresh response
         * frames continue to arrive. Once the link times out, a retained E5/E6/E7
         * byte is stale and the visible/current fault must become E0. */
        g_sys_err_code = 0;
        sys_state = SYS_STATE_COMM_ERR;
        RCV315_SetSpeed(0.0f);
        Beep_Tem = 800;
    }
#else
    /* TEST_BYPASS_COMM_ERR: E0 (UART1 ACK timeout) suppressed for bench testing.
     * State is left unchanged so the Bluetooth command path can be exercised
     * without the downstream motor controller connected. E17 (PA4) and E2
     * (overheat/ESTOP) paths remain fully active. `last_ack` continues to be
     * maintained by the ACK-received branch above, so no other logic changes. */
#endif

    /* 下位机系统错误检查 (寄存器 0x05: E5/E6/E7 错误代码) */
    if (ack_received)
    {
        uint8_t sys_err = UART1_GetSystemError();
        if (sys_err != 0)
        {
            uint8_t is_new_fault = (g_sys_err_code != sys_err);
            e0_active = 0;
            e0_release_pending = 0;
            if (is_new_fault)
                s_fault_recovery_pending = 0;
            g_sys_err_code = sys_err;
            if (sys_state != SYS_STATE_COMM_ERR)
            {
                sys_state = SYS_STATE_COMM_ERR;
                RCV315_SetSpeed(0.0f);
                Beep_Tem = 800;
            }
            /* 即使卡扣同时脱落，也锁存真实故障，避免插回后错误恢复运动。 */
            if (is_new_fault)
                g_evt_system_fault = 1;
        }
        else if (s_fault_recovery_pending && g_sys_err_code != 0)
        {
            /* 下控已在解除命令后回传 error=0，至此才真正退出错误态。 */
            s_fault_recovery_pending = 0;
            g_sys_err_code = 0;
            SYS_RUN_HandleSystemFaultRecovered();
        }
    }

    /* 退出震动模式时关断下位机震动, 持续发送直到 ACK 确认 */
    if (last_state == SYS_STATE_VIBRATION && sys_state != SYS_STATE_VIBRATION) {
        vib_off_pending = 1;
    }
    if (sys_state == SYS_STATE_VIBRATION) {
        vib_off_pending = 0;
    }
    if (last_state == SYS_STATE_ESTOP && sys_state != SYS_STATE_ESTOP) {
        estop_release_pending = 1;
    }
    if (sys_state == SYS_STATE_ESTOP) {
        estop_release_pending = 0;
    }
    last_state = sys_state;

    switch (sys_state)
    {
    case SYS_STATE_RUNNING:
        UART1_SendSpeed(RCV315_GetSpeed());
        break;

    case SYS_STATE_VIBRATION:
        UART1_SendVibration(RCV315_GetVibLevel());
        break;

    case SYS_STATE_ESTOP:
        /* E2 uses the same controlled emergency ramp as the safety key. */
        UART1_SendEmergencyStop();
        break;

    case SYS_STATE_COMM_ERR:
        if (s_fault_recovery_pending) {
            UART1_SendEmergencyRelease();
        } else if (g_safety_err && !safety_high) {
            UART1_SendEmergencyRelease();
            safety_releasing = 1;
        } else if (e0_active && e0_release_pending) {
            UART1_SendEmergencyRelease();
        } else {
            UART1_SendEmergencyStop();
        }
        break;

    default:
        if (estop_release_pending) {
            UART1_SendEmergencyRelease();
        } else if (vib_off_pending) {
            UART1_SendVibration(0);
        } else {
            UART1_SendSpeed(0.0f);
        }
        break;
    }
}
