// Header
#include "MeasureUtils.h"

// Includes
#include "Board.h"
#include "SysConfig.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Definitions
#define DEV_OBJ_DICT_CHAN_STEP	12

// Заготовки для быстрого деления
#define VOLTAGE_MPY_DIV			(1.0f / ADC_DMA_VOLTAGE_SAMPLES)
#define CURRENT_MPY_DIV			(2.0f / ADC_DMA_CURRENT_SAMPLES)
#define RESOLUTION_MPY_DIV		(1.0f / ADC_RESOLUTION)

typedef struct __Coefficients
{
	float K;
	float P2;
	float P1;
	float P0;
	float RawShift;

} Coefficients;

// Variables
static uint16_t ScopeDivCounter, ScopeDivErrCounter;
static uint16_t ScopeDivCounterMax, ScopeDivErrCounterMax;
static uint16_t ScopeValuesCounter, ErrorValuesCounter;

uint16_t DMAVoltage[ADC_DMA_VOLTAGE_SAMPLES];
uint16_t DMACurrent12[ADC_DMA_CURRENT_SAMPLES];
uint16_t DMACurrent34[ADC_DMA_CURRENT_SAMPLES_PLUS1];
static Coefficients VoltageCoeff, CurrentCoeff[CURRENT_CHANNELS];
static float ShuntResistanceDiv;

// Forward functions
void MU_LoadParams(Coefficients *coeff, uint16_t RegStartIndex);
float MU_SingleConversionX(Coefficients *coeff, float Value, bool IsCurrent);
float MU_SingleFineTuningX(Coefficients *coeff, float Value);

// Functions
void MU_CacheVariables(CurrentChannel SelectedChannel)
{
	MU_LoadParams(&VoltageCoeff, REG_COEFF_VOLTAGE_K);
	VoltageCoeff.RawShift = DataTable[REG_RAW_ZERO_SVOLTAGE];

	uint16_t BaseCurrentReg = 0;
	switch(SelectedChannel)
	{
		case CC_R0:
			BaseCurrentReg = REG_COEFF_CURRENT1_R0_K;
			ShuntResistanceDiv = 1.0f / DataTable[REG_R0_VALUE];
			break;

		case CC_R1:
			BaseCurrentReg = REG_COEFF_CURRENT1_R1_K;
			ShuntResistanceDiv = 1.0f / DataTable[REG_R1_VALUE];
			break;

		case CC_R2:
			BaseCurrentReg = REG_COEFF_CURRENT1_R2_K;
			ShuntResistanceDiv = 1.0f / DataTable[REG_R2_VALUE];
			break;

		default:
			break;
	}

	int i;
	for(i = 0; i < CURRENT_CHANNELS; i++)
	{
		MU_LoadParams(&CurrentCoeff[i], BaseCurrentReg + DEV_OBJ_DICT_CHAN_STEP * i);
		CurrentCoeff[i].RawShift = DataTable[REG_RAW_ZERO_SCURRENT1 + i];
	}
}
//------------------------------------------

void MU_LoadParams(Coefficients *coeff, uint16_t RegStartIndex)
{
	coeff->K = DataTable[RegStartIndex];
	coeff->P2 = DataTable[RegStartIndex + 1];
	coeff->P1 = DataTable[RegStartIndex + 2];
	coeff->P0 = DataTable[RegStartIndex + 3];
}
//------------------------------------------

CCMRAM float MU_SingleConversionX(Coefficients *coeff, float Value, bool IsCurrent)
{
	// Смещение
	Value -= coeff->RawShift;

	// Преобразование в напряжение входа АЦП
	Value = Value * RESOLUTION_MPY_DIV * ADC_REF_VOLTAGE;

	// Преобразование в напряжение входа ОУ
	Value = Value * coeff->K;

	// Преобразование в ток
	if(IsCurrent)
		Value *= ShuntResistanceDiv;

	return Value;
}
//------------------------------------------

CCMRAM float MU_SingleFineTuningX(Coefficients *coeff, float Value)
{
	// Тонкая корректировка
	return Value * Value * coeff->P2 + Value * coeff->P1 + coeff->P0;
}
//------------------------------------------

CCMRAM void MU_GetSampleData(pSampleData Result)
{
	int i;
	float Voltage = 0;
	float Current[CURRENT_CHANNELS] = {0};

	// Обработка данных напряжения
	for(i = 0; i < ADC_DMA_VOLTAGE_SAMPLES; i++)
		Voltage += DMAVoltage[i];
	Voltage *= VOLTAGE_MPY_DIV;
	Result->Voltage = MU_SingleConversionX(&VoltageCoeff, Voltage, false);

	// Обработка данных тока
	for(i = 0; i < ADC_DMA_CURRENT_SAMPLES; i += 2)
	{
		Current[0] += DMACurrent12[i];
		Current[1] += DMACurrent12[i + 1];
		Current[2] += DMACurrent34[i];
		Current[3] += DMACurrent34[i + 1];
	}
	for(i = 0; i < CURRENT_CHANNELS; i++)
		Result->Current[i] = MU_SingleConversionX(&CurrentCoeff[i], Current[i] * CURRENT_MPY_DIV, true);
}
//------------------------------------------

CCMRAM void MU_ResultFineTuning(pSampleData Result)
{
	for(int i = 0; i < CURRENT_CHANNELS; i++)
		Result->Current[i] = MU_SingleFineTuningX(&CurrentCoeff[i], Result->Current[i]);
	Result->Voltage = MU_SingleFineTuningX(&VoltageCoeff, Result->Voltage);
}
//------------------------------------------

void MU_StartScope()
{
	ScopeDivCounterMax = ScopeDivErrCounterMax = DataTable[REG_SCOPE_RATE];
	ScopeDivCounter = ScopeDivErrCounter = 0;

	ScopeValuesCounter = MEMBUF_ScopeValues_Counter;
	ErrorValuesCounter = MEMBUF_ErrorValues_Counter;
}
//------------------------------------------

void MU_LogScopeError(float Value)
{
	if(ScopeDivErrCounter++ >= ScopeDivErrCounterMax)
	{
		ScopeDivErrCounter = 0;
		MEMBUF_Values_Err[ErrorValuesCounter++] = (int16_t)Value;

		if(MEMBUF_ErrorValues_Counter < VALUES_x_SIZE)
			MEMBUF_ErrorValues_Counter = ErrorValuesCounter;

		if(ErrorValuesCounter >= VALUES_x_SIZE)
			ErrorValuesCounter = 0;
	}
}
//------------------------------------------

CCMRAM void MU_LogScopeValues(pSampleData Instant, pSampleData RMS, float *CosPhi, Int16S PWM)
{
	if(ScopeDivCounter++ >= ScopeDivCounterMax)
	{
		ScopeDivCounter = 0;

		MEMBUF_Values_V[ScopeValuesCounter] = Instant->Voltage;
		MEMBUF_Values_I1[ScopeValuesCounter] = Instant->Current[0];
		MEMBUF_Values_I2[ScopeValuesCounter] = Instant->Current[1];
		MEMBUF_Values_I3[ScopeValuesCounter] = Instant->Current[2];
		MEMBUF_Values_I4[ScopeValuesCounter] = Instant->Current[3];

		MEMBUF_Values_Vrms[ScopeValuesCounter] = RMS->Voltage;
		MEMBUF_Values_Irms1[ScopeValuesCounter] = RMS->Current[0];
		MEMBUF_Values_Irms2[ScopeValuesCounter] = RMS->Current[1];
		MEMBUF_Values_Irms3[ScopeValuesCounter] = RMS->Current[2];
		MEMBUF_Values_Irms4[ScopeValuesCounter] = RMS->Current[3];

		MEMBUF_Values_CosPhi1[ScopeValuesCounter] = (int16_t)(CosPhi[0] * 1000);
		MEMBUF_Values_CosPhi2[ScopeValuesCounter] = (int16_t)(CosPhi[1] * 1000);
		MEMBUF_Values_CosPhi3[ScopeValuesCounter] = (int16_t)(CosPhi[2] * 1000);
		MEMBUF_Values_CosPhi4[ScopeValuesCounter] = (int16_t)(CosPhi[3] * 1000);

		MEMBUF_Values_PWM[ScopeValuesCounter] = PWM;
		ScopeValuesCounter++;

		if(MEMBUF_ScopeValues_Counter < VALUES_x_SIZE)
			MEMBUF_ScopeValues_Counter = ScopeValuesCounter;

		if(ScopeValuesCounter >= VALUES_x_SIZE)
			ScopeValuesCounter = 0;
	}
}
//------------------------------------------

float MU_GetPrimarySideVoltage()
{
	if(DataTable[REG_PRIM_IGNORE_CHECK])
		return DataTable[REG_PRIM_VOLTAGE];
	else
		return LL_ReadInputVoltageADC() * RESOLUTION_MPY_DIV * ADC_REF_VOLTAGE * DataTable[REG_CAP_COEFF];
}
//------------------------------------------
