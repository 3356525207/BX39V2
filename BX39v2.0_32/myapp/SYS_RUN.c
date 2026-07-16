#include "SYS_RUN.h"
#include "RCV315.h"
#include "aip1640.h"
#include "beep.h"
#include "addr_store.h"
#include "uart_comm.h"
#include "ctrl_tx.h"
#include "bt_transparent.h"
#include <string.h>

/*==============================================================================
 * 全局变量
 *============================================================================*/
uint8_t  sys_state       = SYS_STATE_STANDBY;
uint16_t sys_run_time    = 0;
uint16_t sys_steps       = 0;
float    g_distance_miles = 0.0f;
uint16_t g_countdown     = 3;
uint16_t g_session_id    = 0;
uint16_t g_calories      = 0;
float    g_target_speed  = 1.0f;   /* FW-6 §6.4：目标速度记忆，独立上报 targetSpeed */
volatile uint8_t g_telem_now = 0;  /* 状态切换即时遥测请求标志（EnterState 置位，主循环发送后清零） */

/*==============================================================================
 * 模块内部静态变量
 *============================================================================*/
static RunDispMode_t s_run_disp = RUN_DISP_SPEED;
static uint32_t s_state_timer  = 0;
static uint32_t s_phase_timer  = 0;
static uint8_t s_pause_flash_on = 1;
static float   s_ramp_speed     = 0;
static uint32_t s_ramp_timer    = 0;
static uint32_t s_runtime_acc   = 0;
static uint16_t s_step_base     = 0;   /* 会话起始步数基准值 (增量模式) */

#define VIBRATION_LIMIT_MS          300000U
#define VIBRATION_MODE_DISPLAY_MS     5000U

/* IPM 温度分级状态；降温只解除速度上限，不恢复被主动降低的目标速度。 */
static uint8_t  s_temp_confirmed_lvl = 0;
static uint8_t  s_prev_conf_lvl      = 0;

/* 安全卡扣只中断会话，不销毁会话。业务状态保存在上控；下控仅执行急停并等待重发。 */
typedef struct {
    uint8_t     valid;
    SYS_State_t state;
    float       target_speed;
    uint8_t     vibration_level;
    uint16_t    countdown;
    uint32_t    state_timer;
    uint32_t    phase_timer;
} SYS_SafetySnapshot_t;

static SYS_SafetySnapshot_t s_safety_snapshot = {0};
static uint8_t s_paired_addr[2] = {0, 0};
static uint8_t  s_last_key        = 0;
static uint32_t s_key_release_tmr = 0;

/*==============================================================================
 * 外部变量
 *============================================================================*/
extern volatile uint32_t g_ms_tick;
extern uint8_t re_pairing;
extern uint8_t re_pairing_done;
extern void   RCV315_SetAddr(uint8_t addr_high, uint8_t addr_low);

/*==============================================================================
 * RCV315.h 中声明的 extern 变量 (由应用层实现)
 *============================================================================*/
uint16_t Beep_Tem      = 0;
uint8_t  DISPLAY_MOD   = 0;
uint8_t  DSPY_Tem      = 0;
uint8_t  RUN_MOD       = 0;
uint8_t  VIBRATINO_Flag = 0;

/*==============================================================================
 * 内部辅助函数声明
 *============================================================================*/
static void SYS_RUN_HandleKey(uint8_t event, uint8_t raw_key);
/* SYS_RUN_UpdateDisplay 在主循环中调用，见 SYS_RUN.h */
static void SYS_RUN_EnterState(SYS_State_t new_state);
static void seg_show_speed(float speed);
static void seg_show_time(uint16_t total_seconds);
static void seg_show_distance(float distance_miles);
static void seg_show_pause(void);

static float SYS_RUN_GetThermalSpeedMax(void)
{
    switch (s_temp_confirmed_lvl)
    {
    case 3:  return 2.8f;
    case 2:  return 3.0f;
    case 1:  return 3.4f;
    default: return SPEED_MAX_DEF;
    }
}

/*==============================================================================
 * 按键映射
 *============================================================================*/
#define KEY_EVENT_NONE      0
#define KEY_EVENT_START     1
#define KEY_EVENT_SPEED     2
#define KEY_EVENT_MODE      3

static uint8_t key_to_event(uint8_t raw_key)
{
    switch (raw_key)
    {
    case KEY_START:      return KEY_EVENT_START;
    case KEY_SPEED_UP:
    case KEY_SPEED_DOWN: return KEY_EVENT_SPEED;
    case KEY_MODE:       return KEY_EVENT_MODE;
    default:             return KEY_EVENT_NONE;
    }
}

/*==============================================================================
 * SYS_RUN_Init
 *============================================================================*/
void SYS_RUN_Init(void)
{
    RCV315_GetPairedAddr(s_paired_addr);

    sys_state        = SYS_STATE_STANDBY;
    sys_run_time     = 0;
    sys_steps        = 0;
    s_step_base      = 0;
    g_distance_miles = 0.0f;
    g_countdown      = 3;
    g_session_id     = 0;
    g_calories       = 0;
    RCV315_SetVibLevel(1);

    s_run_disp      = RUN_DISP_SPEED;
    s_state_timer   = 0;
    s_phase_timer   = 0;
    s_pause_flash_on = 1;
    s_ramp_speed    = 0;
    s_ramp_timer    = 0;
    s_runtime_acc   = 0;
    s_last_key      = 0;
    memset(&s_safety_snapshot, 0, sizeof(s_safety_snapshot));

    SYS_RUN_UpdateDisplay();
}

/*==============================================================================
 * SYS_RUN_Process — 主状态机 (每 ~50ms 调用)
 *============================================================================*/
void SYS_RUN_Process(void)
{
    uint32_t now = g_ms_tick;
    static uint32_t last_call = 0;
    uint32_t elapsed;

    elapsed = now - last_call;
    if (elapsed == 0) return;
    last_call = now;

    /* 配对模式 */
    if (re_pairing)
    {
        if (RCV315_IsNewData())
        {
            uint8_t raw_key = RCV315_GetKey();
            if (raw_key == KEY_START)
            {
                extern uint8_t re_addr[2];
                RCV315_SetAddr(re_addr[0], re_addr[1]);
                AddrStore_Save(re_addr[0], re_addr[1]);
                re_pairing_done = 1;
                RCV315_GetPairedAddr(s_paired_addr);
            }
            RCV315_ClearFlag();
        }
        return;
    }

    /* 检查遥控新数据 */
    if (RCV315_IsNewData())
    {
        uint8_t addr[2];
        RCV315_GetAddr(addr);

        if (addr[0] == s_paired_addr[0] &&
            addr[1] == s_paired_addr[1])
        {
            uint8_t raw_key = RCV315_GetKey();
            uint8_t event   = key_to_event(raw_key);

            if (raw_key != s_last_key || s_last_key == 0)
            {
                if (event != KEY_EVENT_NONE)
                    SYS_RUN_HandleKey(event, raw_key);
                s_last_key = raw_key;
            }
            s_key_release_tmr = 0;
        }
        RCV315_ClearFlag();
    }
    else
    {
        s_key_release_tmr += elapsed;
        if (s_key_release_tmr >= 120)
        {
            s_last_key = 0;
            s_key_release_tmr = 0;
        }
    }

    s_state_timer += elapsed;

    switch (sys_state)
    {
    case SYS_STATE_COUNTDOWN_START:
        if (s_state_timer >= 1000 && g_countdown > 0)
        {
            g_countdown--;
            s_state_timer -= 1000;
            Beep_Tem = 100;
        }
        if (g_countdown == 0)
            SYS_RUN_EnterState(SYS_STATE_RUNNING);
        break;

    case SYS_STATE_RUNNING:
        {
            /*
             * IPM 分级主动降速：98°C→3.4、100°C→3.0、102°C→2.8 mph。
             * 降温只解除限速门，不恢复被主动降低的速度；恢复正常后由用户
             * 通过加速键或 App setSpeed 手动恢复。105°C 的 E7 停机由下控执行。
             */
            uint8_t temp = UART1_GetIPMTemp();
            uint8_t inst_level;
            float   speed_max;

            /* 0=正常，1=98°C，2=100°C，3=102°C。 */
            if      (temp >= UART_IPM_DERATE_LEVEL3_C) inst_level = 3;
            else if (temp >= UART_IPM_DERATE_LEVEL2_C) inst_level = 2;
            else if (temp >= UART_IPM_DERATE_LEVEL1_C) inst_level = 1;
            else                                       inst_level = 0;

            /* IPM 温度来自下控滤波后的状态帧，达到阈值立即应用对应限速。 */
            s_temp_confirmed_lvl = inst_level;

            /* 等级 1~3 发生跳变时，上报 OVERHEAT_DOWNSPEED。 */
            if (s_temp_confirmed_lvl != s_prev_conf_lvl)
            {
                if (s_temp_confirmed_lvl >= 1)
                    g_evt_overheat_downspeed = 1;
                s_prev_conf_lvl = s_temp_confirmed_lvl;
            }

            speed_max = SYS_RUN_GetThermalSpeedMax();
            RCV315_SetSpeedRange(SPEED_MIN_DEF, speed_max, SPEED_STEP_DEF);

            if (s_temp_confirmed_lvl >= 1)
            {
                /* 主动降速只向下修改当前/目标速度；降温后绝不自动回升。 */
                if (g_target_speed > speed_max || RCV315_GetSpeed() > speed_max)
                {
                    RCV315_SetSpeed(speed_max);
                    g_target_speed = speed_max;
                }
            }

            s_runtime_acc += elapsed;
            if (s_runtime_acc >= 1000)
            {
                sys_run_time++;
                s_runtime_acc -= 1000;
            }

            /* 累加运动距离: 距离(mi) = 速度(mph) * 时间(ms) / 3600000 */
            g_distance_miles += RCV315_GetSpeed() * elapsed / 3600000.0f;

            /* 累加卡路里: 约 100 千卡/英里 */
            g_calories = (uint16_t)(g_distance_miles * 100.0f);

            s_phase_timer += elapsed;
            if (s_phase_timer >= 3000)
            {
                s_phase_timer -= 3000;
                s_run_disp = (RunDispMode_t)((s_run_disp + 1) % 4);
            }
            sys_steps = UART1_GetSteps() - s_step_base;  /* 增量步数 */
        }
        break;

    case SYS_STATE_ESTOP:
        break;

    case SYS_STATE_PAUSED:
        s_ramp_timer += elapsed;
        if (s_ramp_speed > 0.0f)
        {
            if (s_ramp_timer >= 150)
            {
                s_ramp_timer -= 150;
                s_ramp_speed -= SPEED_STEP_DEF;
                if (s_ramp_speed < 0.0f) s_ramp_speed = 0.0f;
            }
        }
        s_phase_timer += elapsed;
        if (s_phase_timer >= 500)
        {
            s_phase_timer -= 500;
            s_pause_flash_on = !s_pause_flash_on;
        }
        break;

    case SYS_STATE_COUNTDOWN_RESUME:
        if (s_state_timer >= 1000 && g_countdown > 0)
        {
            g_countdown--;
            s_state_timer -= 1000;
        }
        if (g_countdown == 0)
        {
            /* FW-6 §6.4：恢复到记忆的 targetSpeed，不再固定 1.0；
             * 实际调速在 SYS_RUN_EnterState(RUNNING) 中执行。 */
            SYS_RUN_EnterState(SYS_STATE_RUNNING);
        }
        break;

    case SYS_STATE_STOPPING:
        s_ramp_timer += elapsed;
        if (s_ramp_speed > 0.0f)
        {
            if (s_ramp_timer >= 150)
            {
                s_ramp_timer -= 150;
                s_ramp_speed -= SPEED_STEP_DEF;
                if (s_ramp_speed < 0.0f) s_ramp_speed = 0.0f;
            }
        }
        else
        {
            /* FW-4：减速到 0 后进入 STOPPED 保持相（不清会话数据） */
            SYS_RUN_EnterState(SYS_STATE_STOPPED);
        }
        break;

    case SYS_STATE_STOPPED:
        /* FW-4 §6.3：停机保持 ≥1s（约 ≥5 条 telemetry）后回到 idle；
         * 会话数据 (sessionId/distance/calories/duration) 保留，直到下次 start。 */
        if (s_state_timer >= 1000)
        {
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
        }
        break;

    case SYS_STATE_VIBRATION:
        /* 律动模式 5 分钟限时：超时自动退回待机。s_phase_timer 仅控制
         * P1~P4 档位提示，切档不会重置 s_state_timer 和总倒计时。 */
        s_phase_timer += elapsed;
        if (s_state_timer >= VIBRATION_LIMIT_MS)
        {
            g_countdown = 0;
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
            Beep_Tem = BEEP_LONG;
        }
        else
        {
            uint32_t remaining_ms = VIBRATION_LIMIT_MS - s_state_timer;
            g_countdown = (uint16_t)((remaining_ms + 999U) / 1000U);
        }
        break;

    case SYS_STATE_STANDBY:
    default:
        break;
    }

    /* SYS_RUN_UpdateDisplay() 移至主循环调用，避免与 aip1640_display() 数据竞争 */
}

/*==============================================================================
 * SYS_RUN_HandleKey
 *============================================================================*/
static void SYS_RUN_HandleKey(uint8_t event, uint8_t raw_key)
{
    switch (sys_state)
    {
    case SYS_STATE_STANDBY:
    case SYS_STATE_STOPPED:
        if (event == KEY_EVENT_START)
        {
            SYS_RUN_EnterState(SYS_STATE_COUNTDOWN_START);
            Beep_Tem = BEEP_SHORT;
        }
        else if (event == KEY_EVENT_MODE)
        {
            SYS_RUN_EnterState(SYS_STATE_VIBRATION);
            Beep_Tem = BEEP_SHORT;
        }
        break;

    case SYS_STATE_COUNTDOWN_START:
        break;

    case SYS_STATE_RUNNING:
        if (event == KEY_EVENT_SPEED)
        {
            if (raw_key == KEY_SPEED_UP)
            {
                if (s_temp_confirmed_lvl != 0)
                {
                    /* 仍处于任一过温等级时禁止回升；温度恢复正常后才允许手动加速。 */
                    Beep_Tem = BEEP_LONG;
                }
                else
                {
                    float spd = RCV315_GetSpeed() + SPEED_STEP_DEF;
                    if (spd > SPEED_MAX_DEF) spd = SPEED_MAX_DEF;
                    RCV315_SetSpeed(spd);
                    g_target_speed = spd;
                    Beep_Tem = (spd == SPEED_MAX_DEF) ? BEEP_LONG : BEEP_SHORT;
                }
            }
            else
            {
                float spd = RCV315_GetSpeed() - SPEED_STEP_DEF;
                if (spd < SPEED_MIN_DEF) spd = SPEED_MIN_DEF;
                RCV315_SetSpeed(spd);
                g_target_speed = spd;
                Beep_Tem = (spd == SPEED_MIN_DEF) ? BEEP_LONG : BEEP_SHORT;
            }
            s_run_disp    = RUN_DISP_SPEED;
            s_phase_timer = 0;
        }
        else if (event == KEY_EVENT_MODE)
        {
            SYS_RUN_EnterState(SYS_STATE_PAUSED);
            Beep_Tem = BEEP_LONG;
        }
        else if (event == KEY_EVENT_START)
        {
            SYS_RUN_EnterState(SYS_STATE_STOPPING);
            Beep_Tem = BEEP_LONG;
        }
        break;

    case SYS_STATE_PAUSED:
        if (event == KEY_EVENT_START)
        {
            SYS_RUN_EnterState(SYS_STATE_COUNTDOWN_RESUME);
            Beep_Tem = BEEP_SHORT;
        }
        break;

    case SYS_STATE_COUNTDOWN_RESUME:
    case SYS_STATE_STOPPING:
        break;

    case SYS_STATE_VIBRATION:
        if (event == KEY_EVENT_START)
        {
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
            Beep_Tem = BEEP_LONG;
        }
        else if (event == KEY_EVENT_MODE)
        {
            uint8_t vib = RCV315_GetVibLevel() + 1;
            if (vib > 4) vib = 1;
            RCV315_SetVibLevel(vib);
            s_phase_timer = 0;
            Beep_Tem = BEEP_SHORT;
        }
        break;

    case SYS_STATE_ESTOP:
        if (event == KEY_EVENT_START)
        {
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
            Beep_Tem = BEEP_LONG;
        }
        break;

    case SYS_STATE_COMM_ERR:
        if (event == KEY_EVENT_START && !g_safety_err)
        {
            if (g_sys_err_code != 0)
            {
                /* E5 可解除；E7 还需先满足 IPM 降温门槛。 */
                Beep_Tem = CTRL_TX_RequestFaultRecovery() ? BEEP_SHORT : BEEP_LONG;
            }
            else
            {
                /* E0 must complete the stop/release ACK handshake in ctrl_tx
                 * before leaving COMM_ERR. A key press must not bypass it and
                 * recreate a display-running/lower-controller-locked split. */
                Beep_Tem = BEEP_LONG;
            }
        }
        break;

    default:
        break;
    }
}

/*==============================================================================
 * 安全卡扣会话快照/恢复
 *============================================================================*/
void SYS_RUN_HandleSafetyRemoved(void)
{
    if (!s_safety_snapshot.valid && sys_state != SYS_STATE_COMM_ERR)
    {
        s_safety_snapshot.valid            = 1;
        s_safety_snapshot.state            = (SYS_State_t)sys_state;
        s_safety_snapshot.target_speed     = g_target_speed;
        s_safety_snapshot.vibration_level  = RCV315_GetVibLevel();
        s_safety_snapshot.countdown        = g_countdown;
        s_safety_snapshot.state_timer      = s_state_timer;
        s_safety_snapshot.phase_timer      = s_phase_timer;
    }

    if (sys_state != SYS_STATE_COMM_ERR)
        SYS_RUN_EnterState(SYS_STATE_COMM_ERR);
    else
        g_telem_now = 1;
}

uint8_t SYS_RUN_IsSafetyInterrupted(void)
{
    return s_safety_snapshot.valid;
}

uint8_t SYS_RUN_SafetySnapshotIsVibration(void)
{
    return (s_safety_snapshot.valid &&
            s_safety_snapshot.state == SYS_STATE_VIBRATION) ? 1U : 0U;
}

uint8_t SYS_RUN_GetSafetyVibrationLevel(void)
{
    return s_safety_snapshot.valid
        ? s_safety_snapshot.vibration_level
        : RCV315_GetVibLevel();
}

uint16_t SYS_RUN_GetSafetyCountdown(void)
{
    return s_safety_snapshot.valid
        ? s_safety_snapshot.countdown
        : g_countdown;
}

float SYS_RUN_GetSafetyTargetSpeed(void)
{
    return s_safety_snapshot.valid
        ? s_safety_snapshot.target_speed
        : g_target_speed;
}

void SYS_RUN_DiscardSafetySnapshot(void)
{
    memset(&s_safety_snapshot, 0, sizeof(s_safety_snapshot));
}

void SYS_RUN_HandleSafetyRestored(void)
{
    SYS_SafetySnapshot_t snapshot;

    if (!s_safety_snapshot.valid)
    {
        SYS_RUN_EnterState(SYS_STATE_STANDBY);
        return;
    }

    snapshot = s_safety_snapshot;
    memset(&s_safety_snapshot, 0, sizeof(s_safety_snapshot));
    g_target_speed = snapshot.target_speed;

    switch (snapshot.state)
    {
    case SYS_STATE_RUNNING:
    case SYS_STATE_COUNTDOWN_START:
    case SYS_STATE_COUNTDOWN_RESUME:
        /* 跑带重新启动必须重新经过 3 秒倒计时；RUNNING 后沿用原目标缓升。 */
        SYS_RUN_EnterState(SYS_STATE_COUNTDOWN_RESUME);
        break;

    case SYS_STATE_PAUSED:
        SYS_RUN_EnterState(SYS_STATE_PAUSED);
        break;

    case SYS_STATE_VIBRATION:
        RCV315_SetVibLevel(snapshot.vibration_level);
        SYS_RUN_EnterState(SYS_STATE_VIBRATION);
        /* 拔卡期间冻结 5 分钟总倒计时；恢复时从原剩余时间继续。 */
        s_state_timer  = snapshot.state_timer;
        s_phase_timer  = snapshot.phase_timer;
        g_countdown    = snapshot.countdown;
        break;

    case SYS_STATE_STOPPING:
        SYS_RUN_EnterState(SYS_STATE_STOPPING);
        break;

    case SYS_STATE_STOPPED:
        SYS_RUN_EnterState(SYS_STATE_STOPPED);
        break;

    case SYS_STATE_ESTOP:
        SYS_RUN_EnterState(SYS_STATE_ESTOP);
        break;

    case SYS_STATE_STANDBY:
    default:
        SYS_RUN_EnterState(SYS_STATE_STANDBY);
        break;
    }
}

void SYS_RUN_HandleSystemFaultRecovered(void)
{
    SYS_RUN_DiscardSafetySnapshot();
    g_target_speed = 1.0f;
    RCV315_SetSpeedRange(SPEED_MIN_DEF, SPEED_MAX_DEF, SPEED_STEP_DEF);
    SYS_RUN_EnterState(SYS_STATE_STANDBY);
    Beep_Tem = BEEP_LONG;
}

/*==============================================================================
 * SYS_RUN_HandleBTCtrl — 蓝牙运行控制, 流程与遥控器一致
 *   cmd: 00停 01启 02暂停 03恢复 04~07震动1~4
 *
 *   本函数仅在 bt_transparent.c 的状态×指令矩阵判定为 ACCEPT 时被调用，
 *   因此这里覆盖矩阵所有 A(接受) 单元格对应的状态迁移 (FW-5 §6.2)。
 *   幂等(OK)/拒绝(REJ)/繁忙(BUSY) 单元格不会进入本函数，只回 ack。
 *============================================================================*/
void SYS_RUN_HandleBTCtrl(uint8_t cmd)
{
    switch (sys_state)
    {
    case SYS_STATE_STANDBY:
    case SYS_STATE_STOPPED:
        /* idle/stopped: start=A(新会话), 律动 mode=A */
        if (cmd == 0x01)
        {
            SYS_RUN_EnterState(SYS_STATE_COUNTDOWN_START);
            Beep_Tem = BEEP_SHORT;
        }
        else if (cmd >= 0x04 && cmd <= 0x07)
        {
            RCV315_SetVibLevel(cmd - 0x03);
            SYS_RUN_EnterState(SYS_STATE_VIBRATION);
            Beep_Tem = BEEP_SHORT;
        }
        break;

    case SYS_STATE_COUNTDOWN_START:   /* starting: stop=A→stopping, pause=A→paused */
    case SYS_STATE_COUNTDOWN_RESUME:  /* resuming: stop=A, pause=A */
        if (cmd == 0x00)
        {
            SYS_RUN_EnterState(SYS_STATE_STOPPING);
            Beep_Tem = BEEP_LONG;
        }
        else if (cmd == 0x02)
        {
            SYS_RUN_EnterState(SYS_STATE_PAUSED);
            Beep_Tem = BEEP_LONG;
        }
        break;

    case SYS_STATE_RUNNING:           /* running: stop=A, pause=A */
        if (cmd == 0x00)
        {
            SYS_RUN_EnterState(SYS_STATE_STOPPING);
            Beep_Tem = BEEP_LONG;
        }
        else if (cmd == 0x02)
        {
            SYS_RUN_EnterState(SYS_STATE_PAUSED);
            Beep_Tem = BEEP_LONG;
        }
        break;

    case SYS_STATE_PAUSED:            /* paused: stop=A, resume=A */
        if (cmd == 0x00)
        {
            SYS_RUN_EnterState(SYS_STATE_STOPPING);
            Beep_Tem = BEEP_LONG;
        }
        else if (cmd == 0x03)
        {
            SYS_RUN_EnterState(SYS_STATE_COUNTDOWN_RESUME);
            Beep_Tem = BEEP_SHORT;
        }
        break;

    case SYS_STATE_VIBRATION:         /* 律动: stop=A→idle, mode=A(仅切档，不重发多帧) */
        if (cmd == 0x00)
        {
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
            Beep_Tem = BEEP_LONG;
        }
        else if (cmd >= 0x04 && cmd <= 0x07)
        {
            RCV315_SetVibLevel(cmd - 0x03);
            s_phase_timer = 0;
            Beep_Tem = BEEP_SHORT;
        }
        break;

    case SYS_STATE_ESTOP:
        /* 过热急停: stop → 退出急停回待机 (保留原有恢复路径; 冷却门在
         *  bt_transparent.c 仍会拦截 start/resume/setSpeed) */
        if (cmd == 0x00)
        {
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
            Beep_Tem = BEEP_LONG;
        }
        break;

    case SYS_STATE_COMM_ERR:
        /* 通信错误/下位机系统错误: stop → 退出回待机 */
        if (cmd == 0x00 && !g_safety_err)
        {
            g_sys_err_code = 0;
            SYS_RUN_EnterState(SYS_STATE_STANDBY);
            Beep_Tem = BEEP_LONG;
        }
        break;

    default:
        break;
    }
}

/*==============================================================================
 * SYS_RUN_EnterState
 *============================================================================*/
static void SYS_RUN_EnterState(SYS_State_t new_state)
{
    sys_state     = new_state;
    s_state_timer = 0;
    s_phase_timer = 0;
    g_telem_now   = 1;   /* 状态切换：请求主循环立即补发一帧 telemetry */

    switch (new_state)
    {
    case SYS_STATE_STANDBY:
        /* FW-4 §6.3：会话数据 (sys_run_time/sys_steps/g_distance_miles/g_calories/
         * g_session_id) 在停机后保留，不在此清零；仅在下次 start(COUNTDOWN_START)
         * 时清零。此处只做与会话无关的复位（震动档位、按键、速度变量）。 */
        RCV315_SetVibLevel(1);
        s_last_key    = 0;
        RCV315_SetSpeedRange(SPEED_MIN_DEF, SPEED_MAX_DEF, SPEED_STEP_DEF);
        RCV315_SetSpeed(SPEED_MIN_DEF);
        g_bt_regs[1] = 10;
        break;

    case SYS_STATE_STOPPED:
        /* FW-4：停机保持相，跑带已停；会话数据保留待上报 */
        RCV315_SetSpeed(0.0f);
        break;

    case SYS_STATE_COUNTDOWN_START:
        g_countdown = 3;
        /* FW-4/FW-6 §6.3/§6.4：从 idle/stopped 启动 = 新会话。
         * sessionId 自增、里程/卡路里/时长清零、targetSpeed 重置 1.0。 */
        g_session_id++;
        sys_run_time     = 0;
        sys_steps        = 0;
        s_step_base      = UART1_GetSteps();  /* 记录会话起始步数基准 */
        g_distance_miles = 0.0f;
        g_calories       = 0;
        s_runtime_acc    = 0;
        g_target_speed   = 1.0f;
        RCV315_SetSpeedRange(SPEED_MIN_DEF, SPEED_MAX_DEF, SPEED_STEP_DEF);
        RCV315_SetSpeed(SPEED_MIN_DEF);
        g_bt_regs[1]     = 10;
        s_temp_confirmed_lvl = 0;
        s_prev_conf_lvl      = 0;
        break;

    case SYS_STATE_RUNNING:
        /* FW-6：进入运行态时应用记忆的目标速度
         *   - 从 COUNTDOWN_START 进入：targetSpeed=1.0 (启动)
         *   - 从 COUNTDOWN_RESUME 进入：targetSpeed=暂停前保留值 (自动恢复) */
        RCV315_SetSpeed(g_target_speed);
        s_run_disp    = RUN_DISP_SPEED;
        s_phase_timer = 0;
        s_runtime_acc = 0;
        /* 温控状态只在新会话重置；暂停/安全卡扣恢复不能丢失过温快照。 */
        break;

    case SYS_STATE_PAUSED:
        s_ramp_speed  = RCV315_GetSpeed();
        s_ramp_timer  = 0;
        s_pause_flash_on = 1;
        break;

    case SYS_STATE_COUNTDOWN_RESUME:
        g_countdown = 3;
        break;

    case SYS_STATE_STOPPING:
        s_ramp_speed = RCV315_GetSpeed();
        s_ramp_timer = 0;
        g_target_speed = 1.0f;   /* FW-6 §6.4：stop 后 targetSpeed 重置 1.0 */
        break;

    case SYS_STATE_VIBRATION:
        g_countdown = 300;   /* 5 分钟限时倒计时 (秒) */
        break;

    case SYS_STATE_ESTOP:
        /* 电机过热急停: 速度立即置零, 蜂鸣器长鸣报警 */
        RCV315_SetSpeed(0.0f);
        g_target_speed = 1.0f;   /* FW-6 §6.4：error 后 targetSpeed 重置 1.0 */
        Beep_Tem = BEEP_LONG;
        break;

    case SYS_STATE_COMM_ERR:
        RCV315_SetSpeed(0.0f);
        g_target_speed = 1.0f;   /* FW-6 §6.4：error 后 targetSpeed 重置 1.0 */
        Beep_Tem = 800;
        break;

    default:
        break;
    }
}

/*==============================================================================
 * SYS_RUN_UpdateDisplay
 *============================================================================*/
void SYS_RUN_UpdateDisplay(void)
{
    uint8_t i;

    /* 非运行态灭掉模式指示灯 */
    display_buffer[5] = SEG_BLANK;

    switch (sys_state)
    {
    case SYS_STATE_STANDBY:
        for (i = 0; i < 5; i++) display_buffer[i] = SEG_DASH;
        break;

    case SYS_STATE_COUNTDOWN_START:
    case SYS_STATE_COUNTDOWN_RESUME:
        memset(display_buffer, 0, 5);
        if (g_countdown > 0) display_buffer[4] = SEG_TABLE[g_countdown];
        break;

    case SYS_STATE_RUNNING:
        switch (s_run_disp)
        {
        case RUN_DISP_SPEED:
            seg_show_speed(RCV315_GetSpeed());
            display_buffer[5] = SEG_BIT_A;    /* a 段 → 速度 */
            break;
        case RUN_DISP_TIME:
            seg_show_time(sys_run_time);
            display_buffer[5] = SEG_BIT_E;    /* e 段 → 时间 */
            break;
        case RUN_DISP_STEPS:
            aip1640_Display_Number5(sys_steps);
            display_buffer[5] = SEG_BIT_F;    /* f 段 → 步数 */
            break;
        case RUN_DISP_DISTANCE:
            seg_show_distance(g_distance_miles);
            display_buffer[5] = SEG_BIT_B;    /* b 段 → 距离 */
            break;
        }
        break;

    case SYS_STATE_PAUSED:
        if (s_pause_flash_on)
            seg_show_pause();
        else
            memset(display_buffer, 0, 5);
        break;

    case SYS_STATE_STOPPING:
        seg_show_speed(s_ramp_speed);
        break;

    case SYS_STATE_STOPPED:
        /* FW-4：停机保持相，显示 0.0 */
        seg_show_speed(0.0f);
        break;

    case SYS_STATE_VIBRATION:
        {
            static const uint8_t vib_led[] = {
                0, SEG_BIT_D, SEG_BIT_H, SEG_BIT_C, SEG_BIT_G
            };
            uint8_t lv = RCV315_GetVibLevel();
            if (lv > 4) lv = 4;

            if (s_phase_timer < VIBRATION_MODE_DISPLAY_MS)
            {
                /* 进入律动或切档后，先显示当前档位 P1~P4 5 秒。 */
                display_buffer[0] = SEG_P;
                display_buffer[1] = SEG_BLANK;
                display_buffer[2] = SEG_BLANK;
                display_buffer[3] = SEG_BLANK;
                display_buffer[4] = SEG_TABLE[lv];
            }
            else
            {
                /* 其余时间显示 5 分钟剩余时间，格式为 MM.SS。 */
                seg_show_time(g_countdown);
            }
            display_buffer[5] = vib_led[lv];
        }
        break;

    case SYS_STATE_ESTOP:
        /* 电机过热急停 → 数码管显示 "___E2" */
        display_buffer[0] = SEG_BLANK;
        display_buffer[1] = SEG_BLANK;
        display_buffer[2] = SEG_BLANK;
        display_buffer[3] = SEG_E;
        display_buffer[4] = SEG_TABLE[2];
        break;

    case SYS_STATE_COMM_ERR:
        if (g_sys_err_code) {
            /* 下位机系统错误 → E5 (堵转) / E6 (校准失败) / E7 (IPM 过温) */
            display_buffer[0] = SEG_BLANK;
            display_buffer[1] = SEG_BLANK;
            display_buffer[2] = SEG_BLANK;
            display_buffer[3] = SEG_E;
            display_buffer[4] = SEG_TABLE[g_sys_err_code];
        } else if (g_safety_err) {
            /* 安全扣脱落 → E17 */
            display_buffer[0] = SEG_BLANK;
            display_buffer[1] = SEG_BLANK;
            display_buffer[2] = SEG_E;
            display_buffer[3] = SEG_TABLE[1];
            display_buffer[4] = SEG_TABLE[7];
        } else {
            /* 通信超时 → E0 */
            display_buffer[0] = SEG_BLANK;
            display_buffer[1] = SEG_BLANK;
            display_buffer[2] = SEG_BLANK;
            display_buffer[3] = SEG_E;
            display_buffer[4] = SEG_TABLE[0];
        }
        break;

    default:
        break;
    }
}

/*==============================================================================
 * 7 段码显示辅助函数
 *============================================================================*/
static void seg_show_speed(float speed)
{
    uint8_t speed_int;
    uint8_t speed_frac;



    uint8_t total_tenths = (uint8_t)(speed * 10.0f + 0.5f);
	
	  if (speed < 0.0f) speed = 0.0f;
    if (speed > 9.9f) speed = 9.9f;
	
    speed_int  = total_tenths / 10;
    speed_frac = total_tenths % 10;

    memset(display_buffer, 0, 5);
    display_buffer[2] = SEG_TABLE[speed_int] | 0x80;
    display_buffer[3] = SEG_TABLE[speed_frac];
}

static void seg_show_time(uint16_t total_seconds)
{
    uint8_t hi, lo;   /* 高位, 低位 */

    memset(display_buffer, 0, 5);

    if (total_seconds < 3600)
    {
        /* 不足 60 分钟: MM.SS */
        uint8_t minutes = total_seconds / 60;
        uint8_t seconds = total_seconds % 60;

        if (minutes >= 10)
        {
            display_buffer[0] = SEG_TABLE[minutes / 10];
            display_buffer[1] = SEG_TABLE[minutes % 10] | 0x80;
            display_buffer[2] = SEG_TABLE[seconds / 10];
            display_buffer[3] = SEG_TABLE[seconds % 10];
        }
        else
        {
            display_buffer[1] = SEG_TABLE[minutes] | 0x80;
            display_buffer[2] = SEG_TABLE[seconds / 10];
            display_buffer[3] = SEG_TABLE[seconds % 10];
        }
    }
    else
    {
        /* 满 60 分钟: HH.MM (最大 99:59) */
        uint8_t hours   = total_seconds / 3600;
        uint8_t minutes = (total_seconds % 3600) / 60;

        if (hours > 99) { hours = 99; minutes = 59; }

        if (hours >= 10)
        {
            display_buffer[0] = SEG_TABLE[hours / 10];
            display_buffer[1] = SEG_TABLE[hours % 10] | 0x80;
            display_buffer[2] = SEG_TABLE[minutes / 10];
            display_buffer[3] = SEG_TABLE[minutes % 10];
        }
        else
        {
            display_buffer[1] = SEG_TABLE[hours] | 0x80;
            display_buffer[2] = SEG_TABLE[minutes / 10];
            display_buffer[3] = SEG_TABLE[minutes % 10];
        }
    }
}

static void seg_show_distance(float distance_miles)
{
    uint16_t hundredths;

    /* 限制最大显示值 999.99 英里 */
    if (distance_miles < 0.0f) distance_miles = 0.0f;
    if (distance_miles > 999.99f) distance_miles = 999.99f;

    hundredths = (uint16_t)(distance_miles * 100.0f + 0.5f);

    memset(display_buffer, 0, 5);

    /* xxx.hh 格式: 3位整数 + 小数点 + 2位小数 */
    display_buffer[0] = SEG_TABLE[(hundredths / 10000) % 10];        /* 百位 */
    display_buffer[1] = SEG_TABLE[(hundredths /  1000) % 10];        /* 十位 */
    display_buffer[2] = SEG_TABLE[(hundredths /   100) % 10] | 0x80; /* 个位 + dp */
    display_buffer[3] = SEG_TABLE[(hundredths /    10) % 10];        /* 十分位 */
    display_buffer[4] = SEG_TABLE[ hundredths          % 10];        /* 百分位 */
}

static void seg_show_pause(void)
{
    display_buffer[0] = SEG_P;
    display_buffer[1] = SEG_A;
    display_buffer[2] = SEG_U;
    display_buffer[3] = SEG_S;
    display_buffer[4] = SEG_E;
}
