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
void s_loadSerialNumberFromExternalMemory(char* loadedSerialNumber){
	/* write code here to load serial number from memory. */



}
#endif /* INIT_LOAD_FROM_MEMORY */
/************************************************************************************/
/* This function will read the deviceSerialNumber and write it to sysData.sys.serialNumber. */
void s_loadSerialNumber(void){
#if INIT_LOAD_SERIAL_FROM_MEMORY
	s_loadSerialNumberFromExternalMemory(sysData.sys.serialNumber);
#else
	memcpy(sysData.sys.serialNumber, hardcodedSerial, sizeof(hardcodedSerial));
#endif
}
