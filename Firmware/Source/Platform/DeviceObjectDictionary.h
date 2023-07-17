#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning

#define ACT_DBG_SYNC_1_IMPULSE			50 // Сигнал синхронизации
#define ACT_DBG_SYNC_2_IMPULSE			51 // Сигнал синхронизации
#define ACT_DBG_SET_STATE_CTRL_RELAY	52 // Изменить состояние реле цепи CTRL
#define ACT_DBG_SET_STATE_POW_RELAY		53 // Изменить состояние реле цепи POW

#define ACT_START_SIGNAL				100	// Запуск формирования сигнала
#define ACT_STOP_SIGNAL					101	// Остановка формирования сигнала

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------

// Регистры
// Сохраняемые регистры
#define REG_KP							0	// Коэффициент регулятора П (x1000)
#define REG_KI							1	// Коэффициент регулятора И (x1000)

#define REG_CURRENT_RANGE1_RES			2	// Сопротивление шунта диаразона тока 1
#define REG_CURRENT_RANGE2_RES			3	// Сопротивление шунта диаразона тока 2
#define REG_CURRENT_RANGE3_RES			4	// Сопротивление шунта диаразона тока 3

#define REG_ADC_V1_CONV_K_DENOM			5	// Знаменатель коэфф пересчета напряжения диапазона 1 x1000
#define REG_ADC_V2_CONV_K_DENOM			6	// Знаменатель коэфф пересчета напряжения диапазона 1 x1000
#define REG_ADC_I1_CONV_K_DENOM			7	// Знаменатель коэфф пересчета тока диапазона 1 x1000
#define REG_ADC_I2_CONV_K_DENOM			8	// Знаменатель коэфф пересчета тока диапазона 2 x1000
#define REG_ADC_I3_CONV_K_DENOM			9	// Знаменатель коэфф пересчета тока диапазона 3 x1000

#define REG_VOLTAGE_RANGE1_LIMIT		10	// Граница диапазона 1 по действующему напряжению (в В)
#define REG_CURRENT_RANGE1_LIMIT		11	// Граница диапазона 1 по действующему току (в мкА)
#define REG_CURRENT_RANGE2_LIMIT		12	// Граница диапазона 2 по действующему току (в мкА)
#define REG_CURRENT_RANGE3_LIMIT		13	// Граница диапазона 3 по действующему току (в мА)
//14
#define REG_ADC_V1_FINE_P2				15	// Коэффициент тонкой подстройки напряжения диапазона 1 P2 x1e6
#define REG_ADC_V1_FINE_P1				16  // Коэффициент тонкой подстройки напряжения диапазона 1 P1 x1000
#define REG_ADC_V1_FINE_P0				17	// Коэффициент тонкой подстройки напряжения диапазона 1 P0
#define REG_ADC_V1_CONV_K				18	// Коэффициент пересчёта напряжения диапазона 1 x1000
#define REG_ADC_V1_CONV_B				19	// Оффсет напряжения диапазона 1 (в мВ входа АЦП)

#define REG_ADC_V2_FINE_P2				20	// Коэффициент тонкой подстройки напряжения диапазона 2 P2 x1e6
#define REG_ADC_V2_FINE_P1				21  // Коэффициент тонкой подстройки напряжения диапазона 2 P1 x1000
#define REG_ADC_V2_FINE_P0				22	// Коэффициент тонкой подстройки напряжения диапазона 2 P0
#define REG_ADC_V2_CONV_K				23	// Коэффициент пересчёта напряжения диапазона 2 x1000
#define REG_ADC_V2_CONV_B				24	// Оффсет напряжения диапазона 2 (в мВ входа АЦП)

#define REG_ADC_I1_FINE_P2				25	// Коэффициент тонкой подстройки тока диапазона 1 P2 x1e6
#define REG_ADC_I1_FINE_P1				26  // Коэффициент тонкой подстройки тока диапазона 1 P1 x1000
#define REG_ADC_I1_FINE_P0				27	// Коэффициент тонкой подстройки тока диапазона 1 P0
#define REG_ADC_I1_CONV_K				28	// Коэффициент пересчёта тока диапазона 1 x1000
#define REG_ADC_I1_CONV_B				29	// Оффсет тока диапазона 1 (в мВ входа АЦП)

#define REG_ADC_I2_FINE_P2				30	// Коэффициент тонкой подстройки тока диапазона 2 P2 x1e6
#define REG_ADC_I2_FINE_P1				31  // Коэффициент тонкой подстройки тока диапазона 2 P1 x1000
#define REG_ADC_I2_FINE_P0				32	// Коэффициент тонкой подстройки тока диапазона 2 P0
#define REG_ADC_I2_CONV_K				33	// Коэффициент пересчёта тока диапазона 2 x1000
#define REG_ADC_I2_CONV_B				34	// Оффсет тока диапазона 2 (в мВ входа АЦП)

#define REG_ADC_I3_FINE_P2				35	// Коэффициент тонкой подстройки тока диапазона 3 P2 x1e6
#define REG_ADC_I3_FINE_P1				36  // Коэффициент тонкой подстройки тока диапазона 3 P1 x1000
#define REG_ADC_I3_FINE_P0				37	// Коэффициент тонкой подстройки тока диапазона 3 P0
#define REG_ADC_I3_CONV_K				38	// Коэффициент пересчёта тока диапазона 3 x1000
#define REG_ADC_I3_CONV_B				39	// Оффсет тока диапазона 3 (в мВ входа АЦП)

#define REG_PWM_DUTY_OFFSET				40	// Оффсет минимальной ширины импульса (тик)

#define REG_RMS_V1_FINE_P2				41	// Коэффициент тонкой подстройки RMS напряжения диапазона 1 P2 x1e6
#define REG_RMS_V1_FINE_P1				42  // Коэффициент тонкой подстройки RMS напряжения диапазона 1 P1 x1000
#define REG_RMS_V1_FINE_P0				43	// Коэффициент тонкой подстройки RMS напряжения диапазона 1 P0
#define REG_RMS_V2_FINE_P2				44	// Коэффициент тонкой подстройки RMS напряжения диапазона 2 P2 x1e6
#define REG_RMS_V2_FINE_P1				45  // Коэффициент тонкой подстройки RMS напряжения диапазона 2 P1 x1000
#define REG_RMS_V2_FINE_P0				46	// Коэффициент тонкой подстройки RMS напряжения диапазона 2 P0
#define REG_RMS_I1_FINE_P2				47	// Коэффициент тонкой подстройки RMS тока диапазона 1 P2 x1e6
#define REG_RMS_I1_FINE_P1				48  // Коэффициент тонкой подстройки RMS тока диапазона 1 P1 x1000
#define REG_RMS_I1_FINE_P0				49	// Коэффициент тонкой подстройки RMS тока диапазона 1 P0 x1000
#define REG_RMS_I2_FINE_P2				50	// Коэффициент тонкой подстройки RMS тока диапазона 2 P2 x1e6
#define REG_RMS_I2_FINE_P1				51  // Коэффициент тонкой подстройки RMS тока диапазона 2 P1 x1000
#define REG_RMS_I2_FINE_P0				52	// Коэффициент тонкой подстройки RMS тока диапазона 2 P0 x1000
#define REG_RMS_I3_FINE_P2				53	// Коэффициент тонкой подстройки RMS тока диапазона 3 P2 x1e6
#define REG_RMS_I3_FINE_P1				54  // Коэффициент тонкой подстройки RMS тока диапазона 3 P1 x1000
#define REG_RMS_I3_FINE_P0				55	// Коэффициент тонкой подстройки RMS тока диапазона 3 P0
// 56 - 59
#define REG_PWM_TRANS_RATIO				60	// Коэффициент трансформации
#define REG_PWM_OUT_VOLTAGE_LIMIT		61	// Ограничение по выходному напряжению (в В)
#define REG_PWM_VOLTAGE_RISE_RATE		62	// Скорость нарастания выходного напряжения (в В/с)
#define REG_PWM_VOLTAGE_READY_THR		63	// Относительная погрешность срабатывания флага готовности напряжения (в %)
// 63 - 69
#define REG_FE_ENABLE					70	// Разрешение контроля FollowingError
#define REG_FE_RELATIVE_LEVEL			71	// Относительный уровень ошибки срабатывания счётчика FE (в %)
#define REG_FE_COUNTER_MAX				72	// Максимальное значение счётчика FE
#define REG_FE_ABSOLUTE_LEVEL			73	// Абсолютный уровень ошибки срабатывания счётчика FE (в В)

// Несохраняемы регистры чтения-записи
#define REG_VOLTAGE_SETPOINT			128 // Действующее значение напряжения выходного сигнала - младшее слово, мВ
#define REG_VOLTAGE_SETPOINT_32			129 // Действующее значение напряжения выходного сигнала - старшее слово, мВ
#define REG_CURRENT_SETPOINT			130 // Действующее значение тока выходного сигнала - младшее слово, мкА
#define REG_CURRENT_SETPOINT_32			131 // Действующее значение тока выходного сигнала - старшее слово, мкА
#define REG_OUTPUT_LINE					132 // Канал подачи выходного сигнала

#define REG_SCOPE_RATE					150	// Шаг логгирования в эндпоинты

#define REG_CURR_RANGE_1_SAFETY_LIMIT	181	// Запас по току диапазона 1 до срабатывания ограничения (%)
#define REG_CURR_RANGE_2_SAFETY_LIMIT	182	// Запас по току диапазона 2 до срабатывания ограничения (%)
#define REG_CURR_RANGE_3_SAFETY_LIMIT	183	// Запас по току диапазона 3 до срабатывания ограничения (%)

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
#define EP_V_SETPOINT_FAST				1	// Быстрая уставка по напряжению
#define EP_PWM_SETPOINT_FAST			2	// Быстрая уставка по ШИМ
#define EP_V_RESULT_FAST				3	// Быстрая оцифровка напряжения
#define EP_I_RESULT_FAST				4	// Быстрая оцифровка тока

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
