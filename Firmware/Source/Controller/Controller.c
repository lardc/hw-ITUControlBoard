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
#include "PWM.h"
#include "Measure.h"

// Types
typedef enum __DeviceState
{
	DS_None = 0,
	DS_Fault = 1,
	DS_Disabled = 2,
	DS_Ready = 3,
	DS_InProcess = 4
} DeviceState;

typedef enum __DeviceSubState
{
	DSS_None = 0,

	DSS_RequestStart = 1,
	DSS_ConnectRelays = 2,

	DSS_RequestStop = 3,
	DSS_RequestDisconnect = 4,
	DSS_DisconnectRelays = 5,
	DSS_WaitDisconnection = 6
} DeviceSubState;

typedef void (*FUNC_AsyncDelegate)();

// Variables
//
DeviceSubState CONTROL_SubState = DSS_None;
volatile DeviceState CONTROL_State = DS_None;
static Boolean CycleActive = false;
volatile Int64U CONTROL_TimeCounter = 0;
// Storage
volatile Int16U CONTROL_VSetFast[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_PWMSetFast[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_VResultFast[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_IResultFast[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_VSetRMS[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_VControlRMS[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_VResultRMS[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_IResultRMS[VALUES_x_SIZE] = {0};
volatile Int16U CONTROL_CounterFast = 0;
volatile Int16U CONTROL_CounterRMS = 0;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetResults();
void CONTROL_ResetToDefaultState();
void CONTROL_ProcessSubStates();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_V_SETPOINT_FAST, EP_PWM_SETPOINT_FAST, EP_V_RESULT_FAST, EP_I_RESULT_FAST,
			EP_VRMS_SETPOINT, EP_VRMS_CTRL_SETPOINT, EP_VRMS_RESULT, EP_IRMS_RESULT};

	Int16U EPSized[EP_COUNT] = {VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE,
			VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};

	// Сокращения
	pInt16U cf = (pInt16U)&CONTROL_CounterFast;
	pInt16U cr = (pInt16U)&CONTROL_CounterRMS;
	pInt16U EPCounters[EP_COUNT] = {cf, cf, cf, cf, cr, cr, cr, cr};

	pInt16U EPDatas[EP_COUNT] = {(pInt16U)CONTROL_VSetFast, (pInt16U)CONTROL_PWMSetFast,
			(pInt16U)CONTROL_VResultFast, (pInt16U)CONTROL_IResultFast,
			(pInt16U)CONTROL_VSetRMS, (pInt16U)CONTROL_VControlRMS,
			(pInt16U)CONTROL_VResultRMS, (pInt16U)CONTROL_IResultRMS};

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SLAVE_NID, 0);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();
}
//------------------------------------------

void CONTROL_ResetResults()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;

	DataTable[REG_VOLTAGE_RESULT] = 0;
	DataTable[REG_VOLTAGE_RESULT_32] = 0;
	DataTable[REG_CURRENT_RESULT] = 0;
	DataTable[REG_CURRENT_RESULT_32] = 0;
	DataTable[REG_VOLTAGE_READY] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	LL_SetSync1State(false);
	LL_SetSync2State(false);
	LL_ConnectPOWRelay(false);
	LL_ConnectCTRLRelay(false);

	CONTROL_ResetResults();
	CONTROL_SetDeviceState(DS_None, DSS_None);
}
//------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();

	CONTROL_ProcessSubStates();
}
//------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if(CONTROL_State == DS_None)
					CONTROL_SetDeviceState(DS_Ready, DSS_None);
				else if(CONTROL_State != DS_Ready)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if(CONTROL_State == DS_Ready)
					CONTROL_ResetToDefaultState();
				else if(CONTROL_State != DS_None)
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;

		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
					CONTROL_ResetToDefaultState();
			}
			break;

		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_START_SIGNAL:
			{
				if(CONTROL_State == DS_Ready)
				{
					if(MEASURE_InputParametersCorrect())
						CONTROL_SetDeviceState(DS_InProcess, DSS_RequestStart);
					else
						*pUserError = ERR_BAD_CONFIG;
				}
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;

		case ACT_STOP_SIGNAL:
			{
				if(CONTROL_State == DS_InProcess)
				{
					// Стандартная процедура завершения
					if(CONTROL_SubState == DSS_None)
						CONTROL_SetDeviceState(DS_InProcess, DSS_RequestStop);
					// Прерывание запуска
					else if(CONTROL_SubState == DSS_ConnectRelays)
					{
						LL_OutputSelector(AC_None);
						CONTROL_SetDeviceState(DS_Ready, DSS_None);
					}
				}
			}
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, pUserError);
			
	}
	return true;
}
//------------------------------------------

void CONTROL_ProcessSubStates()
{
	static Int64U Timeout = 0;
	if(CONTROL_State == DS_InProcess)
	{
		switch(CONTROL_SubState)
		{
			case DSS_RequestStart:
				{
					CONTROL_ResetResults();
					LL_OutputSelector(DataTable[REG_OUTPUT_LINE]);
					MEASURE_SetMeasureRange();
					PWM_CacheParameters();

					Timeout = CONTROL_TimeCounter + RELAY_SWITCH_DELAY;
					CONTROL_SetDeviceState(DS_InProcess, DSS_ConnectRelays);
				}
				break;

			case DSS_ConnectRelays:
				{
					if(CONTROL_TimeCounter > Timeout)
					{
						PWM_SignalStart();
						CONTROL_SetDeviceState(DS_InProcess, DSS_None);
					}
				}
				break;

			case DSS_RequestStop:
				{
					PWM_SignalStop();
				}
				break;

			case DSS_RequestDisconnect:
				{
					Timeout = CONTROL_TimeCounter + RELAY_SWITCH_DELAY;
					CONTROL_SetDeviceState(DS_InProcess, DSS_DisconnectRelays);
				}
				break;

			case DSS_DisconnectRelays:
				{
					if(CONTROL_TimeCounter > Timeout)
					{
						LL_OutputSelector(AC_None);
						Timeout = CONTROL_TimeCounter + RELAY_SWITCH_DELAY;
						CONTROL_SetDeviceState(DS_InProcess, DSS_WaitDisconnection);
					}
				}
				break;

			case DSS_WaitDisconnection:
				{
					if(CONTROL_TimeCounter > Timeout)
						CONTROL_SetDeviceState(DS_Ready, DSS_None);
				}
				break;

			default:
				break;
		}
	}
}
//------------------------------------------

void CONTROL_ProcessPWMStop(uint16_t Problem)
{
	DataTable[REG_OP_RESULT] = (Problem == PROBLEM_NONE) ? OPRESULT_OK : OPRESULT_FAIL;
	DataTable[REG_PROBLEM] = Problem;
	CONTROL_SetDeviceState(DS_InProcess, DSS_RequestDisconnect);
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;

	CONTROL_SubState = NewSubState;
	DataTable[REG_SUB_STATE] = NewSubState;
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------
