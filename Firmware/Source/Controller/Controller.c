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
//
float MEMBUF_Values_V[VALUES_x_SIZE];
float MEMBUF_Values_I1[VALUES_x_SIZE];
float MEMBUF_Values_I2[VALUES_x_SIZE];
float MEMBUF_Values_I3[VALUES_x_SIZE];
float MEMBUF_Values_I4[VALUES_x_SIZE];

float MEMBUF_Values_Vrms[VALUES_x_SIZE];
float MEMBUF_Values_I1rms[VALUES_x_SIZE];
float MEMBUF_Values_I2rms[VALUES_x_SIZE];
float MEMBUF_Values_I3rms[VALUES_x_SIZE];
float MEMBUF_Values_I4rms[VALUES_x_SIZE];

Int16U MEMBUF_Values_CosPhi1[VALUES_x_SIZE];
Int16U MEMBUF_Values_CosPhi2[VALUES_x_SIZE];
Int16U MEMBUF_Values_CosPhi3[VALUES_x_SIZE];
Int16U MEMBUF_Values_CosPhi4[VALUES_x_SIZE];

Int16U MEMBUF_Values_PWM[VALUES_x_SIZE];
Int16U MEMBUF_Values_Err[VALUES_x_SIZE];

Int16U MEMBUF_ScopeValues_Counter = 0, MEMBUF_ErrorValues_Counter = 0;

// Forward functions
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
	Int16U EPIndexes[EP_COUNT] = {EP_COS_PHI1, EP_COS_PHI2, EP_COS_PHI3, EP_COS_PHI4,
			EP_PWM, EP_VOLTAGE_ERROR};

	Int16U EPSized[EP_COUNT] = {VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE,
			VALUES_x_SIZE, VALUES_x_SIZE};

	// Сокращения
	pInt16U sv = &MEMBUF_ScopeValues_Counter;
	pInt16U EPCounters[EP_COUNT] = {sv, sv, sv, sv, sv, &MEMBUF_ErrorValues_Counter};

	pInt16U EPDatas[EP_COUNT] = {MEMBUF_Values_CosPhi1, MEMBUF_Values_CosPhi2,
			MEMBUF_Values_CosPhi3, MEMBUF_Values_CosPhi4,
			MEMBUF_Values_PWM, MEMBUF_Values_Err};

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

		case ACT_START:
			break;

		case ACT_STOP:
			{
				if(CONTROL_State == DS_InProcess)
				{
					// Стандартная процедура завершения
					if(CONTROL_SubState == DSS_None)
						CONTROL_SetDeviceState(DS_InProcess, DSS_RequestStop);
					// Прерывание запуска
					else if(CONTROL_SubState == DSS_ConnectRelays)
					{
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
}
//------------------------------------------

void CONTROL_ProcessPWMStop(uint16_t Problem)
{
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
