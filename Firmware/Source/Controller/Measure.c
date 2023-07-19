// Header
#include "Measure.h"

// Includes
#include "Board.h"
#include "SysConfig.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Definitions
#define CURRENT_CHANNELS		4
#define DEV_OBJ_DICT_CHAN_STEP	12

typedef struct __Coefficients
{
	float K;
	float P2;
	float P1;
	float P0;

} Coefficients;

// Variables
uint16_t DMAVoltage[ADC_DMA_VOLTAGE_SAMLES];
uint16_t DMACurrent12[ADC_DMA_CURRENT_SAMLES];
uint16_t DMACurrent34[ADC_DMA_CURRENT_SAMLES];
static Coefficients VoltageCoeff, CurrentCoeff[CURRENT_CHANNELS];
static float ShuntResistance;

// Forward functions
void ME_LoadParams(Coefficients *coeff, uint16_t RegStartIndex);

// Functions
void ME_CacheVariables(CurrentChannel SelectedChannel)
{
	ME_LoadParams(&VoltageCoeff, REG_COEFF_VOLTAGE_K);

	uint16_t BaseCurrentReg = 0;
	switch(SelectedChannel)
	{
		case CC_R0:
			BaseCurrentReg = REG_COEFF_CURRENT1_R0_K;
			ShuntResistance = DataTable[REG_R0_VALUE];
			break;

		case CC_R1:
			BaseCurrentReg = REG_COEFF_CURRENT1_R1_K;
			ShuntResistance = DataTable[REG_R1_VALUE];
			break;

		case CC_R2:
			BaseCurrentReg = REG_COEFF_CURRENT1_R2_K;
			ShuntResistance = DataTable[REG_R2_VALUE];
			break;

		default:
			break;
	}

	uint16_t i;
	for(i = 0; i < CURRENT_CHANNELS; i++)
		ME_LoadParams(&CurrentCoeff[i], BaseCurrentReg + DEV_OBJ_DICT_CHAN_STEP * i);
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
