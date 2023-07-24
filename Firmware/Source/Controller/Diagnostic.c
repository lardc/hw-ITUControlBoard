// Header
#include "Diagnostic.h"

// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "LowLevel.h"
#include "Delay.h"
#include "Controller.h"

// Functions
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch (ActionID)
	{
		case ACT_DBG_SWITCH_R0:
			LL_SelectCurrentChannel(CC_R0);
			break;

		case ACT_DBG_SWITCH_R1:
			LL_SelectCurrentChannel(CC_R1);
			break;

		case ACT_DBG_SWITCH_R2:
			LL_SelectCurrentChannel(CC_R2);
			break;

		case ACT_DBG_PULSE_EXT_LED:
			LL_EnableExtLed(true);
			DELAY_US(1000000);
			LL_EnableExtLed(false);
			break;

		case ACT_DBG_PULSE_FAN:
			LL_EnableFan(true);
			DELAY_US(1000000);
			LL_EnableFan(false);
			break;

		case ACT_DBG_READ_SAFETY:
			DataTable[REG_DBG_GENERAL] = LL_IsSafetyOK() ? 1 : 0;
			break;

		case ACT_DBG_READ_INPUT_V_ADC:
			DataTable[REG_DBG_GENERAL] = LL_ReadInputVoltageADC();
			break;

		case ACT_DBG_SET_DAC_OUTPUT:
			LL_SetDACOutput((Int16U)DataTable[REG_DBG_GENERAL]);
			break;

		default:
			return false;
	}
	return true;
}
//-----------------------------
