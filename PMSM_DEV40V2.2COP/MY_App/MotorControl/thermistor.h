#ifndef __THERMISTOR_H
#define __THERMISTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/* ADC rail detection for open/short temperature sensors. Three consecutive
 * samples are required both to declare a fault and to declare recovery. */
#define TEMP_SENSOR_ADC_MIN          8u
#define TEMP_SENSOR_ADC_MAX          4087u
#define TEMP_SENSOR_STABLE_SAMPLES   3u

/* NTC热敏电阻参数定义 */
#define THERMISTOR_R25         10000      /* 25°C时的标准电阻值，单位：欧 */
#define THERMISTOR_B_VALUE     3950       /* B值，单位：K */
#define PULL_UP_RESISTANCE     10000      /* 分压电阻，单位：欧 */
#define ADC_REFERENCE_VOLTAGE  3300       /* ADC参考电压，单位：mV */
#define ADC_MAX_VALUE          4095       /* ADC最大值（12位ADC） */

/* 标准参考温度 */
#define STANDARD_TEMP          25         /* 标准温度 25°C */
#define ABSOLUTE_ZERO          273.15f    /* 绝对零度 -273.15°C */

/**
 * @brief 根据ADC原始值计算热敏电阻的阻值
 * @param adc_value: ADC采集的原始值
 * @return 热敏电阻阻值，单位：欧
 */
float Thermistor_GetResistance(uint16_t adc_value);

/**
 * @brief 根据热敏电阻阻值计算温度（使用Beta参数模型）
 * @param resistance: 热敏电阻阻值，单位：欧
 * @return 温度值，单位：°C
 */
float Thermistor_CalculateTemperature(float resistance);

/**
 * @brief 直接从ADC值计算温度
 * @param adc_value: ADC采集的原始值
 * @return 温度值，单位：°C
 */
float Thermistor_GetTemperature(uint16_t adc_value);

/**
 * @brief 获取滤波后的温度（移动平均）
 * @param adc_value: ADC采集的原始值
 * @return 滤波后的温度值，单位：°C
 */
float Thermistor_GetFilteredTemperature(uint16_t adc_value);

/**
 * @brief 初始化热敏电阻模块
 */
void Thermistor_Init(void);

/* ==================== 温度保护参数 ==================== */
#define TEMP_WARNING_LEVEL1    110.0f    /* 第1级过热警告温度 °C */
#define TEMP_WARNING_LEVEL2    135.0f    /* 第2级过热警告温度 °C */
#define TEMP_EMERGENCY_LEVEL   135.0f    /* 紧急停机温度 °C */
#define TEMP_RECOVERY_LEVEL    125.0f    /* 电机温度降到该值后允许手动恢复 */

#define TEMP_DELAY_LEVEL1      60         /* 第1级降速延迟时间 秒 */
#define TEMP_DELAY_LEVEL2      60         /* 第2级降速延迟时间 秒 */
#define TEMP_DELAY_EMERGENCY   20         /* 紧急停机延迟时间 秒 */

#define SPEED_LIMIT_LEVEL1     3.6f      /* 第1级降速后目标速度 (Hz) */
#define SPEED_LIMIT_LEVEL2     3.0f      /* 第2级降速后目标速度 (Hz) */

/* 温度保护状态 */
typedef enum
{
    TEMP_STATE_NORMAL = 0,           /* 温度正常 */
    TEMP_STATE_WARNING_110,          /* 第1级过热 - 110°C 60s后降速 */
    TEMP_STATE_WARNING_130,          /* 第2级过热 - 130°C 60s后降速 */
    TEMP_STATE_EMERGENCY             /* 紧急停机 - 140°C 20s后停机 */
} TEMP_PROTECT_STATE;

/* 温度保护结构体 */
typedef struct
{
    float current_temp;        /* 当前温度 */
    TEMP_PROTECT_STATE state;  /* 当前保护状态 */
    uint8_t over_temp_flag;    /* 过温标志 */
    uint8_t emergency_flag;    /* 紧急停机标志 */
    float target_speed_limit;  /* 目标速度限制 */
    uint16_t over_temp_counter; /* 过温持续时间计数器 */
    uint8_t motor_sensor_valid;
    uint8_t ipm_sensor_valid;
    uint8_t motor_sensor_transition_count;
    uint8_t ipm_sensor_transition_count;
} TEMP_PROTECT_STRUCT;

extern TEMP_PROTECT_STRUCT g_TempProtect;

/**
 * @brief 温度保护处理（需在主循环中调用）
 * @param temperature: 当前温度值
 * @return 过温报警标志，返回1表示需要紧急停机
 */
uint8_t Temp_Protect_Process(float temperature);

/**
 * @brief 获取温度保护状态
 * @return 当前温度保护状态
 */
TEMP_PROTECT_STATE Temp_Protect_GetState(void);

/**
 * @brief 获取速度限制值
 * @return 当前允许的最大速度
 */
float Temp_Protect_GetSpeedLimit(void);

/**
 * @brief 清除过温标志（需在温度降低后手动调用）
 */
void Temp_Protect_Clear(void);

/**
 * @brief 判断温度是否已经回落到允许人工恢复的安全范围
 */
uint8_t Temp_Protect_CanRecover(void);

/**
 * @brief Update debounced IPM sensor validity from the raw ADC sample.
 * @return 1 when this sample is inside the electrical range, otherwise 0.
 */
uint8_t Temp_Protect_UpdateIPMSensorValidity(uint16_t adc_value);

#ifdef __cplusplus
}
#endif

#endif /* __THERMISTOR_H */
