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
GPIO_PortPinSettingMacro GPIO_CTRL_PWMSD_1 = {GPIOA, Pin_6};
GPIO_PortPinSettingMacro GPIO_CTRL_PWMSD_2 = {GPIOA, Pin_7};

GPIO_PortPinSettingMacro GPIO_HIGH_VOLTAGE = {GPIOA, Pin_15};

GPIO_PortPinSettingMacro GPIO_POW_RELAY = {GPIOB, Pin_5};
GPIO_PortPinSettingMacro GPIO_CTRL_RELAY = {GPIOB, Pin_4};

GPIO_PortPinSettingMacro GPIO_CTRL_SYNC_1 = {GPIOB, Pin_9};
GPIO_PortPinSettingMacro GPIO_CTRL_SYNC_2 = {GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_LED = {GPIOB, Pin_10};
GPIO_PortPinSettingMacro GPIO_LED_EXT = {GPIOB, Pin_11};
GPIO_PortPinSettingMacro GPIO_I_RANGE_M = {GPIOB, Pin_12};
GPIO_PortPinSettingMacro GPIO_I_RANGE_H = {GPIOB, Pin_13};
GPIO_PortPinSettingMacro GPIO_I_RANGE_L = {GPIOB, Pin_14};
GPIO_PortPinSettingMacro GPIO_U_RANGE = {GPIOB, Pin_15};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_MEAS_U = {GPIOA, Pin_0};
GPIO_PortPinSettingMacro GPIO_MEAS_I = {GPIOA, Pin_5};
GPIO_PortPinSettingMacro GPIO_SYNC_1 = {GPIOB, Pin_6};
GPIO_PortPinSettingMacro GPIO_SYNC_2 = {GPIOB, Pin_7};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX = {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX = {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART_RX = {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART_TX = {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_CTRL_PWM_1 = {GPIOA, Pin_8};
GPIO_PortPinSettingMacro GPIO_CTRL_PWM_2 = {GPIOB, Pin_0};

#endif // __BOARD_H
