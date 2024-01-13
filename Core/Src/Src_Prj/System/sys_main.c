/*
 * sys.c
 *
 *  Created on: Jul 11, 2023
 *      Author: PoriaTheGreat
 */
#include "system_internal.h"
#include "interface.h"
#include "tasks.h"
/* The watch dog handle. We will refresh the watch dog in the sys() function. */
extern IWDG_HandleTypeDef hiwdg;
/************************************************************************************/
/*                                 Private Variables                                */
/************************************************************************************/
///* Initialize the sysData structure. This structure is only initialized here
// * it is defined and exported in the sys.h file to be used across the system. */
//sysDataStructure_t sysData = {0};
/* The timer and flag arrays. This array is built during compile time
 * with a fixed size of NUMBER_OF_SYS_ which is automatically adjusted in the
 * relevant ENUM. */
uint32_t sysTimersArray[SYSTIMERS_TOTAL] = {0};
/* The debug space requested. */
#if INIT_SYS_REQUEST_CONSOLE_DEBUG
uint8_t sys_consoleSpace = RESET;
#endif /* INIT_SYS_REQUEST_DEBUG */
sysInfoStructure_t sysData = {0};

/************************************************************************************/
/*                                Private Functions                                 */
/************************************************************************************/
#if INIT_SYS_REQUEST_CONSOLE_DEBUG
/* This function will add a debug space to the console window. */
void sys_consoleRequest(void){
	sys_consoleSpace = console_requestSpace(CONSOLE_DEBUG_LINES_NEEDED, "SYS DEBUG SPACE");
}
#endif /* INIT_SYS_REQUEST_CONSOLE_DEBUG */

/* This function is always being called. It will check different parts of the system.
 * Place this function in the superloop. */
void sys(void){
	/* Refresh the system so that it does not reset. */
	HAL_IWDG_Refresh(&hiwdg);

	/* This function is called every 1m.
	 * This 1ms timer is not very exact. */
	sys_systemClock();


#if INIT_SAVE_TO_MEMORY
	/* Periodically save the status to the memory */
	s_ROM_main();
#endif /* INIT_SAVE_TO_MEMORY */

#if INIT_STANDBY_FUNCTIONALITY
	/* Check if to put the system to sleep or not. */
	if(s_checkSleep() == SLEEP) {
#if INIT_SAVE_TO_MEMORY
		/* Save the device state before sleeping */
		s_ROM_saveNow();
#endif /* INIT_SAVE_INTERNAL_MEMORY */
		s_enterStandbyNow();
	}
#endif /* INIT_STANDBY_FUNCTIONALITY */

#if INIT_SYS_HAS_BATTERY
	s_battery_main();
#endif /* INIT_SYS_HAS_BATTERY */

#if INIT_SYS_STAT_LED
	/* blink led to show device status */
	s_statusLed_main();
#endif /* INIT_SYS_STAT_LED */

#if INIT_SYS_BUFFER
	/* Handles the data buffer */
	s_buffer_main();
#endif /* INIT_SYS_BUFFER */

	/* Shows the system up time on the top status bar */
	sys_upTime();

	tasks_main();
}

/* Add this function to a 1ms timer call back to enable timed multitasking */
void system_tick(void){
	for(uint8_t timer = RESET ; timer < SYSTIMERS_TOTAL ; timer++){
		sysTimersArray[timer]++;
	}
}

/* Use this function to check if a specific timer has reached a specific number */
uint32_t sys_timerCheck(uint8_t timer){
	return sysTimersArray[timer];
}


/* Use this function to reset a specific timer */
void sys_timerReset(uint8_t timer){
	sysTimersArray[timer]=RESET;
}















