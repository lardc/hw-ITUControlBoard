#ifndef __GLOBAL_H
#define __GLOBAL_H

// Параметры интерфейса
#define	SCCI_TIMEOUT_TICKS		1000		// Таймаут интерфейса SCCI (в мс)
#define EP_WRITE_COUNT			0			// Количество массивов для записи
#define EP_COUNT				6			// Количество массивов для чтения
#define FEP_COUNT				0			// Количество массивов float для чтения
#define ENABLE_LOCKING			FALSE		// Защита NV регистров паролем
#define VALUES_x_SIZE			1000		// Размер буфера Endpoint

// Временные параметры
#define TIME_LED_BLINK			500			// Мигание светодиодом (в мс)

// Ограничения задаваемой величины тока (в мкА)
#define CURRENT_OUTPUT_MIN		10
#define CURRENT_OUTPUT_MAX		110000

// Параметры оцифровки
#define ADC_REF_VOLTAGE			3000.0f		// Опорное напряжение (в В)
#define ADC_RESOLUTION			4095		// Разрешение АЦП
#define ADC_DMA_SAMPLES 		4			// Количество сэмплов одного канала АЦП для ДМА
#define ADC_DMA_VOLTAGE_SAMPLES	ADC_DMA_SAMPLES
#define ADC_DMA_CURRENT_SAMPLES	(ADC_DMA_SAMPLES * 2)

// Параметры остановки / снижения ШИМ
#define PWM_MIN_REDUCE_RATE			50		// Минимальная скорость снижения ШИМ в тиках
#define PWM_REDUCE_RATE_MAX_STEPS	4		// Максимум шагов снижения
#define PWM_TRIM_CTRL_VOLTAGE		2500	// Амплитудное напряжение, на котором выполняется подрезка ШИМ снизу (в В)

// Параметры обработки КЗ
#define SC_VOLTAGE_THR				200		// Максимальное напряжение при КЗ (в В)
#define BR_DOWN_VOLTAGE_RATIO		0.7f	// Минимальное соотношение фактического напряжения уставки при пробое
#define BR_DOWM_VOLTAGE_SET_MIN		500		// Минимальное напряжение уставки для определения пробоя (в В)

#endif //  __GLOBAL_H
