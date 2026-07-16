#ifndef __PID_H
#define __PID_H

//#include "main.h"
#include "at32m412_416_wk_config.h"

// PID参数结构体（每个电流轴需要独立的PID）
typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数
    float integral;     // 积分累积值
    float prev_error;   // 上一次误差（用于微分项）
    float output_limit; // 输出限幅（例如：PWM最大值或电压限幅）
    float integral_limit; // 积分抗饱和限幅
} PID_Controller;

extern PID_Controller pid_id;  // 直轴电流PID
extern PID_Controller pid_iq;  // 交轴电流PID
extern PID_Controller pid_speed;  // 速度PID
extern PID_Controller pid_pos ;  // 位置PID

void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float output_limit, float integral_limit);
void PID_Reset(PID_Controller *pid);
void PID_SetTunings(PID_Controller *pid, float Kp, float Ki, float Kd);
void PID_SetLimits(PID_Controller *pid, float output_limit, float integral_limit);
float PID_Update(PID_Controller *pid, float target, float feedback, float dt);
void CurrentLoop_Init(void);

#define PID_DEFAULTS {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 12.0f, 100.0f} // 默认PID参数


#endif

