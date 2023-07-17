// Header
#include "InitConfig.h"

// Include
#include "BCCIxParams.h"
#include "Board.h"
#include "Delay.h"
#include "LowLevel.h"
#include "PWM.h"
#include "SysConfig.h"
#include "ZwDMA.h"
#include "Global.h"
#include "ZwADC.h"
#include "Measure.h"
#include "Timer1PWM.h"

// Functions
Boolean INITCFG_ConfigSystemClock()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------

void INITCFG_ConfigIO()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	
	// Выходы
	GPIO_InitPushPullOutput(GPIO_LED);
	GPIO_InitPushPullOutput(GPIO_LED_EXT);
	GPIO_InitPushPullOutput(GPIO_CTRL_SYNC_1);
	GPIO_InitPushPullOutput(GPIO_CTRL_SYNC_2);
	GPIO_InitPushPullOutput(GPIO_I_RANGE_H);
	GPIO_InitPushPullOutput(GPIO_I_RANGE_M);
	GPIO_InitPushPullOutput(GPIO_I_RANGE_L);
	GPIO_InitPushPullOutput(GPIO_U_RANGE);
	GPIO_InitPushPullOutput(GPIO_POW_RELAY);
	GPIO_InitPushPullOutput(GPIO_CTRL_RELAY);
	GPIO_InitPushPullOutput(GPIO_CTRL_PWMSD_1);
	GPIO_InitPushPullOutput(GPIO_CTRL_PWMSD_2);
	GPIO_InitPushPullOutput(GPIO_HIGH_VOLTAGE);
	
	// Входы аналоговые
	GPIO_InitAnalog(GPIO_MEAS_U);
	GPIO_InitAnalog(GPIO_MEAS_I);
	
	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART_TX, AltFn_7);
	GPIO_InitAltFunction(GPIO_CTRL_PWM_1, AltFn_6);
	GPIO_InitAltFunction(GPIO_CTRL_PWM_2, AltFn_6);
	
	LL_EnablePWMOut(true);
	LL_ConnectPOWRelay(false);
	LL_ConnectCTRLRelay(false);
}
//------------------------------------------------

void INITCFG_ConfigCAN()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, false);
	NCAN_FIFOInterrupt(true);
	NCAN_FilterInit(0, CAN_SLAVE_FILTER_ID, CAN_SLAVE_NID_MASK);
}
//------------------------------------------------

void INITCFG_ConfigUART()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//------------------------------------------------

void INITCFG_ConfigTimer7()
{
	TIM_Clock_En(TIM_7);
	TIM_Config(TIM7, SYSCLK, TIMER7_uS);
	TIM_Interupt(TIM7, 0, true);
	
	TIM_Start(TIM7);
}
//------------------------------------------------

void INITCFG_ConfigWatchDog()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//------------------------------------------------

void INITCFG_PWM()
{
	T1PWM_Init(SYSCLK, PWM_PERIOD);
}
//------------------------------------------------

void INITCFG_DMA()
{
	DMA_Clk_Enable(DMA1_ClkEN);
	DMA_Clk_Enable(DMA2_ClkEN);
	
	// ADC1 voltage sampling
	DMA_Reset(DMA_ADC1_V_CHANNEL);
	DMA_Interrupt(DMA_ADC1_V_CHANNEL, DMA_TRANSFER_COMPLETE, 0, true);
	DMAChannelX_DataConfig(DMA_ADC1_V_CHANNEL, (uint32_t)ADC1DMAVoltageBuffer, (uint32_t)(&ADC1->DR), ADC_DMA_BUFF_SIZE);
	DMAChannelX_Config(DMA_ADC1_V_CHANNEL, DMA_MEM2MEM_DIS, DMA_LvlPriority_LOW, DMA_MSIZE_16BIT, DMA_PSIZE_16BIT,
			DMA_MINC_EN, DMA_PINC_DIS, DMA_CIRCMODE_DIS, DMA_READ_FROM_PERIPH);
	
	// ADC2 current sampling
	DMA_Reset(DMA_ADC2_I_CHANNEL);
	DMA_Interrupt(DMA_ADC2_I_CHANNEL, DMA_TRANSFER_COMPLETE, 0, true);
	DMAChannelX_DataConfig(DMA_ADC2_I_CHANNEL, (uint32_t)ADC2DMACurrentBuffer, (uint32_t)(&ADC2->DR), ADC_DMA_BUFF_SIZE);
	DMAChannelX_Config(DMA_ADC2_I_CHANNEL, DMA_MEM2MEM_DIS, DMA_LvlPriority_LOW, DMA_MSIZE_16BIT, DMA_PSIZE_16BIT,
			DMA_MINC_EN, DMA_PINC_DIS, DMA_CIRCMODE_DIS, DMA_READ_FROM_PERIPH);
	
	DMA_ChannelEnable(DMA_ADC1_V_CHANNEL, true);
	DMA_ChannelEnable(DMA_ADC2_I_CHANNEL, true);
}
//------------------------------------------------

void INITCFG_ADC()
{
	RCC_ADC_Clk_EN(ADC_12_ClkEN);
	ADC1_2_SetDualMode(true);
	
	ADC_Calibration(ADC1);
	ADC_Calibration(ADC2);
	
	ADC_Enable(ADC1);
	ADC_Enable(ADC2);
	
	ADC_SoftTrigConfig(ADC1);
	ADC_SoftTrigConfig(ADC2);
	
	ADC_ChannelSeqReset(ADC1);
	ADC_ChannelSeqReset(ADC2);

	for (uint8_t i = 1; i <= ADC_DMA_BUFF_SIZE; ++i)
	{
		ADC_ChannelSet_Sequence(ADC1, ADC1_VOLTAGE_CHANNEL, i);
		ADC_ChannelSet_Sequence(ADC2, ADC2_CURRENT_CHANNEL, i);
	}

	ADC_ChannelSeqLen(ADC1, ADC_DMA_BUFF_SIZE);
	ADC_ChannelSeqLen(ADC2, ADC_DMA_BUFF_SIZE);
	
	ADC_DMAEnable(ADC1, true);
	ADC_DMAEnable(ADC2, true);
}
//------------------------------------------------
