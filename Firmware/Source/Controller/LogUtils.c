// Header
#include "LogUtils.h"
//
// Includes
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "Controller.h"
#include "Global.h"
#include "Measure.h"

// Functions
void MU_LogFast(float VSet, float PWMSet, float ResultV, float ResultI)
{
	static uint16_t ScopeLogStep = 0, LocalCounter = 0;
	
	// Сброс локального счётчика в начале логгирования
	if(CONTROL_CounterFast == 0)
		LocalCounter = 0;
	
	if(ScopeLogStep++ >= DataTable[REG_SCOPE_RATE])
	{
		ScopeLogStep = 0;
		
		CONTROL_VSetFast[LocalCounter] = (int16_t)VSet;
		CONTROL_PWMSetFast[LocalCounter] = (int16_t)PWMSet;
		CONTROL_VResultFast[LocalCounter] = (int16_t)ResultV;
		CONTROL_IResultFast[LocalCounter] = (int16_t)(ResultI / (MEASURE_InMilliAmperes ? 10 : 1));
		
		// Сохранение указателя на последний элемент
		DataTable[REG_EP_LAST_POINTER_FAST] = LocalCounter;
		++LocalCounter;
	}
	
	// Условие обновления глобального счётчика данных
	if(CONTROL_CounterFast < VALUES_x_SIZE)
		CONTROL_CounterFast = LocalCounter;
	
	// Сброс локального счётчика
	if(LocalCounter >= VALUES_x_SIZE)
		LocalCounter = 0;
}
// ----------------------------------------

// Functions
void MU_LogRMS(float VSet, float VControlSet, float ResultV, float ResultI)
{
	static uint16_t ScopeLogStep = 0, LocalCounter = 0;

	// Сброс локального счётчика в начале логгирования
	if(CONTROL_CounterRMS == 0)
		LocalCounter = 0;

	if(ScopeLogStep++ >= DataTable[REG_SCOPE_RATE])
	{
		ScopeLogStep = 0;

		CONTROL_VSetRMS[LocalCounter] = (int16_t)VSet;
		CONTROL_VControlRMS[LocalCounter] = (int16_t)VControlSet;
		CONTROL_VResultRMS[LocalCounter] = (int16_t)ResultV;
		CONTROL_IResultRMS[LocalCounter] = (int16_t)(ResultI / (MEASURE_InMilliAmperes ? 10 : 1));

		// Сохранение указателя на последний элемент
		DataTable[REG_EP_LAST_POINTER_RMS] = LocalCounter;
		++LocalCounter;
	}

	// Условие обновления глобального счётчика данных
	if(CONTROL_CounterRMS < VALUES_x_SIZE)
		CONTROL_CounterRMS = LocalCounter;

	// Сброс локального счётчика
	if(LocalCounter >= VALUES_x_SIZE)
		LocalCounter = 0;
}
// ----------------------------------------
