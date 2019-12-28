/*
 * LowPowerDriver.h
 *
 *  Created on: Dec 28, 2019
 *      Author: janus
 */




/**
 * This library can put MCU into 3 different modes:
 *
 * \par SLEEP mode
 *
 * - Cortex-M4 core is stopped
 * - Peripherals kept running
 * - How to enter this mode?
 *    - Use TM_lowPowerSleepUntilInterrupt() or
 *    - Use TM_lowPowerSleepUntilEvent();
 * - How to exit this mode?
 *    - Any peripheral interrupt acknowledged by the nested vectored interrupt controller (NVIC)
 *
 * \par STOP mode
 *
 * - All clocks in 1.2V domain are stopped
 * - PLL, HSI and HSE RC oscillators disabled
 * - Internal SRAM and register contents are preserved
 * - Voltage regulator in low-power mode
 * - How to enter this mode?
 *    - Use TM_lowPowerStopUntilEvent() or
 *    - Use TM_lowPowerStopUntilInterrupt()
 * - How to exit this mode?
 *    - Any EXTI Line (Internal or External) configured in Interrupt/Event mode
 *
 * \par STANDBY mode
 *
 * - Cortex-M4 deepsleep mode
 * - Voltage regulator disabled
 * - 1.2V domain consequently powered off
 * - PLL, HSI and HSE RC oscillators disabled
 * - SRAM and register contents are lost except for the RTC registers, RTC backup registers, backup SRAM and Standby circuitry
 * - How to enter this mode?
 *    - Use TM_lowPowerStandby() function
 * - How to exit this mode?
 *    - Wakeup pin rising edge
 *    - RTC alarm (Alarm A and B) and wakeup
 *    - tamper and time-stamp event
 *    - external reset in NRST pin
 *    - IWDG reset
 *
 */

/**
 * @brief  Put device into sleep mode
 *
 * @note   MCU will be in sleep mode until next interrupt occured.
 *         This means, that delay timer (Systick or any user selectable delay timer) can wakeup STM32F4 too.
 *         If you want prevent this, you have to disable delay timer as well it.
 *         This can be done with this function.
 *
 * @param  delay_timer: Set to > 0 and delay timer will be disabled or set to 0 and it will stay as is
 * @retval None
 */

#pragma once
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_pwr.h"
#include "gpio.h"

void lowPowerGPIOdisable(void);
void resumeClockFromStop(void);
bool isResetFromStandby(void);
void lowPowerResume(void);
void lowPowerSleepUntilInterrupt();
void lowPowerSleepUntilEvent(void);
void lowPowerStopUntilInterrupt(void);
void lowPowerStopUntilEvent(void);
void lowPowerStandby(void);

