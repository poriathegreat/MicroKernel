/*
 * sys_savetomemory.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
#include "sys_savetomemory.h"
/************************************************************************************/
#if INIT_SAVE_INTERNAL_MEMORY
/* This function is called when the system starts.
 * It will change the keepOn flag to SET.
 * If anything happens with the system and it re-starts,
 * This flag is checked upon wake up and the system does not sleep. */
#define CELL_SIZE			56
void sys_ROMRead(void){
	  uint32_t Rx_Data[CELL_SIZE] = {0};
	  char string[CELL_SIZE*4] = {0};

	  Flash_Read_Data(0x0801FC00 , Rx_Data, CELL_SIZE);
	  Convert_To_Str(Rx_Data, string);
	  task_ROMreadPreviousStatus(string);
}

/* This function is called before system enters StandBy
 * It will change the keepOn flag to SET and
 * it will save all the critical system data.
 * This will ensure the device stays off after the watchdog wakes (resets)
 * the device a few seconds later. */
void sys_ROMSave(char* bufferToSave, size_t sizeOfTheBuffer){

	char data[CELL_SIZE*4] = {0};
	memcpy(data, bufferToSave, sizeOfTheBuffer);
	Flash_Write_Data(0x0801FC00 , (uint32_t *)data, CELL_SIZE);
}

void sys_saveBeforeSleep(void){
	task_ROMsaveCurrentStatusNow();
}
#endif
