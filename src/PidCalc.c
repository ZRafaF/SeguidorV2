#include "PidCalc.h"

int32_t calculatePid(Pid* _pid, int16_t error, uint16_t deltaTime)
{
    int32_t p = error * _pid->kp;
    
    _pid->intergral += error * deltaTime;
    int32_t i = _pid->intergral * _pid->ki;

    int32_t derivative = (error - _pid->lastError) / deltaTime;
    int32_t d = derivative * _pid->kd;

    int32_t output = p + i + d;

    _pid -> lastError = error;

    return output;
}
