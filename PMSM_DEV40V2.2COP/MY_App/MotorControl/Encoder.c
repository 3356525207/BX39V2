#include "wk_system.h"
#include "Encoder.h"
#include "arm_math.h"
#include "wk_tmr.h"

#include "at32m412_416_conf.h"

uint16_t Enc_Count_last, speed_delay;
int32_t speed_rps = 0; // 转速（转/秒）
float Enc_VFsingle = 0;
uint8_t VFsingle_EN = 1;

void Enc_Init(void)
{

    //    HAL_TIM_Base_Start_IT(&htim3); // 使能编码器计数
}

float Gat_Encsingle_float(void)

{

    uint16_t Enc_Count;
    float Encsingle_float;
    Enc_Count = tmr_counter_value_get(TMR3);
    Encsingle_float = Enc_Count *2;
    return Encsingle_float;
}

float Gat_VFsingle_float(void)

{
    float VFsingle_float;

    Enc_VFsingle += 1;
    if (Enc_VFsingle >= 4095)
        Enc_VFsingle = 0;
    VFsingle_float = (Enc_VFsingle / 2000.0f) * (2 * PI);

    return Enc_VFsingle;
}

int32_t Get_Encoder_Speed(void)
{
    int32_t Enc_Count = tmr_counter_value_get(TMR3); // 获取当前计数值
    int32_t delta = 0;

    speed_delay++;

    if (speed_delay >= 10)
    {

        // 1. 计算增量并处理溢出（每圈2000脉冲）
        if (Enc_Count - Enc_Count_last > 1000)
        {
            // 向下溢出（如从1999→0）
            delta = Enc_Count - (Enc_Count_last + 2000);
        }
        else if (Enc_Count_last - Enc_Count > 1000)
        {
            // 向上溢出（如从0→1999）
            delta = (Enc_Count + 2000) - Enc_Count_last;
        }
        else
        {
            // 正常差值
            delta = Enc_Count - Enc_Count_last;
        }

        Enc_Count_last = Enc_Count; // 更新历史值

        // 计算转速（转/秒）：
        // delta = 脉冲变化量，2000脉冲/转，采样时间间隔 Δt = 0.0002秒（5kHz）
        speed_rps = (delta / 2000.0f) * 50 * 60;
        speed_delay = 0;
    }

    return speed_rps;
}
