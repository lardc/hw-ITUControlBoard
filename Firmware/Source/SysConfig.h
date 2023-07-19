#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H


// Flash loader options
#define BOOT_LOADER_VARIABLE			(*((volatile uint32_t *)0x20000000))
#define BOOT_LOADER_REQUEST				0x12345678
#define BOOT_LOADER_MAIN_PR_ADDR		0x08008000
//-----------------------------------------------

// System clock
#define SYSCLK							70000000	// Тактовая частота системной шины процессора
#define QUARTZ_FREQUENCY				20000000	// Частота кварца
// ---------------------------------------------

// USART
#define USART_BAUDRATE					115200		// Скорость USART
#define USART_FIFOlen					32			// Длина FIFO USART
// ---------------------------------------------

// PWM
#define PWM_FREQUENCY					20000		// в Гц
#define PWM_PERIOD						(1000000 / PWM_FREQUENCY)	// в мкс
#define PWM_SINE_FREQ					50			// Частота формируемой синусоиды (в Гц)
#define PWM_SINE_COUNTER_MAX			(PWM_FREQUENCY / PWM_SINE_FREQ)
#define PWM_PRIMARY_VOLTAGE_LOW			12			// в В
#define PWM_PRIMARY_VOLTAGE_HIGH		50			// в В
// ---------------------------------------------

// MEAS RANGE
#define MEASURE_RANGE_LOW				1
#define MEASURE_RANGE_MIDDLE			2
#define MEASURE_RANGE_HIGH				3
// ---------------------------------------------

// Timers
#define TIMER7_uS						1000		// в мкс
// ---------------------------------------------

// CAN
#define CAN_BAUDRATE					1000000		// Битрейт CAN
// ---------------------------------------------

// High-speed sampling
#define DMA_ADC1_I_CHANNEL34			DMA1_Channel1	// Канал DMA для АЦП1 каналов тока 3 и 4
#define DMA_ADC2_I_CHANNEL12			DMA2_Channel1	// Канал DMA для АЦП2 каналов тока 1 и 2
#define DMA_ADC3_V_CHANNEL				DMA2_Channel5	// Канал DMA для АЦП3 канала напряжения
//
#define ADC1_CURRENT_CHANNEL4			1				// АЦП1 номер канала тока 4
#define ADC1_CURRENT_CHANNEL3			2				// АЦП1 номер канала тока 3
#define ADC2_CURRENT_CHANNEL2			1				// АЦП2 номер канала тока 2
#define ADC2_CURRENT_CHANNEL1			3				// АЦП2 номер канала тока 1
#define ADC3_OUT_VOLTAGE_CHANNEL		1				// АЦП3 номер канала выходного напряжения
#define ADC4_IN_VOLTAGE_CHANNEL			3				// АЦП4 номер канала входного напряжения
// ---------------------------------------------

#endif // __SYSCONFIG_H
