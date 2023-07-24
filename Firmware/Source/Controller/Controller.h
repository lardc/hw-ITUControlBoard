#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
#include "Global.h"

// Variables
extern volatile Int64U CONTROL_TimeCounter;

extern float MEMBUF_Values_V[];
extern float MEMBUF_Values_I1[];
extern float MEMBUF_Values_I2[];
extern float MEMBUF_Values_I3[];
extern float MEMBUF_Values_I4[];

extern float MEMBUF_Values_Vrms[];
extern float MEMBUF_Values_Irms1[];
extern float MEMBUF_Values_Irms2[];
extern float MEMBUF_Values_Irms3[];
extern float MEMBUF_Values_Irms4[];

extern Int16S MEMBUF_Values_CosPhi1[];
extern Int16S MEMBUF_Values_CosPhi2[];
extern Int16S MEMBUF_Values_CosPhi3[];
extern Int16S MEMBUF_Values_CosPhi4[];

extern Int16S MEMBUF_Values_PWM[];
extern Int16S MEMBUF_Values_Err[];

extern Int16U MEMBUF_ScopeValues_Counter, MEMBUF_ErrorValues_Counter;

// Functions
void CONTROL_Init();
void CONTROL_Idle();
void CONTROL_RequestStop();

#endif // __CONTROLLER_H
