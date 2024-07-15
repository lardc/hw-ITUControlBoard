#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"

// Definitions
#define NO		0
#define YES		1

#define TICK_LIMIT_MAX			4095

#define K_MIN					1e-4f
#define K_MAX					1000
#define K_DEF					1

#define P2_MAX					1e-1f
#define P2_MIN					(-P2_MAX)

#define P1_MIN					0.1f
#define P1_MAX					2

#define P0_MAX					1e3f
#define P0_MIN					(-P0_MAX)

#define RES_MIN					1
#define RES_MAX					10000
#define RES_DEF					100

// in Vrms
#define START_VOLTAGE_MIN		100
#define START_VOLTAGE_MAX		1000
#define START_VOLTAGE_DEF		500

// in kV/sec
#define VOLTAGE_RATE_MIN		0.1f
#define VOLTAGE_RATE_MAX		3
#define VOLTAGE_RATE_DEF		1

#define K_REG_MAX				100

// in Vrms
#define FE_ABS_MAX				1000
#define FE_ABS_DEF				100

// in %
#define FE_REL_MAX				100
#define FE_REL_DEF				10

#define FE_COUNTER_MAX			10
#define FE_COUNTER_DEF			3

// in X:1
#define TRANS_COEFF_MIN			50
#define TRANS_COEFF_MAX			150
#define TRANS_COEFF_DEF			100

// in Vdc
#define PRIM_VOLTAGE_MIN		1
#define PRIM_VOLTAGE_MAX		200
#define PRIM_VOLTAGE_DEF		150

// in %
#define PRIM_VOLTAGE_DIFF_MIN	0
#define PRIM_VOLTAGE_DIFF_MAX	30
#define PRIM_VOLTAGE_DIFF_DEF	2

// in mA
#define ILIM_MIN				0.1f
#define ILIM_MAX				1000
#define ILIM_DEF				10

// in sec
#define FAN_ON_MIN				0
#define FAN_ON_MAX				600
#define FAN_ON_DEF				30

// in Vrms
#define TARGET_VOLTAGE_MIN		500
#define TARGET_VOLTAGE_MAX		14000
#define TARGET_VOLTAGE_DEF		500

// in Irms mA
#define ILIMIT_mA_MAX			10
#define ILIMIT_mA_DEF			5

// in sec
#define VOLTAGE_PLATE_TIME_MIN	5
#define VOLTAGE_PLATE_TIME_MAX	300
#define VOLTAGE_PLATE_TIME_DEF	5

#define SCOPE_RATE_MAX			100		// skip 99 point, write 1
#define SCOPE_RATE_DEF			4

// in regulator ticks
#define SPIKE_COUNTER_MAX		10

// Types
typedef struct __TableItemConstraint
{
	float Min;
	float Max;
	float Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint Constraint[];

#endif // __CONSTRAINTS_H
