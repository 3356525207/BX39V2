#ifndef __LUENBERGER_OBSERVER_H__
#define __LUENBERGER_OBSERVER_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 扩展状态观测器(ESO)，用于从机械角度测量估计电机速度。
 * 估计输出：角度、角速度、等效扰动。
 */
typedef struct
{
    float z1;      // 角度估计值 [rad]
    float z2;      // 角速度估计值 [rad/s]
    float z3;      // 等效扰动估计值 [rad/s^2]

    float beta1;   // 观测增益1
    float beta2;   // 观测增益2
    float beta3;   // 观测增益3
    float dt;      // 采样周期 [s]
    float b;       // 控制输入增益
} LuenbergerObserver_t;

extern LuenbergerObserver_t gObs;

void LuenbergerObserver_Init(LuenbergerObserver_t *obs,
                             float dt,
                             float beta1,
                             float beta2,
                             float beta3,
                             float b);

float LuenbergerObserver_Update(LuenbergerObserver_t *obs,
                                float theta_mech,
                                float u);

float LuenbergerObserver_GetSpeedRadS(const LuenbergerObserver_t *obs);
float LuenbergerObserver_GetSpeedRPM(const LuenbergerObserver_t *obs);
float LuenbergerObserver_GetDisturbance(const LuenbergerObserver_t *obs);

#ifdef __cplusplus
}
#endif

#endif /* __LUENBERGER_OBSERVER_H__ */
