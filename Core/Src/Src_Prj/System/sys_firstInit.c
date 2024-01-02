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
/* This function will run once when the system starts */
void sys_firstInit(void){
#if INIT_CONFIGURE_PIN_STATE
	/* Configure the system IO */
	s_configureio_init();
#endif /* INIT_CONFIGURE_PIN_STATE */

#if INIT_STANDBY_FUNCTIONALITY
	/* Keep the system on, or go back to sleep */
	if(sys_checkWakeup() == SLEEP) {sys_enterStandby();}
#endif /* INIT_STANDBY_FUNCTIONALITY */


#if INIT_SAVE_TO_MEMORY
	/* Read the status from the memory */
	s_ROM_init();
#endif /* INIT_SAVE_TO_MEMORY */

	/* Load the serial Number */
	s_loadSerialNumber();

	/* The project name and info shown on the console window. */
	sys_consoleSysInfo();

#if INIT_SYS_HAS_BATTERY
	s_battery_init();
#endif /* INIT_SYS_HAS_BATTERY */

#if INIT_SYS_LOCK_UUID
	s_readUuid();
#endif /* INIT_SYS_LOCK_UUID */

#if	INIT_SYS_REQUEST_CONSOLE_DEBUG
	/* Request debug space on the console */
	sys_consoleRequest();
#endif /* INIT_SYS_REQUEST_CONSOLE_DEBUG */



	interface_firstInit();

}

/************************************************************************************/
