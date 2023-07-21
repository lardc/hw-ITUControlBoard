// ----------------------------------------
// Measuring logic AC
// ----------------------------------------

// Header
#include "MeasureAC.h"

// Includes
#include "SysConfig.h"
#include "Board.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Controller.h"
#include "Global.h"
#include "MeasureUtils.h"
#include "Timer1PWM.h"
#include <math.h>
#include <stdlib.h>

// Definitions
#define SQROOT2						1.4142f
#define SINE_PERIOD_PULSES			(PWM_FREQUENCY / PWM_SINE_FREQ)
#define SINE_PERIOD_PULSES_MPY_DIV	(1.0f / SINE_PERIOD_PULSES)
#define SIGN(a)						(((a) >= 0) ? 1 : -1)

// Types
typedef enum __ProcessState
{
	PS_None = 0,
	PS_Ramp,
	PS_Plate,
	PS_Break
} ProcessState;

typedef struct __PowerData
{
	SampleData Sample;
	float Power[CURRENT_CHANNELS];

} PowerData, *pPowerData;
const PowerData PowerDataZero = {0};

// Variables
static PowerData RingBuffer[SINE_PERIOD_PULSES], RealSquareSum;
static Int16U RingBufferPointer;
static bool RingBufferFull;

static Int16S MinSafePWM, PWM, PWMReduceRate;
static Int16U RawZeroVoltage, RawZeroCurrent, FECounter, FECounterMax;
static float TransAndPWMCoeff, Ki_err, Kp, Ki, FEAbsolute, FERelative;
static float TargetVrms, ControlVrms, PeriodCorrection, VrmsRateStep, ActualInstantVoltageSet;
static float LimitIrms, Isat_level, Irange;
static float PWMLimit;
static Int32U TimeCounter, PlateCounterTop, FailedCurrentChannel;
static bool DbgMutePWM, DbgSRAM, StopByActiveCurrent, RequireSoftStop;

static ProcessState State;
static ProcessBreakReason BreakReason;

// Forward functions
void MAC_PowerDataAdd(pPowerData BaseVale, pPowerData AddValue, bool Sum);
void MAC_CalculateRingBufferValue(pPowerData Result, pSampleData Sample);
void MAC_CalculateRMS(pPowerData Result, pPowerData InputValue, float MultiplyValue);
void MAC_CalculateCosinusPhi(pPowerData SquareSum, pPowerData PowerRMS, float MultiplyValue, float *CosPhi);
Int16S MAC_CalcPWMFromVoltageAmplitude();
void MAC_HandleVI(pSampleData Instant, pSampleData RMS, float *CosPhi);
//float MAC_PeriodController();
void MAC_ControlCycle();
bool MAC_InitStartState();
void MAC_SaveResultToDT(pSampleData RMS, float *CosPhi);

// Functions
bool MAC_StartProcess()
{
	if(MAC_InitStartState())
	{
		T1PWM_Start();
		return true;
	}
	else
		return false;
}
// ----------------------------------------

void MAC_SetPWM(Int16S pwm)
{
	T1PWM_SetDutyCycle(DbgMutePWM ? 0 : pwm);
}
// ----------------------------------------

Int16S MAC_GetPWMReduceRate(Int16S PWMDelta)
{
	Int16S res = PWMDelta / PWM_REDUCE_RATE_MAX_STEPS + SIGN(PWMDelta);
	return (abs(res) > PWM_MIN_REDUCE_RATE) ? res : (PWM_MIN_REDUCE_RATE * SIGN(PWMDelta));
}
// ----------------------------------------

void MAC_RequestStop(ProcessBreakReason Reason)
{
	if(State != PS_Break)
	{
		if(Reason == PBR_RequestSoftStop)
			RequireSoftStop = true;

		// Запрос на мягкую остановку имеет низший приоритет
		else
		{
			PWMReduceRate = MAC_GetPWMReduceRate(PWM);
			State = PS_Break;
			RequireSoftStop = false;
		}
		BreakReason = Reason;
	}
}
// ----------------------------------------

float MAC_PeriodController(float ActualVrms)
{
	float err = TimeCounter ? (ControlVrms - ActualVrms) : 0;
	Ki_err += err * Ki;
	PeriodCorrection = Ki_err + err * Kp;

	return err;
}
// ----------------------------------------

void MAC_PowerDataAdd(pPowerData BaseValue, pPowerData AddValue, bool Sum)
{
	float Sign = Sum ? 1 : -1;

	BaseValue->Sample.Voltage += Sign * AddValue->Sample.Voltage;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		BaseValue->Sample.Current[i] += Sign * AddValue->Sample.Current[i];
		BaseValue->Power[i] += Sign * AddValue->Power[i];
	}
}
// ----------------------------------------

void MAC_CalculateRingBufferValue(pPowerData Result, pSampleData Sample)
{
	Result->Sample.Voltage = Sample->Voltage * Sample->Voltage;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		Result->Sample.Current[i] = Sample->Current[i] * Sample->Current[i];
		Result->Power[i] = Sample->Voltage * Sample->Current[i];
	}
}
// ----------------------------------------

void MAC_CalculateRMS(pPowerData Result, pPowerData InputValue, float MultiplyValue)
{
	Result->Sample.Voltage = sqrtf(InputValue->Sample.Voltage * MultiplyValue);
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		Result->Sample.Current[i] = sqrtf(InputValue->Sample.Current[i] * MultiplyValue);
		Result->Power[i] = Result->Sample.Voltage * Result->Sample.Current[i];
	}
}
// ----------------------------------------

void MAC_CalculateCosinusPhi(pPowerData SquareSum, pPowerData PowerRMS, float MultiplyValue, float *CosPhi)
{
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		float Wreal_abs = fabsf(SquareSum->Power[i]) * MultiplyValue;
		float Wrms = PowerRMS->Power[i];

		if(Wreal_abs == 0)
			CosPhi[i] = 0;
		else
		{
			CosPhi[i] = Wreal_abs / Wrms;

			// Если из-за погрешностей значение больше 1
			if(CosPhi[i] > 1.0f)
				CosPhi[i] = 1.0f;
			else if(CosPhi[i] < -1.0f)
				CosPhi[i] = -1.0f;
		}
	}
}
// ----------------------------------------

void MAC_HandleVI(pSampleData Instant, pSampleData RMS, float *CosPhi)
{
	// Вычитание из суммы затираемого значения
	if(RingBufferPointer != 0 || RingBufferFull)
		MAC_PowerDataAdd(&RealSquareSum, &RingBuffer[RingBufferPointer], false);

	// Считывание последнего результата
	MU_GetSampleData(Instant);

	// Сохранение нового значения в кольцевой буфер
	MAC_CalculateRingBufferValue(&RingBuffer[RingBufferPointer], Instant);

	// Суммирование добавленного значения
	MAC_PowerDataAdd(&RealSquareSum, &RingBuffer[RingBufferPointer], true);

	// Сдвиг кольцевого буфера
	RingBufferPointer++;
	if(RingBufferPointer >= SINE_PERIOD_PULSES)
	{
		RingBufferPointer = 0;
		RingBufferFull = true;
	}

	// Определение делителя
	float cntDiv = RingBufferFull ? SINE_PERIOD_PULSES_MPY_DIV : (1.0f / RingBufferPointer);

	// Расчёт действующих значений
	PowerData PowerRMS;
	MAC_CalculateRMS(&PowerRMS, &RealSquareSum, cntDiv);

	// Корректировка действующих значений
	MU_ResultFineTuning(&PowerRMS.Sample);

	// Копирование скорректированных значений
	RMS->Voltage = PowerRMS.Sample.Voltage;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
		RMS->Current[i] = PowerRMS.Sample.Current[i];

	// Расчёт косинус фи
	MAC_CalculateCosinusPhi(&RealSquareSum, &PowerRMS, cntDiv, CosPhi);
}
// ----------------------------------------

void MAC_ControlCycle()
{
	int i;
	static ProcessState PrevState = PS_None;

	// Считывание оцифрованных значений
	float CosPhi[CURRENT_CHANNELS];
	SampleData Instant, RMS;
	MAC_HandleVI(&Instant, &RMS, CosPhi);

	// Сохранение копии значений
	SampleData SavedRMS = RMS;
	float SavedCosPhi[CURRENT_CHANNELS];
	for(i = 0; i < CURRENT_CHANNELS; i++)
		SavedCosPhi[i] = CosPhi[i];

	// Проверка превышения значения тока
	if(State != PS_Break)
	{
		for(i = 0; i < CURRENT_CHANNELS; i++)
		{
			float CompareCurrent = StopByActiveCurrent ? fabsf(CosPhi[i]) * RMS.Current[i] : RMS.Current[i];
			if(CompareCurrent >= LimitIrms)
			{
				SavedCosPhi[i] = fabs(RMS.Voltage) > SC_VOLTAGE_THR ? CosPhi[i] : 1.0f;
				MAC_RequestStop(PBR_CurrentLimit);
				FailedCurrentChannel = i;
				break;
			}
		}
	}

	// Работа амплитудного регулятора
	if(TimeCounter % SINE_PERIOD_PULSES == 0)
	{
		float PeriodError = MAC_PeriodController(RMS.Voltage);
		MU_LogScopeError(PeriodError);

		// Проверка на запрос остановки
		if(State != PS_Break && RequireSoftStop)
		{
			PWMReduceRate = MAC_GetPWMReduceRate(PWM);
			State = PS_Break;
		}

		// Проверка на ошибку следования
		if(State != PS_Break && Kp && Ki && !DbgMutePWM &&
				fabsf(PeriodError) > (FERelative * ControlVrms) && fabsf(PeriodError) > FEAbsolute)
		{
			FECounter++;
		}
		else
			FECounter = 0;

		// Триггер ошибки следования
		if(FECounter >= FECounterMax)
			MAC_RequestStop(PBR_FollowingError);

		// Нормальное функционирование
		else
		{
			switch(State)
			{
				case PS_Ramp:
					ControlVrms += VrmsRateStep;
					if(ControlVrms > TargetVrms)
					{
						PlateCounterTop += TimeCounter;
						ControlVrms = TargetVrms;
						DataTable[REG_VOLTAGE_READY] = 1;
						State = PS_Plate;
					}
					break;

				case PS_Plate:
					if(TimeCounter >= PlateCounterTop)
					{
						for(i = 0; i < CURRENT_CHANNELS; i++)
							SavedCosPhi[i] = CosPhi[i];
						MAC_RequestStop(PBR_None);
					}
					break;

				default:
					break;
			}
		}
	}

	// Расчёт и уставка ШИМ
	if(State == PS_Break)
	{
		PWM = (abs(PWM) > abs(PWMReduceRate)) ? (PWM - PWMReduceRate) : 0;

		// Завершение процесса
		if(PWM == 0)
		{
			T1PWM_Stop();
			switch(BreakReason)
			{
				case PBR_None:
					DataTable[REG_TEST_PASSED] = 1;
				case PBR_CurrentSaturation:
				case PBR_CurrentLimit:
					DataTable[REG_FINISHED] = OPRESULT_OK;
					break;

				case PBR_FollowingError:
					DataTable[REG_PROBLEM] = PROBLEM_FOLLOWING_ERROR;
					DataTable[REG_FINISHED] = OPRESULT_FAIL;
					break;

				case PBR_RequestSoftStop:
				case PBR_RequestFastStop:
					DataTable[REG_PROBLEM] = PROBLEM_STOP;
					DataTable[REG_FINISHED] = OPRESULT_FAIL;
					break;

				case PBR_PWMSaturation:
					DataTable[REG_PROBLEM] = PROBLEM_PWM_SATURATION;
					DataTable[REG_FINISHED] = OPRESULT_FAIL;
					break;
			}

			CONTROL_RequestStop();
		}
	}
	else
	{
		// Условие определения КЗ
		for(i = 0; i < CURRENT_CHANNELS; i++)
		{
			float absActualInstantVoltageSet = fabsf(ActualInstantVoltageSet);
			if(fabsf(Instant.Current[i]) >= Isat_level && absActualInstantVoltageSet > BR_DOWM_VOLTAGE_SET_MIN &&
					fabsf(Instant.Voltage) < BR_DOWN_VOLTAGE_RATIO * absActualInstantVoltageSet)
			{
				SavedCosPhi[i] = 1.0f;
				SavedRMS.Current[i] = Irange;

				DataTable[REG_WARNING] = WARNING_CURR_RANGE_SAT;
				MAC_RequestStop(PBR_CurrentSaturation);
				FailedCurrentChannel = i;
				break;
			}
		}

		// Если ошибок нет, то расчёт нового значения ШИМ
		if(BreakReason == PBR_None)
		{
			PWM = MAC_CalcPWMFromVoltageAmplitude();

			// Проверка на насыщение
			if(abs(PWM) == PWMLimit)
				MAC_RequestStop(PBR_PWMSaturation);
		}
	}
	MAC_SetPWM(PWM);

	// Логгирование мгновенных данных
	MU_LogScopeValues(&Instant, &RMS, CosPhi, PWM);

	// Запись значений в регистры результатов
	if(PrevState == PS_Ramp || PrevState == PS_Plate)
	{
		MAC_SaveResultToDT(&SavedRMS, SavedCosPhi);
		DataTable[REG_FAILED_CURRENT_CHANNEL] = FailedCurrentChannel;
	}

	PrevState = State;
	TimeCounter++;
}
// ----------------------------------------

void MAC_SaveResultToDT(pSampleData RMS, float *CosPhi)
{
	const Int16U RegStep = REG_RESULT_I2_mA - REG_RESULT_I1_mA, BaseReg = REG_RESULT_I1_mA;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		// RMS
		float mApart;
		float uApart = modff(RMS->Current[i], &mApart) * 1000;

		DataTable[BaseReg + i * RegStep] = (Int16U)mApart;
		DataTable[BaseReg + i * RegStep + 1] = (Int16U)uApart;

		// Активная составляющая
		uApart = modff(RMS->Current[i] * fabsf(CosPhi[i]), &mApart) * 1000;

		DataTable[BaseReg + i * RegStep + 2] = (Int16U)mApart;
		DataTable[BaseReg + i * RegStep + 3] = (Int16U)uApart;

		// Косинус фи
		DataTable[BaseReg + i * RegStep + 4] = (Int16S)(CosPhi[i] * 1000);
	}
	DataTable[REG_RESULT_V] = (Int16U)RMS->Voltage;
}
// ----------------------------------------

Int16S MAC_PWMTrim(Int16S pwm)
{
	// Обрезка верхних значений
	if(abs(pwm) > PWMLimit)
		return SIGN(pwm) * PWMLimit;

	// Порог обрезки нижних значений
	else if((ControlVrms + PeriodCorrection) > PWM_TRIM_CTRL_VOLTAGE)
	{
		if(abs(pwm) < (MinSafePWM / 2))
			return 0;
		else if(abs(pwm) < MinSafePWM)
			return MinSafePWM * SIGN(pwm);
		else
			return pwm;
	}
	else
		return pwm;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
#pragma CODE_SECTION(MAC_CalcPWMFromVoltageAmplitude, "ramfuncs");
#endif
Int16S MAC_CalcPWMFromVoltageAmplitude()
{
	// Расчёт мгновенного значения напряжения
	// Отбрасывание целых периодов счётчика времени
	Int32U TrimmedCounter = TimeCounter % SINE_PERIOD_PULSES;
	float SinValue = _IQsinPU(_FPtoIQ2(TrimmedCounter, SINE_PERIOD_PULSES));
	ActualInstantVoltageSet = _IQmpy(_IQmpy(SQROOT2, ControlVrms + PeriodCorrection), SinValue);

	// Пересчёт в ШИМ
	Int16S pwm = _IQint(_IQmpy(ActualInstantVoltageSet, TransAndPWMCoeff));
	return MAC_PWMTrim(pwm);
}
// ----------------------------------------

bool MAC_InitStartState()
{
	TargetVrms = _IQI(DataTable[REG_TEST_VOLTAGE]);
	LimitIrms = _IQI(DataTable[REG_LIMIT_CURRENT_mA]) + _FPtoIQ2(DataTable[REG_LIMIT_CURRENT_uA], 1000);
	
	ControlVrms = _IQI(DataTable[REG_START_VOLTAGE]);

	Kp = _FPtoIQ2(DataTable[REG_KP], 100);
	Ki = _FPtoIQ2(DataTable[REG_KI], 100);
	
	VrmsRateStep = _FPtoIQ2(DataTable[REG_VOLTAGE_RATE] * 100, PWM_SINE_FREQ);
	PlateCounterTop = CONTROL_FREQUENCY * DataTable[REG_TEST_TIME];
	
	TransAndPWMCoeff = _FPtoIQ2(ZW_PWM_DUTY_BASE, DataTable[REG_PRIM_VOLTAGE] * DataTable[REG_TRANSFORMER_COFF]);
	MinSafePWM = (PWM_FREQUENCY / 1000L) * PWM_MIN_TH * ZW_PWM_DUTY_BASE / 1000000L;
	RawZeroVoltage = DataTable[REG_RAW_ZERO_SVOLTAGE];
	RawZeroCurrent = DataTable[REG_RAW_ZERO_SCURRENT];
	StopByActiveCurrent = DataTable[REG_STOP_BY_ACTIVE_CURRENT];
	
	FEAbsolute = _IQI(DataTable[REG_FE_ABSOLUTE]);
	FERelative = _FPtoIQ2(DataTable[REG_FE_RELATIVE], 100);
	FECounterMax = DataTable[REG_FE_COUNTER_MAX];

	PWMLimit = T1PWM_GetPWMBase() * PWM_MAX_SAT;

	DbgSRAM = DataTable[REG_DBG_SRAM] ? true : false;
	DbgMutePWM = DataTable[REG_DBG_MUTE_PWM] ? true : false;
	
	MU_InitCoeffVoltage();
	MU_InitCoeffCurrent1();
	MU_InitCoeffCurrent2();
	MU_InitCoeffCurrent3();

	// Сброс переменных
	PWM = 0;
	FECounter = TimeCounter = 0;
	Ki_err = PeriodCorrection = 0;
	ActualInstantVoltageSet = 0;
	RequireSoftStop = false;

	// Очистка кольцевого буфера
	Int16U i;
	for(i = 0; i < SINE_PERIOD_PULSES; i++)
		RingBuffer[i] = PowerDataZero;

	RingBufferFull = false;
	RingBufferPointer = 0;

	RealSquareSum = PowerDataZero;


	State = PS_Ramp;
	BreakReason = PBR_None;

	// Конфигурация оцифровщика
	bool res;
	if(LimitIrms <= I_RANGE_LOW)
	{
		Irange = I_RANGE_LOW;
		MAC_CurrentCalc = MU_CalcCurrent3;
		res = SS_SelectShunt(SwitchConfig_I3);
	}
	else if(LimitIrms <= I_RANGE_MID)
	{
		Irange = I_RANGE_MID;
		MAC_CurrentCalc = MU_CalcCurrent2;
		res = SS_SelectShunt(SwitchConfig_I2);
	}
	else
	{
		Irange = I_RANGE_HIGH;
		MAC_CurrentCalc = MU_CalcCurrent1;
		res = SS_SelectShunt(SwitchConfig_I1);
	}
	Isat_level = _IQmpy(Irange, SQROOT2);

	if(res)
	{
		// Первый запрос данных
		res = SS_GetData(true);

		// Задержка на переключение оптопар
		if(res)
			DELAY_US(5000);
	}

	return res;
}
// ----------------------------------------
