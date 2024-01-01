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
/* The device state. The state will count the led blinks.
 * This function should return a number between 1 and 15.
 * the numbers shouldn't be too close to each other
 * so the that user could tell the difference. */
uint8_t sys_getDeviceState(void){
	/* Min of 1 and maximum of 15*/
	uint8_t deviceState = 2;

	if(gsm_flagCheck(FLAGS_GSM_TCP_OPEN)){
		deviceState = 16;
	}else if( ( gsm_flagCheck(FLAGS_GSM_TCP_DATA_READY) ) && ( gsm_flagCheck(FLAGS_GSM_HAS_IP) ) ){
		deviceState = 8;
	}else if( gsm_flagCheck(FLAGS_GSM_HAS_REGISTERED) ){
		deviceState = 4;
	}else{
		deviceState = 2;
	}
	return deviceState;
}



/* Blink LED to show current status of the device */
void sys_statusLED(void){
#if INIT_CHARGING_LED
#if INIT_COMMON_CHARGIN_AND_STAT_LED
	if(HAL_GPIO_ReadPin(_CHARGER_GPIO, _CHARGER_Pin) == WHEN_CHARGER_CONNECTED){
		sys_chargingLed(sysData.sys.batteryPercent);
	}else {
		sys_defaultLed();
	}
#else
	if(HAL_GPIO_ReadPin(_CHARGER_GPIO, _CHARGER_Pin) == WHEN_CHARGER_CONNECTED){
		sys_chargingLed(sysData.sys.batteryPercent);
	}
	sys_defaultLed();
#endif /* INIT_COMMON_CHARGIN_AND_STAT_LED */
#else
	sys_defaultLed();
#endif /* INIT_CHARGING_LED */
}


/* Blink LED to show current status of the device */
void sys_defaultLed(void){
	const uint32_t statusLedRepeatTimer = 2000;
	const uint16_t statusLedBlinkTimer = 600;
	static uint8_t statusLedToggleTotal = 0;
	static uint8_t statusLedToggleCounter = 0;

	statusLedToggleTotal = sys_getDeviceState();

	if(sys_timerCheck(TIMERS_SYS_STATUS_LED_BLINK_EXT) > statusLedRepeatTimer){
		if(statusLedToggleCounter > statusLedToggleTotal){
			sys_timerReset(TIMERS_SYS_STATUS_LED_BLINK_EXT);
			statusLedToggleCounter = RESET;

		}else if(sys_timerCheck(TIMERS_SYS_STATUS_LED_BLINK_INT) > (statusLedBlinkTimer / statusLedToggleTotal)){
			statusLedToggleCounter++;
			if(!(statusLedToggleCounter > statusLedToggleTotal)){
				HAL_GPIO_TogglePin(_STAT_LED_GPIO, _STAT_LED_Pin);
			}
			sys_timerReset(TIMERS_SYS_STATUS_LED_BLINK_INT);
		}
	}else {
		if(gsm_flagCheck(FLAGS_GSM_INIT)){
			HAL_GPIO_WritePin(_STAT_LED_GPIO, _STAT_LED_Pin, SET);
		}else {
			HAL_GPIO_WritePin(_STAT_LED_GPIO, _STAT_LED_Pin, RESET);
		}
	}
}

























#endif
