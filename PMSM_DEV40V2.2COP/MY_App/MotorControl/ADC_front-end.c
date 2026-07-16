#include "ADC-front-end.h"
#include "wk_system.h"
#include "foc_drv.h" 
#include "foc_drv.h"
#define FOC_ZERO_SAMPLES 128U


 uint16_t adc_odt[2]; 


// 初始化 FOC_PhaseData_t 结构体
FOC_PhaseData_t g_foc_phase_data = {
    .adc_raw = {0, 0, 0},
    .i_abc   = {0.0f, 0.0f},
    .i_abc_float = {0.0f, 0.0f, 0.0f},
    .adc_Injected = {0, 0},
    .offset  = {0.0f, 0.0f},

};



//adc偏置电压校准函数
void FOC_PhaseData_Calibrate(FOC_PhaseData_t *data)
{
    uint32_t adc_sum[2] = {0, 0};
    
    // 累加多次 ADC 采样值
    for (uint16_t i = 0; i < FOC_ZERO_SAMPLES; i++) {
        adc_sum[0] += data->adc_Injected[0];
        adc_sum[1] += data->adc_Injected[1];
        wk_delay_ms (10); // 延时等待下一次采样
    }
    
    // 计算平均值作为偏置
    data->offset[0] = (float)adc_sum[0] / FOC_ZERO_SAMPLES;
    data->offset[1] = (float)adc_sum[1] / FOC_ZERO_SAMPLES;
}


// 将 ADC 原始值转换为电流值（单位安培）
void FOC_phaseData_current_folat(FOC_PhaseData_t *data)
{

    data->i_abc[0] = (float)(data->adc_Injected[0] - data->offset[0])*-1;
    data->i_abc[1] = (float)(data->adc_Injected[1] - data->offset[1])*-1;
    data->i_abc_float[0] = data->i_abc[0] * adc_ref_voltage / adc_resolution / adc_magnify / sample_resistance;
    data->i_abc_float[1] = data->i_abc[1] * adc_ref_voltage / adc_resolution / adc_magnify / sample_resistance;
    data->i_abc_float[2] = -data->i_abc_float[0] - data->i_abc_float[1];

    //电流重映射
    FocStruct.Iu = data->i_abc_float[2];
    FocStruct.Iv = data->i_abc_float[1];
    FocStruct.Iw = data->i_abc_float[0];

}





