/* ============================================================
 * motor_sensoruse.c — PMSM FOC 有感电机控制
 *
 * 有感编码器 FOC 电机控制主状态机。
 * 功能：编码器校准、稳定延时、强制拖动、
 *       位置/速度/电流闭环、振动测试、紧急停止、
 *       Flash 恢复检测、多圈位置跟踪。
 *
 * 控制中断频率：10kHz（100us 周期）
 * ============================================================ */

#include "motor_sensoruse.h"
#include "at32m412_416_wk_config.h"
#include "foc_drv.h"
#include "math_drv.h"
#include "Encoder.h"
#include "arm_math.h"
#include "PID.h"
#include "UART_CAN.h"
#include "flash.h"
#include "kth78xx.h"
#include "vofa.h"
#include "thermistor.h"

/* ============================================================
 * 外部接口全局变量
 * ============================================================ */
SENSORUSE_STRUCT g_Sensoruse = {0};
float   electrical_angle  = 0.0f;
float   Iq_ref_raw;              // 原始 Iq 参考值（未使用，保留兼容）
uint16_t Calibration_Bx39;      // 校准命令（来自寄存器）
uint16_t VIBRATINO_1_flag;     // 振动测试命令（来自寄存器）
uint8_t  g_RecoveryStable = 0;       // 当前启动已标记为稳定（>20s）
volatile uint8_t g_RecoveryWritePending = 0;  // 主循环待执行 flash 写标志
static uint8_t   g_RecoveryFlashDone = 0;      // flash 写已成功完成（防重复触发）
extern float obs_speed;         // 观测速度（来自 ADRC 观测器，global_control.c）

/* ============================================================
 * 校准子状态
 * ============================================================ */
#define CALIB_SUB_ALIGN          0u  // 强制对齐
#define CALIB_SUB_VERIFY         1u  // 验证传感器
#define CALIB_SUB_BEEP           2u  // 通过提示音

/* ============================================================
 * 模块内部状态
 * ============================================================ */
typedef struct
{
    float    Delay;             // 稳定/对齐延时计数器（秒）
    uint8_t  CalibrationEn;     // 0=空闲, 1=校准中, 2=Flash已校验
    uint16_t TimeVibration;     // 振动测试周期计数器
    uint16_t AccelRate;         // 梯形加减速速率
    uint8_t  VibrationMode;     // 振动测试模式（1-4）
    uint8_t  EmergencyFlag;     // 紧急停止激活标志
    uint8_t  EmergencyWasTreadmill; // 急停前为跑带闭环：保留原有斜坡减速
    uint8_t  CalibSubState;     // 校准子状态机
    uint8_t  CalibRetryCount;   // 校准重试计数
    uint16_t CalibStartAngle;   // 验证阶段起始传感器角度
    uint16_t AudioPhase;        // 提示音相位累加器
    uint32_t StallTimer;        // 堵转计时器（10kHz ticks）
} SensoruseIntern;

static SensoruseIntern g_Intern = {
    .AccelRate = ACCEL_RATE_NORMAL,
};

/* ============================================================
 * 静态辅助函数（FOC 管线）
 * ============================================================ */

/** 采样相电流，执行 Clarke/Park 变换，更新低通滤波器 */
static void FOC_SampleAndTransform(void)
{
    Clark_Transform(&FocStruct);   // Clarke 变换
    Pack_Transform(&FocStruct);    // Park 变换
    // 电流低通滤波
    FocStruct.IdLPF = FocStruct.Id * ID_LPF_ALPHA + FocStruct.IdLPF * (1.0f - ID_LPF_ALPHA);
    FocStruct.IqLPF = FocStruct.Iq * IQ_LPF_ALPHA + FocStruct.IqLPF * (1.0f - IQ_LPF_ALPHA);
}

/** 执行逆 Park 变换和 SVPWM 调制 */
static void FOC_Modulate(void)
{
    IPack_Transform(&FocStruct);    // 逆 Park 变换
    Calculate_SVPWM(&FocStruct);    // SVPWM 计算
}

/** 将编码器原始角度归一到 [0, 4095) 范围 */
static float normalize_encoder_angle(float raw)
{
    while (raw < 0.0f)            raw += (float)ENCODER_RESOLUTION;
    while (raw >= (float)ENCODER_RESOLUTION) raw -= (float)ENCODER_RESOLUTION;
    return raw;
}

/* ============================================================
 * 公有函数
 * ============================================================ */

/**
 * @brief  恢复模式检测：上电时检查快速启动计数，连续多次快速断电则清除校准标志
 * @note   必须在 Factory_Calibration_Strong 之前调用
 */
void Recovery_Check(void)
{
    flash_read(TEST_FLASH_ADDRESS_START, buffer_read, TEST_BUFEER_SIZE);

    uint16_t recovery_count = buffer_read[FLASH_RECOVERY_COUNT_OFFSET];
    uint16_t stable_flag    = buffer_read[FLASH_RECOVERY_STABLE_OFFSET];

    /* 擦除态（0xFFFF）归一化为 0 */
    if (recovery_count == 0xFFFF) recovery_count = 0;
    if (stable_flag    == 0xFFFF) stable_flag    = 0;

    /* 上次启动已稳定运行超过20s，复位快速启动计数 */
    if (stable_flag == RECOVERY_STABLE_MAGIC)
    {
        recovery_count = 0;
    }
    else
    {
        recovery_count++;
    }

    /* 先读取完整 Flash 缓冲区，避免写回时清零其他字段 */
    flash_read(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);

    /* 达到恢复阈值：清除校准标志，强制重新校准 */
    if (recovery_count >= RECOVERY_THRESHOLD)
    {
        buffer_write[FLASH_CALIB_FLAG_OFFSET]  = 0x0000;  // 清除校准完成标志
        buffer_write[FLASH_CALIB_VALUE_OFFSET] = 0x0000;  // 清除校准偏置值
        recovery_count = 0;
    }

    /* 更新恢复计数，标记当前启动尚未稳定 */
    buffer_write[FLASH_RECOVERY_COUNT_OFFSET]  = recovery_count;
    buffer_write[FLASH_RECOVERY_STABLE_OFFSET] = 0x0000;
    flash_write(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);

    g_RecoveryStable = 0;
}

/**
 * @brief  标记当前启动已稳定运行超过20s，复位快速启动计数
 */
void Recovery_MarkStable(void)
{
    if (g_RecoveryFlashDone) return;  // Flash 已写过，跳过（防重复擦除）

    flash_read(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);
    buffer_write[FLASH_RECOVERY_COUNT_OFFSET]  = 0;
    buffer_write[FLASH_RECOVERY_STABLE_OFFSET] = RECOVERY_STABLE_MAGIC;
    flash_write(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);

    g_RecoveryFlashDone = 1;
}

/**
 * @brief  主 FOC 状态机 — 在 10kHz 控制中断中调用
 */
void Sensoruse_Control(void)
{
    /* 恢复模式计时：累计10kHz周期，20s后标记稳定
     *
     * 逻辑稳定性标记（g_RecoveryStable）在 ISR 内即时置位（RAM 操作）。
     *
     * Flash 持久化标记（Recovery_MarkStable → flash_write）推迟到
     * Taspeed ≤ 阈值时执行。用 Taspeed 而非 RunMode 判断电机是否停转：
     * 用户"软停机"（设 g_speed=0）时 RunMode 保持 POS_SPEED_CURRENT_LOOP
     * 不变，但 Taspeed 已斜坡降到 0，此时 flash 写安全无顿挫。 */
    {
        static uint32_t recovery_timer = 0;
        if (!g_RecoveryStable)
        {
            if (recovery_timer < STABLE_TIMEOUT_TICKS)
                recovery_timer++;

            if (recovery_timer >= STABLE_TIMEOUT_TICKS)
            {
                g_RecoveryStable = 1;

                if (FocStruct.Taspeed <= PWM_SPEED_THRESHOLD)
                {
                    g_RecoveryWritePending = 1;
                }
            }
        }

        /* 若稳定性已判定但 flash 写因电机运行被推迟，
         * Taspeed 降到阈值以下后自动触发 */
        if (g_RecoveryStable && !g_RecoveryFlashDone && g_RecoveryWritePending == 0)
        {
            if (FocStruct.Taspeed <= PWM_SPEED_THRESHOLD)
            {
                g_RecoveryWritePending = 1;
            }
        }
    }

    Calculate_Sin_Cos((s32)update_electrical_angle(), &FocStruct.SinVal, &FocStruct.CosVal);

    /* ================================================================
     * 堵转保护：梯形斜坡目标速度高于阈值但编码器实测速度持续低于阈值 → 紧急停机
     *
     * 使用编码器低通滤波速度（speedLPF × 5000 → RPM）替代 ADRC 观测器速度，
     * 避免因 ESO 观测器滞后/参数失配导致的正常运行误触发 E5。
     *
     * 注意：使用 FocStruct.Taspeed（梯形斜坡当前值）而非 g_Sensoruse.g_speed
     * （用户瞬时指令），避免加速阶段误判为堵转。
     * Taspeed 从 0 斜坡上升，到达阈值后才启动堵转计时，
     * 电机有充裕时间在计时超限前加速越过阈值。
     * 仅在闭环运行模式下生效（位置/速度/电流闭环 & 振动测试）
     * ================================================================ */
    if (g_Sensoruse.RunMode == POS_SPEED_CURRENT_LOOP ||
        g_Sensoruse.RunMode == VIBRATINO_1)
    {
        /* speedLPF 为编码器角度差分值（未除 dt），乘以 5000 还原为 RPM */
        float encoder_speed_rpm = FocStruct.speedLPF * 5000.0f;
        if (FocStruct.Taspeed > STALL_SPEED_THRESHOLD_RPM &&
            encoder_speed_rpm < STALL_SPEED_THRESHOLD_RPM)
        {
            g_Intern.StallTimer++;
            if (g_Intern.StallTimer >= STALL_COUNT_THRESHOLD)
            {
                g_Sensoruse.RunMode = EMERGENCY_STOP;
                g_Intern.StallTimer = 0;
                g_Sensoruse.SYS_error = 5;   // 堵转错误代码，通过统一通道通知主机
            }
        }
        else
        {
            g_Intern.StallTimer = 0;
        }
    }
    else
    {
        g_Intern.StallTimer = 0;
    }

    switch (g_Sensoruse.RunMode)
    {
    /* ----- 编码器校准 ----- */
    case ENCODER_CALIB:
        Factory_Calibration_Strong();
        break;

    /* ----- 稳定延时（等待传感器信号稳定） ----- */
    case DELAY_COUNT:
        g_Intern.Delay += CONTROL_PERIOD_S;
        FocStruct.Ud = 0.0f;
        FocStruct.Uq = 0.0f;

        FOC_SampleAndTransform();
        CurrentLoop_Init();                        // 初始化电流环 PID
        FocStruct.Iq_ref  = 0.0f;
        FocStruct.Taspeed = 0.0f;

        if (g_Intern.Delay >= DELAY_STABILIZE_S)
        {
            g_Intern.Delay = 0.0f;
            g_Sensoruse.RunMode = POS_SPEED_CURRENT_LOOP;
        }
        FOC_Modulate();
        break;

    /* ----- 强制拖动（V/F 开环） ----- */
    case FORCE_DRAG:
        Calculate_Sin_Cos((s32)Gat_VFsingle_float(), &FocStruct.SinVal, &FocStruct.CosVal);
        FocStruct.Ud = FORCE_DRAG_VOLTAGE;
        FocStruct.Uq = 0.0f;

        Clark_Transform(&FocStruct);   // Clarke 变换
        Pack_Transform(&FocStruct);    // Park 变换
        FOC_Modulate();
        break;

    /* ----- 位置/速度/电流闭环 ----- */
    case POS_SPEED_CURRENT_LOOP:
    {
        static uint8_t pwm_was_off = 1;  /* 记录上一周期 PWM 是否关闭 */

        Trapezoidal_Acceleration(g_Sensoruse.g_speed, (float)g_Intern.AccelRate,
                                 &FocStruct.Taspeed, TRAPEZOIDAL_DT);

        if (FocStruct.Taspeed <= PWM_SPEED_THRESHOLD)
        {
            /* 低于速度阈值：关闭 PWM 输出 */
            FocStruct.DutyCycleA = 0;
            FocStruct.DutyCycleB = 0;
            FocStruct.DutyCycleC = 0;
            pwm_was_off = 1;
        }
        else
        {
            /*
             * 冷启动预加载：当 PWM 从关闭→开启跨越阈值时，
             * IqLPF 可能冻结在 0（首次上电）或旧值（上次运行残留）。
             * 用当前 Iq_ref 预初始化 IqLPF，避免电流环初始误差
             * 过大导致 Uq 突变 → 电机顿挫。
             */
            if (pwm_was_off)
            {
                FocStruct.IqLPF = FocStruct.Iq_ref;
                FocStruct.IdLPF = 0.0f;
                pwm_was_off = 0;
            }

            FOC_SampleAndTransform();
            // D轴电流PI控制，目标为0
            FocStruct.Ud = PID_Update(&pid_id, 0.0f, FocStruct.IdLPF, PID_DT);
            // Q轴电流PI控制
            FocStruct.Uq = PID_Update(&pid_iq, FocStruct.Iq_ref, FocStruct.IqLPF, PID_DT);
            FOC_Modulate();
        }
        break;
    }

    /* ----- 振动测试：初始化 ----- */
    case VIBRATINO_SATART:
        PID_Init(&pid_speed, 0.05f, 0.01f, 0.0f, 5.5f, 1.0f);
        g_Sensoruse.RunMode = VIBRATINO_1;
        break;

    /* ----- 振动测试：运行 ----- */
    case VIBRATINO_1:
    {
        g_Intern.TimeVibration++;

        uint16_t duration = VIB_DURATION_MODE1;
        if      (g_Intern.VibrationMode == 2) duration = VIB_DURATION_MODE2;
        else if (g_Intern.VibrationMode == 3) duration = VIB_DURATION_MODE3;
        else if (g_Intern.VibrationMode == 4) duration = VIB_DURATION_MODE4;

        if (g_Intern.TimeVibration < duration)
            FocStruct.Taposition = VIB_POS_FORWARD;
        else if (g_Intern.TimeVibration < duration * 2)
            FocStruct.Taposition = VIB_POS_REVERSE;
        else
            g_Intern.TimeVibration = 0;

        FOC_SampleAndTransform();
        // D轴电流PI控制，目标为0
        FocStruct.Ud = PID_Update(&pid_id, 0.0f, FocStruct.IdLPF, PID_DT);
        // Q轴电流PI控制
        FocStruct.Uq = PID_Update(&pid_iq, FocStruct.Iq_ref, FocStruct.IqLPF, PID_DT);

        /* 寄存器命令清零时退出振动测试 */
        if (VIBRATINO_1_flag == 0x0000)
            g_Sensoruse.RunMode = DELAY_COUNT;

        FOC_Modulate();
    }
    break;

    /* ----- 紧急停止 ----- */
    case EMERGENCY_STOP:
        FocStruct.Ud = 0.0f;
        FocStruct.Uq = 0.0f;
        FOC_Modulate();
        break;

    /* ----- 暂停/空闲 ----- */
    case PAUSE_STOP:
    default:
        break;
    }
}

/**
 * @brief  电压限制圆法 SVPWM 过调制保护
 *         当电压矢量幅值超过 vbus/√3 时，等比例缩放 Ud/Uq
 * @param  ud_in  D轴电压（输入/输出）
 * @param  uq_in  Q轴电压（输入/输出）
 * @param  vbus   直流母线电压 (V)
 */
void Voltage_DQLimit(float *ud_in, float *uq_in, float vbus)
{
    const float SQRT3 = 1.7320508f;
    float v_max    = vbus / SQRT3;   // SVPWM 最大线性输出电压幅值
    float u_sq     = (*ud_in) * (*ud_in) + (*uq_in) * (*uq_in);
    float v_max_sq = v_max * v_max;

    if (u_sq > v_max_sq)
    {
        float scale = v_max / sqrtf(u_sq);
        *ud_in *= scale;
        *uq_in *= scale;
    }
}

/**
 * @brief  初始化有感运行状态及 FOC 默认参数
 */
void Sensoruse_Init(void)
{
    /* 清零内部状态 */
    g_Intern.Delay          = 0.0f;
    g_Intern.CalibrationEn  = 0;
    g_Intern.TimeVibration  = 0;
    g_Intern.AccelRate      = ACCEL_RATE_NORMAL;
    g_Intern.VibrationMode  = 0;
    g_Intern.EmergencyFlag  = 0;
    g_Intern.EmergencyWasTreadmill = 0;
    g_Intern.CalibSubState  = 0;
    g_Intern.CalibRetryCount = 0;
    g_Intern.CalibStartAngle = 0;
    g_Intern.AudioPhase     = 0;
    g_Intern.StallTimer     = 0;

    /* 初始化有感结构体 */
    g_Sensoruse.CalibFlag       = 0;
    g_Sensoruse.CalibOffset     = 0.0f;
    g_Sensoruse.RunMode         = 0x00;
    g_Sensoruse.Iq_ref_last     = 0.0f;  // 初始化斜率限制器状态

    /* 初始化 FOC 默认参数 */
    FocStruct.PwmCycle  = PWM_CYCLE_DEFAULT;
    FocStruct.PwmLimit  = PWM_LIMIT_DEFAULT;
    FocStruct.Ud        = 0.0f;
    FocStruct.Uq        = 0.0f;
    FocStruct.Ubus      = DEFAULT_BUS_VOLTAGE;
}

/**
 * @brief  读取编码器角度并计算电角度
 * @return 电角度，单位为编码器计数值 [0, 4095)
 */
float update_electrical_angle(void)
{
    uint16_t angle_raw = KTH78_ReadAngle() / 16;
    FocStruct.MCangle    = (float)angle_raw;
    FocStruct.anglefloat = (float)angle_raw / (float)ENCODER_RESOLUTION * 2.0f * PI;

    // 电角度 = 极对数 * (当前角度 - 校准偏置)
    float elec = (float)POLE_PAIRS * (float)((int32_t)angle_raw - (int32_t)g_Sensoruse.CalibOffset_flash);
    electrical_angle = normalize_encoder_angle(elec);
    FocStruct.ElectricalVal = electrical_angle;

    return electrical_angle;
}

/**
 * @brief  根据编码器角度差分计算原始速度
 *         处理 0 ↔ 2π 边界处的过零跳变
 * @param  angle  当前编码器角度 (弧度, 0 到 2π)
 * @param  dt     距上次更新的时间间隔 (秒)
 * @return 原始速度（等效 RPM，未缩放）
 */
float Calculate_velocity_raw(float angle, float dt)
{
    if (dt <= 0.0f)
        return 0.0f;

    float delta = angle - FocStruct.last_anglefloat;
    float velocity_raw;

    /* 过零检测：角度变化超过 80% 满圈则视为过零 */
    if (fabsf(delta) > (0.8f * 2.0f * PI))
    {
        if (delta < 0.0f)
            velocity_raw = (2.0f * PI - FocStruct.last_anglefloat + angle);  // 正转
        else
            velocity_raw = -(2.0f * PI - angle + FocStruct.last_anglefloat); // 反转
    }
    else
    {
        velocity_raw = delta * 9.5493f; /* rad/s 转 RPM: 60 / (2π) ≈ 9.5493 */
    }

    FocStruct.last_anglefloat = angle;
    return velocity_raw;
}

/**
 * @brief  梯形加减速曲线
 *         以恒定加速度将当前速度向目标速度斜坡逼近
 * @param  target_speed   目标速度
 * @param  acceleration   加速度 (units/s²)
 * @param  current_speed  当前速度（输入/输出，需保持跨调用持久）
 * @param  dt             时间步长 (秒)
 */
void Trapezoidal_Acceleration(float target_speed, float acceleration,
                               float *current_speed, float dt)
{
    if (*current_speed < target_speed)
    {
        *current_speed += acceleration * dt;
        if (*current_speed > target_speed)
            *current_speed = target_speed;
    }
    else if (*current_speed > target_speed)
    {
        *current_speed -= acceleration * dt;
        if (*current_speed < target_speed)
            *current_speed = target_speed;
    }
}

/**
 * @brief  斜率限制器 — 限制信号的变化速率，平缓电流冲击
 *         用于防止 Iq_ref 突变引起的电流尖峰
 * @param  target    目标值
 * @param  current   当前值（输入/输出，需保持跨调用持久）
 * @param  max_rate  最大允许变化率 (units/s)
 * @param  dt        时间步长 (秒)
 * @return 限制后的新值
 */
float RateLimiter(float target, float *current, float max_rate, float dt)
{
    if (current == NULL || max_rate <= 0.0f)
        return target;

    float max_change = max_rate * dt;    // 一个周期内允许的最大变化量
    float delta = target - *current;     // 目标与当前的差值

    if (delta > max_change)
        delta = max_change;
    else if (delta < -max_change)
        delta = -max_change;

    *current += delta;
    return *current;
}

/**
 * @brief  请求受控急停；跑步机复用安全开关减速斜坡，其他模式立即撤销转矩
 */
void Sensoruse_RequestEmergencyStop(void)
{
    if (!g_Intern.EmergencyFlag)
    {
        g_Intern.EmergencyWasTreadmill =
            (g_Sensoruse.RunMode == POS_SPEED_CURRENT_LOOP) ? 1 : 0;
        g_Intern.EmergencyFlag = 1;
    }

    /* E2/E7 and the safety key share this controlled-stop path. */
    g_Intern.AccelRate       = ACCEL_RATE_EMERGENCY;
    g_Sensoruse.g_speed      = 0;
    g_device_regs[10]        = 0;
    g_device_regs[11]        = 0;
    g_device_regs[14]        = 0;
    g_device_regs[15]        = 0;
    VIBRATINO_1_flag         = 0;
    g_Intern.VibrationMode   = 0;
    g_Intern.TimeVibration   = 0;

    if (g_Intern.EmergencyWasTreadmill)
    {
        /* Keep the speed loop active until the emergency ramp reaches zero. */
        if (FocStruct.Taspeed <= PWM_SPEED_THRESHOLD)
            g_Sensoruse.RunMode = PAUSE_STOP;
    }
    else
    {
        /* Vibration/calibration modes have no speed ramp to preserve. */
        FocStruct.Taspeed    = 0.0f;
        g_Sensoruse.RunMode = EMERGENCY_STOP;
    }
}

/**
 * @brief  主机命令处理 — 解析寄存器命令字并设置运行模式
 *         每个控制周期调用，检查通信接口是否有新命令
 * @return 始终返回 1
 */
uint8_t g_Master(void)
{
    uint16_t emergency_cmd = ((uint16_t)g_device_regs[16] << 8) | g_device_regs[17];
    uint8_t requested_vibration_mode = 0;

    /* CMD_EMERGENCY_OFF is an edge command, not a persistent permission bit.
     * Consume every received copy immediately so it cannot silently recover a
     * later E5 fault after a safety-key or E0 recovery has completed. */
    if (emergency_cmd == CMD_EMERGENCY_OFF)
    {
        g_device_regs[16] = 0;
        g_device_regs[17] = 0;
    }

    /*
     * E5/E7 由下控锁存，必须由上控的开始键显式发送解除命令。
     * E5 可直接恢复；E7 只有 IPM 和电机温度都回到安全区后才能恢复。
     * E6 属于校准故障，仍保持锁存，避免绕过传感器校准保护。
     */
    if (emergency_cmd == CMD_EMERGENCY_OFF && g_Sensoruse.SYS_error != SYS_ERROR_NONE)
    {
        uint8_t recoverable =
            (g_Sensoruse.SYS_error == SYS_ERROR_MOTOR_STALL) ||
            (g_Sensoruse.SYS_error == SYS_ERROR_OVER_TEMP && Temp_Protect_CanRecover());

        if (recoverable)
        {
            if (g_Sensoruse.SYS_error == SYS_ERROR_OVER_TEMP)
                Temp_Protect_Clear();

            g_Sensoruse.SYS_error = SYS_ERROR_NONE;
            g_Sensoruse.g_speed = 0;
            g_device_regs[10] = 0;
            g_device_regs[11] = 0;
            g_device_regs[14] = 0;
            g_device_regs[15] = 0;
            VIBRATINO_1_flag = 0;
            g_Intern.VibrationMode = 0;
            g_Intern.TimeVibration = 0;
            g_Intern.StallTimer = 0;
            g_Intern.EmergencyFlag = 0;
            g_Intern.EmergencyWasTreadmill = 0;
            g_Intern.AccelRate = ACCEL_RATE_NORMAL;
            g_Intern.Delay = 0.0f;
            FocStruct.Taspeed = 0.0f;
            FocStruct.Ud = 0.0f;
            FocStruct.Uq = 0.0f;
            g_Sensoruse.RunMode = DELAY_COUNT;
        }

        return 1;
    }

    /*
     * 急停是最高优先级，必须先于速度/振动命令处理。
     * 上控在安全卡扣恢复后会重新下发原模式，因此下控只负责：
     * 1) 跑带沿用现有梯形减速到 0；2) 振动立即清零；3) 急停期间忽略旧命令。
     */
    if (emergency_cmd == CMD_EMERGENCY_ON)
    {
        Sensoruse_RequestEmergencyStop();
        return 1;
    }

    if (g_Intern.EmergencyFlag)
    {
        Sensoruse_RequestEmergencyStop();
        if (emergency_cmd == CMD_EMERGENCY_OFF)
        {
            /*
             * Safety-key recovery does not invalidate the completed encoder
             * calibration. Re-entering ENCODER_CALIB can leave the display
             * resumed while the motor is still held in calibration. Clear the
             * loop outputs, then reuse the normal stabilization delay before
             * returning to the speed loop.
             */
            g_Intern.AccelRate      = ACCEL_RATE_NORMAL;
            g_Intern.Delay          = 0.0f;
            FocStruct.Taspeed       = 0.0f;
            FocStruct.Ud            = 0.0f;
            FocStruct.Uq            = 0.0f;
            g_Sensoruse.g_speed     = 0;
            g_Sensoruse.RunMode     = DELAY_COUNT;
            g_Intern.EmergencyFlag  = 0;
            g_Intern.EmergencyWasTreadmill = 0;
        }
        /* 未收到明确解除命令前，继续锁住所有速度/振动控制。 */
        return 1;
    }

    /* 从寄存器 [10:11] 解析目标速度 */
    g_Sensoruse.g_speed = ((uint16_t)g_device_regs[10] << 8) | g_device_regs[11];
    if (g_Sensoruse.g_speed > MOTOR_SPEED_COMMAND_MAX)
        g_Sensoruse.g_speed = MOTOR_SPEED_COMMAND_MAX;

    /* 从寄存器 [14:15] 解析振动测试命令 */
    VIBRATINO_1_flag = g_device_regs[14] ;

    if (VIBRATINO_1_flag >= CMD_VIB_MODE1 &&
        VIBRATINO_1_flag <= CMD_VIB_MODE4)
    {
        requested_vibration_mode = (uint8_t)VIBRATINO_1_flag;
        g_Sensoruse.g_speed = 0;
        g_device_regs[10] = 0;
        g_device_regs[11] = 0;

        /* The upper controller refreshes the active mode every 200 ms and the
         * register remains non-zero between frames. Re-enter initialization
         * only on first entry or an actual mode change; repeated refreshes must
         * leave the running vibration control loop intact. */
        if (g_Intern.VibrationMode != requested_vibration_mode ||
            (g_Sensoruse.RunMode != VIBRATINO_SATART &&
             g_Sensoruse.RunMode != VIBRATINO_1))
        {
            g_Sensoruse.RunMode = VIBRATINO_SATART;
        }
        g_Intern.VibrationMode = requested_vibration_mode;
    }

    /* 从寄存器 [12:13] 解析校准命令 */
    Calibration_Bx39 = ((uint16_t)g_device_regs[12] << 8) | g_device_regs[13];
    if (Calibration_Bx39 == CMD_CALIB_START)
    {
        g_Sensoruse.RunMode = ENCODER_CALIB;
    }

    return 1;
}

/**
 * @brief  强校准：强制对齐 → 传感器验证 → 通过提示音
 *         若 Flash 中已有有效校准数据，直接加载跳过对齐；
 *         否则在固定角度通电 CALIB_DELAY_S 秒执行强制对齐，
 *         记录偏置值写入 Flash，随后施加 Uq 电压旋转电机，
 *         验证传感器角度变化量是否在合理范围内。
 *         验证失败则擦除 Flash 标志并重试一次；
 *         两次均失败则紧急停机；通过则注入音频提示音。
 * @return 0
 */
uint8_t Factory_Calibration_Strong(void)
{
    POWER_ENABLE();

    switch (g_Intern.CalibSubState)
    {
    /* ---- 第1阶段：强制对齐到电角度 0 ---- */
    case CALIB_SUB_ALIGN:
    {
        /* 仅首次进入时检查 Flash 是否已有有效校准数据 */
        if (g_Intern.Delay == 0.0f)
        {
            flash_read(TEST_FLASH_ADDRESS_START, buffer_read, TEST_BUFEER_SIZE);
            if (buffer_read[FLASH_CALIB_FLAG_OFFSET] == FLASH_CALIB_MAGIC)
            {
                g_Intern.CalibrationEn        = 2;
                g_Sensoruse.CalibOffset_flash = buffer_read[FLASH_CALIB_VALUE_OFFSET];
                g_Sensoruse.RunMode           = DELAY_COUNT;
                return 0;
            }
        }

        g_Intern.CalibrationEn = 1;
        Calculate_Sin_Cos(0, &FocStruct.SinVal, &FocStruct.CosVal);
        FocStruct.Ud = CALIB_ALIGN_VOLTAGE;
        FocStruct.Uq = 0.0f;
        FOC_Modulate();

        g_Intern.Delay += CONTROL_PERIOD_S;

        if (g_Intern.Delay >= CALIB_DELAY_S)
        {
            g_Intern.Delay = 0.0f;

            /* 保存校准偏置到 Flash */
            flash_read(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);
            buffer_write[FLASH_CALIB_VALUE_OFFSET] = KTH78_ReadAngle() / 16;
            g_Sensoruse.CalibOffset_flash = buffer_write[FLASH_CALIB_VALUE_OFFSET];
            buffer_write[FLASH_CALIB_FLAG_OFFSET] = FLASH_CALIB_MAGIC;
            flash_write(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);

            /* 在对齐电压保持下记录起始角度，然后释放 */
            g_Intern.CalibStartAngle = KTH78_ReadAngle() / 16;
            FocStruct.Ud = 0.0f;
            FocStruct.Uq = 0.0f;
            FOC_Modulate();
            g_Intern.CalibSubState = CALIB_SUB_VERIFY;
        }
    }
    break;

    /* ---- 第2阶段：电角度 +90° 拖动，验证传感器变化量 ---- */
    case CALIB_SUB_VERIFY:
    {
        Calculate_Sin_Cos((s32)CALIB_VERIFY_ANGLE_STEP, &FocStruct.SinVal, &FocStruct.CosVal);
        FocStruct.Ud = CALIB_VERIFY_VOLTAGE;
        FocStruct.Uq = 0.0f;
        FOC_Modulate();

        g_Intern.Delay += CONTROL_PERIOD_S;

        if (g_Intern.Delay >= CALIB_VERIFY_DELAY_S)
        {
            g_Intern.Delay = 0.0f;

            /* 在对齐电压保持下读取终止角度，然后释放 */
            uint16_t current_angle = KTH78_ReadAngle() / 16;
            FocStruct.Ud = 0.0f;
            FocStruct.Uq = 0.0f;
            FOC_Modulate();

            /* 计算传感器角度变化量（处理 0/4095 回绕） */
            int16_t angle_change = (int16_t)(current_angle - g_Intern.CalibStartAngle);
            if (angle_change > 2048)
                angle_change -= 4096;
            else if (angle_change < -2048)
                angle_change += 4096;
            if (angle_change < 0)
                angle_change = -angle_change;

            /* 与期望变化量比较（5 对极: 90° 电角度 → 18° 机械角度 ≈ 205 counts） */
            int16_t expected = (int16_t)CALIB_VERIFY_EXPECTED_CHANGE;
            int16_t diff = (int16_t)(angle_change - expected);
            if (diff < 0)
                diff = -diff;

            if (diff <= (int16_t)CALIB_VERIFY_TOLERANCE)
            {
                /* 验证通过 — 进入提示音阶段 */
                g_Intern.CalibSubState = CALIB_SUB_BEEP;
                g_Intern.AudioPhase    = 0;
            }
            else if (g_Intern.CalibRetryCount < CALIB_MAX_RETRIES)
            {
                /* 验证失败，擦除 Flash 校准标志后重试 */
                g_Intern.CalibRetryCount++;
                flash_read(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);
                buffer_write[FLASH_CALIB_FLAG_OFFSET]  = 0x0000;
                buffer_write[FLASH_CALIB_VALUE_OFFSET] = 0x0000;
                flash_write(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);
                g_Intern.CalibSubState = CALIB_SUB_ALIGN;
            }
            else
            {

				flash_read(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);
                buffer_write[FLASH_CALIB_FLAG_OFFSET]  = 0x0000;
                buffer_write[FLASH_CALIB_VALUE_OFFSET] = 0x0000;
                flash_write(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);
                /* 两次校准均失败 — 紧急停机 */
                g_Sensoruse.RunMode = EMERGENCY_STOP;
                g_Sensoruse.SYS_error = 6;   // 编码器校准错误代码
            }
        }
    }
    break;

    /* ---- 第3阶段：电机注入音频提示音 ---- */
    case CALIB_SUB_BEEP:
    {
        g_Intern.AudioPhase += CALIB_BEEP_PHASE_STEP;
        if (g_Intern.AudioPhase >= 4096u)
            g_Intern.AudioPhase -= 4096u;

        /* 方波调制 d 轴电压：相位 0-2047 输出 +V，2048-4095 输出 -V
           方波谐波丰富，比正弦波更易听见 */
        Calculate_Sin_Cos(0, &FocStruct.SinVal, &FocStruct.CosVal);
        FocStruct.Ud = 0;
        FocStruct.Uq = (g_Intern.AudioPhase < 2048u) ? 30: -30;
        FOC_Modulate();

        g_Intern.Delay += CONTROL_PERIOD_S;

        if (g_Intern.Delay >= CALIB_BEEP_DURATION_S)
        {
            g_Intern.Delay = 0.0f;
            FocStruct.Ud = 0.0f;
            FocStruct.Uq = 0.0f;
            FOC_Modulate();
            g_Sensoruse.RunMode = DELAY_COUNT;
        }
    }
    break;

    default:
        g_Intern.CalibSubState = CALIB_SUB_ALIGN;
        break;
    }

    return 0;
}

/**
 * @brief  读取单个 ADC 通道（普通转换）
 */
uint16_t adc_ordinary_channel(adc_type *adc_x, uint8_t channel)
{
    uint16_t adc_data = 0;
    adc_ordinary_channel_set(adc_x, channel, 1, ADC_SAMPLETIME_239_5);
    adc_ordinary_software_trigger_enable(adc_x, TRUE);
    while (adc_flag_get(adc_x, ADC_OCCE_FLAG) == RESET);
    adc_data = adc_ordinary_conversion_data_get(adc_x);
    return adc_data;
}

/**
 * @brief  更新多圈位置计数器
 *         通过检测编码器过零点（半圈分辨率跨越）来
 *         递增/递减圈数计数器
 * @return 累计多圈位置 = 圈数 * 4095 + 当前角度
 */
int32_t update_multi_turn_position(void)
{
    uint16_t raw_angle = (uint16_t)FocStruct.MCangle;
    int16_t delta;

    if (!g_Sensoruse.multi_turn_valid)
    {
        g_Sensoruse.last_raw_angle      = raw_angle;
        g_Sensoruse.multi_turn_valid    = 1;
        g_Sensoruse.multi_turn_position = (int32_t)raw_angle;
        return g_Sensoruse.multi_turn_position;
    }

    delta = (int16_t)(raw_angle - g_Sensoruse.last_raw_angle);

    if (delta < -(int16_t)ENCODER_HALF_RESOLUTION)
        g_Sensoruse.multi_turn_count++;
    else if (delta > (int16_t)ENCODER_HALF_RESOLUTION)
        g_Sensoruse.multi_turn_count--;

    g_Sensoruse.last_raw_angle      = raw_angle;
    g_Sensoruse.multi_turn_position = g_Sensoruse.multi_turn_count * (int32_t)ENCODER_RESOLUTION
                                      + (int32_t)raw_angle;

    return g_Sensoruse.multi_turn_position;
}

/**
 * @brief  读取 IPM 温度（ADC2 通道1，热敏电阻分压）
 *         转换公式：ADC值 → 电压 → 温度（线性近似）
 */
void GET_IPM_TEMP(void)
{
    uint16_t adc_value = adc_ordinary_channel(ADC2, ADC_CHANNEL_1);
    float voltage;
    float temp;

    /* Preserve the last valid temperature while open/short detection is
     * debounced; Temp_Protect_Process latches E7 after three bad samples. */
    if (Temp_Protect_UpdateIPMSensorValidity(adc_value) == 0u)
    {
        return;
    }

    voltage = ((float)adc_value / 4095.0f) * IPM_TEMP_VREF;  // adc值/4095 * 3.3V 转为电压
    temp = (voltage - IPM_TEMP_VOFFSET) * IPM_TEMP_GAIN + IPM_TEMP_BIAS;

    if (temp < 0.0f)
        temp = 0.0f;
    else if (temp > IPM_TEMP_OUTPUT_MAX_C)
        temp = IPM_TEMP_OUTPUT_MAX_C;

    g_Sensoruse.IPM_TEMP = (uint8_t)temp;
}
