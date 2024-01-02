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
void s_enterStandbyNow(void){
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_PWR_EnterSTANDBYMode();
}


/* This function will check if the requirements to start the system have been met.
 * The way the wake up function works is:
 * The Watchdog resets the system, then the wakeup parameter is checked,
 * if the wake up requirement is not met, the system goes back to sleep. */
syswakeup_t s_checkWakeup(void){
	syswakeup_t result = SLEEP;

	/* For the system to wake up, one of the following conditions must be met. */
	if(
			/* Edit these conditions to suit the needs of the project */
			(sysData.sys.keepOn == SET)	/* If the keepAwake flag is set*/
#if INIT_SYS_HAS_BATTERY
			||
			(sysData.sys.batteryPercent > 1)	/* If we have enough power. */
#endif /* INIT_SYS_HAS_BATTERY */
#if CHARGER_DETECT_GPIO
			||
			(HAL_GPIO_ReadPin(CHARGER_DETECT_GPIO_Port, CHARGER_DETECT_Pin) == GPIO_PIN_RESET)	/* If the charger is connected */
#endif /* CHARGER_DETECT_GPIO */

#if BUTTON_PRESENT
			||
			(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)	/* If a button is being pressed. */
#endif
	){
		result = AWAKE;
	}
	return result;
}



/* This function will check if the requirements to put the system to sleep have been met.*/
syswakeup_t s_checkSleep(void){

	syswakeup_t result = AWAKE;
	/* For the system to sleep, all of the following conditions must be met. */
	if(
			/* Edit these conditions to suit the needs of the project */
			(sysData.sys.keepOn == RESET)	/* If the keepAwake flag is not set*/
#if INIT_SYS_HAS_BATTERY
			||
			(sysData.sys.batteryPercent == 0)
#endif /* INIT_SYS_HAS_BATTERY */
	){
		result = SLEEP;
	}
	return result;
}








#endif /* INIT_STANDBY_FUNCTIONALITY */
/************************************************************************************/
