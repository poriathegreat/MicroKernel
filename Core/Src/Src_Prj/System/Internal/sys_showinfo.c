/*
 * sys_showinfo.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
#include "system_internal.h"


/* The debug space at the top of the program. Representing the device information. */
uint8_t sys_infoSpace = RESET;
/************************************************************************************/
/* Project information shown on the console when the device is turned on. */
void sys_consoleSysInfo(void){
	/* This will clear the current console. (This command may not be supported by some of the console viewers.)
	 * It IS supported on PuTTy */
#if DEBUGGING
	debugprintf("\e[1;1H\e[2J\b");

	/* Write the system version down for later use. */
	sprintf(sysData.version, SYS_VERSION);

	/* View the project info when the debug window starts up */
	char BuildVersion[] = SYS_PROJECT_NAME " - Built (" __DATE__ "::" __TIME__ ")";
	sys_infoSpace = console_requestSpace(3, BuildVersion);
	console_printf(sys_infoSpace, CONSOLE_PART_ONE, "Serial Number");
	console_printf(sys_infoSpace, CONSOLE_PART_TWO, "[%s]", sysData.serialNumber);
	console_printf(sys_infoSpace+1, CONSOLE_PART_ONE, "Software Version");
	console_printf(sys_infoSpace+1, CONSOLE_PART_TWO, "[%s]", sysData.version);
	console_printf(sys_infoSpace+2, CONSOLE_PART_ONE, "System Counter");
#endif /* DEBUGGING */
}
/************************************************************************************/
