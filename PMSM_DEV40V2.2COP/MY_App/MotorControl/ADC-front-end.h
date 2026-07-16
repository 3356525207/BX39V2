#ifndef __ADC_FRONT_END_H
#define __ADC_FRONT_END_H
#include <stdint.h>



#define sample_resistance 0.010f //采样电阻值，单位欧姆
#define adc_ref_voltage 3.3f    //ADC参考电压，单位伏特
#define adc_resolution 4096.0f  //ADC分辨率（12位ADC为4096）
#define adc_magnify 10.0f        //电流放大倍数

typedef struct
{
	uint16_t adc_raw[3];   // 三相 ADC 原始值
    uint16_t adc_Injected[2]; // 三相 ADC 注入值
	float    i_abc[2];     // 三相电流计算值
	float	i_abc_float[3]; // 三相电流计算值（浮点数，单位安培）
	float    offset[2];    // 三相偏置校准值
} FOC_PhaseData_t;


extern  uint16_t adc_odt[2]; 

void FOC_PhaseData_Calibrate(FOC_PhaseData_t *data);
void FOC_phaseData_current_folat(FOC_PhaseData_t *data);


extern FOC_PhaseData_t g_foc_phase_data;

#endif
