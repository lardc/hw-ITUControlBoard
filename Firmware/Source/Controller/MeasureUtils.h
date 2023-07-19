#ifndef __MEASURE_UTILS_H
#define __MEASURE_UTILS_H

// Include
#include "stdinc.h"
#include "LowLevel.h"

// Definitions
#define CURRENT_CHANNELS		4
typedef struct __SampleResult
{
	float Voltage;
	float Current[CURRENT_CHANNELS];

} SampleResult;

// Variables
extern uint16_t DMAVoltage[];
extern uint16_t DMACurrent12[];
extern uint16_t DMACurrent34[];

// Functions
void MU_CacheVariables(CurrentChannel SelectedChannel);
SampleResult MU_GetSampleResult();

#endif // __MEASURE_UTILS_H
