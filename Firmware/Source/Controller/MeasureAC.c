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
#include "Delay.h"
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
static Int16U FECounter, FECounterMax;
static Int16U ShortCircuitCounters[CURRENT_CHANNELS];
static float TransAndPWMCoeff, Ki_err, Kp, Ki, FEAbsolute, FERelative;
static float TargetVrms, ControlVrms, PeriodCorrection, VrmsRateStep, ActualInstantVoltageSet;
static float LimitIrms, Isat_level, Irange, MinIrms;
static float PWMLimit;
static Int32U TimeCounter, PlateCounter, PlateCounterTop, FailedCurrentChannel;
static bool DbgMutePWM, StopByActiveCurrent, RequireSoftStop;

static ProcessState State;
static ProcessBreakReason BreakReason;

// Forward functions
void MAC_PowerDataAdd(pPowerData BaseVale, pPowerData AddValue, bool Sum);
void MAC_CalculateRingBufferValue(pPowerData Result, pSampleData Sample);
void MAC_CalculateRMS(pPowerData Result, pPowerData InputValue, float MultiplyValue);
void MAC_CalculateCosinusPhi(pPowerData SquareSum, pPowerData PowerRMS, float MultiplyValue, float *CosPhi);
Int16S MAC_CalcPWMFromVoltageAmplitude();
void MAC_HandleVI(pSampleData Instant, pSampleData RMS, float *CosPhi);
float MAC_PeriodController(float ActualVrms);
void MAC_InitStartState();
void MAC_SaveResultToDT(pSampleData RMS, float *CosPhi);

// Functions
void MAC_StartProcess()
{
	MAC_InitStartState();
	T1PWM_Start();
}
// ----------------------------------------

void MAC_SetPWM(Int16S pwm)
{
	T1PWM_SetDutyCycle(DbgMutePWM ? 0 : pwm);
}
// ----------------------------------------

Int16S MAC_GetPWMReduceRate()
{
	Int16S res = PWM / PWM_REDUCE_RATE_MAX_STEPS;
	return (abs(res) > PWM_MIN_REDUCE_RATE) ? res : (PWM_MIN_REDUCE_RATE * SIGN(res));
}
// ----------------------------------------

void MAC_RequestStop(ProcessBreakReason Reason)
{
	// Условие мягкой остановки
	if(Reason == PBR_RequestSoftStop)
	{
		if(State != PS_Break)
		{
			RequireSoftStop = true;
			BreakReason = Reason;
		}
	}
	else
	{
		PWMReduceRate = MAC_GetPWMReduceRate();
		State = PS_Break;
		RequireSoftStop = false;
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

CCMRAM void MAC_PowerDataAdd(pPowerData BaseValue, pPowerData AddValue, bool Sum)
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

CCMRAM void MAC_CalculateRingBufferValue(pPowerData Result, pSampleData Sample)
{
	Result->Sample.Voltage = Sample->Voltage * Sample->Voltage;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		Result->Sample.Current[i] = Sample->Current[i] * Sample->Current[i];
		Result->Power[i] = Sample->Voltage * Sample->Current[i];
	}
}
// ----------------------------------------

CCMRAM void MAC_CalculateRMS(pPowerData Result, pPowerData InputValue, float MultiplyValue)
{
	Result->Sample.Voltage = sqrtf(InputValue->Sample.Voltage * MultiplyValue);
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		Result->Sample.Current[i] = sqrtf(InputValue->Sample.Current[i] * MultiplyValue);
		Result->Power[i] = Result->Sample.Voltage * Result->Sample.Current[i];
	}
}
// ----------------------------------------

CCMRAM void MAC_CalculateCosinusPhi(pPowerData SquareSum, pPowerData PowerRMS, float MultiplyValue, float *CosPhi)
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

CCMRAM void MAC_HandleVI(pSampleData Instant, pSampleData RMS, float *CosPhi)
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

CCMRAM void MAC_ControlCycle()
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
				SavedCosPhi[i] = fabsf(RMS.Voltage) > SC_VOLTAGE_THR ? CosPhi[i] : 1.0f;
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
		if(RequireSoftStop)
			State = PS_Break;

		// Проверка на минимальный ток
		if(State == PS_Plate && MinIrms)
		{
			for(i = 0; i < CURRENT_CHANNELS; i++)
			{
				if(RMS.Current[i] < MinIrms)
				{
					MAC_RequestStop(PBR_MinCurrent);
					FailedCurrentChannel = i;
					break;
				}
			}
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
		PWM = ((abs(PWM) > abs(PWMReduceRate)) && !RequireSoftStop) ? (PWM - PWMReduceRate) : 0;

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

				case PBR_MinCurrent:
					DataTable[REG_PROBLEM] = PROBLEM_MIN_CURRENT;
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
			// В режиме игнорирования спайков считаем число тиков с превышением тока
			bool CurrentSaturation = (fabsf(Instant.Current[i]) >= Isat_level);
			if(SC_IGNORE_SPIKE)
			{
				if(CurrentSaturation)
					ShortCircuitCounters[i]++;
				else
					ShortCircuitCounters[i] = 0;
			}

			float absActualInstantVoltageSet = fabsf(ActualInstantVoltageSet);
			if((SC_IGNORE_SPIKE && ShortCircuitCounters[i] >= SC_COUNTER) || (!SC_IGNORE_SPIKE &&
					CurrentSaturation && absActualInstantVoltageSet > BR_DOWM_VOLTAGE_SET_MIN &&
					fabsf(Instant.Voltage) < BR_DOWN_VOLTAGE_RATIO * absActualInstantVoltageSet))
			{
				SavedCosPhi[i] = 1.0f;
				SavedRMS.Current[i] = Irange;

				DataTable[REG_WARNING] = WARNING_CURR_RANGE_SAT;
				MAC_RequestStop(PBR_CurrentSaturation);
				FailedCurrentChannel = i;
				break;
			}
		}

		// Расчёт нового значения ШИМ
		PWM = MAC_CalcPWMFromVoltageAmplitude();

		// Проверка на насыщение
		if(abs(PWM) == PWMLimit)
			MAC_RequestStop(PBR_PWMSaturation);
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
	if(State == PS_Plate)
		PlateCounter++;

	// Запись текущих показаний времени
	// Делитель 1 000 000 для результата в секундах
	DataTable[REG_INFO_TOTAL_TIME] = TimeCounter * PWM_PERIOD / 1000000ul;
	DataTable[REG_INFO_PLATE_TIME] = PlateCounter * PWM_PERIOD / 1000000ul;

	// Запись показаний напряжения и 4 каналов тока
	if(State == PS_Ramp || State == PS_Plate)
	{
		// Запись значения напряжения
		DataTable[REG_INFO_V] = SavedRMS.Voltage;

		// Запись значений тока
		const Int16U RegStep = REG_INFO_I2_mA - REG_INFO_I1_mA, BaseReg = REG_INFO_I1_mA;
		for(int i = 0; i < CURRENT_CHANNELS; i++)
		{
			DataTable[BaseReg + i * RegStep] = SavedRMS.Current[i];
		}
	}
}
// ----------------------------------------


void MAC_SaveResultToDT(pSampleData RMS, float *CosPhi)
{
	const Int16U RegStep = REG_RESULT_I2 - REG_RESULT_I1, BaseReg = REG_RESULT_I1;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
	{
		// RMS
		DataTable[BaseReg + i * RegStep] = RMS->Current[i];
		// Активная составляющая
		DataTable[BaseReg + i * RegStep + 1] = (RMS->Current[i] * fabsf(CosPhi[i]));
		// Косинус фи
		DataTable[BaseReg + i * RegStep + 2] = CosPhi[i];
	}
	DataTable[REG_RESULT_V] = RMS->Voltage;
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
		if(abs(pwm) < (MinSafePWM >> 1))
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

Int16S MAC_CalcPWMFromVoltageAmplitude()
{
	// Расчёт мгновенного значения напряжения
	// Отбрасывание целых периодов счётчика времени
	float TrimmedCounter = (float)(TimeCounter % SINE_PERIOD_PULSES);
	float SinValue = sinf(2 * M_PI * SINE_PERIOD_PULSES_MPY_DIV * TrimmedCounter);
	ActualInstantVoltageSet = SQROOT2 * (ControlVrms + PeriodCorrection) * SinValue;

	// Пересчёт в ШИМ
	Int16S pwm = (Int16S)(ActualInstantVoltageSet * TransAndPWMCoeff);
	return MAC_PWMTrim(pwm);
}
// ----------------------------------------

void MAC_InitStartState()
{
	TargetVrms = DataTable[REG_TEST_VOLTAGE];
	LimitIrms = DataTable[REG_LIMIT_CURRENT];
	MinIrms = DataTable[REG_MIN_CURRENT];
	
	ControlVrms = DataTable[REG_START_VOLTAGE];

	Kp = DataTable[REG_KP] * 0.01f;
	Ki = DataTable[REG_KI] * 0.01f;
	
	VrmsRateStep = DataTable[REG_VOLTAGE_RATE] * 1000 / PWM_SINE_FREQ;
	PlateCounterTop = PWM_FREQUENCY * (Int16U)DataTable[REG_TEST_TIME];
	
	TransAndPWMCoeff = T1PWM_GetPWMBase() / (DataTable[REG_PRIM_VOLTAGE] * DataTable[REG_TRANSFORMER_COFF]);
	MinSafePWM = (PWM_FREQUENCY / 1000L) * PWM_MIN_TH * T1PWM_GetPWMBase() / 1000000L;
	StopByActiveCurrent = DataTable[REG_STOP_BY_ACTIVE_CURRENT];
	
	FEAbsolute = DataTable[REG_FE_ABSOLUTE];
	FERelative = DataTable[REG_FE_RELATIVE] / 100;
	FECounterMax = DataTable[REG_FE_COUNTER_MAX];

	PWMLimit = T1PWM_GetPWMBase() * PWM_MAX_SAT;

	DbgMutePWM = DataTable[REG_DBG_MUTE_PWM] ? true : false;
	
	// Сброс переменных
	PWM = 0;
	FECounter = TimeCounter = PlateCounter = 0;
	Ki_err = PeriodCorrection = 0;
	ActualInstantVoltageSet = 0;
	RequireSoftStop = false;
	for(int i = 0; i < CURRENT_CHANNELS; i++)
		ShortCircuitCounters[i] = 0;

	// Очистка кольцевого буфера
	for(int i = 0; i < SINE_PERIOD_PULSES; i++)
		RingBuffer[i] = PowerDataZero;
	RealSquareSum = PowerDataZero;
	RingBufferFull = false;
	RingBufferPointer = 0;

	State = PS_Ramp;
	BreakReason = PBR_None;

	// Конфигурация модуля измерений
	if(LimitIrms <= DataTable[REG_I_RANGE_LOW])
	{
		Irange = DataTable[REG_I_RANGE_LOW];
		MU_CacheVariables(CC_R2);
		LL_SelectCurrentChannel(CC_R2);
	}
	else if(LimitIrms <= DataTable[REG_I_RANGE_MID])
	{
		Irange = DataTable[REG_I_RANGE_MID];
		MU_CacheVariables(CC_R1);
		LL_SelectCurrentChannel(CC_R1);
	}
	else
	{
		Irange = DataTable[REG_I_RANGE_HIGH];
		MU_CacheVariables(CC_R0);
		LL_SelectCurrentChannel(CC_R0);
	}
	Isat_level = Irange * SQROOT2;

	// Задержка на переключение оптопар
	DELAY_US(5000);
}
// ----------------------------------------
