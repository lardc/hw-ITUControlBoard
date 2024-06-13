// Include
//
#include "Global.h"
#include "Controller.h"
#include "Interrupts.h"
#include "SysConfig.h"
#include "BoardConfig.h"
#include "BCCIxParams.h"

// Forward functions
//
void ConfigSysClk();
void ConfigGPIO();
void ConfigCAN();
void ConfigUART();
void ConfigTimer2();
void ConfigWatchDog();

// Functions
//
int main()
{
	// Set request flag if firmware update is required
	if(*ProgramAddressStart == 0xFFFFFFFF || BOOT_LOADER_VARIABLE == BOOT_LOADER_REQUEST)
		WaitForFWUpload = TRUE;
	
	// Init peripherals
	ConfigSysClk();
	ConfigGPIO();
	ConfigCAN();
	ConfigUART();
	ConfigTimer2();
	ConfigWatchDog();
	
	// Init controller
	CONTROL_Init();
	
	// Infinite cycle
	while(true)
		CONTROL_Idle();
	
	return 0;
}
//--------------------------------------------

void ConfigSysClk()
{
	RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//--------------------------------------------

void ConfigGPIO()
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
	GPIO_InitPushPullOutput(GPIO_ALT_PWM1);
	GPIO_InitPushPullOutput(GPIO_ALT_PWM2);
	GPIO_InitPushPullOutput(GPIO_PWR_EN1);
	GPIO_InitPushPullOutput(GPIO_PWR_EN2);
	GPIO_InitPushPullOutput(GPIO_PWR_EN3);
	
	// Альтернативные функции портов
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART_TX, AltFn_7);

	// Смена состояния портов
	GPIO_SetState(GPIO_PWR_EN3, true);
}
//--------------------------------------------

void ConfigCAN()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, CAN_SLAVE_FILTER_ID, CAN_SLAVE_NID_MASK);
}
//--------------------------------------------

void ConfigUART()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//--------------------------------------------

void ConfigTimer2()
{
	TIM_Clock_En(TIM_2);
	TIM_Config(TIM2, SYSCLK, TIMER2_uS);
	TIM_Interupt(TIM2, 0, true);
	TIM_Start(TIM2);
}
//--------------------------------------------

void ConfigWatchDog()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//--------------------------------------------
