#include "thermistor.h"
#include "at32m412_416_wk_config.h"
#include "arm_math.h"
#include "motor_sensoruse.h"
/* 声明外部变量，避免循环包含 */

#define EMERGENCY_STOP 0x05

/* 滤波缓冲区 - 移动平均算法 */
#define FILTER_SIZE 5
static float temperature_buffer[FILTER_SIZE] = {0};
static uint8_t filter_index = 0;
static uint8_t filter_count = 0;

static uint8_t Temp_Protect_IsAdcSampleValid(uint16_t adc_value)
{
    return (adc_value >= TEMP_SENSOR_ADC_MIN &&
            adc_value <= TEMP_SENSOR_ADC_MAX) ? 1u : 0u;
}

static void Temp_Protect_UpdateSensorState(uint8_t sample_valid,
                                           uint8_t *sensor_valid,
                                           uint8_t *transition_count)
{
    if (sample_valid == *sensor_valid)
    {
        *transition_count = 0u;
        return;
    }

    if (*transition_count < TEMP_SENSOR_STABLE_SAMPLES)
    {
        (*transition_count)++;
    }

    if (*transition_count >= TEMP_SENSOR_STABLE_SAMPLES)
    {
        *sensor_valid = sample_valid;
        *transition_count = 0u;
    }
}

/**
 * @brief 根据ADC原始值计算热敏电阻的阻值
 * 分压电路：Vout = Vref * R_ntc / (R_pull_up + R_ntc)
 * 反推：R_ntc = R_pull_up * Vout / (Vref - Vout)
 * 其中 Vout对应ADC值：Vout = ADC_value / ADC_MAX_VALUE * Vref
 */
float Thermistor_GetResistance(uint16_t adc_value)
{
    float adc_voltage;
    float resistance;
    
    /* 防止除以零 */
    if (adc_value == 0 || adc_value >= ADC_MAX_VALUE)
    {
        return 0;
    }
    
    /* ADC值转换为电压（mV）*/
    adc_voltage = (float)adc_value * ADC_REFERENCE_VOLTAGE / ADC_MAX_VALUE;
    
    /* 根据分压公式计算热敏电阻阻值
     * Vout = Vref * R_ntc / (R_pull_up + R_ntc)
     * 反推：R_ntc = R_pull_up * Vout / (Vref - Vout)
     */
    resistance = PULL_UP_RESISTANCE * adc_voltage / (ADC_REFERENCE_VOLTAGE - adc_voltage);
    
    return resistance;
}

/**
 * @brief 根据热敏电阻阻值计算温度
 * 使用Beta参数模型（两参数模型）：
 * 1/T = (1/T0) + (1/B)*ln(R/R0)
 * 其中：
 *   T0 = 298.15K (25°C)
 *   R0 = THERMISTOR_R25 (25°C时的电阻值)
 *   B = THERMISTOR_B_VALUE
 *   R = 实际电阻值
 */
float Thermistor_CalculateTemperature(float resistance)
{
    float temperature_k;
    float temperature_c;
    
    /* 防止对数计算异常 */
    if (resistance <= 0)
    {
        return 0;
    }
    
    /* T0的倒数（单位：1/K）*/
    float inv_T0 = 1.0f / (STANDARD_TEMP + ABSOLUTE_ZERO);
    
    /* 计算 ln(R/R0) */
    float ln_ratio = logf(resistance / THERMISTOR_R25);
    
    /* 计算温度倒数（单位：1/K）*/
    float inv_T = inv_T0 + (1.0f / THERMISTOR_B_VALUE) * ln_ratio;
    
    /* 计算温度（单位：K）*/
    temperature_k = 1.0f / inv_T;
    
    /* 转换为摄氏度 */
    temperature_c = temperature_k - ABSOLUTE_ZERO;
    
    return temperature_c;
}

/**
 * @brief 直接从ADC值计算温度
 */
float Thermistor_GetTemperature(uint16_t adc_value)
{
    float resistance = Thermistor_GetResistance(adc_value);
    float temperature = Thermistor_CalculateTemperature(resistance);
    
    return temperature;
}

/**
 * @brief 获取滤波后的温度（移动平均）
 * 用于去除ADC采样的随机噪声
 */
float Thermistor_GetFilteredTemperature(uint16_t adc_value)
{
    uint8_t sample_valid = Temp_Protect_IsAdcSampleValid(adc_value);
    float current_temp;
    float average_temp = 0;
    uint8_t i;

    Temp_Protect_UpdateSensorState(sample_valid,
                                   &g_TempProtect.motor_sensor_valid,
                                   &g_TempProtect.motor_sensor_transition_count);

    /* Do not feed an open/short sample into the moving average. */
    if (sample_valid == 0u)
    {
        return g_TempProtect.current_temp;
    }

    current_temp = Thermistor_GetTemperature(adc_value);
    
    /* 添加当前值到缓冲区 */
    temperature_buffer[filter_index] = current_temp;
    filter_index = (filter_index + 1) % FILTER_SIZE;
    
    /* 增加计数器（直到达到FILTER_SIZE）*/
    if (filter_count < FILTER_SIZE)
    {
        filter_count++;
    }
    
    /* 计算平均值 */
    for (i = 0; i < filter_count; i++)
    {
        average_temp += temperature_buffer[i];
    }
    average_temp = average_temp / filter_count;
    
    return average_temp;
}

/**
 * @brief 初始化热敏电阻模块
 */
void Thermistor_Init(void)
{
    uint8_t i;
    
    /* 清空滤波缓冲区 */
    for (i = 0; i < FILTER_SIZE; i++)
    {
        temperature_buffer[i] = 0;
    }
    filter_index = 0;
    filter_count = 0;
    
    /* 初始化温度保护结构体 */
    g_TempProtect.current_temp = 0;
    g_TempProtect.state = TEMP_STATE_NORMAL;
    g_TempProtect.over_temp_flag = 0;
    g_TempProtect.emergency_flag = 0;
    g_TempProtect.target_speed_limit = 0;
    g_TempProtect.over_temp_counter = 0;
    g_TempProtect.motor_sensor_valid = 1u;
    g_TempProtect.ipm_sensor_valid = 1u;
    g_TempProtect.motor_sensor_transition_count = 0u;
    g_TempProtect.ipm_sensor_transition_count = 0u;
}

/* 温度保护结构体 */
TEMP_PROTECT_STRUCT g_TempProtect =
{
    0.0f, TEMP_STATE_NORMAL, 0u, 0u, 0.0f, 0u,
    1u, 1u, 0u, 0u
};

uint8_t Temp_Protect_UpdateIPMSensorValidity(uint16_t adc_value)
{
    uint8_t sample_valid = Temp_Protect_IsAdcSampleValid(adc_value);

    Temp_Protect_UpdateSensorState(sample_valid,
                                   &g_TempProtect.ipm_sensor_valid,
                                   &g_TempProtect.ipm_sensor_transition_count);
    return sample_valid;
}

/**
 * @brief 温度保护处理（需在主循环中调用）
 * 逻辑：IPM 达到 105°C 立即锁存 E7；电机热敏达到原紧急阈值时
 * 保留既有延时停机保护。E7 只能在温度回落后由上控开始键解除。
 */
uint8_t Temp_Protect_Process(float temperature)
{
    uint8_t emergency_flag = 0;
    
    /* 更新当前温度 */
    g_TempProtect.current_temp = temperature;
    
    /* IPM 达到 105°C 时立即锁存 E7；电机热敏保护保留原延时策略。 */
    /* Open/short sensors use the existing latched E7 path. Normal temperature
     * thresholds and recovery temperatures remain unchanged. */
    if (g_TempProtect.motor_sensor_valid == 0u ||
        g_TempProtect.ipm_sensor_valid == 0u)
    {
        Sensoruse_RequestEmergencyStop();
        g_TempProtect.state = TEMP_STATE_EMERGENCY;
        g_TempProtect.emergency_flag = 1;
        g_TempProtect.over_temp_flag = 1;
        g_TempProtect.target_speed_limit = 0;
        emergency_flag = EMERGENCY_STOP;
        g_Sensoruse.SYS_error = SYS_ERROR_OVER_TEMP;
    }
    else if (g_Sensoruse.IPM_TEMP >= IPM_TEMP_TRIP_C)
    {
        Sensoruse_RequestEmergencyStop();
        g_TempProtect.state = TEMP_STATE_EMERGENCY;
        g_TempProtect.emergency_flag = 1;
        g_TempProtect.over_temp_flag = 1;
        g_TempProtect.target_speed_limit = 0;
        emergency_flag = EMERGENCY_STOP;
        g_Sensoruse.SYS_error = SYS_ERROR_OVER_TEMP;
    }
    else if (temperature >= TEMP_EMERGENCY_LEVEL)
    {
        g_TempProtect.over_temp_counter++;

        if (g_TempProtect.over_temp_counter >= TEMP_DELAY_EMERGENCY * 10)
        {
            Sensoruse_RequestEmergencyStop();
            g_TempProtect.state = TEMP_STATE_EMERGENCY;
            g_TempProtect.emergency_flag = 1;
            g_TempProtect.over_temp_flag = 1;
            g_TempProtect.target_speed_limit = 0;
            emergency_flag = EMERGENCY_STOP;
            g_Sensoruse.SYS_error = SYS_ERROR_OVER_TEMP;
        }
    }
    else
    {
        /* 只停止过温计时；已锁存的 E7 必须降温后再由开始键清除。 */
        g_TempProtect.over_temp_counter = 0;
    }

    return emergency_flag;
}

/**
 * @brief 获取温度保护状态
 */
TEMP_PROTECT_STATE Temp_Protect_GetState(void)
{
    return g_TempProtect.state;
}

/**
 * @brief 获取速度限制值
 */
float Temp_Protect_GetSpeedLimit(void)
{
    return g_TempProtect.target_speed_limit;
}

/**
 * @brief 清除过温标志（需在温度降低后手动调用）
 */
void Temp_Protect_Clear(void)
{
    g_TempProtect.over_temp_counter = 0;
    g_TempProtect.state = TEMP_STATE_NORMAL;
    g_TempProtect.over_temp_flag = 0;
    g_TempProtect.emergency_flag = 0;
    g_TempProtect.target_speed_limit = 0;
}

uint8_t Temp_Protect_CanRecover(void)
{
    return (g_TempProtect.motor_sensor_valid != 0u &&
            g_TempProtect.ipm_sensor_valid != 0u &&
            g_Sensoruse.IPM_TEMP <= IPM_TEMP_RECOVER_C &&
            g_TempProtect.current_temp <= TEMP_RECOVERY_LEVEL) ? 1u : 0u;
}

/**
 * @brief 测试函数 - 计算已知阻值对应的温度
 * 使用此函数验证算法正确性
 */
float Thermistor_CalculateTemperatureByResistance(uint32_t resistance_ohm)
{
    return Thermistor_CalculateTemperature((float)resistance_ohm);
}

/*
 * 使用示例：
 * 
 * // 初始化
 * Thermistor_Init();
 * 
 * // 获取ADC值（假设从wk_adc模块获取）
 * uint16_t adc_value = get_adc_value();
 * 
 * // 方法1：直接计算温度（无滤波）
 * float temp = Thermistor_GetTemperature(adc_value);
 * printf("Temperature: %.2f°C\n", temp);
 * 
 * // 方法2：获取滤波后的温度（推荐）
 * float filtered_temp = Thermistor_GetFilteredTemperature(adc_value);
 * printf("Filtered Temperature: %.2f°C\n", filtered_temp);
 * 
 * // 方法3：已知阻值，直接计算温度
 * float temp_from_r = Thermistor_CalculateTemperature(10000);  // 10K电阻
 * printf("Temperature at 10K: %.2f°C\n", temp_from_r);
 * 
 * 验证公式：
 * 25°C时，阻值为10K，应该计算出约25°C
 */
