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
uint16_t DMAVoltage[ADC_DMA_VOLTAGE_SAMPLES];
uint16_t DMACurrent12[ADC_DMA_CURRENT_SAMPLES];
uint16_t DMACurrent34[ADC_DMA_CURRENT_SAMPLES];
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

float MU_SingleConversionX(Coefficients *coeff, float Value, bool IsCurrent)
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

float MU_SingleFineTuningX(Coefficients *coeff, float Value)
{
	// Тонкая корректировка
	return Value * Value * coeff->P2 + Value * coeff->P1 + coeff->P0;
}
//------------------------------------------

void MU_GetSampleData(pSampleData Result)
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

void MU_ResultFineTuning(pSampleData Result)
{
	for(int i = 0; i < CURRENT_CHANNELS; i++)
		Result->Current[i] = MU_SingleFineTuningX(&CurrentCoeff[i], Result->Current[i]);
	Result->Voltage = MU_SingleFineTuningX(&VoltageCoeff, Result->Voltage);
}
//------------------------------------------
