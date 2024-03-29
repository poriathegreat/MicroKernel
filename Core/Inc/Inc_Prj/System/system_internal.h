/*
 * system_internal.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSTEM_SYSTEM_INTERNAL_H_
#define INC_INC_PRJ_SYSTEM_SYSTEM_INTERNAL_H_
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include types */
#include "sys.h"
/************************************************************************************/
/*                                  Private Macros                                  */
/************************************************************************************/
/* The system timers used to run main */
enum sysTimers{
#if INIT_SYS_HAS_BATTERY
	TIMERS_SYS_CHECKBATTERY,
#if INIT_CHARGING_LED
	TIMERS_SYS_CHARGING_LED_BLINK_INT,
	TIMERS_SYS_CHARGING_LED_BLINK_EXT,
#endif /* INIT_CHARGING_LED */
#endif /* INIT_SYS_HAS_BATTERY */

#if INIT_SYS_STAT_LED
	TIMERS_SYS_STATUS_LED_BLINK_INT,
	TIMERS_SYS_STATUS_LED_BLINK_EXT,
#endif /* INIT_SYS_STAT_LED */

	TIMERS_SYS_UPTIME,
	SYSTIMERS_TOTAL				//This has to be the last element on the list.
};
/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
extern uint8_t sys_infoSpace;
/************************************************************************************/
/*                                Exported functions                                */
/************************************************************************************/
/* Project information shown on the console when the device is turned on. */
void sys_consoleSysInfo(void);

/* This will show the system up time on top of the status bar. */
void sys_upTime(void);

/* Add this function to the system 1ms timer.   */
void system_tick(void);

/* Use this function to check a specific timer. */
uint32_t sys_timerCheck(uint8_t timer);

/* Use this function to reset a specific timer.	*/
void sys_timerReset(uint8_t timer);

/* This function will read the unique id of the device. */
void s_readUuid(void);

/* This function will add a debug space to the console window. */
void sys_consoleRequest(void);

/* Add this to the 1ms timer of the system.
 * */
void sys_systemClock(void);

#endif /* INC_INC_PRJ_SYSTEM_SYSTEM_INTERNAL_H_ */
