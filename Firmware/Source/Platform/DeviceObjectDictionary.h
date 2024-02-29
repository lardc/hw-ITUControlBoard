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
#define ACT_DBG_READ_INPUT_V_ADC		16	// Чтение оцифровки напряжения первичной стороны
#define ACT_DBG_SET_DAC_OUTPUT			17	// Установить значение на выходе ЦАП
#define ACT_DBG_READ_OUTPUT_V_ADC		18	// Чтение оцифровки напряжения HV стороны

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
#define REG_PRIM_VOLTAGE_MAX_ERR		70	// Максимальное отклонение первичного напряжения (в %)
#define REG_PRIM_IGNORE_CHECK			71	// Не проверять уровень напряжения первичной стороны (в В)

#define REG_IGNORE_HW_SAFETY			72	// Не обрабатывать аппаратный сигнал контура безопасности

#define REG_I_RANGE_LOW					73	// Предел нижнего диапазона тока, мА
#define REG_I_RANGE_MID					74	// Предел среднего диапазона тока, мА
#define REG_I_RANGE_HIGH				75	// Предел верхнего диапазона тока, мА

#define REG_FAN_ON_TIME					76	// Время работы вентилятора после процесса формирования, с

// Несохраняемы регистры чтения-записи
#define REG_TEST_VOLTAGE				128	// Действующее напряжение уставки (в В)
#define REG_LIMIT_CURRENT_mA			129	// Предельный ток (часть мА)
#define REG_LIMIT_CURRENT_uA			130	// Предельный ток (часть мкА)
#define REG_STOP_BY_ACTIVE_CURRENT		131	// Режим остановки по активной составляющей тока
#define REG_TEST_TIME					132	// Длительность формирования полки (в сек)
#define REG_SCOPE_RATE					133	// Делитель логгирования данных

#define REG_DBG_MUTE_PWM				171	// Mute PWM output

#define REG_DBG_GENERAL					175	// Отладочный регистр общего назначения

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_FINISHED					197	// Регистр результата операции
#define REG_VOLTAGE_READY				198	// Флаг выхода на уставку по напряжению
#define REG_TEST_PASSED					199	// Флаг успешного прохождения испытания прибором

#define REG_RESULT_V					200	// Результат по напряжению, В

#define REG_RESULT_I1_mA				201	// Результат по току канала 1 часть значения мА
#define REG_RESULT_I1_uA				202	// Результат по току канала 1 часть значения мкА
#define REG_RESULT_I1_ACT_mA			203	// Результат по активному току канала 1 часть значения мА
#define REG_RESULT_I1_ACT_uA			204	// Результат по активному току канала 1 часть значения мкА
#define REG_RESULT_COS_PHI1				205	// Значение косинус фи канала 1 x1000

#define REG_RESULT_I2_mA				206	// Результат по току канала 2 часть значения мА
#define REG_RESULT_I2_uA				207	// Результат по току канала 2 часть значения мкА
#define REG_RESULT_I2_ACT_mA			208	// Результат по активному току канала 2 часть значения мА
#define REG_RESULT_I2_ACT_uA			209	// Результат по активному току канала 2 часть значения мкА
#define REG_RESULT_COS_PHI2				210	// Значение косинус фи канала 2 x1000

#define REG_RESULT_I3_mA				211	// Результат по току канала 3 часть значения мА
#define REG_RESULT_I3_uA				212	// Результат по току канала 3 часть значения мкА
#define REG_RESULT_I3_ACT_mA			213	// Результат по активному току канала 3 часть значения мА
#define REG_RESULT_I3_ACT_uA			214	// Результат по активному току канала 3 часть значения мкА
#define REG_RESULT_COS_PHI3				215	// Значение косинус фи канала 3 x1000

#define REG_RESULT_I4_mA				216	// Результат по току канала 4 часть значения мА
#define REG_RESULT_I4_uA				217	// Результат по току канала 4 часть значения мкА
#define REG_RESULT_I4_ACT_mA			218	// Результат по активному току канала 4 часть значения мА
#define REG_RESULT_I4_ACT_uA			219	// Результат по активному току канала 4 часть значения мкА
#define REG_RESULT_COS_PHI4				220	// Значение косинус фи канала 4 x1000

#define REG_FAILED_CURRENT_CHANNEL		221	// Номер отказавшего канала тока

#define REG_INFO_TOTAL_TIME				222	// Время с момента начала измерения (в сек)
#define REG_INFO_PLATE_TIME				223	// Время с момента выхода на задание (в сек)

#define REG_INFO_V						224	// Информация о напряжении
#define REG_INFO_I1_mA					225	// Информация о токе, канал 1 (в mA)
#define REG_INFO_I2_mA					226	// Информация о токе, канал 2 (в mA)
#define REG_INFO_I3_mA					227	// Информация о токе, канал 3 (в mA)
#define REG_INFO_I4_mA					228	// Информация о токе, канал 4 (в mA)

#define REG_PRIMARY_SIDE_VOLTAGE		230	// Напряжение первичной стороны, В
#define REG_SAFETY_IS_OK				231	// Состояние контура безопасности

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

#define FEP_VOLTAGE						1	// Мгновенное напряэжение, В
#define FEP_CURRENT1					2	// Мгновенный ток канала 1, мА
#define FEP_CURRENT2					3	// Мгновенный ток канала 2, мА
#define FEP_CURRENT3					4	// Мгновенный ток канала 3, мА
#define FEP_CURRENT4					5	// Мгновенный ток канала 4, мА

#define FEP_VOLTAGE_RMS					6	// Действующее напряэжение, В
#define FEP_CURRENT_RMS1				7	// Действующий ток канала 1, мА
#define FEP_CURRENT_RMS2				8	// Действующий ток канала 2, мА
#define FEP_CURRENT_RMS3				9	// Действующий ток канала 3, мА
#define FEP_CURRENT_RMS4				10	// Действующий ток канала 4, мА

//  Fault and disable codes
#define DF_NONE							0	// No fault
// 1 - режим совместимости
#define DF_PRIMARY_VOLTAGE				2	// Primary voltage failure

// Problem
#define PROBLEM_NONE					0	// No problem
#define PROBLEM_STOP					1	// Stop by user command or safety signal
#define PROBLEM_FOLLOWING_ERROR			2	// Following error
#define PROBLEM_PWM_SATURATION			3	// PWM reached upper limit

//  Warning
#define WARNING_NONE					0	// No warning
#define WARNING_CURR_RANGE_SAT			1	// Current exceeds upper limit level

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ
#define ERR_BAD_CONFIG					5	//  Попытка запуска измерения с неверной конфигурацией

#endif //  __DEV_OBJ_DIC_H
