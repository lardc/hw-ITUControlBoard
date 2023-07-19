﻿#ifndef __MEASURE_H
#define __MEASURE_H

// Include
#include "stdinc.h"
#include "LowLevel.h"

// Variables
extern uint16_t DMAVoltage[];
extern uint16_t DMACurrent12[];
extern uint16_t DMACurrent34[];

// Functions
void ME_CacheVariables(CurrentChannel SelectedChannel);

#endif // __MEASURE_H
