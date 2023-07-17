#ifndef __MEASURE_H
#define __MEASURE_H

// Include
#include "stdinc.h"
#include "Global.h"

// Variables
extern volatile uint16_t ADC1DMAVoltageBuffer[];
extern volatile uint16_t ADC2DMACurrentBuffer[];
extern volatile bool MEASURE_InMilliAmperes;
extern volatile float MEASURE_PrimaryVoltage;
extern volatile uint16_t MEASURE_RangeI, MEASURE_RangeU;

// Functions
bool MEASURE_InputParametersCorrect();
void MEASURE_SetMeasureRange();
float MEASURE_Voltage();
float MEASURE_Current();
float MEASURE_GetCurrentPeakLimit();

#endif //__MEASURE_H
