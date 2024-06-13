#ifndef __BOARD_H
#define __BOARD_H

#include "stm32f30x.h"

#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwTIM.h"
#include "ZwDAC.h"
#include "ZwDMA.h"
#include "ZwADC.h"
#include "ZwEXTI.h"
#include "ZwSCI.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"
#include "ZwSPI.h"

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_R0 =				{GPIOB, Pin_9};
GPIO_PortPinSettingMacro GPIO_R1 = 				{GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_R2 = 				{GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_EXT_IND = 		{GPIOA, Pin_15};
GPIO_PortPinSettingMacro GPIO_LED = 			{GPIOB, Pin_4};
GPIO_PortPinSettingMacro GPIO_FAN = 			{GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_PWR_EN1 =			{GPIOB, Pin_2};
GPIO_PortPinSettingMacro GPIO_PWR_EN2 =			{GPIOB, Pin_10};
GPIO_PortPinSettingMacro GPIO_PWR_EN3 =			{GPIOB, Pin_11};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_SAFETY =			{GPIOB, Pin_13};

// Определения для аналоговых входов
GPIO_PortPinSettingMacro GPIO_MEASURE_VOUT =	{GPIOB, Pin_0};
GPIO_PortPinSettingMacro GPIO_MEASURE_I1 =		{GPIOA, Pin_6};
GPIO_PortPinSettingMacro GPIO_MEASURE_I2 =		{GPIOA, Pin_4};
GPIO_PortPinSettingMacro GPIO_MEASURE_I3 =		{GPIOA, Pin_1};
GPIO_PortPinSettingMacro GPIO_MEASURE_I4 =		{GPIOA, Pin_0};
GPIO_PortPinSettingMacro GPIO_MEASURE_VIN =		{GPIOB, Pin_12};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX =		{GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX =		{GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART_RX =		{GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART_TX =		{GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_PWM1 =		{GPIOA, Pin_8};
GPIO_PortPinSettingMacro GPIO_ALT_PWM2 =		{GPIOB, Pin_14};

#endif // __BOARD_H
