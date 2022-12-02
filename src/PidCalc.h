#ifndef PIDCALC_H
#define PIDCALC_H

#include <stdint.h>

typedef struct Pid
{
    float kp;
    float ki;
    float kd;

    float gain;

    float lastError;

    float intergral;
} Pid;


int32_t calculatePid(Pid* Pid, int16_t error, uint16_t deltaTime);

#endif