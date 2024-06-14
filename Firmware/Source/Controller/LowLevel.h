#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Deinitions
typedef enum __CurrentChannel
{
	CC_R0,
	CC_R1,
	CC_R2
} CurrentChannel;

// Functions
void LL_ToggleBoardLED();
void LL_DMAReload();
void LL_SelectCurrentChannel(CurrentChannel Channel);
void LL_EnableExtLed(bool NewState);
void LL_EnableFan(bool NewState);
bool LL_IsSafetyOK();
uint16_t LL_ReadInputVoltageADC();
uint16_t LL_ReadOutputVoltageADC();
void LL_SetDACOutput(uint16_t Value);
void LL_PWR_EN1(bool NewState);
void LL_PWR_EN2(bool NewState);
void LL_DIS_EN3(bool NewState);

#endif //__LOWLEVEL_H
