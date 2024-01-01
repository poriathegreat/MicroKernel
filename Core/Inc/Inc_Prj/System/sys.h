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
/* Include the system configurations */
#include "system_setup.h"
/* Include standard libraries used in the system. */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
/* Include the main system headers. */
#include "buffer.h"
#include "tasks.h"
#include "debug.h"
#include "interface.h"
/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
/* The structure holding the variables for the sys.c file. */
typedef struct{
	char				serialNumber[16];    /* The serial number of the device. Should be 16 digits max. */
	char				version[10];
#if INIT_SYS_HAS_BATTERY
	uint8_t				batteryPercent;
	uint16_t			batteryVoltage;
#endif /* INIT_SYS_HAS_BATTERY */
#if INIT_SYS_LOCK_UUID
	uint8_t 			uuid[12];
#endif /* INIT_SYS_LOCK_UUID */

#if INIT_SYS_LOCK_UUID
	_Bool 			keepOn;
#endif /* INIT_SYS_LOCK_UUID */


}sysInfoStructure_t;
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

/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
/* The main system structure.
 * All the data from the different parts of the system will be saved here.
 * There should be ONE parameter per system part.
 * All the information related to that specific part of the system should be
 * written in its own structure, within this structure. */
typedef struct {
	/* This is how every part of the system should be added here */
	sysInfoStructure_t 		sys;
	/***************************************
	 * Sub-system structures should each be
	 * added here as they are added to the
	 *  project.
	 ***************************************/


	/* The memory exported data structure */
//	bufferDataStructure_t	buffer;
}sysDataStructure_t; extern sysDataStructure_t sysData;


































#endif /* INC_SYS_H_ */
