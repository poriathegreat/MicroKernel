/*
 * sys.h
 *
 *  Created on: Jul 11, 2023
 *      Author: HP
 */

#ifndef INC_SYS_H_
#define INC_SYS_H_
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/

/* Include the system */
#include "main.h"

/* Include standard libraries used in the system. */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
/* Include the main system headers. */
#include "sys_addons.h"


//#include "sys_buffer.h"
#include "tasks.h"
#include "sys_debug.h"
/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
/* The structure holding the variables for the sys.c file. */
typedef struct{
	char                     serialNumber[16];    /* The serial number of the device. Should be 16 digits max. */
	char                     version[10];

#if INIT_SYS_LOCK_UUID
	uint8_t                  uuid[12];
#endif /* INIT_SYS_LOCK_UUID */

	_Bool                    keepOn;

	bufferDataStructure_t*   buffer;

#if INIT_SYS_HAS_BATTERY
	s_batteryStructure*      battery;
#endif /* INIT_SYS_HAS_BATTERY */

#if INIT_SAVE_TO_MEMORY
	s_ROMdataStruct*         rom;
#endif /* INIT_SAVE_TO_MEMORY */

#if INIT_SYS_STAT_LED
	s_statLed_struct*        ledStat;
#endif /* INIT_SYS_STAT_LED */

}sysInfoStructure_t;
extern sysInfoStructure_t sysData;
/************************************************************************************/
/*                                Exported functions                                */
/************************************************************************************/
/* Run this function before the super loop.
 * This function will configure the system to run properly. */
void sys_firstInit(void);
/************************************************************************************/
/* This function is always being called. It will check different parts of the system.
 * This function will keep the system running properly
 * and it will show the device status to the user.
 * Place this function in the super loop. */
void sys(void);
/************************************************************************************/
/* Add this to the 1ms timer of the system.
 * */
void sys_systemClock(void);

/* Place copy structure in this function. */
void sys_fetchData(void);




































#endif /* INC_SYS_H_ */
