/*
 * sys_saveToRom.c
 *
 *  Created on: Jan 1, 2024
 *      Author: HP
 */
#include "sys_savetomemory.h"
#include "interface.h"
#if INIT_SAVE_TO_MEMORY


#if INIT_SAVE_EXTERNAL_MEMORY
#include "w25qxx.h"
#else
#include "internalFlash.h"
#endif /* INIT_SAVE_EXTERNAL_MEMORY */


#define DEBUGROM 					1
#define ROM_SAVE_PERIOD_MINUTES		5


#if DEBUGROM
uint8_t s_ROMConsoleSpace = RESET;
#endif
extern uint8_t key[16];

uint32_t s_tickSave = RESET;
s_ROMdataStruct s_ROMdata = {0};

typedef struct {
	/* Add parameters to be saved here.
	 * Everything should be copied into this structure. This
	 * structure is what will be saved/read from the memory. */

	uint8_t flags;
	_Bool keepOn;
	uint8_t key[16];
#if INIT_SYS_HAS_BATTERY
	uint8_t batt;
#endif /* INIT_SYS_HAS_BATTERY */
}s_saveToFlashStruct;
s_saveToFlashStruct save = {0};

void s_saveToMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer);
void s_saveToInternalMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer);
void s_saveToExternalMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer);

void s_loadFromMemory(s_saveToFlashStruct* saveStruct, size_t sizeOfTheBuffer);
void s_loadFromInternalMemory(s_saveToFlashStruct* saveStruct, size_t sizeOfTheBuffer);
void s_loadFromExternalMemory(s_saveToFlashStruct* saveStruct, size_t sizeOfTheBuffer);






void s_ROMSave(void){
	/* Configure this part of the code to copy what every needs to be saved
	 * to 'save' structure. That structure will automatically be written to
	 * the non volatile memory chosen. */
	uint8_t flags = 0;

	save.keepOn = interfaceData.sys->keepOn;
	memcpy(save.key, key, sizeof(key));
#if INIT_SYS_HAS_BATTERY
	save.batt = interfaceData.sys->battery->percentage;
#endif /* INIT_SYS_HAS_BATTERY */
	save.flags = flags;




	/*******************************************************/
	/* This part of the code will handle saving the data   */
	/*******************************************************/
	uint8_t tempBuff[sizeof(s_saveToFlashStruct)] = {0};
	memset(tempBuff, 0, sizeof(tempBuff));
	memcpy(tempBuff, &save, sizeof(s_saveToFlashStruct));
	s_saveToMemory(tempBuff, sizeof(tempBuff));
}



void s_ROMLoad(void){
	/*******************************************************/
	/* This part of the code will handle reading the data   */
	/*******************************************************/
	s_loadFromMemory(&save, sizeof(s_saveToFlashStruct));
	/*******************************************************/



	/* Configure this part of the code to copy what every needs to be saved
	 * to 'save' structure. That structure will automatically be written to
	 * the non volatile memory chosen. */
	interfaceData.sys->keepOn = save.keepOn;
#if INIT_SYS_HAS_BATTERY
	interfaceData.sys->battery->percentage = save.batt;
#endif /* INIT_SYS_HAS_BATTERY */
	memcpy(key, save.key, sizeof(key));
}






/* This function is called when the system starts.
 *
 * This function will load the saved data from the internal memory,
 * the data read is then parsed in a different function. */
void s_ROM_init(void){
#if DEBUGGING
#if DEBUGROM
	s_ROMConsoleSpace = console_requestSpace(2, "ROM");
#endif /* DEBUGROM */
#endif /* DEBUGGING */
	s_ROMLoad();
}


void s_ROM_main(void){
	if(
		(s_tickSave > ROM_SAVE_PERIOD_MINUTES*60000)||(s_ROMdata.forceSave)
	){
		s_tickSave = RESET;
		s_ROMdata.forceSave = RESET;
		s_ROMSave();
	}
}

void s_ROM_saveNow(void){
	s_ROMSave();
}


void s_saveToMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer){
#if INIT_SAVE_EXTERNAL_MEMORY
	s_saveToExternalMemory(bufferToSave, sizeOfTheBuffer);
#else
	s_saveToInternalMemory(bufferToSave, sizeOfTheBuffer);
#endif
#if DEBUGGING
#if DEBUGROM
	console_printf(s_ROMConsoleSpace + 1, CONSOLE_PART_ONE, "Wrote: %s", bufferToSave);
#endif /* DEBUGROM */
#endif /* DEBUGGING */
}



void s_loadFromMemory(s_saveToFlashStruct* saveStruct, size_t sizeOfTheBuffer){
#if INIT_SAVE_EXTERNAL_MEMORY
	s_loadFromExternalMemory(saveStruct, sizeOfTheBuffer);
#else
	s_loadFromInternalMemory(saveStruct, sizeOfTheBuffer);
#endif
}




/* These two functions will save the data to the actual memory space.
 * The memory to be saved to is chosen by the s_saveToMemory() function. */
#if INIT_SAVE_EXTERNAL_MEMORY
void s_saveToExternalMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer){
	if(W25qxx_IsEmptySector(0, 0, w25qxx.SectorSize)){
		W25qxx_WriteSector(bufferToSave, 0, 0, sizeOfTheBuffer);
	}else{
		W25qxx_EraseSector(0);
		W25qxx_WriteSector(bufferToSave, 0, 0, sizeOfTheBuffer);
	}
}
#else
void s_saveToInternalMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer){
	size_t newSizeOfTheBuffer = sizeOfTheBuffer/4;
	if(sizeOfTheBuffer%4){newSizeOfTheBuffer++;}
	newSizeOfTheBuffer = newSizeOfTheBuffer*4;

	Flash_Write_Data(INTERNALMEMORY_ENDADRESS-newSizeOfTheBuffer , (uint32_t *)bufferToSave, newSizeOfTheBuffer);
}
#endif /* INIT_SAVE_EXTERNAL_MEMORY */


/* These two functions will read the data from the actual memory space.
 * The memory to be saved to is chosen by the s_loadFromMemory() function. */
#if INIT_SAVE_EXTERNAL_MEMORY
void s_loadFromExternalMemory(s_saveToFlashStruct* saveStruct, size_t sizeOfTheBuffer){
	uint8_t tempBuff[sizeof(s_saveToFlashStruct)] = {0}; // Maximum size of the buffer needed.
	memset(tempBuff, 0, sizeof(s_saveToFlashStruct));

	/* We check if the memory is empty, we write the default values. */
	if(W25qxx_IsEmptySector(0, 0, w25qxx.SectorSize)){

		uint8_t flags = 0;

		save.keepOn = interfaceData.sys->keepOn;
		memcpy(save.key, key, sizeof(key));
#if INIT_SYS_HAS_BATTERY
		save.batt = interfaceData.sys->battery->percentage;
#endif /* INIT_SYS_HAS_BATTERY */
		save.flags = flags;


		memcpy(tempBuff, &save, sizeof(s_saveToFlashStruct));
		W25qxx_EraseSector(0);
		W25qxx_WriteSector(tempBuff, 0, 0, sizeof(s_saveToFlashStruct));
		/**************************************************************/
	}else {
	/* The memory is not empty so we read the values previously written. */

		memset(tempBuff, 0, sizeof(s_saveToFlashStruct));
		W25qxx_ReadSector(tempBuff, 0, 0, sizeof(s_saveToFlashStruct));
		memcpy(&save, tempBuff, sizeof(s_saveToFlashStruct));
	}
#if DEBUGGING
#if DEBUGROM
	console_printf(s_ROMConsoleSpace, CONSOLE_PART_TWO, "Read: %s", tempBuff);
#endif /* DEBUGROM */
#endif /* DEBUGGING */
}
#else
void s_loadFromInternalMemory(s_saveToFlashStruct* saveStruct, size_t sizeOfTheBuffer){
	uint32_t tempBuff[(((sizeof(s_saveToFlashStruct))/4) + 1)] = {0}; // Maximum size of the buffer needed.
	memset(tempBuff, 0, sizeof(tempBuff));

	size_t newSizeOfTheBuffer = sizeOfTheBuffer/4;
	if(sizeOfTheBuffer%4){newSizeOfTheBuffer++;}
	newSizeOfTheBuffer = newSizeOfTheBuffer*4;

	Flash_Read_Data(INTERNALMEMORY_ENDADRESS - newSizeOfTheBuffer, tempBuff, newSizeOfTheBuffer);
#if DEBUGROM
	console_printf(s_ROMConsoleSpace, CONSOLE_PART_ONE, "Read: %s", tempBuff);
#endif /* DEBUGROM */

	memset(saveStruct, 0, sizeof(s_saveToFlashStruct));
	memcpy(saveStruct, tempBuff, sizeof(s_saveToFlashStruct));
}
#endif /* INIT_SAVE_EXTERNAL_MEMORY */



/* This function is added to the sys clock function that will call it every 1ms. */
void s_ROM_tick(void){
	s_tickSave++;
}


#endif /* INIT_SAVE_TO_MEMORY */







