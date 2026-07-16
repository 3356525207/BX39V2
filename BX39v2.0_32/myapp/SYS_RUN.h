#ifndef __SYS_RUN_H
#define __SYS_RUN_H

#include "main.h"

/*==============================================================================
 * 状态枚举
 *============================================================================*/
typedef enum {
    SYS_STATE_STANDBY           = 0,
    SYS_STATE_COUNTDOWN_START   = 1,
    SYS_STATE_RUNNING           = 2,
    SYS_STATE_PAUSED            = 3,
    SYS_STATE_COUNTDOWN_RESUME  = 4,
    SYS_STATE_STOPPING          = 5,
    SYS_STATE_VIBRATION         = 6,
    SYS_STATE_ESTOP             = 7,
    SYS_STATE_COMM_ERR          = 8,
    SYS_STATE_STOPPED           = 9   /* 停机保持相 (FW-4): STOPPING→STOPPED(≥1s)→STANDBY */
} SYS_State_t;

/*==============================================================================
 * 运行态显示子模式
 *============================================================================*/
typedef enum {
    RUN_DISP_SPEED    = 0,
    RUN_DISP_TIME     = 1,
    RUN_DISP_STEPS    = 2,
    RUN_DISP_DISTANCE = 3
} RunDispMode_t;

/*==============================================================================
 * 7 段码字母定义 (dp-g-f-e-d-c-b-a)
 *============================================================================*/
#define SEG_DASH    0x40    /* - */
#define SEG_P       0x73    /* P */
#define SEG_A       0x77    /* A */
#define SEG_U       0x3E    /* U */
#define SEG_S       0x6D    /* S */
#define SEG_E       0x79    /* E */
#define SEG_BLANK   0x00

/* 独立段位 (用于 LED 指示灯) */
#define SEG_BIT_A   0x01    /* a 段 */
#define SEG_BIT_B   0x02    /* b 段 */
#define SEG_BIT_C   0x04    /* c 段 */
#define SEG_BIT_D   0x08    /* d 段 */
#define SEG_BIT_E   0x10    /* e 段 */
#define SEG_BIT_F   0x20    /* f 段 */
#define SEG_BIT_G   0x40    /* g 段 */
#define SEG_BIT_H   0x80    /* h 段 (dp) */


/*==============================================================================
 * 全局变量
 *============================================================================*/
extern uint8_t  sys_state;
extern uint8_t  g_safety_err;
extern uint8_t  g_sys_err_code;
extern uint16_t sys_run_time;
extern uint16_t sys_steps;
extern float    g_distance_miles;
extern uint16_t g_countdown;
extern uint16_t g_session_id;
extern uint16_t g_calories;

/* 状态切换即时遥测标志：SYS_RUN_EnterState 中置 1（可能在 UART2 中断上下文），
 * 主循环检测后立即补发一帧 telemetry 并清零。使 App 在 starting→running 等状态
 * 切换瞬间即可收到最新 deviceState，不必等下一个 200ms 周期，也能抵御蓝牙模组
 * 偶发丢帧导致的「设备已启动 App 仍显示启动中」延迟。 */
extern volatile uint8_t g_telem_now;

/* 目标速度记忆 (FW-6, §6.4)：与瞬时 RCV315_GetSpeed() 独立上报为 targetSpeed。
 * idle/stopped 启动重置 1.0；setSpeed 接受时设定(钳位 1.0~3.8)；暂停/恢复保留；
 * 过热降速时与当前速度同步；stop/error 后重置 1.0。 */
extern float    g_target_speed;

/*==============================================================================
 * API 函数
 *============================================================================*/
void SYS_RUN_Init(void);
void SYS_RUN_Process(void);
void SYS_RUN_UpdateDisplay(void);
void SYS_RUN_HandleBTCtrl(uint8_t cmd);
void SYS_RUN_HandleSafetyRemoved(void);
void SYS_RUN_HandleSafetyRestored(void);
void SYS_RUN_HandleSystemFaultRecovered(void);
void SYS_RUN_DiscardSafetySnapshot(void);
uint8_t SYS_RUN_IsSafetyInterrupted(void);
uint8_t SYS_RUN_SafetySnapshotIsVibration(void);
uint8_t SYS_RUN_GetSafetyVibrationLevel(void);
uint16_t SYS_RUN_GetSafetyCountdown(void);
float SYS_RUN_GetSafetyTargetSpeed(void);

#endif /* __SYS_RUN_H */
