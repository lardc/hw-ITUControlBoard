// Header
#include "LowLevel.h"

// Includes
#include "Board.h"
#include "SysConfig.h"
#include "Global.h"

// Functions
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_LED);
}
//-----------------------------

void LL_DMAReload()
{
	DMA_ChannelReload(DMA_ADC1_I_CHANNEL34, ADC_DMA_CURRENT_SAMLES);
	DMA_ChannelReload(DMA_ADC2_I_CHANNEL12, ADC_DMA_CURRENT_SAMLES);
	DMA_ChannelReload(DMA_ADC3_V_CHANNEL, ADC_DMA_VOLTAGE_SAMLES);

	DMA_ChannelEnable(DMA_ADC1_I_CHANNEL34, true);
	DMA_ChannelEnable(DMA_ADC2_I_CHANNEL12, true);
	DMA_ChannelEnable(DMA_ADC3_V_CHANNEL, true);
}
//-----------------------------

In16U LL_ReadInputVoltageADC()
{
	return ADC_Measure(ADC4, ADC4_IN_VOLTAGE_CHANNEL);
}
//-----------------------------

void LL_SelectCurrentChannel(CurrentChannel Channel)
{
	switch(Channel)
	{
		case CC_R0:
			GPIO_SetState(GPIO_R0, true);
			GPIO_SetState(GPIO_R1, false);
			GPIO_SetState(GPIO_R2, false);
			break;

		case CC_R1:
			GPIO_SetState(GPIO_R0, false);
			GPIO_SetState(GPIO_R1, true);
			GPIO_SetState(GPIO_R2, false);
			break;

		case CC_R2:
			GPIO_SetState(GPIO_R0, false);
			GPIO_SetState(GPIO_R1, false);
			GPIO_SetState(GPIO_R2, true);
			break;

		default:
			break;
	}
}
//-----------------------------

void LL_EnableExtLed(bool NewState)
{
	GPIO_SetState(GPIO_EXT_IND, NewState);
}
//-----------------------------

void LL_EnableFan(bool NewState)
{
	GPIO_SetState(GPIO_FAN, NewState);
}
//-----------------------------

bool LL_IsSafetyOK()
{
	return GPIO_GetState(GPIO_SAFETY);
}
//-----------------------------
