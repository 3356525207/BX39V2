#include "PID.h"


PID_Controller pid_id = PID_DEFAULTS;  // 直轴电流PID
PID_Controller pid_iq = PID_DEFAULTS;  // 交轴电流PID
PID_Controller pid_speed = PID_DEFAULTS;  // 速度PID
PID_Controller pid_pos = PID_DEFAULTS;  // 位置PID

// 初始化PID控制器参数
void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float output_limit, float integral_limit) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->output_limit = (output_limit > 0.0f ? output_limit : 0.0f);
    pid->integral_limit = (integral_limit > 0.0f ? integral_limit : 0.0f);
}

// 重置PID状态（用于模式切换或启动时清零）
void PID_Reset(PID_Controller *pid) {
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

// 动态修改PID参数接口
void PID_SetTunings(PID_Controller *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}
// 修改PID限幅参数接口
void PID_SetLimits(PID_Controller *pid, float output_limit, float integral_limit) {
    pid->output_limit = (output_limit > 0.0f ? output_limit : 0.0f);
    pid->integral_limit = (integral_limit > 0.0f ? integral_limit : 0.0f);
}

void CurrentLoop_Init(void)
{
    // 初始化Id环PID参数Kp=5.8, Ki=0.5, Kd=0.0
    PID_Init(&pid_id, 3.8f, 90.5f, 0.0f, 50.0f, 0.4f);

    // 初始化Iq环PID参数Kp=8.8, Ki=0.8, Kd=0.0
    PID_Init(&pid_iq, 3.8f, 130.8f, 0.0f, 50.0f, 0.4f);

    PID_Init(&pid_speed, 0.012f, 0.001f, 0.00007f, 9.50f, 3.0f);
	    PID_Init(&pid_pos, 0.50f, 0.01f, 0.05f, 2000.0f, 300.0f);
}

// 计算PID输出（需周期性调用，例如在PWM中断中）
float PID_Update(PID_Controller *pid, float target, float feedback, float dt) {
    if (pid == NULL) {
        return 0.0f;
    }

    if (dt <= 0.0f) {
        // 避免除以0或负时间步长，确保安全
        return 0.0f;
    }

    // 计算误差
    float error = target - feedback;

    // 积分项计算（防止积分饱和）
    pid->integral += error * dt;
    if (pid->integral > pid->integral_limit) {
        pid->integral = pid->integral_limit;
    } else if (pid->integral < -pid->integral_limit) {
        pid->integral = -pid->integral_limit;
    }

    float P = pid->Kp * error;
    float I = pid->Ki * pid->integral;
    float D = pid->Kd * ((error - pid->prev_error) / dt);

    pid->prev_error = error;

    float output = P + I + D;

    if (pid->output_limit > 0.0f) {
        if (output > pid->output_limit) {
            output = pid->output_limit;
        } else if (output < -pid->output_limit) {
            output = -pid->output_limit;
        }
    }

    return output;
}
