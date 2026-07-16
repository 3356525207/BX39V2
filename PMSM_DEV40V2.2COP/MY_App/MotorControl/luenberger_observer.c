#include "luenberger_observer.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

LuenbergerObserver_t gObs;

static float WrapAngle(float angle)
{
    float wrapped = fmodf(angle, 2.0f * M_PI);
    if (wrapped < 0.0f)
    {
        wrapped += 2.0f * M_PI;
    }
    return wrapped;
}

static float AngleError(float measured, float estimated)
{
    float error = measured - estimated;
    if (error > M_PI)
    {
        error -= 2.0f * M_PI;
    }
    else if (error < -M_PI)
    {
        error += 2.0f * M_PI;
    }
    return error;
}

void LuenbergerObserver_Init(LuenbergerObserver_t *obs,
                             float dt,
                             float beta1,
                             float beta2,
                             float beta3,
                             float b)
{
    if (obs == NULL)
    {
        return;
    }

    obs->z1 = 0.0f;
    obs->z2 = 0.0f;
    obs->z3 = 0.0f;
    obs->dt = dt;
    obs->beta1 = beta1;
    obs->beta2 = beta2;
    obs->beta3 = beta3;
    obs->b = b;
}

float LuenbergerObserver_Update(LuenbergerObserver_t *obs, float theta_mech, float u)
{
    if (obs == NULL)
    {
        return 0.0f;
    }

    float z1_mod = WrapAngle(obs->z1);
    float error = AngleError(theta_mech, z1_mod);

    float dz1 = obs->z2 + obs->beta1 * error;
    float dz2 = obs->z3 + obs->b * u + obs->beta2 * error;
    float dz3 = obs->beta3 * error;

    obs->z1 += dz1 * obs->dt;
    obs->z2 += dz2 * obs->dt;
    obs->z3 += dz3 * obs->dt;

    return obs->z2;
}

float LuenbergerObserver_GetSpeedRadS(const LuenbergerObserver_t *obs)
{
    return (obs != NULL) ? obs->z2 : 0.0f;
}

float LuenbergerObserver_GetSpeedRPM(const LuenbergerObserver_t *obs)
{
    return (obs != NULL) ? (obs->z2 * 60.0f / (2.0f * M_PI)) : 0.0f;
}

float LuenbergerObserver_GetDisturbance(const LuenbergerObserver_t *obs)
{
    return (obs != NULL) ? obs->z3 : 0.0f;
}
