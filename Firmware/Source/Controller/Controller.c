// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "Delay.h"
#include "DeviceProfile.h"
#include "Interrupts.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "Diagnostic.h"
#include "BCCIxParams.h"
#include "MeasureAC.h"
#include "MeasureUtils.h"
#include <math.h>

// Types
typedef enum __DeviceState
{
	DS_None = 0,
	DS_Fault = 1,
	DS_Disabled = 2,
	DS_Ready = 3,
	DS_InProcess = 4
} DeviceState;

typedef void (*FUNC_AsyncDelegate)();

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
static Boolean CycleActive = false;
volatile Int64U CONTROL_TimeCounter = 0;
static float LastBatteryVoltage = 0;

// Storage
//
float MEMBUF_Values_V[VALUES_x_SIZE];
float MEMBUF_Values_I1[VALUES_x_SIZE];
float MEMBUF_Values_I2[VALUES_x_SIZE];
float MEMBUF_Values_I3[VALUES_x_SIZE];
float MEMBUF_Values_I4[VALUES_x_SIZE];

float MEMBUF_Values_Vrms[VALUES_x_SIZE];
float MEMBUF_Values_Irms1[VALUES_x_SIZE];
float MEMBUF_Values_Irms2[VALUES_x_SIZE];
float MEMBUF_Values_Irms3[VALUES_x_SIZE];
float MEMBUF_Values_Irms4[VALUES_x_SIZE];

Int16S MEMBUF_Values_CosPhi1[VALUES_x_SIZE];
Int16S MEMBUF_Values_CosPhi2[VALUES_x_SIZE];
Int16S MEMBUF_Values_CosPhi3[VALUES_x_SIZE];
Int16S MEMBUF_Values_CosPhi4[VALUES_x_SIZE];

Int16S MEMBUF_Values_PWM[VALUES_x_SIZE];
Int16S MEMBUF_Values_Err[VALUES_x_SIZE];

Int16U MEMBUF_ScopeValues_Counter = 0, MEMBUF_ErrorValues_Counter = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState);
void CONTROL_SwitchStateToFault(Int16U FaultReason);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetResults();
void CONTROL_ResetToDefaultState();
void CONTROL_StartSequence();

// Functions
//
void CONTROL_Init()
{
	// Целочисленные EP
	Int16U EPIndexes[EP_COUNT] = {EP_COS_PHI1, EP_COS_PHI2, EP_COS_PHI3, EP_COS_PHI4,
			EP_PWM, EP_VOLTAGE_ERROR};

	Int16U EPSized[EP_COUNT] = {VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE,
			VALUES_x_SIZE, VALUES_x_SIZE};

	pInt16U sv = &MEMBUF_ScopeValues_Counter;
	pInt16U EPCounters[EP_COUNT] = {sv, sv, sv, sv, sv, &MEMBUF_ErrorValues_Counter};

	pInt16U EPDatas[EP_COUNT] = {(pInt16U)MEMBUF_Values_CosPhi1, (pInt16U)MEMBUF_Values_CosPhi2,
			(pInt16U)MEMBUF_Values_CosPhi3, (pInt16U)MEMBUF_Values_CosPhi4,
			(pInt16U)MEMBUF_Values_PWM, (pInt16U)MEMBUF_Values_Err};

	// Float EP
	Int16U FEPIndexes[FEP_COUNT] = {FEP_VOLTAGE, FEP_CURRENT1, FEP_CURRENT2, FEP_CURRENT3, FEP_CURRENT4,
			FEP_VOLTAGE_RMS, FEP_CURRENT_RMS1, FEP_CURRENT_RMS2, FEP_CURRENT_RMS3, FEP_CURRENT_RMS4};

	Int16U FEPSized[FEP_COUNT] = {VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE,
			VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};

	pInt16U FEPCounters[FEP_COUNT] = {sv, sv, sv, sv, sv, sv, sv, sv, sv, sv};

	pFloat32 FEPDatas[FEP_COUNT] = {MEMBUF_Values_V, MEMBUF_Values_I1, MEMBUF_Values_I2,
			MEMBUF_Values_I3, MEMBUF_Values_I4,
			MEMBUF_Values_Vrms, MEMBUF_Values_Irms1, MEMBUF_Values_Irms2,
			MEMBUF_Values_Irms3, MEMBUF_Values_Irms4};

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};

	// Инициализация data table
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SLAVE_NID, 0);

	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	DEVPROFILE_InitFEPService(FEPIndexes, FEPSized, FEPCounters, FEPDatas);

	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();
}
//------------------------------------------

void CONTROL_ResetResults()
{
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_FINISHED] = OPRESULT_NONE;
	DataTable[REG_VOLTAGE_READY] = 0;
	DataTable[REG_TEST_PASSED] = 0;
	DataTable[REG_FAILED_CURRENT_CHANNEL] = 0;

	// Очистка результатов
	for(int i = 200; i <= 220; i++)
		DataTable[i] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;

	CONTROL_ResetResults();
	CONTROL_SetDeviceState(DS_None);
}
//------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();

	// Оцифровка напряжения батареи
	static Int64U LastBatterySample = 0;
	if(CONTROL_TimeCounter > LastBatterySample + PRIMARY_SAMPLE_DELAY)
	{
		LastBatterySample = CONTROL_TimeCounter;
		DataTable[REG_PRIMARY_SIDE_VOLTAGE] = LastBatteryVoltage = MU_GetPrimarySideVoltage();
	}

	// Обработка контура безопасности
	bool SafetyOK = LL_IsSafetyOK();
	DataTable[REG_SAFETY_IS_OK] = SafetyOK ? 1 : 0;
	if(!DataTable[REG_IGNORE_HW_SAFETY] && !SafetyOK && CONTROL_State == DS_InProcess)
		MAC_RequestStop(PBR_RequestSoftStop);

	// Обработка вентилятора
	static Int64U FanTurnOff = 0;
	if(CONTROL_State == DS_InProcess)
	{
		LL_EnableFan(true);
		FanTurnOff = CONTROL_TimeCounter + (Int64U)(DataTable[REG_FAN_ON_TIME] * 1000);
	}
	else if(CONTROL_TimeCounter > FanTurnOff)
		LL_EnableFan(false);
}
//------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			if(CONTROL_State == DS_None)
				CONTROL_SetDeviceState(DS_Ready);
			else if(CONTROL_State != DS_Ready)
				*pUserError = ERR_DEVICE_NOT_READY;
			break;

		case ACT_DISABLE_POWER:
			if(CONTROL_State == DS_InProcess)
				*pUserError = ERR_OPERATION_BLOCKED;
			else
				CONTROL_SetDeviceState(DS_None);
			break;

		case ACT_START:
			if(CONTROL_State == DS_InProcess)
				*pUserError = ERR_OPERATION_BLOCKED;
			else if(CONTROL_State == DS_Ready)
			{
				// Проверка попадания тока в доступный диапазон
				float LimitIrms = DataTable[REG_LIMIT_CURRENT_mA] + DataTable[REG_LIMIT_CURRENT_uA] * 0.001f;
				if(LimitIrms > DataTable[REG_I_RANGE_HIGH])
					*pUserError = ERR_BAD_CONFIG;
				else
				{
					CONTROL_ResetResults();
					CONTROL_StartSequence();
				}
			}
			else
				*pUserError = ERR_DEVICE_NOT_READY;
			break;

		case ACT_STOP:
			if(CONTROL_State == DS_InProcess)
				MAC_RequestStop(PBR_RequestSoftStop);
			break;

		case ACT_FAULT_CLEAR:
			if(CONTROL_State == DS_Fault)
				CONTROL_ResetToDefaultState();
			break;

		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, pUserError);
	}
	return true;
}
//------------------------------------------

void CONTROL_RequestStop()
{
	CONTROL_SetDeviceState(DS_Ready);
}
//------------------------------------------

void CONTROL_StartSequence()
{
	// Проверка напряжения первичной стороны
	float RelativeVoltage = fabsf(LastBatteryVoltage - DataTable[REG_PRIM_VOLTAGE]) / DataTable[REG_PRIM_VOLTAGE];
	if(fabsf(1.0f - RelativeVoltage) <= DataTable[REG_PRIM_VOLTAGE_MAX_ERR] / 100)
	{
		MU_StartScope();
		MAC_StartProcess();
		CONTROL_SetDeviceState(DS_InProcess);
	}
	else
		CONTROL_SwitchStateToFault(DF_PRIMARY_VOLTAGE);
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	DataTable[REG_DEV_STATE] = CONTROL_State = NewState;
	LL_EnableExtLed(NewState == DS_InProcess);
}
//------------------------------------------

void CONTROL_SwitchStateToFault(Int16U FaultReason)
{
	DataTable[REG_FAULT_REASON] = FaultReason;
	CONTROL_SetDeviceState(DS_Fault);
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------
