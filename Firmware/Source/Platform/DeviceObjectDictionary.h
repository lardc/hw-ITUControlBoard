#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning

#define ACT_DBG_SWITCH_R0				10	// Включить шунтовой резистор R0
#define ACT_DBG_SWITCH_R1				11	// Включить шунтовой резистор R1
#define ACT_DBG_SWITCH_R2				12	// Включить шунтовой резистор R2
#define ACT_DBG_PULSE_EXT_LED			13	// Мигнуть внешним светодиодом (1 сек)
#define ACT_DBG_PULSE_FAN				14	// Мигнуть вентилятором (1 сек)
#define ACT_DBG_READ_SAFETY				15	// Прочитать состояние входа безопасности
#define ACT_DBG_READ_INPUT_V_ADC		16	// Чтение оцифровки входного напряжения
#define ACT_DBG_SET_DAC_OUTPUT			17	// Установить значение на выходе ЦАП

#define ACT_START						100	// Запуск формирования
#define ACT_STOP						101	// Остановка формирования

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------

// Регистры
// Сохраняемые регистры
#define REG_RAW_ZERO_SVOLTAGE			0	// Значение нулевого уровня канала напряжения в тиках оцифровки
#define REG_RAW_ZERO_SCURRENT1			1	// Значение нулевого уровня канала 1 тока в тиках оцифровки
#define REG_RAW_ZERO_SCURRENT2			2	// Значение нулевого уровня канала 2 тока в тиках оцифровки
#define REG_RAW_ZERO_SCURRENT3			3	// Значение нулевого уровня канала 3 тока в тиках оцифровки
#define REG_RAW_ZERO_SCURRENT4			4	// Значение нулевого уровня канала 4 тока в тиках оцифровки

#define REG_COEFF_VOLTAGE_K				5	// Пропорциональный коэффициент пересёта напряжения
#define REG_COEFF_VOLTAGE_P2			6	// Коэффициент тонкой подстройки напряжения P2
#define REG_COEFF_VOLTAGE_P1			7	// Коэффициент тонкой подстройки напряжения P1
#define REG_COEFF_VOLTAGE_P0			8	// Коэффициент тонкой подстройки напряжения P0

#define REG_COEFF_CURRENT1_R0_K			9	// Пропорциональный коэффициент пересёта тока 1 на сопротивлении R0
#define REG_COEFF_CURRENT1_R0_P2		10	// Коэффициент тонкой подстройки тока 1 на сопротивлении R0 P2
#define REG_COEFF_CURRENT1_R0_P1		11	// Коэффициент тонкой подстройки тока 1 на сопротивлении R0 P1
#define REG_COEFF_CURRENT1_R0_P0		12	// Коэффициент тонкой подстройки тока 1 на сопротивлении R0 P0

#define REG_COEFF_CURRENT1_R1_K			13	// Пропорциональный коэффициент пересёта тока 1 на сопротивлении R0
#define REG_COEFF_CURRENT1_R1_P2		14	// Коэффициент тонкой подстройки тока 1 на сопротивлении R1 P2
#define REG_COEFF_CURRENT1_R1_P1		15	// Коэффициент тонкой подстройки тока 1 на сопротивлении R1 P1
#define REG_COEFF_CURRENT1_R1_P0		16	// Коэффициент тонкой подстройки тока 1 на сопротивлении R1 P0

#define REG_COEFF_CURRENT1_R2_K			17	// Пропорциональный коэффициент пересёта тока 1 на сопротивлении R0
#define REG_COEFF_CURRENT1_R2_P2		18	// Коэффициент тонкой подстройки тока 1 на сопротивлении R2 P2
#define REG_COEFF_CURRENT1_R2_P1		19	// Коэффициент тонкой подстройки тока 1 на сопротивлении R2 P1
#define REG_COEFF_CURRENT1_R2_P0		20	// Коэффициент тонкой подстройки тока 1 на сопротивлении R2 P0

#define REG_COEFF_CURRENT2_R0_K			21	// Пропорциональный коэффициент пересёта тока 2 на сопротивлении R0
#define REG_COEFF_CURRENT2_R0_P2		22	// Коэффициент тонкой подстройки тока 2 на сопротивлении R0 P2
#define REG_COEFF_CURRENT2_R0_P1		23	// Коэффициент тонкой подстройки тока 2 на сопротивлении R0 P1
#define REG_COEFF_CURRENT2_R0_P0		24	// Коэффициент тонкой подстройки тока 2 на сопротивлении R0 P0

#define REG_COEFF_CURRENT2_R1_K			25	// Пропорциональный коэффициент пересёта тока 2 на сопротивлении R0
#define REG_COEFF_CURRENT2_R1_P2		26	// Коэффициент тонкой подстройки тока 2 на сопротивлении R1 P2
#define REG_COEFF_CURRENT2_R1_P1		27	// Коэффициент тонкой подстройки тока 2 на сопротивлении R1 P1
#define REG_COEFF_CURRENT2_R1_P0		28	// Коэффициент тонкой подстройки тока 2 на сопротивлении R1 P0

#define REG_COEFF_CURRENT2_R2_K			29	// Пропорциональный коэффициент пересёта тока 2 на сопротивлении R0
#define REG_COEFF_CURRENT2_R2_P2		30	// Коэффициент тонкой подстройки тока 2 на сопротивлении R2 P2
#define REG_COEFF_CURRENT2_R2_P1		31	// Коэффициент тонкой подстройки тока 2 на сопротивлении R2 P1
#define REG_COEFF_CURRENT2_R2_P0		32	// Коэффициент тонкой подстройки тока 2 на сопротивлении R2 P0

#define REG_COEFF_CURRENT3_R0_K			33	// Пропорциональный коэффициент пересёта тока 3 на сопротивлении R0
#define REG_COEFF_CURRENT3_R0_P2		34	// Коэффициент тонкой подстройки тока 3 на сопротивлении R0 P2
#define REG_COEFF_CURRENT3_R0_P1		35	// Коэффициент тонкой подстройки тока 3 на сопротивлении R0 P1
#define REG_COEFF_CURRENT3_R0_P0		36	// Коэффициент тонкой подстройки тока 3 на сопротивлении R0 P0

#define REG_COEFF_CURRENT3_R1_K			37	// Пропорциональный коэффициент пересёта тока 3 на сопротивлении R0
#define REG_COEFF_CURRENT3_R1_P2		38	// Коэффициент тонкой подстройки тока 3 на сопротивлении R1 P2
#define REG_COEFF_CURRENT3_R1_P1		39	// Коэффициент тонкой подстройки тока 3 на сопротивлении R1 P1
#define REG_COEFF_CURRENT3_R1_P0		40	// Коэффициент тонкой подстройки тока 3 на сопротивлении R1 P0

#define REG_COEFF_CURRENT3_R2_K			41	// Пропорциональный коэффициент пересёта тока 3 на сопротивлении R0
#define REG_COEFF_CURRENT3_R2_P2		42	// Коэффициент тонкой подстройки тока 3 на сопротивлении R2 P2
#define REG_COEFF_CURRENT3_R2_P1		43	// Коэффициент тонкой подстройки тока 3 на сопротивлении R2 P1
#define REG_COEFF_CURRENT3_R2_P0		44	// Коэффициент тонкой подстройки тока 3 на сопротивлении R2 P0

#define REG_COEFF_CURRENT4_R0_K			45	// Пропорциональный коэффициент пересёта тока 4 на сопротивлении R0
#define REG_COEFF_CURRENT4_R0_P2		46	// Коэффициент тонкой подстройки тока 4 на сопротивлении R0 P2
#define REG_COEFF_CURRENT4_R0_P1		47	// Коэффициент тонкой подстройки тока 4 на сопротивлении R0 P1
#define REG_COEFF_CURRENT4_R0_P0		48	// Коэффициент тонкой подстройки тока 4 на сопротивлении R0 P0

#define REG_COEFF_CURRENT4_R1_K			49	// Пропорциональный коэффициент пересёта тока 4 на сопротивлении R0
#define REG_COEFF_CURRENT4_R1_P2		50	// Коэффициент тонкой подстройки тока 4 на сопротивлении R1 P2
#define REG_COEFF_CURRENT4_R1_P1		51	// Коэффициент тонкой подстройки тока 4 на сопротивлении R1 P1
#define REG_COEFF_CURRENT4_R1_P0		52	// Коэффициент тонкой подстройки тока 4 на сопротивлении R1 P0

#define REG_COEFF_CURRENT4_R2_K			53	// Пропорциональный коэффициент пересёта тока 4 на сопротивлении R0
#define REG_COEFF_CURRENT4_R2_P2		54	// Коэффициент тонкой подстройки тока 4 на сопротивлении R2 P2
#define REG_COEFF_CURRENT4_R2_P1		55	// Коэффициент тонкой подстройки тока 4 на сопротивлении R2 P1
#define REG_COEFF_CURRENT4_R2_P0		56	// Коэффициент тонкой подстройки тока 4 на сопротивлении R2 P0

#define REG_R0_VALUE					57	// Значение сопротивления R0, Ом
#define REG_R1_VALUE					58	// Значение сопротивления R1, Ом
#define REG_R2_VALUE					59	// Значение сопротивления R2, Ом

#define REG_CAP_COEFF					60	// Коэффициент пересчёта напряжения первичной стороны

#define REG_START_VOLTAGE				61	// Стартовое напряжение формирования (в В)
#define REG_VOLTAGE_RATE				62	// Скорость нарастания напряжения (в кВ/с)

#define REG_KP							63	// Пропорциональный коэффициент регулятора х100
#define REG_KI							64	// Интегральный коэффициент регулятора х100

#define REG_FE_ABSOLUTE					65	// Пороговая абсолютная ошибка регулирования действующего напряжения (в В)
#define REG_FE_RELATIVE					66	// Пороговая относительная ошибка регулирования действующего напряжения (в %)
#define REG_FE_COUNTER_MAX				67	// Максимальное количество срабатываний ошибки следования

#define REG_TRANSFORMER_COFF			68	// Коэффициент трансформации
#define REG_PRIM_VOLTAGE				69	// Напряжение первичной сторны (в В)
#define REG_PRIM_IGNORE_CHECK			70	// Не проверять уровень напряжения первичной стороны (в В)

#define REG_IGNORE_HW_SAFETY			71	// Не обрабатывать аппаратный сигнал контура безопасности

#define REG_I_RANGE_LOW					72	// Предел нижнего диапазона тока, мА
#define REG_I_RANGE_MID					73	// Предел среднего диапазона тока, мА
#define REG_I_RANGE_HIGH				74	// Предел верхнего диапазона тока, мА

// Несохраняемы регистры чтения-записи
#define REG_TEST_VOLTAGE				128	// Действующее напряжение уставки (в В)
#define REG_LIMIT_CURRENT_mA			129	// Предельный ток (часть мА)
#define REG_LIMIT_CURRENT_uA			130	// Предельный ток (часть мкА)
#define REG_STOP_BY_ACTIVE_CURRENT		131	// Режим остановки по активной составляющей тока
#define REG_TEST_TIME					132	// Длительность формирования полки (в сек)
#define REG_SCOPE_RATE					133	// Делитель логгирования данных

#define REG_DBG_SRAM					170	// Write saw-shape debug sequence to SRAM
#define REG_DBG_MUTE_PWM				171	// Mute PWM output

#define REG_DBG							190	// Отладочный регистр

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции
#define REG_SUB_STATE					198	// Регистр вспомогательного состояния

#define REG_VOLTAGE_RESULT				200 // Результат измерения напряжения (в мВ)
#define REG_VOLTAGE_RESULT_32			201 // Результат измерения напряжения (в мВ)
#define REG_CURRENT_RESULT				202 // Результат измерения тока (в мкА)
#define REG_CURRENT_RESULT_32			203 // Результат измерения тока (в мкА)
#define REG_VOLTAGE_READY				204	// Флаг готовности напряжения

#define REG_EP_LAST_POINTER_FAST		250	// Указатель на последий элемент быстрого логгирования
#define REG_EP_LAST_POINTER_RMS			251	// Указатель на последий элемент логгирования RMS
// -----------------------------
#define REG_FWINFO_SLAVE_NID			256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID			257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN				260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN			261	// Begining of the information string record

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

// ENDPOINTS
#define EP_COS_PHI1						1	// Косинус фи канала тока 1
#define EP_COS_PHI2						2	// Косинус фи канала тока 2
#define EP_COS_PHI3						3	// Косинус фи канала тока 3
#define EP_COS_PHI4						4	// Косинус фи канала тока 4
#define EP_PWM							5	// Мгновенное значение ШИМ
#define EP_VOLTAGE_ERROR				6	// Ошибка регулирования напряжения, В

#define EP_VRMS_SETPOINT				5	// Уставка по действующему значению напряжения
#define EP_VRMS_CTRL_SETPOINT			6	// Регулируемая уставка по действующему значению напряжения
#define EP_VRMS_RESULT					7	// Результат по действующему значению напряжения
#define EP_IRMS_RESULT					8	// Результат по действующему значению напряжения

//  Fault and disable codes
#define DF_NONE							0

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_OUTPUT_SATURATION		1	// Напряжение регулирования достигло предела
#define PROBLEM_FOLLOWING_ERROR			2	// Ошибка регулирования

//  Warning
#define WARNING_NONE					0
#define WARNING_RMS_OVER_CURRENT		1	// Превышение установленного действующего значения тока
#define WARNING_INSTANT_OVERCURRENT		2	// Превышение мгновенного значения тока

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ
#define ERR_BAD_CONFIG					5	//  Попытка запуска измерения с неверной конфигурацией

#endif //  __DEV_OBJ_DIC_H
