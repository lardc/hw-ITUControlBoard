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

#define P2_MAX					1e-3f
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

// in Vrms
#define TARGET_VOLTAGE_MIN		1000
#define TARGET_VOLTAGE_MAX		10500
#define TARGET_VOLTAGE_DEF		1000

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
