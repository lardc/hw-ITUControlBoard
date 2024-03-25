// Header
#include "Timer1PWM.h"

// Includes
#include "ZwTIM.h"
#include "ZwRCC.h"
#include "math.h"
#include "LowLevel.h"

// Variables
static uint32_t PWMBase = 0;

// Functions
void T1PWM_Init(uint32_t SystemClock, uint32_t Period)
{
	// Расчёт размерности ШИМ
	uint32_t Prescaler = (uint32_t)((float)SystemClock / 1000000 * Period / 65536);
	PWMBase = (uint32_t)((SystemClock / ((Prescaler + 1) * 1000000)) * Period);

	// Стандартная инициализация
	TIM_Clock_En(TIM_1);

	TIM1->PSC = Prescaler;
	TIM1->ARR = PWMBase;

	// Канал 1 - положительный
	// Режим ШИМ - PWM mode 1, c функцией Preload
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
	TIM1->CCR1 = 0;

	// Канал 2 - отрицательный
	// Режим ШИМ - PWM mode 1, c функцией Preload
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
	TIM1->CCR2 = 0;

	// Разрешение выход CH1 и CH2N
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2NE;
	// Разрешение работы таймера на выход
	TIM1->BDTR |= TIM_BDTR_MOE;

	// Сброс выхода
	T1PWM_SetDutyCycle(0);

	// Инициализация обновления регистров
	TIM1->SR &= ~TIM_SR_UIF;
	TIM1->EGR |= TIM_EGR_UG;
	while(!(TIM1->SR & TIM_SR_UIF));
	TIM1->SR &= ~TIM_SR_UIF;
}
//------------------------------------------------

void T1PWM_SetDutyCycle(int16_t Value)
{
	// Выбор полярности формирователя
	if(Value > 0)
	{
		TIM1->CCR1 = Value;
		TIM1->CCR2 = 0;
	}
	else if(Value < 0)
	{
		TIM1->CCR1 = 0;
		TIM1->CCR2 = -Value;
	}
	else
		TIM1->CCR1 = TIM1->CCR2 = 0;
}
//------------------------------------------------

uint32_t T1PWM_GetPWMBase()
{
	return PWMBase;
}
//------------------------------------------------

void T1PWM_Start()
{
	TIM_Start(TIM1);
}
//------------------------------------------------

void T1PWM_Stop()
{
	T1PWM_SetDutyCycle(0);
	TIM_Stop(TIM1);

	// Запрет прерывания и очистка флага
	TIM1->DIER &= ~TIM_DIER_UIE;
	TIM1->SR &= ~TIM_SR_UIF;

	// Форсирование обновления регистров таймера
	TIM1->EGR |= TIM_EGR_UG;
	while(!(TIM1->SR & TIM_SR_UIF));
	TIM1->SR &= ~TIM_SR_UIF;

	// Разрешение прерывания
	TIM1->DIER |= TIM_DIER_UIE;
}
//------------------------------------------------
