#ifndef __LOG_UTILS_H
#define __LOG_UTILS_H

// Include
#include "stdinc.h"

void MU_LogFast(float VSet, float PWMSet, float ResultV, float ResultI);
void MU_LogRMS(float VSet, float VControlSet, float ResultV, float ResultI);

#endif // __LOG_UTILS_H
