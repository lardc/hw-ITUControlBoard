﻿// Header
#include "Measure.h"

// Includes
#include "Board.h"
#include "SysConfig.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Definitions
#define DEV_OBJ_DICT_CHAN_STEP	12
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
void ME_LoadParams(Coefficients *coeff, uint16_t RegStartIndex);
float ME_SingleConversionX(Coefficients *coeff, float Value, bool IsCurrent);

// Functions
void ME_CacheVariables(CurrentChannel SelectedChannel)
{
	ME_LoadParams(&VoltageCoeff, REG_COEFF_VOLTAGE_K);
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
		ME_LoadParams(&CurrentCoeff[i], BaseCurrentReg + DEV_OBJ_DICT_CHAN_STEP * i);
		CurrentCoeff[i].RawShift = DataTable[REG_RAW_ZERO_SCURRENT1 + i];
	}
}
//------------------------------------------

void ME_LoadParams(Coefficients *coeff, uint16_t RegStartIndex)
{
	coeff->K = DataTable[RegStartIndex];
	coeff->P2 = DataTable[RegStartIndex + 1];
	coeff->P1 = DataTable[RegStartIndex + 2];
	coeff->P0 = DataTable[RegStartIndex + 3];
}
//------------------------------------------

float ME_SingleConversionX(Coefficients *coeff, float Value, bool IsCurrent)
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

	// Тонкая корректировка
	return Value * Value * coeff->P2 + Value * coeff->P1 + coeff->P0;
}
//------------------------------------------

SampleResult ME_GetSampleResult()
{
	int i;
	SampleResult result;
	float Voltage = 0;
	float Current[CURRENT_CHANNELS] = {0};

	// Обработка данных напряжения
	for(i = 0; i < ADC_DMA_VOLTAGE_SAMPLES; i++)
		Voltage += DMAVoltage[i];
	Voltage *= VOLTAGE_MPY_DIV;
	result.Voltage = ME_SingleConversionX(&VoltageCoeff, Voltage, false);

	// Обработка данных тока
	for(i = 0; i < ADC_DMA_CURRENT_SAMPLES; i += 2)
	{
		Current[0] += DMACurrent12[i];
		Current[1] += DMACurrent12[i + 1];
		Current[2] += DMACurrent34[i];
		Current[3] += DMACurrent34[i + 1];
	}
	for(i = 0; i < CURRENT_CHANNELS; i++)
		result.Current[i] = ME_SingleConversionX(&CurrentCoeff[i], Current[i] * CURRENT_MPY_DIV, true);

	return result;
}
//------------------------------------------
