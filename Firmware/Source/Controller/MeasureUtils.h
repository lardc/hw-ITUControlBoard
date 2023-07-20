#ifndef __MEASURE_UTILS_H
#define __MEASURE_UTILS_H

// Include
#include "stdinc.h"
#include "LowLevel.h"

// Definitions
#define CURRENT_CHANNELS		4
typedef struct __SampleData
{
	float Voltage;
	float Current[CURRENT_CHANNELS];

} SampleData, *pSampleData;

// Variables
extern uint16_t DMAVoltage[];
extern uint16_t DMACurrent12[];
extern uint16_t DMACurrent34[];

// Functions
void MU_CacheVariables(CurrentChannel SelectedChannel);
void MU_GetSampleData(pSampleData Result);
void MU_ResultFineTuning(pSampleData Result);

#endif // __MEASURE_UTILS_H
