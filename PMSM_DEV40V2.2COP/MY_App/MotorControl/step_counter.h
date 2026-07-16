/* ============================================================
 * step_counter.h — 跑步机计步器（基于负载/速度波动检测）
 *
 * 原理：人在跑步机上踏步时，每步落地瞬间会增加电机负载，
 *       导致 Iq 电流短时上升（或速度短时下降）；抬脚后恢复。
 *       通过检测这种周期性波动实现计步。
 *
 * 支持两种数据源：
 *   - IqLPF（推荐）：踏步 → 电流脉冲 ↑，需设置 .invert_signal = 1
 *   - speedLPF：    踏步 → 速度凹陷 ↓，需设置 .invert_signal = 0
 *
 * 调用频率：需恒定（默认适配 10kHz 控制中断）
 * 数据格式：uint16，建议 IqLPF * 100（10mA 分辨率）
 * ============================================================ */

#ifndef __STEP_COUNTER_H
#define __STEP_COUNTER_H

#include "at32m412_416_wk_config.h"

/* ============================================================
 * 计步状态机状态定义
 * ============================================================ */
#define STEP_STATE_IDLE       0u  // 空闲：等待下一次踏步
#define STEP_STATE_DIP        1u  // 脉冲/凹陷：检测到踏步落地
#define STEP_STATE_COOLDOWN   2u  // 冷却期：计步后短暂禁用检测

/* ============================================================
 * 默认配置参数（可通过 struct 字段运行时覆盖）
 * ============================================================ */

/* 信号处理 */
#define SC_BASELINE_ALPHA       0.0005f   // 基线跟踪 LPF 系数（τ≈200ms@10kHz）
#define SC_FLUCT_LPF_ALPHA      0.05f     // 波动信号平滑系数（抑制高频噪声）

/*
 * 步态检测阈值 — 需根据实际信号标定！
 *
 * 标定方法：
 *   1. 跑步机恒速运行，人正常踏步
 *   2. 用 VOFA/串口 观察 fluctuation_raw 波形
 *   3. 取踏步脉冲峰值的 1/3 ~ 1/2 作为 dip_threshold
 *   4. hysteresis 设为 dip_threshold 的 1/6 ~ 1/4
 *
 * 参考值（IqLPF * 100，即 10mA/bit）：
 *   空载 Iq≈50-100，跑步 Iq≈200-500，踏步脉冲≈20-80
 *   dip_threshold 建议 15-30
 */
#define SC_DIP_THRESHOLD_DEFAULT    70.0f   // 脉冲超过此阈值 → 判定踏步落地
#define SC_HYSTERESIS_DEFAULT        5.0f   // 恢复迟滞：回到基线附近才计数

/* 时序约束（单位：调用周期，默认匹配 10kHz） */
#define SC_MIN_STEP_TICKS_DEFAULT    1200u   // 最小步间隔 ≈120ms（500步/分钟上限）
#define SC_MAX_DIP_TICKS_DEFAULT     4000u   // 最大脉冲持续时间 ≈400ms（超时复位）
#define SC_COOLDOWN_TICKS_DEFAULT     6000u   // 冷却期 ≈60ms（防连击）
#define SC_FLUCT_LPF_ALPHA_DEFAULT  0.02f    // 波动平滑系数（默认）
#define SC_DIP_CONFIRM_TICKS_DEFAULT 80u     // DIP确认延时（5ms@10kHz）
#define SC_NODATA_TIMEOUT_TICKS     30000u   // 无步超时 ≈3s（自动重置基线）

/* 步频估计 */
#define SC_CADENCE_BUF_SIZE           8u     // 步频计算窗口（最近 N 步）

/* ============================================================
 * 计步器结构体
 * ============================================================ */
typedef struct
{
    /* ---- 可调参数（初始化时设默认值，可运行时修改） ---- */
    uint8_t  invert_signal;          // 0=速度模式（踏步→凹陷↓）
                                     // 1=Iq模式（踏步→脉冲↑，内部翻转）
    float    dip_threshold;          // 脉冲/凹陷检测阈值
    float    hysteresis;             // 恢复迟滞带宽度
    uint16_t min_step_ticks;         // 最小步间隔（调用周期数）
    uint16_t max_dip_ticks;          // 最大脉冲持续时间（超时复位）
    uint16_t cooldown_ticks;         // 计步后冷却期
    float    fluct_lpf_alpha;        // 波动平滑系数（越小滤波越强，默认0.02）
    uint16_t dip_confirm_ticks;      // DIP 持续确认周期数（默认50=5ms，抗噪声尖峰）

    /* ---- 滤波器状态 ---- */
    float    baseline;               // 信号基线（DC 分量，长时指数平滑）
    float    fluctuation;            // 当前波动值（AC 分量，已平滑）
    float    fluctuation_raw;        // 原始波动（未平滑，调试/标定用）

    /* ---- 状态机 ---- */
    uint8_t  state;                  // 当前状态（IDLE / DIP / COOLDOWN）
    uint16_t state_timer;            // 当前状态持续周期数
    float    noise_floor;             // 自适应噪声地板（IDLE期间跟踪）
    uint16_t dip_confirm_counter;    // DIP 确认计数器
    uint16_t step_interval_timer;    // 距上一步的周期数（用于最小间隔判断）
    float    dip_extremum;           // 当前脉冲/凹陷的极值

    /* ---- 计步结果 ---- */
    uint16_t step_count;             // 累计总步数
    uint16_t step_count_session;     // 本次会话步数（可复位）

    /* ---- 步频估计 ---- */
    uint16_t recent_intervals[SC_CADENCE_BUF_SIZE]; // 最近步间隔环形缓冲
    uint8_t  interval_idx;           // 环形缓冲写指针
    uint16_t cadence;                // 当前步频（步/分钟，每秒更新）

    /* ---- 调试/诊断 ---- */
    float    last_dip_magnitude;     // 最近一次脉冲/凹陷幅度（用于标定）
    uint16_t total_events_detected;  // 检测到的脉冲总数（含无效）
    uint8_t  signal_valid;           // 信号有效标志

} StepCounter;

/* 全局计步器实例（定义于 global_control.c，可直接用于寄存器上报） */
extern StepCounter g_stepCounter;

/* ============================================================
 * API 函数声明
 * ============================================================ */

/**
 * @brief  初始化计步器，加载默认参数并清零状态
 * @param  sc  计步器指针
 */
void StepCounter_Init(StepCounter *sc);

/**
 * @brief  喂入数据并执行计步检测（每个控制周期调用）
 *
 *         典型用法：
 *           // Iq 模式
 *           uint16_t val = (uint16_t)(FocStruct.IqLPF * 100.0f);
 *           StepCounter_Update(&g_step, val);
 *
 *           // 速度模式
 *           uint16_t val = (uint16_t)FocStruct.speedLPF;
 *           StepCounter_Update(&g_step, val);
 *
 * @param  sc   计步器指针
 * @param  val  当前数据值（uint16），IqLPF*100 或 speedLPF
 * @return 本周期是否检测到新步（1=有新步，0=无）
 */
uint8_t StepCounter_Update(StepCounter *sc, uint16_t val);

/**
 * @brief  运行时修改检测阈值（用于动态标定）
 * @param  sc        计步器指针
 * @param  threshold 新阈值
 * @param  hysteresis 新迟滞值
 */
void StepCounter_SetThresholds(StepCounter *sc, float threshold, float hysteresis);

/* ---- 查询接口 ---- */
uint16_t StepCounter_GetCount(const StepCounter *sc);
uint16_t StepCounter_GetSessionCount(const StepCounter *sc);
uint16_t StepCounter_GetCadence(const StepCounter *sc);
float    StepCounter_GetLastDip(const StepCounter *sc);
float    StepCounter_GetFluctuation(const StepCounter *sc);   // 当前平滑波动值（VOFA 调试用）
float    StepCounter_GetBaseline(const StepCounter *sc);      // 当前基线值（VOFA 调试用）

/* ---- 复位接口 ---- */
void StepCounter_ResetSession(StepCounter *sc);
void StepCounter_ResetAll(StepCounter *sc);

#endif /* __STEP_COUNTER_H */
