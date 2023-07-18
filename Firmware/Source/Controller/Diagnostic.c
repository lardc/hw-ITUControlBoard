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
		default:
			return false;
	}
	return true;
}
//-----------------------------
