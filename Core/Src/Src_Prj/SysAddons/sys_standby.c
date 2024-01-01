/*
 * sys_standby.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
#include "sys_standby.h"

#if INIT_STANDBY_FUNCTIONALITY

/* This function will make the MCU enter Standby Mode
 * In this mode, the MCU will wake up by
 * 1. The WAKEUP interrupt occurs.
 * 2. WATCHDOG timer overflows. */
void sys_enterStandby(void){
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_PWR_EnterSTANDBYMode();
}


/* This function will check if the requirements to start the system have been met.
 * The way the wake up function works is:
 * The Watchdog resets the system, then the wakeup parameter is checked,
 * if the wake up requirement is not met, the system goes back to sleep. */
syswakeup_t sys_checkWakeup(void){
	syswakeup_t result = SLEEP;

	/* For the system to wake up, one of the following conditions must be met. */
	if(
			/* Edit these conditions to suit the needs of the project */
			(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)	/* If the button is being held*/
			||
			(sysData.sys.keepOn == SET)	/* Or if it's been written in the memory before hand */
	){
		result = AWAKE;
	}
	return result;
}

/* This function will check if the requirements to put the system to sleep have been met.*/
syswakeup_t sys_checkSleep(void){
	syswakeup_t result = AWAKE;
	/* For the system to sleep, all of the following conditions must be met. */
	if(
			/* Edit these conditions to suit the needs of the project */
			(sysData.sys.keepOn == RESET)	/* If the keepAwake flag is not set*/
			&&
			(sysData.gsm.idle == SET)	/* The GSM is IDLE */
	){
		result = SLEEP;
	}
	return result;
}
#endif /* INIT_STANDBY_FUNCTIONALITY */
/************************************************************************************/
