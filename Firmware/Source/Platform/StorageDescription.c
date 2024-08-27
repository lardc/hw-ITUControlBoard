// Header
#include "StorageDescription.h"
#include "Global.h"

// Variables
RecordDescription StorageDescription[] =
{
	{"FEP1 Voltage",				DT_Float,	VALUES_x_SIZE},
	{"FEP2 Current1",				DT_Float, 	VALUES_x_SIZE},
	{"FEP3 Current2",				DT_Float, 	VALUES_x_SIZE},
	{"FEP4 Current3",				DT_Float, 	VALUES_x_SIZE},
	{"FEP5 Current4",				DT_Float, 	VALUES_x_SIZE},
	{"FEP Counter",					DT_Int16U, 	1},

	{"REG_TEST_VOLTAGE",			DT_Float, 	1},
	{"REG_LIMIT_CURRENT",			DT_Float, 	1},
	{"REG_STOP_BY_ACTIVE_CURRENT",	DT_Float, 	1},
	{"REG_TEST_TIME",				DT_Float, 	1},
	{"REG_SCOPE_RATE",				DT_Float, 	1},
	{"REG_MIN_CURRENT",				DT_Float, 	1},
	{"REG_DUT_PRESENSE_MASK",		DT_Float, 	1},

	{"REG_RESULT_V",				DT_Float, 	1},
	{"REG_RESULT_I1",				DT_Float, 	1},
	{"REG_RESULT_I1_ACT",			DT_Float, 	1},
	{"REG_RESULT_I2",				DT_Float, 	1},
	{"REG_RESULT_I2_ACT",			DT_Float, 	1},
	{"REG_RESULT_I3",				DT_Float, 	1},
	{"REG_RESULT_I3_ACT",			DT_Float, 	1},
	{"REG_RESULT_I4",				DT_Float, 	1},
	{"REG_RESULT_I4_ACT",			DT_Float, 	1},
	{"REG_FAILED_CURRENT_CHANNEL",	DT_Float, 	1}
};
Int32U TablePointers[sizeof(StorageDescription) / sizeof(StorageDescription[0])] = {0};
const Int16U StorageSize = sizeof(StorageDescription) / sizeof(StorageDescription[0]);
