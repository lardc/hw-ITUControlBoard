// Header
#include "Interrupts.h"

// Includes
#include "Controller.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "Global.h"
#include "Board.h"
#include "ZwDMA.h"
#include "PWM.h"

// Variables
static volatile bool VoltageSamplingDone = false;
static volatile bool CurrentSamplingDone = false;

// Forward functions
void INT_CheckCompleteCondition();

// Functions
void USART1_IRQHandler()
{
	if(ZwSCI_RecieveCheck(USART1))
	{
		ZwSCI_RegisterToFIFO(USART1);
		ZwSCI_RecieveFlagClear(USART1);
	}
}
//-----------------------------------------

void USB_LP_CAN_RX0_IRQHandler()
{
	if(NCAN_RecieveCheck())
	{
		NCAN_RecieveData();
		NCAN_RecieveFlagReset();
	}
}
//-----------------------------------------

void TIM7_IRQHandler()
{
	static uint16_t LED_BlinkTimeCounter = 0;
	
	if(TIM_StatusCheck(TIM7))
	{
		CONTROL_TimeCounter++;
		if(++LED_BlinkTimeCounter > TIME_LED_BLINK)
		{
			LL_ToggleBoardLED();
			LED_BlinkTimeCounter = 0;
		}
		
		TIM_StatusClear(TIM7);
	}
}
//-----------------------------------------

void TIM1_UP_TIM16_IRQHandler()
{
	if(TIM_StatusCheck(TIM1))
	{
		if(++PWMTimerCounter == PWM_SINE_COUNTER_MAX)
			PWMTimerCounter = 0;

		ADC_SamplingStart(ADC1);
		TIM_StatusClear(TIM1);
	}
}
//-----------------------------------------

void DMA1_Channel1_IRQHandler()
{
	if(DMA_IsTransferComplete(DMA1, DMA_ISR_TCIF1))
	{
		VoltageSamplingDone = true;
		INT_CheckCompleteCondition();
		DMA_TransferCompleteReset(DMA1, DMA_IFCR_CGIF1);
	}
}
//-----------------------------------------

void DMA2_Channel1_IRQHandler()
{
	if(DMA_IsTransferComplete(DMA2, DMA_ISR_TCIF1))
	{
		CurrentSamplingDone = true;
		INT_CheckCompleteCondition();
		DMA_TransferCompleteReset(DMA2, DMA_IFCR_CGIF1);
	}
}
//-----------------------------------------

void INT_CheckCompleteCondition()
{
	if(VoltageSamplingDone && CurrentSamplingDone)
	{
		VoltageSamplingDone = CurrentSamplingDone = false;
		LL_DMAReload();

		uint16_t Problem;
		if(PWM_SinRegulation(&Problem))
			CONTROL_ProcessPWMStop(Problem);
	}
}
//-----------------------------------------
