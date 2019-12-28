/*
 * LowPowerDriver.c
 *
 *  Created on: Dec 28, 2019
 *      Author: janus
 */

#include "LowPowerDriver.h"

void lowPowerGPIOdisable(void) {
	//turn of all gpio expect exti pin
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOE_CLK_DISABLE();
	__HAL_RCC_GPIOF_CLK_DISABLE();
	__HAL_RCC_GPIOG_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
	__HAL_RCC_GPIOI_CLK_DISABLE();
}

void resumeClockFromStop(void) {
	//need to check it with MCO pin and osciloscope
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	uint32_t pFLatency = 0;

	/* Get the Oscillators configuration according to the internal RCC registers */
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

	/* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/* Get the Clocks configuration according to the internal RCC registers */
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK) {
		Error_Handler();
	}
}

bool isResetFromStandby(void){
	return (PWR->CSR & (PWR_FLAG_SB | PWR_FLAG_WU)) == (PWR_FLAG_SB | PWR_FLAG_WU);
}

void lowPowerResume(void) {
	MX_GPIO_Init();
	//add more peripherials if needed
}
void lowPowerSleepUntilInterrupt() {
	lowPowerGPIOdisable();
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	lowPowerResume();
	HAL_ResumeTick();
}
void lowPowerSleepUntilEvent(void) {
	lowPowerGPIOdisable();
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
	lowPowerResume();
	HAL_ResumeTick();
}
void lowPowerStopUntilInterrupt(void) {
	lowPowerGPIOdisable();
	HAL_PWREx_EnableFlashPowerDown();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	resumeClockFromStop();
	lowPowerResume();
}
void lowPowerStopUntilEvent(void) {
	lowPowerGPIOdisable();
	HAL_PWREx_EnableFlashPowerDown();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
	resumeClockFromStop();
	lowPowerResume();
}
void lowPowerStandby(void) {
//	there is no need to configure PA0 as SYSWKUP.
//	HAL_EnableDBGStandbyMode(); //for debug
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
//  /* Reset RTC Domain */
  __HAL_RCC_BACKUPRESET_FORCE();
  __HAL_RCC_BACKUPRESET_RELEASE();

	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	HAL_PWR_EnterSTANDBYMode();
}

