#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"

// Definitions
#define NO		0
#define YES		1

#define REGULATOR_K_MAX			10000

// в Ом
#define SHUNT_RES_MIN			1
#define SHUNT_RES_MAX			INT16U_MAX
#define SHUNT_RES_DEF			1000

#define TRANSFORMER_RATIO_MIN	5
#define TRANSFORMER_RATIO_MAX	60
#define TRANSFORMER_RATIO_DEF	10

// в В (RMS)
#define VOLTAGE_OUTPUT_MIN		5
#define VOLTAGE_OUTPUT_MAX		750
#define VOLTAGE_OUTPUT_DEF		20

// в В/сек
#define VOLTAGE_RISE_RATE_MIN	10
#define VOLTAGE_RISE_RATE_MAX	200
#define VOLTAGE_RISE_RATE_DEF	10

// в мкА
#define CURRENT_OUTPUT_UA_MIN	1
#define CURRENT_OUTPUT_UA_MAX	8000
#define CURRENT_OUTPUT_UA_DEF	10

// в мА
#define CURRENT_OUTPUT_MA_MIN	1
#define CURRENT_OUTPUT_MA_MAX	200
#define CURRENT_OUTPUT_MA_DEF	100

// в %
#define FE_LEVEL_MIN			1
#define FE_LEVEL_MAX			50
#define FE_LEVEL_DEF			10

#define FE_COUNTER_MIN			1
#define FE_COUNTER_MAX			50
#define FE_COUNTER_DEF			3

// в В
#define FE_ABS_LEVEL_MIN		1
#define FE_ABS_LEVEL_MAX		50
#define FE_ABS_LEVEL_DEF		5

// в %
#define VOLTAGE_READY_LVL_MIN	1
#define VOLTAGE_READY_LVL_MAX	10
#define VOLTAGE_READY_LVL_DEF	3

// в %
#define REG_CURR_RANGE_SAFETY_LIMIT 15

// в значении регистра CCR
#define PWM_DUTY_OFFSET_MAX	100
#define PWM_DUTY_OFFSET_DEF	0

// Types
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];

#endif // __CONSTRAINTS_H
