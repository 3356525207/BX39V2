/* ============================================================
 * step_counter.c — 跑步机计步器实现
 *
 * 算法：自适应基线 + 状态机脉冲/凹陷检测
 *
 * 信号链：
 *   uint16 val ─→ [极性翻转] ─→ 指数基线跟踪 ─→ AC波动提取 ─→
 *   低通平滑 ─→ 自适应阈值 + DIP持续确认 ─→ 步数累计
 *
 * 状态机：
 *   IDLE ─(波动<自适应阈值,持续N ticks)─→ DIP
 *   DIP  ─(波动恢复>迟滞带)───────────→ 计步 → COOLDOWN → IDLE
 *   DIP  ─(超时)──────────────────────→ 误检 → IDLE
 *
 * 抗噪机制：
 *   1. 可调波动平滑系数 (fluct_lpf_alpha, 默认0.02)
 *   2. DIP 持续确认 — 须连续 N ticks 低于阈值才触发 (默认50=5ms)
 *   3. 自适应噪声地板 — IDLE 期间跟踪环境噪声, 动态抬高阈值
 *
 * 调用频率：恒定（默认 10kHz = 100us 周期）
 * ============================================================ */

#include "step_counter.h"

/* ============================================================
 * 内部常量
 * ============================================================ */
#define CONTROL_FREQ_HZ          10000.0f  // 10kHz 控制频率

/* 步频更新周期：每秒更新一次 cadence 估计值 */
#define CADENCE_UPDATE_TICKS     10000u    // 1s @ 10kHz

/* 自适应阈值：噪声地板放大倍数 */
#define NOISE_FLOOR_MULTIPLIER   3.0f      // 有效阈值 ≥ noise_floor × 3
#define NOISE_FLOOR_ALPHA        0.005f    // 噪声地板跟踪速率
#define NOISE_QUIET_RATIO        0.25f     // |波动| < 阈值×此比例 视为静默期

/* ============================================================
 * 静态辅助函数
 * ============================================================ */

static uint16_t average_interval(const uint16_t *buf, uint8_t size, uint8_t valid_mask)
{
    uint16_t sum = 0;
    uint8_t  count = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        if (valid_mask & (1u << i))
        {
            sum += buf[i];
            count++;
        }
    }
    if (count == 0) return 0;
    return sum / count;
}

/* ============================================================
 * 公有函数
 * ============================================================ */

void StepCounter_Init(StepCounter *sc)
{
    if (sc == NULL) return;

    /* ---- 默认参数（适配 IqLPF*100） ---- */
    sc->invert_signal     = 1;
    sc->dip_threshold     = SC_DIP_THRESHOLD_DEFAULT;
    sc->hysteresis        = SC_HYSTERESIS_DEFAULT;
    sc->min_step_ticks    = SC_MIN_STEP_TICKS_DEFAULT;
    sc->max_dip_ticks     = SC_MAX_DIP_TICKS_DEFAULT;
    sc->cooldown_ticks    = SC_COOLDOWN_TICKS_DEFAULT;
    sc->fluct_lpf_alpha   = SC_FLUCT_LPF_ALPHA_DEFAULT;   // 0.02, 比旧版 0.05 更强滤波
    sc->dip_confirm_ticks = SC_DIP_CONFIRM_TICKS_DEFAULT;  // 50 ticks = 5ms

    /* ---- 滤波器状态 ---- */
    sc->baseline        = 0.0f;
    sc->fluctuation     = 0.0f;
    sc->fluctuation_raw = 0.0f;

    /* ---- 状态机 ---- */
    sc->state              = STEP_STATE_IDLE;
    sc->state_timer        = 0;
    sc->step_interval_timer = 0;
    sc->dip_extremum       = 0.0f;
    sc->noise_floor        = 0.0f;
    sc->dip_confirm_counter = 0;

    /* ---- 计步结果 ---- */
    sc->step_count         = 0;
    sc->step_count_session = 0;

    /* ---- 步频估计 ---- */
    sc->interval_idx = 0;
    sc->cadence      = 0;
    for (uint8_t i = 0; i < SC_CADENCE_BUF_SIZE; i++)
        sc->recent_intervals[i] = 0;

    /* ---- 调试/诊断 ---- */
    sc->last_dip_magnitude   = 0.0f;
    sc->total_events_detected = 0;
    sc->signal_valid         = 0;
}

void StepCounter_SetThresholds(StepCounter *sc, float threshold, float hysteresis)
{
    if (sc == NULL) return;
    sc->dip_threshold = threshold;
    sc->hysteresis    = hysteresis;
}

/**
 * @brief  计算当前有效检测阈值（配置值 vs 自适应噪声地板，取大者）
 */
static float effective_threshold(const StepCounter *sc)
{
    float adaptive = sc->noise_floor * NOISE_FLOOR_MULTIPLIER;
    return (adaptive > sc->dip_threshold) ? adaptive : sc->dip_threshold;
}

uint8_t StepCounter_Update(StepCounter *sc, uint16_t val)
{
    uint8_t step_detected = 0;

    if (sc == NULL) return 0;

    /* ---- 信号有效性判断 ---- */
    if (val > 0)
        sc->signal_valid = 1;

    if (val == 0)
    {
        if (sc->step_interval_timer > SC_NODATA_TIMEOUT_TICKS)
        {
            sc->signal_valid = 0;
            sc->baseline     = 0.0f;
            sc->state        = STEP_STATE_IDLE;
            sc->state_timer  = 0;
            sc->noise_floor  = 0.0f;
        }
        sc->step_interval_timer++;
        return 0;
    }

    if (!sc->signal_valid)
    {
        sc->baseline    = (float)val;
        sc->signal_valid = 1;
        sc->noise_floor  = 0.0f;
    }

    /* ============================================================
     * 第1步：基线跟踪
     * ============================================================ */
    sc->baseline += SC_BASELINE_ALPHA * ((float)val - sc->baseline);

    /* ============================================================
     * 第2步：波动计算 + 极性适配
     * ============================================================ */
    if (sc->invert_signal)
        sc->fluctuation_raw = sc->baseline - (float)val;
    else
        sc->fluctuation_raw = (float)val - sc->baseline;

    /* 可调强度的低通平滑 (fluct_lpf_alpha 越小滤波越强) */
    sc->fluctuation += sc->fluct_lpf_alpha * (sc->fluctuation_raw - sc->fluctuation);

    /* ============================================================
     * 第3步：状态机步态检测
     * ============================================================ */
    sc->step_interval_timer++;
    sc->state_timer++;

    switch (sc->state)
    {
    /* ---- IDLE：等待踏步造成的脉冲/凹陷 ---- */
    case STEP_STATE_IDLE:
    {
        float threshold = effective_threshold(sc);

        if (sc->fluctuation < -threshold &&
            sc->step_interval_timer >= sc->min_step_ticks)
        {
            /*
             * 持续确认机制：波动必须连续 N ticks 低于阈值
             * 才判定为真实踏步。噪声尖峰通常只持续 1~3 ticks，
             * 无法满足 dip_confirm_ticks (默认 50 = 5ms)，
             * 因此被自动过滤。
             */
            sc->dip_confirm_counter++;

            if (sc->dip_confirm_counter >= sc->dip_confirm_ticks)
            {
                /* 确认！进入 DIP 状态 */
                sc->state              = STEP_STATE_DIP;
                sc->state_timer        = 0;
                sc->dip_extremum       = sc->fluctuation;
                sc->dip_confirm_counter = 0;
            }
        }
        else
        {
            /* 波动回到阈值以上 → 复位确认计数器 */
            sc->dip_confirm_counter = 0;

            /*
             * 自适应噪声地板：仅在静默期跟踪环境噪声。
             * |波动| < 阈值×0.25 时视为静默 → 跟踪噪声
             * |波动| ≥ 阈值×0.25 时视为有踏步 → 噪声地板缓慢衰减
             * 避免踏步幅度被误认为噪声导致阈值错误抬高。
             */
            float abs_fluct = (sc->fluctuation < 0.0f) ? -sc->fluctuation : sc->fluctuation;
            if (abs_fluct < sc->dip_threshold * NOISE_QUIET_RATIO)
            {
                /* 静默期：跟踪环境噪声 */
                sc->noise_floor += NOISE_FLOOR_ALPHA * (abs_fluct - sc->noise_floor);
            }
            else
            {
                /* 有信号活动：缓慢衰减噪声地板，避免虚高 */
                sc->noise_floor *= 0.999f;
            }
        }
    }
    break;

    /* ---- DIP：处于脉冲/凹陷中，等待恢复 ---- */
    case STEP_STATE_DIP:
    {
        if (sc->fluctuation < sc->dip_extremum)
            sc->dip_extremum = sc->fluctuation;

        if (sc->fluctuation > -sc->hysteresis)
        {
            /* ===== 计步！===== */
            sc->step_count++;
            sc->step_count_session++;
            sc->last_dip_magnitude = -sc->dip_extremum;
            sc->total_events_detected++;

            sc->recent_intervals[sc->interval_idx] = sc->step_interval_timer;
            sc->interval_idx = (sc->interval_idx + 1) % SC_CADENCE_BUF_SIZE;

            sc->state              = STEP_STATE_COOLDOWN;
            sc->state_timer        = 0;
            sc->step_interval_timer = 0;
            sc->dip_extremum       = 0.0f;
            sc->dip_confirm_counter = 0;

            step_detected = 1;
        }
        else if (sc->state_timer > sc->max_dip_ticks)
        {
            /* 超时 → 误触发，回 IDLE */
            sc->state              = STEP_STATE_IDLE;
            sc->state_timer        = 0;
            sc->dip_extremum       = 0.0f;
            sc->dip_confirm_counter = 0;
        }
    }
    break;

    /* ---- COOLDOWN：冷却期，防连击 ---- */
    case STEP_STATE_COOLDOWN:
    {
        if (sc->state_timer >= sc->cooldown_ticks)
        {
            sc->state       = STEP_STATE_IDLE;
            sc->state_timer = 0;
        }
    }
    break;

    default:
        sc->state = STEP_STATE_IDLE;
        break;
    }

    /* ============================================================
     * 第4步：步频估计（每秒更新一次）
     * ============================================================ */
    {
        static uint16_t cadence_timer = 0;
        cadence_timer++;

        if (cadence_timer >= CADENCE_UPDATE_TICKS)
        {
            cadence_timer = 0;
            uint16_t avg = average_interval(sc->recent_intervals,
                                            SC_CADENCE_BUF_SIZE, 0xFF);
            sc->cadence = (avg > 0) ? (600000u / avg) : 0;
        }
    }

    return step_detected;
}

/* ============================================================
 * 查询接口
 * ============================================================ */

uint16_t StepCounter_GetCount(const StepCounter *sc)
{
    return (sc != NULL) ? sc->step_count : 0;
}

uint16_t StepCounter_GetSessionCount(const StepCounter *sc)
{
    return (sc != NULL) ? sc->step_count_session : 0;
}

uint16_t StepCounter_GetCadence(const StepCounter *sc)
{
    return (sc != NULL) ? sc->cadence : 0;
}

float StepCounter_GetLastDip(const StepCounter *sc)
{
    return (sc != NULL) ? sc->last_dip_magnitude : 0.0f;
}

float StepCounter_GetFluctuation(const StepCounter *sc)
{
    return (sc != NULL) ? sc->fluctuation : 0.0f;
}

float StepCounter_GetBaseline(const StepCounter *sc)
{
    return (sc != NULL) ? sc->baseline : 0.0f;
}

/* ============================================================
 * 复位接口
 * ============================================================ */

void StepCounter_ResetSession(StepCounter *sc)
{
    if (sc == NULL) return;
    sc->step_count_session = 0;
}

void StepCounter_ResetAll(StepCounter *sc)
{
    if (sc == NULL) return;
    StepCounter_Init(sc);
}
