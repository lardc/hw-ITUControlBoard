// Header
#include "StorageDescription.h"
#include "Global.h"

// Variables
RecordDescription StorageDescription[] =
{
	{"FEP1 Voltage",	DT_Float,	VALUES_x_SIZE},
	{"FEP2 Current1",	DT_Float, 	VALUES_x_SIZE},
	{"FEP3 Current2",	DT_Float, 	VALUES_x_SIZE},
	{"FEP4 Current3",	DT_Float, 	VALUES_x_SIZE},
	{"FEP5 Current4",	DT_Float, 	VALUES_x_SIZE},
	{"FEP Counter",		DT_Int16U, 	VALUES_x_SIZE}
};
Int32U TablePointers[sizeof(StorageDescription) / sizeof(StorageDescription[0])] = {0};
const Int16U StorageSize = sizeof(StorageDescription) / sizeof(StorageDescription[0]);
