// Header
#include "Diagnostic.h"

// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "LowLevel.h"
#include "Delay.h"
#include "Controller.h"

// Forward functions
void DBGACT_GenerateImpulseLineSync1();
void DBGACT_GenerateImpulseLineSync2();
void DBGACT_SetStatePOWRelay(bool NewState);
void DBGACT_SetStateCTRLRelay(bool NewState);

// Functions
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch (ActionID)
	{
		case ACT_DBG_SYNC_1_IMPULSE:
			{
				DBGACT_GenerateImpulseLineSync1();
			}
			break;

		case ACT_DBG_SYNC_2_IMPULSE:
			{
				DBGACT_GenerateImpulseLineSync2();
			}
			break;

		case ACT_DBG_SET_STATE_POW_RELAY:
			{
				LL_ConnectPOWRelay(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_SET_STATE_CTRL_RELAY:
			{
				LL_ConnectCTRLRelay(DataTable[REG_DBG]);
			}
			break;

		default:
			return false;
	}
	return true;
}
//-----------------------------

void DBGACT_GenerateImpulseLineSync1()
{
	LL_SetSync1State(true);
	DELAY_MS(10);
	LL_SetSync1State(false);
}
//-----------------------------

void DBGACT_GenerateImpulseLineSync2()
{
	LL_SetSync2State(true);
	DELAY_MS(10);
	LL_SetSync2State(false);
}
//-----------------------------

void DBGACT_SetStatePOWRelay(bool NewState)
{
	LL_ConnectPOWRelay(NewState);
}
//-----------------------------
void DBGACT_SetStateCTRLRelay(bool NewState)
{
	LL_ConnectCTRLRelay(NewState);
}
//-----------------------------
