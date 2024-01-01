/*
 * sys_statusled.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
#include "sys_statusled.h"

#if INIT_SYS_STAT_LED
/************************************************************************************/
/* The status led function. This function will take the state of the system and show us
 * using a single LED. */
void s_defaultLed(void);

s_statLed_struct s_statLed_data = {0};


uint8_t s_getBlinksFromStatus(void){
	uint8_t result;
	if 		(s_statLed_data.deviceState == 1) result = 2;
	else if (s_statLed_data.deviceState == 2) result = 4;
	else if (s_statLed_data.deviceState == 3) result = 6;
	else if (s_statLed_data.deviceState == 4) result = 10;
	else if (s_statLed_data.deviceState == 5) result = 16;
	else 	result = 2;
	return result;
}


enum led_timers{
	LEDTIMER_STATUS_LED_BLINK_EXT,
	LEDTIMER_STATUS_LED_BLINK_INT,

#if INIT_CHARGING_LED
	LEDTIMER_BATTERY_LED_BLINK_EXT,
	LEDTIMER_BATTERY_LED_BLINK_INT,
#endif /* INIT_CHARGING_LED */

	LEDTIMERS_TOTAL
};
uint32_t ledTimers[LEDTIMERS_TOTAL] = {0};
void s_statLed_tick(void){
	for(uint8_t i = 0; i<LEDTIMERS_TOTAL; i++){
		ledTimers[i]++;
	}
}

#if INIT_CHARGING_LED
/* Blink led to show charging level of device. */
void s_showBattery(_Bool chargerConnected){
	const uint32_t battLedRepeatTimer = 5000;
	const uint16_t battLedBlinkTimer = 2500;
	static uint8_t battLedToggleTotal = 0;
	static uint8_t battLedToggleCounter = 0;

	battLedToggleTotal = (uint8_t)(sysData.sys.batteryPercent / 10);
	battLedToggleTotal = battLedToggleTotal * 2;
	if(ledTimers[LEDTIMER_BATTERY_LED_BLINK_EXT] > battLedRepeatTimer){
		if(battLedToggleCounter > battLedToggleTotal){
			ledTimers[LEDTIMER_BATTERY_LED_BLINK_EXT] = RESET;
			battLedToggleCounter = RESET;

		}else if(ledTimers[LEDTIMER_BATTERY_LED_BLINK_INT] > (battLedBlinkTimer / battLedToggleTotal)){
			battLedToggleCounter++;
			if(!(battLedToggleCounter > battLedToggleTotal)){
				HAL_GPIO_TogglePin(_CHARGING_LED_GPIO, _CHARGING_LED_Pin);
			}
			ledTimers[LEDTIMER_BATTERY_LED_BLINK_INT] = RESET;
		}
	}else {
		HAL_GPIO_WritePin(_CHARGING_LED_GPIO, _CHARGING_LED_Pin, chargerConnected);
	}
}
#endif /* INIT_CHARGING_LED */

/* Blink LED to show current status of the device */
void s_statusLed_main(void){
#if INIT_CHARGING_LED
#if INIT_COMMON_CHARGIN_AND_STAT_LED
	if(HAL_GPIO_ReadPin(_CHARGER_GPIO, _CHARGER_Pin) == WHEN_CHARGER_CONNECTED){
		s_showBattery(RESET);
	}else {
		s_defaultLed();
	}
#else
	s_showBattery(HAL_GPIO_ReadPin(_CHARGER_GPIO, _CHARGER_Pin));
	s_defaultLed();
#endif /* INIT_COMMON_CHARGIN_AND_STAT_LED */
#else
	s_defaultLed();
#endif /* INIT_CHARGING_LED */
}


/* Blink LED to show current status of the device */
void s_defaultLed(void){
	const uint32_t statusLedRepeatTimer = 2000;
	const uint16_t statusLedBlinkTimer = 600;
	static uint8_t statusLedToggleTotal = 0;
	static uint8_t statusLedToggleCounter = 0;

	statusLedToggleTotal = s_getBlinksFromStatus();

	if(ledTimers[LEDTIMER_STATUS_LED_BLINK_EXT] > statusLedRepeatTimer){
		if(statusLedToggleCounter > statusLedToggleTotal){
			ledTimers[LEDTIMER_STATUS_LED_BLINK_EXT] = RESET;
			statusLedToggleCounter = RESET;

		}else if(ledTimers[LEDTIMER_STATUS_LED_BLINK_INT] > (statusLedBlinkTimer / statusLedToggleTotal)){
			statusLedToggleCounter++;
			if(!(statusLedToggleCounter > statusLedToggleTotal)){
				HAL_GPIO_TogglePin(_STAT_LED_GPIO, _STAT_LED_Pin);
			}
			ledTimers[LEDTIMER_STATUS_LED_BLINK_INT] = RESET;
		}
	}else {
		if(s_statLed_data.anti){
			HAL_GPIO_WritePin(_STAT_LED_GPIO, _STAT_LED_Pin, SET);
		}else {
			HAL_GPIO_WritePin(_STAT_LED_GPIO, _STAT_LED_Pin, RESET);
		}
	}
}

























#endif
