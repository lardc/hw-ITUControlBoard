﻿#ifndef __T1PWM_H
#define __T1PWM_H

// Include
#include "stdinc.h"

// Functions
void T1PWM_Init(uint32_t SystemClock, uint32_t Period);
void T1PWM_SetDutyCycle(int16_t Value);
void T1PWM_Start();
void T1PWM_Stop();
uint32_t T1PWM_GetPWMBase();

#endif //__T1PWM_H
