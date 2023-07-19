// Header
#include "InitConfig.h"

// Include
#include "BCCIxParams.h"
#include "Board.h"
#include "Delay.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "ZwDMA.h"
#include "Global.h"
#include "ZwADC.h"
#include "Timer1PWM.h"
#include "MeasureUtils.h"

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
	GPIO_InitPushPullOutput(GPIO_R0);
	GPIO_InitPushPullOutput(GPIO_R1);
	GPIO_InitPushPullOutput(GPIO_R2);
	GPIO_InitPushPullOutput(GPIO_EXT_IND);
	GPIO_InitPushPullOutput(GPIO_LED);
	GPIO_InitPushPullOutput(GPIO_FAN);
	
	// Входы аналоговые
	GPIO_InitAnalog(GPIO_MEASURE_VOUT);
	GPIO_InitAnalog(GPIO_MEASURE_I1);
	GPIO_InitAnalog(GPIO_MEASURE_I2);
	GPIO_InitAnalog(GPIO_MEASURE_I3);
	GPIO_InitAnalog(GPIO_MEASURE_I4);
	GPIO_InitAnalog(GPIO_MEASURE_VIN);
	
	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART_TX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_PWM1, AltFn_6);
	GPIO_InitAltFunction(GPIO_ALT_PWM2, AltFn_6);
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
	
	// ADC current 3-4 sampling
	DMA_Reset(DMA_ADC1_I_CHANNEL34);
	DMA_Interrupt(DMA_ADC1_I_CHANNEL34, DMA_TRANSFER_COMPLETE, 0, true);
	DMAChannelX_DataConfig(DMA_ADC1_I_CHANNEL34, (uint32_t)DMACurrent34, (uint32_t)(&ADC1->DR), ADC_DMA_CURRENT_SAMPLES);
	DMAChannelX_Config(DMA_ADC1_I_CHANNEL34, DMA_MEM2MEM_DIS, DMA_LvlPriority_LOW, DMA_MSIZE_16BIT, DMA_PSIZE_16BIT,
			DMA_MINC_EN, DMA_PINC_DIS, DMA_CIRCMODE_DIS, DMA_READ_FROM_PERIPH);
	
	// ADC current 1-2 sampling
	DMA_Reset(DMA_ADC2_I_CHANNEL12);
	DMA_Interrupt(DMA_ADC2_I_CHANNEL12, DMA_TRANSFER_COMPLETE, 0, true);
	DMAChannelX_DataConfig(DMA_ADC2_I_CHANNEL12, (uint32_t)DMACurrent12, (uint32_t)(&ADC2->DR), ADC_DMA_CURRENT_SAMPLES);
	DMAChannelX_Config(DMA_ADC2_I_CHANNEL12, DMA_MEM2MEM_DIS, DMA_LvlPriority_LOW, DMA_MSIZE_16BIT, DMA_PSIZE_16BIT,
			DMA_MINC_EN, DMA_PINC_DIS, DMA_CIRCMODE_DIS, DMA_READ_FROM_PERIPH);
	
	// ADC voltage sampling
	DMA_Reset(DMA_ADC3_V_CHANNEL);
	DMA_Interrupt(DMA_ADC3_V_CHANNEL, DMA_TRANSFER_COMPLETE, 0, true);
	DMAChannelX_DataConfig(DMA_ADC3_V_CHANNEL, (uint32_t)DMAVoltage, (uint32_t)(&ADC3->DR), ADC_DMA_VOLTAGE_SAMPLES);
	DMAChannelX_Config(DMA_ADC3_V_CHANNEL, DMA_MEM2MEM_DIS, DMA_LvlPriority_LOW, DMA_MSIZE_16BIT, DMA_PSIZE_16BIT,
			DMA_MINC_EN, DMA_PINC_DIS, DMA_CIRCMODE_DIS, DMA_READ_FROM_PERIPH);

	DMA_ChannelEnable(DMA_ADC1_I_CHANNEL34, true);
	DMA_ChannelEnable(DMA_ADC2_I_CHANNEL12, true);
	DMA_ChannelEnable(DMA_ADC3_V_CHANNEL, true);
}
//------------------------------------------------

void INITCFG_ADC()
{
	// Скоростная оцифровка на АЦП1-2
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

	for (uint8_t i = 1; i <= ADC_DMA_CURRENT_SAMPLES; i += 2)
	{
		ADC_ChannelSet_Sequence(ADC1, ADC1_CURRENT_CHANNEL4, i);
		ADC_ChannelSet_Sequence(ADC1, ADC1_CURRENT_CHANNEL3, i + 1);

		ADC_ChannelSet_Sequence(ADC2, ADC2_CURRENT_CHANNEL2, i);
		ADC_ChannelSet_Sequence(ADC2, ADC2_CURRENT_CHANNEL1, i + 1);
	}

	ADC_ChannelSeqLen(ADC1, ADC_DMA_CURRENT_SAMPLES);
	ADC_ChannelSeqLen(ADC2, ADC_DMA_CURRENT_SAMPLES);
	
	ADC_DMAEnable(ADC1, true);
	ADC_DMAEnable(ADC2, true);

	// Настройка АЦП3 для быстройо цифровки и АЦП4 для медленной
	RCC_ADC_Clk_EN(ADC_34_ClkEN);

	ADC_Calibration(ADC3);
	ADC_Calibration(ADC4);

	ADC_Enable(ADC3);
	ADC_Enable(ADC4);

	ADC_SoftTrigConfig(ADC3);
	ADC_SoftTrigConfig(ADC4);

	ADC_ChannelSeqReset(ADC3);
	ADC_ChannelSeqReset(ADC4);

	for (uint8_t i = 1; i <= ADC_DMA_VOLTAGE_SAMPLES; i++)
		ADC_ChannelSet_Sequence(ADC3, ADC3_OUT_VOLTAGE_CHANNEL, i);

	ADC_ChannelSeqLen(ADC3, ADC_DMA_VOLTAGE_SAMPLES);
	ADC_DMAEnable(ADC3, true);
}
//------------------------------------------------

void INITCFG_DAC()
{
	DAC_ClkEnable(DAC1);
	DAC_Reset(DAC1);

	DAC_EnableCh2(DAC1);
	DAC_BufferCh2(DAC1, false);
	DAC_TriggerConfigCh2(DAC1, TRIG2_SOFTWARE, TRIG2_ENABLE);

	DAC_SetValueCh2(DAC1, 0);
}
//------------------------------------------------
