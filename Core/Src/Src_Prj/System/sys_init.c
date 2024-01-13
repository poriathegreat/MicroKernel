/*
 * sys_firstInit.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system internal functions */
#include "system_internal.h"
#include "interface.h"
#include "tasks.h"
/* This function will run once when the system starts */
void sys_firstInit(void){
#if INIT_CONFIGURE_PIN_STATE
	/* Configure the system IO */
	s_configureio_init();
#endif /* INIT_CONFIGURE_PIN_STATE */

#if INIT_STANDBY_FUNCTIONALITY
	/* Keep the system on, or go back to sleep */
	if(s_checkWakeup() == SLEEP) {s_enterStandbyNow();}
#endif /* INIT_STANDBY_FUNCTIONALITY */


#if INIT_SAVE_TO_MEMORY
	/* Read the status from the memory */
	s_ROM_init();
	sysData.rom = &s_ROMdata;
#endif /* INIT_SAVE_TO_MEMORY */

	/* Load the serial Number */
	s_loadSerialNumber();

	/* The project name and info shown on the console window. */
	sys_consoleSysInfo();

#if INIT_SYS_HAS_BATTERY
	s_battery_init();
	sysData.battery = &s_batteryData;
#endif /* INIT_SYS_HAS_BATTERY */

#if INIT_SYS_LOCK_UUID
	s_readUuid();
#endif /* INIT_SYS_LOCK_UUID */

#if	INIT_SYS_REQUEST_CONSOLE_DEBUG
	/* Request debug space on the console */
	sys_consoleRequest();
#endif /* INIT_SYS_REQUEST_CONSOLE_DEBUG */

#if INIT_SYS_BUFFER
	/* Handles the data buffer */
	s_buffer_init();
	sysData.buffer = &s_bufferData;
#endif /* INIT_SYS_BUFFER */

#if INIT_SYS_STAT_LED
	sysData.ledStat = &s_statLedData;
#endif /* INIT_SYS_STAT_LED */


	interface_init();

	/* At last, we initialize the tasks. */
	tasks_init();
}

/************************************************************************************/
