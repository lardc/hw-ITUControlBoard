// ----------------------------------------
// Global definitions
// ----------------------------------------

// Header
#include "Constraints.h"

// Constants
//
const TableItemConstraint Constraint[DATA_TABLE_WP_START] = {
		{0, TICK_LIMIT_MAX, 0},													// 0
		{0, TICK_LIMIT_MAX, 0},													// 1
		{0, TICK_LIMIT_MAX, 0},													// 2
		{0, TICK_LIMIT_MAX, 0},													// 3
		{0, TICK_LIMIT_MAX, 0},													// 4

		{K_MIN, K_MAX, K_DEF},													// 5
		{P2_MIN, P2_MAX, 0},													// 6
		{P1_MIN, P1_MAX, 1},													// 7
		{P0_MIN, P0_MAX, 0},													// 8

		{K_MIN, K_MAX, K_DEF},													// 9
		{P2_MIN, P2_MAX, 0},													// 10
		{P1_MIN, P1_MAX, 1},													// 11
		{P0_MIN, P0_MAX, 0},													// 12

		{K_MIN, K_MAX, K_DEF},													// 13
		{P2_MIN, P2_MAX, 0},													// 14
		{P1_MIN, P1_MAX, 1},													// 15
		{P0_MIN, P0_MAX, 0},													// 16

		{K_MIN, K_MAX, K_DEF},													// 17
		{P2_MIN, P2_MAX, 0},													// 18
		{P1_MIN, P1_MAX, 1},													// 19
		{P0_MIN, P0_MAX, 0},													// 20

		{K_MIN, K_MAX, K_DEF},													// 21
		{P2_MIN, P2_MAX, 0},													// 22
		{P1_MIN, P1_MAX, 1},													// 23
		{P0_MIN, P0_MAX, 0},													// 24

		{K_MIN, K_MAX, K_DEF},													// 25
		{P2_MIN, P2_MAX, 0},													// 26
		{P1_MIN, P1_MAX, 1},													// 27
		{P0_MIN, P0_MAX, 0},													// 28

		{K_MIN, K_MAX, K_DEF},													// 29
		{P2_MIN, P2_MAX, 0},													// 30
		{P1_MIN, P1_MAX, 1},													// 31
		{P0_MIN, P0_MAX, 0},													// 32

		{K_MIN, K_MAX, K_DEF},													// 33
		{P2_MIN, P2_MAX, 0},													// 34
		{P1_MIN, P1_MAX, 1},													// 35
		{P0_MIN, P0_MAX, 0},													// 36

		{K_MIN, K_MAX, K_DEF},													// 37
		{P2_MIN, P2_MAX, 0},													// 38
		{P1_MIN, P1_MAX, 1},													// 39
		{P0_MIN, P0_MAX, 0},													// 40

		{K_MIN, K_MAX, K_DEF},													// 41
		{P2_MIN, P2_MAX, 0},													// 42
		{P1_MIN, P1_MAX, 1},													// 43
		{P0_MIN, P0_MAX, 0},													// 44

		{K_MIN, K_MAX, K_DEF},													// 45
		{P2_MIN, P2_MAX, 0},													// 46
		{P1_MIN, P1_MAX, 1},													// 47
		{P0_MIN, P0_MAX, 0},													// 48

		{K_MIN, K_MAX, K_DEF},													// 49
		{P2_MIN, P2_MAX, 0},													// 50
		{P1_MIN, P1_MAX, 1},													// 51
		{P0_MIN, P0_MAX, 0},													// 52

		{K_MIN, K_MAX, K_DEF},													// 53
		{P2_MIN, P2_MAX, 0},													// 54
		{P1_MIN, P1_MAX, 1},													// 55
		{P0_MIN, P0_MAX, 0},													// 56

		{RES_MIN, RES_MAX, RES_DEF},											// 57
		{RES_MIN, RES_MAX, RES_DEF},											// 58
		{RES_MIN, RES_MAX, RES_DEF},											// 59

		{K_MIN, K_MAX, K_DEF},													// 60

		{START_VOLTAGE_MIN, START_VOLTAGE_MAX, START_VOLTAGE_DEF},				// 61
		{VOLTAGE_RATE_MIN, VOLTAGE_RATE_MAX, VOLTAGE_RATE_DEF},					// 62

		{0, K_REG_MAX, 0},														// 63
		{0, K_REG_MAX, 0},														// 64

		{0, FE_ABS_MAX, FE_ABS_DEF},											// 65
		{0, FE_REL_MAX, FE_REL_DEF},											// 66
		{0, FE_COUNTER_MAX, FE_COUNTER_DEF},									// 67

		{TRANS_COEFF_MIN, TRANS_COEFF_MAX, TRANS_COEFF_DEF},					// 68
		{PRIM_VOLTAGE_MIN, PRIM_VOLTAGE_MAX, PRIM_VOLTAGE_DEF},					// 69
		{PRIM_VOLTAGE_DIFF_MIN, PRIM_VOLTAGE_DIFF_MAX, PRIM_VOLTAGE_DIFF_DEF},	// 70
		{NO, YES, NO},															// 71

		{NO, YES, NO},															// 72

		{ILIM_MIN, ILIM_MAX, ILIM_DEF},											// 73
		{ILIM_MIN, ILIM_MAX, ILIM_DEF},											// 74
		{ILIM_MIN, ILIM_MAX, ILIM_DEF},											// 75

		{0, 0, 0},																// 76
		{0, 0, 0},																// 77
		{0, 0, 0},																// 78
		{0, 0, 0},																// 79
		{0, 0, 0},																// 80
		{0, 0, 0},																// 81
		{0, 0, 0},																// 82
		{0, 0, 0},																// 83
		{0, 0, 0},																// 84
		{0, 0, 0},																// 85
		{0, 0, 0},																// 86
		{0, 0, 0},																// 87
		{0, 0, 0},																// 88
		{0, 0, 0},																// 89
		{0, 0, 0},																// 90
		{0, 0, 0},																// 91
		{0, 0, 0},																// 92
		{0, 0, 0},																// 93
		{0, 0, 0},																// 94
		{0, 0, 0},																// 95
		{0, 0, 0},																// 96
		{0, 0, 0},																// 97
		{0, 0, 0},																// 98
		{0, 0, 0},																// 99
		{0, 0, 0},																// 100
		{0, 0, 0},																// 101
		{0, 0, 0},																// 102
		{0, 0, 0},																// 103
		{0, 0, 0},																// 104
		{0, 0, 0},																// 105
		{0, 0, 0},																// 106
		{0, 0, 0},																// 107
		{0, 0, 0},																// 108
		{0, 0, 0},																// 109
		{0, 0, 0},																// 110
		{0, 0, 0},																// 111
		{0, 0, 0},																// 112
		{0, 0, 0},																// 113
		{0, 0, 0},																// 114
		{0, 0, 0},																// 115
		{0, 0, 0},																// 116
		{0, 0, 0},																// 117
		{0, 0, 0},																// 118
		{0, 0, 0},																// 119
		{0, 0, 0},																// 120
		{0, 0, 0},																// 121
		{0, 0, 0},																// 122
		{0, 0, 0},																// 123
		{0, 0, 0},																// 124
		{0, 0, 0},																// 125
		{0, 0, 0},																// 126
		{0, 0, 0},																// 127

		{TARGET_VOLTAGE_MIN, TARGET_VOLTAGE_MAX, TARGET_VOLTAGE_DEF},			// 128
		{0, ILIMIT_mA_MAX, ILIMIT_mA_DEF},										// 129
		{0, ILIMIT_uA_MAX, 0},													// 130
		{NO, YES, NO},															// 131
		{VOLTAGE_PLATE_TIME_MIN, VOLTAGE_PLATE_TIME_MAX, VOLTAGE_PLATE_TIME_DEF},	// 132
		{0, SCOPE_RATE_MAX, SCOPE_RATE_DEF},									// 133

		{0, 0, 0},																// 134
		{0, 0, 0},																// 135
		{0, 0, 0},																// 136
		{0, 0, 0},																// 137
		{0, 0, 0},																// 138
		{0, 0, 0},																// 139
		{0, 0, 0},																// 140
		{0, 0, 0},																// 141
		{0, 0, 0},																// 142
		{0, 0, 0},																// 143
		{0, 0, 0},																// 144
		{0, 0, 0},																// 145
		{0, 0, 0},																// 146
		{0, 0, 0},																// 147
		{0, 0, 0},																// 148
		{0, 0, 0},																// 149
		{0, 0, 0},																// 150
		{0, 0, 0},																// 151
		{0, 0, 0},																// 152
		{0, 0, 0},																// 153
		{0, 0, 0},																// 154
		{0, 0, 0},																// 155
		{0, 0, 0},																// 156
		{0, 0, 0},																// 157
		{0, 0, 0},																// 158
		{0, 0, 0},																// 159
		{0, 0, 0},																// 160
		{0, 0, 0},																// 161
		{0, 0, 0},																// 162
		{0, 0, 0},																// 163
		{0, 0, 0},																// 164
		{0, 0, 0},																// 165
		{0, 0, 0},																// 166
		{0, 0, 0},																// 167
		{0, 0, 0},																// 168
		{0, 0, 0},																// 169
		{0, 0, 0},																// 170
		{NO, YES, NO},															// 171
		{0, 0, 0},																// 172
		{0, 0, 0},																// 173
		{0, 0, 0},																// 174
		{0, INT16U_MAX, 0},														// 175
		{0, 0, 0},																// 176
		{0, 0, 0},																// 177
		{0, 0, 0},																// 178
		{0, 0, 0},																// 179
		{0, 0, 0},																// 180
		{0, 0, 0},																// 181
		{0, 0, 0},																// 182
		{0, 0, 0},																// 183
		{0, 0, 0},																// 184
		{0, 0, 0},																// 185
		{0, 0, 0},																// 186
		{0, 0, 0},																// 187
		{0, 0, 0},																// 188
		{0, 0, 0},																// 189
		{0, INT16U_MAX, 0},														// 190
		{0, 0, 0}																// 191
};
