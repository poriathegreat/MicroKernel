/*
 * sys_loadserialnumber.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
#include "sys_loadserialnumber.h"
/************************************************************************************/
/* The device serial number if INIT_LOAD_SERIAL_FROM_MEMORY is disabled. */
#if !INIT_LOAD_SERIAL_FROM_MEMORY
uint8_t hardcodedSerial[] = SYS_HARDCODED_SERIAL;
#endif
/************************************************************************************/
#if INIT_LOAD_SERIAL_FROM_MEMORY
void sys_loadSerialNumberFromMemory(uint8_t* loadedSerialNumber){
	/* write code here to load serial number from memory. */



}
#endif /* INIT_LOAD_FROM_MEMORY */
/************************************************************************************/
/* This function will read the deviceSerialNumber and write it to sysData.sys.serialNumber. */
void sys_loadSerialNumber(void){
#if INIT_LOAD_SERIAL_FROM_MEMORY
	sys_loadSerialNumberFromMemory(sysData.sys.serialNumber);
#else
	memcpy(sysData.sys.serialNumber, hardcodedSerial, sizeof(hardcodedSerial));
#endif
}
