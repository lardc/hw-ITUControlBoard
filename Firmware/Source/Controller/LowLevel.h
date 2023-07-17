#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"
#include "Global.h"

// Functions
void LL_ToggleBoardLED();
void LL_SetSync1State(bool NewState);
void LL_SetSync2State(bool NewState);
void LL_ConnectPOWRelay(bool NewState);
void LL_ConnectCTRLRelay(bool NewState);
void LL_EnablePWMOut(bool NewState);
void LL_DMAReload();
void LL_OutputSelector(ACV_OutputLine OutputLine);
void LL_SetStateRedLed(bool NewState);

#endif //__LOWLEVEL_H
