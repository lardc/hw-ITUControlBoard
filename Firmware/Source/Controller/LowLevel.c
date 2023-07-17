// Header
#include "LowLevel.h"

// Includes
#include "Board.h"
#include "Delay.h"
#include "SysConfig.h"

// Functions
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_LED);
}
//-----------------------------

void LL_SetSync1State(bool NewState)
{
	GPIO_SetState(GPIO_CTRL_SYNC_1, NewState);
}
//-----------------------------

void LL_SetSync2State(bool NewState)
{
	GPIO_SetState(GPIO_CTRL_SYNC_2, NewState);
}
//-----------------------------

void LL_ConnectPOWRelay(bool NewState)
{
	GPIO_SetState(GPIO_POW_RELAY, NewState);
}
//-----------------------------

void LL_ConnectCTRLRelay(bool NewState)
{
	GPIO_SetState(GPIO_CTRL_RELAY, NewState);
}
//-----------------------------

void LL_OutputSelector(ACV_OutputLine OutputLine)
{
	switch(OutputLine)
	{
		case AC_None:
			{
				LL_ConnectPOWRelay(false);
				LL_ConnectCTRLRelay(false);
			}
			break;

		case AC_BUS_LV:
			{
				LL_ConnectPOWRelay(true);
				LL_ConnectCTRLRelay(false);
			}
			break;

		case AC_CTRL:
			{
				LL_ConnectPOWRelay(false);
				LL_ConnectCTRLRelay(true);
			}
			break;
	}
}
//-----------------------------

void LL_EnablePWMOut(bool NewState)
{
	GPIO_SetState(GPIO_CTRL_PWMSD_1, NewState);
	GPIO_SetState(GPIO_CTRL_PWMSD_2, NewState);
}
//-----------------------------

void LL_DMAReload()
{
	DMA_ChannelReload(DMA_ADC1_V_CHANNEL, ADC_DMA_BUFF_SIZE);
	DMA_ChannelReload(DMA_ADC2_I_CHANNEL, ADC_DMA_BUFF_SIZE);

	DMA_ChannelEnable(DMA_ADC1_V_CHANNEL, true);
	DMA_ChannelEnable(DMA_ADC2_I_CHANNEL, true);
}
//-----------------------------

void LL_SetStateRedLed(bool NewState)
{
	GPIO_SetState(GPIO_LED_EXT, NewState);
}
//-----------------------------
