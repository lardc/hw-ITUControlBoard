// ----------------------------------------
// Board parameters
// ----------------------------------------

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

// Board includes
//
#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwSCI.h"
#include "ZwTIM.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"

// Definitions
//
// Выходы
GPIO_PortPinSettingMacro GPIO_R0 =				{GPIOB, Pin_9};
GPIO_PortPinSettingMacro GPIO_R1 = 				{GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_R2 = 				{GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_EXT_IND = 		{GPIOA, Pin_15};
GPIO_PortPinSettingMacro GPIO_LED = 			{GPIOB, Pin_4};
GPIO_PortPinSettingMacro GPIO_FAN = 			{GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_ALT_PWM1 =		{GPIOA, Pin_8};
GPIO_PortPinSettingMacro GPIO_ALT_PWM2 =		{GPIOB, Pin_14};
GPIO_PortPinSettingMacro GPIO_PWR_EN1 =			{GPIOB, Pin_2};
GPIO_PortPinSettingMacro GPIO_PWR_EN2 =			{GPIOB, Pin_10};
GPIO_PortPinSettingMacro GPIO_DIS_EN3 =			{GPIOB, Pin_11};

// Альтернативные функции
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX =		{GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX =		{GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART_RX =		{GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART_TX =		{GPIOA, Pin_9};

#define LED_BLINK_PORT		GPIO_LED.Port
#define LED_BLINK_PIN		GPIO_LED.Pin

#endif // __BOARD_CONFIG_H
