// Header
#include "Interrupts.h"

// Includes
#include "Controller.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "Global.h"
#include "Board.h"
#include "ZwDMA.h"
#include "MeasureAC.h"

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

void DMA1_Channel1_IRQHandler()
{
	if(DMA_IsTransferComplete(DMA1, DMA_ISR_TCIF1))
	{
		DMA_TransferCompleteReset(DMA1, DMA_IFCR_CGIF1);
		LL_DMAReload();
		MAC_ControlCycle();
	}
}
//-----------------------------------------
