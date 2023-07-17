// Header
#include "PWM.h"

// Includes
#include "Measure.h"
#include "Timer1PWM.h"
#include "math.h"
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "SysConfig.h"
#include "LogUtils.h"
#include "LowLevel.h"

// Types
typedef struct __VIPair
{
	float Voltage;
	float Current;
} VIPair, pVIPair;

typedef struct __CoefficientsRMS
{
	float P2;
	float P1;
	float P0;
} CoefficientsRMS;

// Variables
volatile uint32_t PWMTimerCounter = 0;
static float TransformerRatio, Kp, Ki, ErrorI;
static float ActualSetVoltageRMS, ControlSetVoltageRMS, ControlSetVoltageMaxRMS, TargetVoltageRMS, VoltageStepRMS;
static float VoltageStorageRMS, CurrentStorageRMS;
static float CurrentLimitRMS;
static bool RequestSoftStop = false, FollowingErrorEnable;
static float FollowingErrorRelativeLevel, FollowingErrorAbsoluteLevel, VoltageReadyErrorLevel;
static uint16_t FollowingErrorCounter, FollowingErrorCounterMax;
static volatile CoefficientsRMS Voltage, Current;


// Forward functions
void PWM_CacheParameters();
float PWM_GetInstantVoltageSetpoint();
float PWM_ConvertVoltageToPWM(float Voltage);
float PWM_GetControlAdjustment(float Error);
void PWM_SaveResultToDataTable(VIPair Pair);
VIPair PWM_GetMeasureData();
void PWM_AddToRMS(VIPair Pair);
VIPair PWM_CalculateRMSValue();
void PWM_ProcessPeriodRegulation(uint16_t *Problem);
void PWM_ProcessInstantPWMOutput(VIPair Pair);
void PWM_CacheRMSCoefficients();

// Functions
void PWM_SignalStart()
{
	LL_SetStateRedLed(true);
	T1PWM_Start();
}
//------------------------------------------------

void PWM_SignalStop()
{
	RequestSoftStop = true;
}
//------------------------------------------------

bool PWM_SinRegulation(uint16_t *Problem)
{
	*Problem = PROBLEM_NONE;
	VIPair Sample = PWM_GetMeasureData();

	// Защита от КЗ
	if(fabsf(Sample.Current) > MEASURE_GetCurrentPeakLimit())
	{
		// "Быстрое" преобразование в RMS
		Sample.Current = fabsf(Sample.Current) * M_SQRT2;
		Sample.Voltage = fabsf(Sample.Voltage) * M_SQRT2;

		PWM_SaveResultToDataTable(Sample);
		DataTable[REG_WARNING] = WARNING_INSTANT_OVERCURRENT;
		*Problem = PROBLEM_NONE;

		T1PWM_Stop();
		return true;
	}

	PWM_AddToRMS(Sample);
	PWM_ProcessPeriodRegulation(Problem);
	PWM_ProcessInstantPWMOutput(Sample);
	return RequestSoftStop;
}
//------------------------------------------------

void PWM_SaveResultToDataTable(VIPair Pair)
{
	DT_Write32(REG_VOLTAGE_RESULT, REG_VOLTAGE_RESULT_32, Pair.Voltage * 1000);
	DT_Write32(REG_CURRENT_RESULT, REG_CURRENT_RESULT_32, Pair.Current);
}
//------------------------------------------------

float PWM_GetControlAdjustment(float Error)
{
	// Если интегральная составляющая задана
	if(Ki)
	{
		ErrorI += Error;
		// Проверка насыщения интегральной ошибки
		if(fabsf(ErrorI) > REGULATOR_I_ERR_SAT)
			ErrorI = (ErrorI > 0) ? REGULATOR_I_ERR_SAT : -REGULATOR_I_ERR_SAT;
	}

	return Error * Kp + ErrorI * Ki;
}
//------------------------------------------------

void PWM_ProcessPeriodRegulation(uint16_t *Problem)
{
	if(PWMTimerCounter == 0)
	{
		VIPair ValuesRMS = PWM_CalculateRMSValue();

		// Сохранение результата при остановке
		if(RequestSoftStop)
			PWM_SaveResultToDataTable(ValuesRMS);

		// Расчёт величины ошибки
		float Error = ActualSetVoltageRMS - ValuesRMS.Voltage;
		float RelativeError = fabsf(Error / ActualSetVoltageRMS);

		// Расчёт FollowingError
		if(FollowingErrorEnable)
		{
			if((RelativeError >= FollowingErrorRelativeLevel) && (fabsf(Error) >= FollowingErrorAbsoluteLevel))
			{
				if(++FollowingErrorCounter >= FollowingErrorCounterMax)
				{
					*Problem = PROBLEM_FOLLOWING_ERROR;
					RequestSoftStop = true;
				}
			}
			else
				FollowingErrorCounter = 0;
		}

		// Проверка готовности напряжения
		if((VoltageReadyErrorLevel >= RelativeError) && (ActualSetVoltageRMS == TargetVoltageRMS))
			DataTable[REG_VOLTAGE_READY] = 1;

		// Получение корректировки по завершённому периоду
		float Control = PWM_GetControlAdjustment(Error);

		// Алгоритм нарастания уставки напряжения
		if(ActualSetVoltageRMS < TargetVoltageRMS)
		{
			ActualSetVoltageRMS += VoltageStepRMS;
			if(ActualSetVoltageRMS > TargetVoltageRMS)
				ActualSetVoltageRMS = TargetVoltageRMS;
		}

		// Задание действующего значения напряжения для следующего импульса
		ControlSetVoltageRMS = ActualSetVoltageRMS + Control;

		// Проверка условий остановки формирования
		// Проверка насыщения выходного напряжения
		if(ControlSetVoltageRMS > ControlSetVoltageMaxRMS)
		{
			*Problem = PROBLEM_OUTPUT_SATURATION;
			RequestSoftStop = true;
		}
		// Проверка превышения действующего значения тока
		else if(ValuesRMS.Current > CurrentLimitRMS)
		{
			PWM_SaveResultToDataTable(ValuesRMS);
			DataTable[REG_WARNING] = WARNING_RMS_OVER_CURRENT;
			*Problem = PROBLEM_NONE;
			RequestSoftStop = true;
		}

		// Сохранение полученных значений
		MU_LogRMS(ActualSetVoltageRMS, ControlSetVoltageRMS, ValuesRMS.Voltage, ValuesRMS.Current);
	}
}
//------------------------------------------------

void PWM_ProcessInstantPWMOutput(VIPair Pair)
{
	// Расчёт, сохранение и запись уставки ШИМ
	float InstantVoltage = PWM_GetInstantVoltageSetpoint();
	float PWMSetpoint = PWM_ConvertVoltageToPWM(InstantVoltage);
	MU_LogFast(InstantVoltage, PWMSetpoint, Pair.Voltage, Pair.Current);

	// Обработка запроса остановки
	if(RequestSoftStop && (PWMTimerCounter == 0))
	{
		T1PWM_Stop();
	}
	else
		T1PWM_SetDutyCycle(PWMSetpoint);
}
//------------------------------------------------

float PWM_GetInstantVoltageSetpoint()
{
	return ControlSetVoltageRMS * M_SQRT2 * sinf(2 * M_PI * PWMTimerCounter / PWM_SINE_COUNTER_MAX);
}
//------------------------------------------------

float PWM_ConvertVoltageToPWM(float Voltage)
{
	return Voltage / TransformerRatio / MEASURE_PrimaryVoltage * T1PWM_GetPWMBase();
}
//------------------------------------------------

VIPair PWM_GetMeasureData()
{
	VIPair Result;
	Result.Voltage = MEASURE_Voltage();
	Result.Current = MEASURE_Current();
	return Result;
}
//------------------------------------------------

void PWM_AddToRMS(VIPair Pair)
{
	VoltageStorageRMS += Pair.Voltage * Pair.Voltage;
	CurrentStorageRMS += Pair.Current * Pair.Current;
}
//------------------------------------------------

VIPair PWM_CalculateRMSValue()
{
	VIPair Result;
	Result.Voltage = sqrtf(VoltageStorageRMS / PWM_SINE_COUNTER_MAX);
	Result.Current = sqrtf(CurrentStorageRMS / PWM_SINE_COUNTER_MAX);

	Result.Voltage = Result.Voltage * Result.Voltage * Voltage.P2 + Result.Voltage * Voltage.P1 + Voltage.P0;
	Result.Current = Result.Current * Result.Current * Current.P2 + Result.Current * Current.P1 + Current.P0;

	VoltageStorageRMS = CurrentStorageRMS = 0;
	return Result;
}
//------------------------------------------------

void PWM_CacheParameters()
{
	RequestSoftStop = false;

	FollowingErrorCounter = 0;
	PWMTimerCounter = 0;
	ActualSetVoltageRMS = ControlSetVoltageRMS = 0;
	VoltageStorageRMS = CurrentStorageRMS = 0;
	ErrorI = 0;

	Kp = (float)DataTable[REG_KP] / 1000;
	Ki = (float)DataTable[REG_KI] / 1000;

	TargetVoltageRMS = (float)DT_Read32(REG_VOLTAGE_SETPOINT, REG_VOLTAGE_SETPOINT_32) / 1000;
	CurrentLimitRMS = (float)DT_Read32(REG_CURRENT_SETPOINT, REG_CURRENT_SETPOINT_32) * (float)DataTable[REG_CURR_RANGE_1_SAFETY_LIMIT];
	CurrentLimitRMS += (float)DT_Read32(REG_CURRENT_SETPOINT, REG_CURRENT_SETPOINT_32);

	TransformerRatio = (float)DataTable[REG_PWM_TRANS_RATIO];
	ControlSetVoltageMaxRMS = (float)DataTable[REG_PWM_OUT_VOLTAGE_LIMIT];
	VoltageStepRMS = (float)DataTable[REG_PWM_VOLTAGE_RISE_RATE] / PWM_SINE_FREQ;

	FollowingErrorEnable = DataTable[REG_FE_ENABLE];
	FollowingErrorRelativeLevel = (float)DataTable[REG_FE_RELATIVE_LEVEL] / 100;
	FollowingErrorAbsoluteLevel = (float)DataTable[REG_FE_ABSOLUTE_LEVEL];
	FollowingErrorCounterMax = DataTable[REG_FE_COUNTER_MAX];

	VoltageReadyErrorLevel = (float)DataTable[REG_PWM_VOLTAGE_READY_THR] / 100;

	T1PWM_Offset = (uint32_t)DataTable[REG_PWM_DUTY_OFFSET];

	PWM_CacheRMSCoefficients();
}
//------------------------------------------------

void PWM_CacheRMSCoefficients()
{
	if(MEASURE_RangeU == MEASURE_RANGE_LOW)
	{
		Voltage.P2 = (float)((int16_t)DataTable[REG_RMS_V1_FINE_P2]) / 1e6;
		Voltage.P1 = (float)DataTable[REG_RMS_V1_FINE_P1] / 1000;
		Voltage.P0 = (float)((int16_t)DataTable[REG_RMS_V1_FINE_P0]) / 1000;
	}
	else
	{
		Voltage.P2 = (float)((int16_t)DataTable[REG_RMS_V2_FINE_P2]) / 1e6;
		Voltage.P1 = (float)DataTable[REG_RMS_V2_FINE_P1] / 1000;
		Voltage.P0 = (float)((int16_t)DataTable[REG_RMS_V2_FINE_P0]) / 1000;
	}

	if(MEASURE_RangeI == MEASURE_RANGE_LOW)
	{
		Current.P2 = (float)((int16_t)DataTable[REG_RMS_I1_FINE_P2]) / 1e6;
		Current.P1 = (float)DataTable[REG_RMS_I1_FINE_P1] / 1000;
		Current.P0 = (float)((int16_t)DataTable[REG_RMS_I1_FINE_P0]) / 1000;
	}
	else if(MEASURE_RangeI == MEASURE_RANGE_MIDDLE)
	{
		Current.P2 = (float)((int16_t)DataTable[REG_RMS_I2_FINE_P2]) / 1e6;
		Current.P1 = (float)DataTable[REG_RMS_I2_FINE_P1] / 1000;
		Current.P0 = (float)((int16_t)DataTable[REG_RMS_I2_FINE_P0]) / 1000;
	}
	else
	{
		Current.P2 = (float)((int16_t)DataTable[REG_RMS_I3_FINE_P2]) / 1e6;
		Current.P1 = (float)DataTable[REG_RMS_I3_FINE_P1] / 1000;
		Current.P0 = (float)((int16_t)DataTable[REG_RMS_I3_FINE_P0]);
	}
}
//------------------------------------------------
