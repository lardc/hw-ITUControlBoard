#ifndef __INITCONFIG_H
#define __INITCONFIG_H

// Include
#include "stdinc.h"

//Functions
Boolean INITCFG_ConfigSystemClock();
void INITCFG_ConfigInterrupt();
void INITCFG_ConfigIO();
void INITCFG_ConfigCAN();
void INITCFG_ConfigUART();
void INITCFG_ConfigTimer7();
void INITCFG_ConfigWatchDog();
void INITCFG_PWM();
void INITCFG_DMA();
void INITCFG_ADC();

#endif //__INITCONFIG_H
