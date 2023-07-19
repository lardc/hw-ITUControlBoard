#ifndef __MEASURE_H
#define __MEASURE_H

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
void ME_CacheVariables(CurrentChannel SelectedChannel);
SampleResult ME_GetSampleResult();

#endif // __MEASURE_H
