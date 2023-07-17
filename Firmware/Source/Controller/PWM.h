#ifndef __PWM_H
#define __PWM_H

// Include
#include "stdinc.h"
#include "Global.h"

// Variables
extern volatile uint32_t PWMTimerCounter;

// Functions
void PWM_CacheParameters();
void PWM_SignalStart();
void PWM_SignalStop();
bool PWM_SinRegulation(uint16_t *Problem);

#endif // __PWM_H
