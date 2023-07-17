// Header
#include "Measure.h"

// Includes
#include "Board.h"
#include "math.h"
#include "Global.h"
#include "DataTable.h"
#include "Constraints.h"

// Types
typedef struct __MeasureSettings
{
	float K;
	float Offset;
	float P2;
	float P1;
	float P0;
	float Rshunt;
} MeasureSettings, *pMeasureSettings;

// Variables
volatile uint16_t ADC1DMAVoltageBuffer[ADC_DMA_BUFF_SIZE] = {0};
volatile uint16_t ADC2DMACurrentBuffer[ADC_DMA_BUFF_SIZE] = {0};
static MeasureSettings VoltageSettings, CurrentSettings;
static float CachedVoltage, CachedCurrent, CachedCurrentPeakLimit;
volatile bool MEASURE_InMilliAmperes = false;
volatile float MEASURE_PrimaryVoltage = 0;
volatile uint16_t MEASURE_RangeI, MEASURE_RangeU;

// Forward functions
void MEASURE_SetCurrentRange(float Current);
void MEASURE_SetVoltageRange(float Voltage);
void MEASURE_CacheSettings(pMeasureSettings Storage, uint16_t RegKN, uint16_t RegKD, uint16_t Offset, uint16_t RegP2, uint16_t RegP1,
		uint16_t RegP0);
void MEASURE_CacheVoltageSettings(uint16_t RegKN, uint16_t RegKD, uint16_t Offset, uint16_t RegP2, uint16_t RegP1, uint16_t RegP0);
void MEASURE_CacheCurrentSettings(uint16_t RegKN, uint16_t RegKD, uint16_t Offset, uint16_t RegP2, uint16_t RegP1, uint16_t RegP0,
		uint16_t RegShunt);
float MEASURE_ArrayToValue(pMeasureSettings Storage, uint16_t *Data, uint16_t DataLen);

// Functions
bool MEASURE_InputParametersCorrect()
{
	// Уставка напряжения (в В)
	float Voltage = (float)DT_Read32(REG_VOLTAGE_SETPOINT, REG_VOLTAGE_SETPOINT_32) / 1000;

	// Уставка тока (в мкА)
	float Current = (float)DT_Read32(REG_CURRENT_SETPOINT, REG_CURRENT_SETPOINT_32);

	bool ParametersOk = true;
	ParametersOk &= (Voltage >= VOLTAGE_OUTPUT_MIN) && (Voltage <= VOLTAGE_OUTPUT_MAX);
	ParametersOk &= (Current >= CURRENT_OUTPUT_MIN) && (Current <= CURRENT_OUTPUT_MAX);

	if(ParametersOk)
	{
		CachedVoltage = Voltage;
		CachedCurrent = Current;
	}

	return ParametersOk;
}
//------------------------------------------------

void MEASURE_SetMeasureRange()
{
	MEASURE_SetVoltageRange(CachedVoltage);
	MEASURE_SetCurrentRange(CachedCurrent);
}
//------------------------------------------------

void MEASURE_SetCurrentRange(float Current)
{
	if(Current <= DataTable[REG_CURRENT_RANGE1_LIMIT])
	{
		MEASURE_RangeI = MEASURE_RANGE_LOW;
		GPIO_SetState(GPIO_I_RANGE_H, false);
		GPIO_SetState(GPIO_I_RANGE_M, false);
		GPIO_SetState(GPIO_I_RANGE_L, true);
		CachedCurrentPeakLimit = (float)DataTable[REG_CURRENT_RANGE1_LIMIT] * (1 + (float)DataTable[REG_CURR_RANGE_1_SAFETY_LIMIT] / 100);
		CachedCurrentPeakLimit = CachedCurrentPeakLimit * M_SQRT2;
		MEASURE_CacheCurrentSettings(REG_ADC_I1_CONV_K, REG_ADC_I1_CONV_K_DENOM, REG_ADC_I1_CONV_B, REG_ADC_I1_FINE_P2, REG_ADC_I1_FINE_P1,
				REG_ADC_I1_FINE_P0, REG_CURRENT_RANGE1_RES);
		MEASURE_InMilliAmperes = false;

	}
	else if(Current <= DataTable[REG_CURRENT_RANGE2_LIMIT])
	{
		MEASURE_RangeI = MEASURE_RANGE_MIDDLE;
		GPIO_SetState(GPIO_I_RANGE_H, false);
		GPIO_SetState(GPIO_I_RANGE_M, true);
		GPIO_SetState(GPIO_I_RANGE_L, false);
		CachedCurrentPeakLimit = (float)DataTable[REG_CURRENT_RANGE2_LIMIT] * (1 + (float)DataTable[REG_CURR_RANGE_2_SAFETY_LIMIT] / 100);
		CachedCurrentPeakLimit = CachedCurrentPeakLimit * M_SQRT2;
		MEASURE_CacheCurrentSettings(REG_ADC_I2_CONV_K, REG_ADC_I2_CONV_K_DENOM, REG_ADC_I2_CONV_B, REG_ADC_I2_FINE_P2, REG_ADC_I2_FINE_P1,
				REG_ADC_I2_FINE_P0, REG_CURRENT_RANGE2_RES);
		MEASURE_InMilliAmperes = false;
	}
	else
	{
		MEASURE_RangeI = MEASURE_RANGE_HIGH;
		GPIO_SetState(GPIO_I_RANGE_H, true);
		GPIO_SetState(GPIO_I_RANGE_M, false);
		GPIO_SetState(GPIO_I_RANGE_L, false);
		CachedCurrentPeakLimit = (float)DataTable[REG_CURRENT_RANGE3_LIMIT] * (1 + (float)DataTable[REG_CURR_RANGE_3_SAFETY_LIMIT] / 100);
		CachedCurrentPeakLimit = CachedCurrentPeakLimit * 1000 * M_SQRT2;
		MEASURE_CacheCurrentSettings(REG_ADC_I3_CONV_K, REG_ADC_I3_CONV_K_DENOM, REG_ADC_I3_CONV_B, REG_ADC_I3_FINE_P2, REG_ADC_I3_FINE_P1,
				REG_ADC_I3_FINE_P0, REG_CURRENT_RANGE3_RES);
		MEASURE_InMilliAmperes = true;
	}
}
//------------------------------------------------

float MEASURE_GetCurrentPeakLimit()
{
	return CachedCurrentPeakLimit;
}
//------------------------------------------------

void MEASURE_SetVoltageRange(float Voltage)
{
	if(Voltage <= DataTable[REG_VOLTAGE_RANGE1_LIMIT])
	{
		MEASURE_RangeU = MEASURE_RANGE_LOW;
		MEASURE_PrimaryVoltage = PWM_PRIMARY_VOLTAGE_LOW;
		GPIO_SetState(GPIO_U_RANGE, true);
		GPIO_SetState(GPIO_HIGH_VOLTAGE, false);
		MEASURE_CacheVoltageSettings(REG_ADC_V1_CONV_K, REG_ADC_V1_CONV_K_DENOM, REG_ADC_V1_CONV_B, REG_ADC_V1_FINE_P2, REG_ADC_V1_FINE_P1,
				REG_ADC_V1_FINE_P0);
	}
	else
	{
		MEASURE_RangeU = MEASURE_RANGE_HIGH;
		MEASURE_PrimaryVoltage = PWM_PRIMARY_VOLTAGE_HIGH;
		GPIO_SetState(GPIO_U_RANGE, false);
		GPIO_SetState(GPIO_HIGH_VOLTAGE, true);
		MEASURE_CacheVoltageSettings(REG_ADC_V2_CONV_K, REG_ADC_V2_CONV_K_DENOM, REG_ADC_V2_CONV_B, REG_ADC_V2_FINE_P2, REG_ADC_V2_FINE_P1,
				REG_ADC_V2_FINE_P0);
	}
}
//------------------------------------------------

void MEASURE_CacheSettings(pMeasureSettings Storage, uint16_t RegKN, uint16_t RegKD, uint16_t Offset, uint16_t RegP2,
		uint16_t RegP1, uint16_t RegP0)
{
	Storage->K = (float)DataTable[RegKN] / DataTable[RegKD];
	Storage->Offset = (float)((int16_t)DataTable[Offset]);
	
	Storage->P2 = (float)((int16_t)DataTable[RegP2]) / 1e6;
	Storage->P1 = (float)DataTable[RegP1] / 1000;
	Storage->P0 = (float)((int16_t)DataTable[RegP0]);

	Storage->Rshunt = 0;
}
//------------------------------------------------

void MEASURE_CacheVoltageSettings(uint16_t RegK, uint16_t RegKDenom, uint16_t Offset, uint16_t RegP2, uint16_t RegP1,
		uint16_t RegP0)
{
	MEASURE_CacheSettings(&VoltageSettings, RegK, RegKDenom, Offset, RegP2, RegP1, RegP0);
}
//------------------------------------------------

void MEASURE_CacheCurrentSettings(uint16_t RegK, uint16_t RegKDenom, uint16_t Offset, uint16_t RegP2, uint16_t RegP1,
		uint16_t RegP0, uint16_t RegShunt)
{
	MEASURE_CacheSettings(&CurrentSettings, RegK, RegKDenom, Offset, RegP2, RegP1, RegP0);
	CurrentSettings.Rshunt = (float)DataTable[RegShunt];
}
//------------------------------------------------

float MEASURE_ArrayToValue(pMeasureSettings Storage, uint16_t *Data, uint16_t DataLen)
{
	// Рассчёт среднего
	float tmp = 0;
	for(uint8_t i = 0; i < DataLen; ++i)
		tmp += (float)Data[i];
	tmp /= DataLen;

	// Пересчёт тиков в милливольты
	tmp = (tmp * ADC_REF_VOLTAGE / ADC_RESOLUTION + Storage->Offset) * Storage->K;

	// Для канала тока - пересчёт в ток
	if(Storage->Rshunt != 0)
		tmp /= Storage->Rshunt;

	// Тонкая корректировка
	return tmp * tmp * Storage->P2 + tmp * Storage->P1 + Storage->P0;
}
//------------------------------------------------

float MEASURE_Voltage()
{
	return MEASURE_ArrayToValue(&VoltageSettings, (uint16_t *)ADC1DMAVoltageBuffer, ADC_DMA_BUFF_SIZE);
}
//------------------------------------------------

float MEASURE_Current()
{
	return MEASURE_ArrayToValue(&CurrentSettings, (uint16_t *)ADC2DMACurrentBuffer, ADC_DMA_BUFF_SIZE);
}
//------------------------------------------------
