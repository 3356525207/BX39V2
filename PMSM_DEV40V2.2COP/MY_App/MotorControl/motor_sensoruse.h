#ifndef __MOTOR_SENSORUSE_H
#define __MOTOR_SENSORUSE_H

#include "at32m412_416_wk_config.h"

/* ============================================================
 * 运行模式定义
 * ============================================================ */
#define ENCODER_CALIB             0x00  // 编码器校准
#define DELAY_COUNT               0x01  // 延时计数（等待状态稳定）
#define FORCE_DRAG                0x02  // 强制拖动（开环电流）
#define SPEED_CURRENT_LOOP        0x03  // 速度+电流闭环
#define POS_SPEED_CURRENT_LOOP    0x04  // 位置+速度+电流闭环
#define EMERGENCY_STOP            0x05  // 紧急停止
#define PAUSE_STOP                0x06  // 暂停/空闲
#define VIBRATINO_1               0x07  // 振动测试运行
#define VIBRATINO_SATART          0x08  // 振动测试初始化

/* ============================================================
 * 电机参数
 * ============================================================ */
#define POLE_PAIRS                5     // 电机极对数
#define ENCODER_RESOLUTION        4095u // 编码器每圈分辨率
#define ENCODER_HALF_RESOLUTION   2048u // 半圈分辨率（过零检测阈值）

/* ============================================================
 * GPIO 宏定义
 * ============================================================ */
#define POWER_ENABLE()            gpio_bits_write(GPIOA, GPIO_PINS_6, 1)  // 电机电源使能
#define POWER_DISABLE()           gpio_bits_write(GPIOA, GPIO_PINS_6, 0)  // 电机电源关闭

/* ============================================================
 * 恢复模式（基于 Flash 的快速启动检测）
 * 上电3次在20s内断电 → 清除校准标志 → 第4次上电触发校准
 * ============================================================ */
#define FLASH_RECOVERY_COUNT_OFFSET   17
#define FLASH_RECOVERY_STABLE_OFFSET  18
#define RECOVERY_STABLE_MAGIC         0x5A5A  // 上次启动已稳定运行>20s
#define RECOVERY_THRESHOLD            3       // 连续快速启动次数阈值

/* ============================================================
 * Flash 校准存储偏移
 * ============================================================ */
#define FLASH_CALIB_FLAG_OFFSET       15  // 校准完成标志位
#define FLASH_CALIB_VALUE_OFFSET      16  // 校准偏置值
#define FLASH_CALIB_MAGIC             0xAA55

/* ============================================================
 * 低通滤波器（LPF）系数
 * ============================================================ */
#define ID_LPF_ALPHA              0.1f  // D轴电流低通滤波系数
#define IQ_LPF_ALPHA              0.5f  // Q轴电流低通滤波系数

/* ============================================================
 * 时间常数（基于 10kHz 控制中断）
 * ============================================================ */
#define CONTROL_PERIOD_S          0.0001f   // 100us 控制周期
#define PID_DT                    0.001f    // PID 更新周期（1ms）
#define DELAY_STABILIZE_S        2.0f      // 校准后稳定等待时间
#define CALIB_DELAY_S             8.0f     // 校准强制对齐保持时间
#define STABLE_TIMEOUT_TICKS      200000UL  // 20s * 10kHz = 恢复稳定标记延时
#define TRAPEZOIDAL_DT            0.002f    // 梯形加减速时间步长

/* ============================================================
 * 堵转保护参数
 * 观测速度 obs_speed 连续低于阈值超过时限 → 紧急停机
 * ============================================================ */
#define STALL_SPEED_THRESHOLD_RPM  650.0f   // 堵转速度阈值 (RPM)
#define STALL_TIME_THRESHOLD_S     4.0f     // 堵转持续时间阈值 (秒)
#define STALL_COUNT_THRESHOLD      ((uint32_t)(STALL_TIME_THRESHOLD_S / CONTROL_PERIOD_S))  // 5s / 100us = 50000 ticks

/* ============================================================
 * 电压等级
 * ============================================================ */
#define CALIB_ALIGN_VOLTAGE       8.0f   // 校准对齐电压
#define FORCE_DRAG_VOLTAGE        10.0f  // 强制拖动电压
#define DEFAULT_BUS_VOLTAGE       120.0f // 默认母线电压

/* ============================================================
 * 校准后传感器验证参数
 * 对齐后电角度增加 90°, 传感器应变化 4096*90/(360*POLE_PAIRS) counts
 * 5 对极: 4096/20 ≈ 205 counts (18° 机械角度)
 * ============================================================ */
#define CALIB_VERIFY_VOLTAGE      5.0f    // 验证阶段对齐电压 (Ud, 与校准电压相同)
#define CALIB_VERIFY_ANGLE_STEP   1024u   // 验证电角度步进 (90° 电角度)
#define CALIB_VERIFY_DELAY_S      1.0f    // 验证阶段持续时间
#define CALIB_VERIFY_EXPECTED_CHANGE  ((uint16_t)(4096UL * 90UL / (360UL * (uint32_t)POLE_PAIRS)))  // 期望传感器变化量 ≈205
#define CALIB_VERIFY_TOLERANCE    40u     // 允许误差范围 (±40 counts)
#define CALIB_MAX_RETRIES         1u      // 校准最大重试次数 (共2次)
#define CALIB_BEEP_VOLTAGE        10.0f   // 校准通过提示音电压幅值
#define CALIB_BEEP_DURATION_S     0.5f    // 提示音持续时间
#define CALIB_BEEP_FREQ_HZ        1000u   // 提示音频率 (1kHz, 较低频率更易听见)
#define CALIB_BEEP_PHASE_STEP     ((uint16_t)((uint32_t)CALIB_BEEP_FREQ_HZ * 4096u / 10000u))  // 相位步进

/* ============================================================
 * PWM 配置
 * ============================================================ */
#define PWM_CYCLE_DEFAULT         9998u  // PWM 周期默认值
#define PWM_LIMIT_DEFAULT         9000u  // 最大占空比限制
#define PWM_SPEED_THRESHOLD       50.0f  // 低于此速度时关闭 PWM 输出

/* ============================================================
 * 振动测试参数
 * ============================================================ */
#define VIB_POS_FORWARD           500     // 正向振动位置指令
#define VIB_POS_REVERSE           (-3000) // 反向振动位置指令
#define VIB_DURATION_MODE1        500u    // 模式1振动持续时间
#define VIB_DURATION_MODE2        600u    // 模式2振动持续时间
#define VIB_DURATION_MODE3        700u    // 模式3振动持续时间
#define VIB_DURATION_MODE4        900u    // 模式4振动持续时间

/* ============================================================
 * 寄存器命令码（来自通信接口）
 * ============================================================ */
#define CMD_VIB_MODE1             0x01  // 振动测试模式1命令
#define CMD_VIB_MODE2             0x02  // 振动测试模式2命令
#define CMD_VIB_MODE3             0x03  // 振动测试模式3命令
#define CMD_VIB_MODE4             0x04  // 振动测试模式4命令
#define CMD_CALIB_START           0xAA66  // 启动校准命令
#define CMD_EMERGENCY_ON          0xAA55  // 紧急停止开启
#define CMD_EMERGENCY_OFF         0x5555  // 紧急停止解除

/* ============================================================
 * 梯形加减速默认速率
 * ============================================================ */
#define ACCEL_RATE_NORMAL         26u   // 正常加减速速率
#define ACCEL_RATE_EMERGENCY      120u   // 紧急停止减速速率
#define MOTOR_SPEED_COMMAND_MAX   4940u  // 3.8 * 1300，下控物理速度指令上限

/* ============================================================
 * IPM 温度 ADC 转换参数
 * ============================================================ */
#define IPM_TEMP_VREF             3.3f   // ADC 参考电压
#define IPM_TEMP_VOFFSET          0.88f  // 测温电压偏置
#define IPM_TEMP_GAIN             34.0f  // 温度转换增益
#define IPM_TEMP_BIAS             25.0f  // 温度转换偏置
#define IPM_TEMP_OUTPUT_MAX_C     107.0f // 对上控输出的 IPM 温度上限
#define IPM_TEMP_TRIP_C           105u   // IPM 过温停机阈值（E7）
#define IPM_TEMP_RECOVER_C        100u   // E7 允许按开始键恢复的温度上限

#define SYS_ERROR_NONE            0u
#define SYS_ERROR_MOTOR_STALL     5u
#define SYS_ERROR_ENCODER_CAL     6u
#define SYS_ERROR_OVER_TEMP       7u

/* ============================================================
 * 外部全局变量（寄存器接口 & 调试）
 * ============================================================ */
extern float    electrical_angle;     // 电角度
extern uint16_t Calibration_Bx39;    // 校准命令寄存器值
extern uint16_t VIBRATINO_1_flag;   // 振动测试命令寄存器值
extern uint8_t  g_RecoveryStable;        // 当前启动是否已标记为稳定
extern volatile uint8_t g_RecoveryWritePending;  // 主循环待执行 flash 写标志（ISR→主循环）

/* ============================================================
 * 有感运行结构体
 * ============================================================ */
typedef struct
{
    uint8_t  CalibFlag;          // 编码器校准标志位
    float    CalibOffset;        // 校准偏置值（运行时）
    float    CalibOffset_flash;  // 从 Flash 读取的校准偏置值
    uint8_t  RunMode;            // 当前控制模式
    float    Iq_ref_last;        // 上一次 Iq 参考电流（用于斜率限制）
    float    CalibOffset_last;   // 上一次校准偏置值（用于编码器异常检测）

    uint16_t g_speed;            // 目标速度（来自寄存器）

    int32_t  multi_turn_count;     // 多圈计数（带符号，正转为正）
    int32_t  multi_turn_position;  // 多圈总位置 = 圈数 * 4095 + 当前角度
    uint16_t last_raw_angle;       // 上一次原始角度值（用于过零检测）
    uint8_t  multi_turn_valid;     // 多圈数据有效标志

    uint8_t  IPM_TEMP;             // IPM 温度（摄氏度）
    
    uint8_t  SYS_error;          // 系统错误标志位
} SENSORUSE_STRUCT;

extern SENSORUSE_STRUCT g_Sensoruse;

/* ============================================================
 * 函数声明
 * ============================================================ */
void     Sensoruse_Control(void);
void     Sensoruse_Init(void);
void     Sensoruse_RequestEmergencyStop(void);
float    update_electrical_angle(void);
float    Calculate_Speed(float currentPos, float *lastPos, float maxVal, float factor);
void     Trapezoidal_Acceleration(float target_speed, float acceleration, float *current_speed, float dt);
float    Calculate_Speedfloat(float currentPos, float *lastPos, float maxVal, float factor);
float    RateLimiter(float target, float *current, float max_rate, float dt);
uint8_t  g_Master(void);
uint8_t  Factory_Calibration(void);
uint8_t  Factory_Calibration_Strong(void);
float    Calculate_velocity_raw(float angle, float dt);
uint16_t adc_ordinary_channel(adc_type *adc_x, uint8_t channel);
void     Voltage_DQLimit(float *ud_in, float *uq_in, float vbus);
int32_t  update_multi_turn_position(void);
void     GET_IPM_TEMP(void);
void     Recovery_Check(void);
void     Recovery_MarkStable(void);

#endif
