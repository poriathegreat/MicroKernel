/*
 * sys_saveToRom.c
 *
 *  Created on: Jan 1, 2024
 *      Author: HP
 */
#include "sys_savetomemory.h"

#if INIT_SAVE_EXTERNAL_MEMORY
#include "w25qxx.h"
#endif


void s_saveToMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer);


#define DEBUGROM 1

#if DEBUGROM
uint8_t s_ROMConsoleSpace = RESET;
#endif
extern uint8_t key[16];



s_ROMdataStruct s_ROMdata = {0};

typedef struct {
	uint8_t flags;
	_Bool keepOn;
	uint8_t key[16];
	uint8_t batt;
}s_saveToFlashStruct;
s_saveToFlashStruct save = {0};

void s_ROMSave(void){
	uint8_t flags = 0;
	uint8_t tempBuff[sizeof(s_saveToFlashStruct)] = {0};
	memset(tempBuff, 0, sizeof(tempBuff));

	save.keepOn = sysData.sys.keepOn;
	memcpy(save.key, key, sizeof(key));
	save.batt = sysData.sys.batteryPercent;
	save.flags = flags;

	memcpy(tempBuff, &save, sizeof(s_saveToFlashStruct));


	s_saveToMemory(tempBuff, sizeof(tempBuff));
}

#if !INIT_SAVE_EXTERNAL_MEMORY
/* This function is called when the system starts.
 *
 * This function will load the saved data from the internal memory,
 * the data read is then parsed in a different function. */

#define CELL_SIZE			56
void sys_ROMRead(void){
	  char string[CELL_SIZE*4] = {0};


	  sizeOfTheBuffer = sizeOfTheBuffer%4;
	  sizeOfTheBuffer = sizeOfTheBuffer + 1;

	  uint32_t Rx_Data[CELL_SIZE] = {0};

	  Flash_Read_Data(0x0801FC00 , Rx_Data, CELL_SIZE);

	  s_parseSavedData(Rx_Data);
}

/* This function is called before system enters StandBy
 * It will change the keepOn flag to SET and
 * it will save all the critical system data.
 * This will ensure the device stays off after the watch dog wakes (resets)
 * the device a few seconds later. */
void s_ROMInternalSave(char* bufferToSave, size_t sizeOfTheBuffer){

}

void sys_saveBeforeSleep(void){
	s_ROMSave();
}
#endif







uint32_t s_tickSave = RESET;
void s_ROM_tick(void){
	s_tickSave++;

}



void s_ROM_main(void){
	if(
			(s_tickSave > 5*60000)||(s_ROMdata.saveNow)
	){

		s_tickSave = RESET;
		s_ROMdata.saveNow = RESET;
		s_ROMSave();
	}
}







#if INIT_SAVE_EXTERNAL_MEMORY
void s_saveToExternalMemory(char* bufferToSave, size_t sizeOfTheBuffer){
	if(W25qxx_IsEmptySector(0, 0, w25qxx.SectorSize)){
		W25qxx_WriteSector(bufferToSave, 0, 0, sizeOfTheBuffer);
	}else{
		W25qxx_EraseSector(0);
		W25qxx_WriteSector(bufferToSave, 0, 0, sizeOfTheBuffer);
	}
}

#else
void s_saveToInternalMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer){
	sizeOfTheBuffer = sizeOfTheBuffer%4;
	sizeOfTheBuffer = sizeOfTheBuffer + 1;
	Flash_Write_Data(0x08020000-sizeOfTheBuffer , (uint32_t *)bufferToSave, sizeOfTheBuffer);
}
#endif /* INIT_SAVE_EXTERNAL_MEMORY */




void s_saveToMemory(uint8_t* bufferToSave, size_t sizeOfTheBuffer){
#if INIT_SAVE_EXTERNAL_MEMORY
	s_saveToExternalMemory(bufferToSave, sizeOfTheBuffer);
#else
	s_saveToInternalMemory(bufferToSave, sizeOfTheBuffer);
#endif
#if DEBUGROM
		console_printf(s_ROMConsoleSpace, CONSOLE_PART_ONE, "WROTE: %s", bufferToSave);
#endif /* DEBUGROM */
}


void s_ROM_init(void){
	s_ROMConsoleSpace = console_requestSpace(2, "ROM");
}












//void sys_EXTROMRead(void){
//	memset(tempBuff, 0, sizeof(saveToFlashStruct_t));
//
//	if(W25qxx_IsEmptySector(0, 0, w25qxx.SectorSize)){
//
//		uint8_t flags = 0;
//		if ( task_flagCheck(FLAGS_TASK_DETECTMOVEMENT) )	 	flags |= 0b00000001;
//		if ( task_flagCheck(FLAGS_TASK_ENCRYPTION_ENABLED) ) 	flags |= 0b00000010;
//		if ( task_flagCheck(FLAGS_TASK_LOCKOPEN) )				flags |= 0b00000100;
//
//
//		memcpy(save.stopPointTime, taskGpsData.stopPoint.time, sizeof(taskGpsData.stopPoint.time));
//		save.stopPointLat = taskGpsData.stopPoint.lat;
//		save.stopPointLon = taskGpsData.stopPoint.lon;
//		save.keepOn = sysData.sys.keepOn;
//		memcpy(save.users, users, sizeof(users));
//		memcpy(save.passwords, passwords, sizeof(passwords));
//		memcpy(save.serverIp, GSM_TCP_SERVER_IP, sizeof(GSM_TCP_SERVER_IP));
//		memcpy(save.serverPort, GSM_TCP_SERVER_PORT, sizeof(GSM_TCP_SERVER_PORT));
//		memcpy(save.key, key, sizeof(key));
//		save.batt = sysData.sys.batteryPercent;
//		save.flags = flags;
//
//		memcpy(tempBuff, &save, sizeof(saveToFlashStruct_t));
//		W25qxx_EraseSector(0);
//		W25qxx_WriteSector(tempBuff, 0, 0, sizeof(saveToFlashStruct_t));
////		console_printf(sys_consoleSpace, CONSOLE_PART_ONE, "1ST INIT");
//
//		/**************************************************************/
//	}else {
//
//
//		memset(tempBuff, 0, sizeof(saveToFlashStruct_t));
//		W25qxx_ReadSector(tempBuff, 0, 0, sizeof(saveToFlashStruct_t));
//		memcpy(&save, tempBuff, sizeof(saveToFlashStruct_t));
//
//		memcpy(taskGpsData.stopPoint.time, save.stopPointTime, sizeof(taskGpsData.stopPoint.time));
//		memcpy(taskGpsData.signalLostPoint.time,taskGpsData.stopPoint.time,12);
//
//
//		taskGpsData.stopPoint.lat = save.stopPointLat;
//		taskGpsData.lastValidPoint.lat = taskGpsData.stopPoint.lat;
//		taskGpsData.signalLostPoint.lat = taskGpsData.stopPoint.lat;
//
//		taskGpsData.stopPoint.lon = save.stopPointLon;
//		taskGpsData.lastValidPoint.lon = taskGpsData.stopPoint.lon;
//		taskGpsData.signalLostPoint.lon =  taskGpsData.stopPoint.lon;
//
//		sysData.sys.keepOn = save.keepOn;
//		memcpy(users, save.users, sizeof(users));
//		memcpy(passwords, save.passwords, sizeof(passwords));
//		memcpy(GSM_TCP_SERVER_IP, save.serverIp, sizeof(GSM_TCP_SERVER_IP));
//		memcpy(GSM_TCP_SERVER_PORT, save.serverPort, sizeof(GSM_TCP_SERVER_PORT));
//		memcpy(key, save.key, sizeof(key));
//		sysData.sys.batteryPercent = save.batt;
//
//		if(save.flags & 0b00000001){
//			task_flagConf(FLAGS_TASK_DETECTMOVEMENT, SET);
//		}else 		task_flagConf(FLAGS_TASK_DETECTMOVEMENT, RESET);
//
//		if(save.flags & 0b00000010){
//			task_flagConf(FLAGS_TASK_ENCRYPTION_ENABLED, SET);
//		}else 		task_flagConf(FLAGS_TASK_ENCRYPTION_ENABLED, RESET);
//
//		if(save.flags & 0b00000100){
//			task_flagConf(FLAGS_TASK_LOCKOPEN, SET);
//		}else 		task_flagConf(FLAGS_TASK_LOCKOPEN, RESET);
////		console_printf(sys_consoleSpace, CONSOLE_PART_ONE, "NOT FIRST");
//
//	}
//
//
//	char tempString[224] = {0};
//	sprintf((char *)tempString,"%s,%2.6f,%2.6f,%d,%s,%s,%s,%s,%s,%s,%s,%s,(%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X),%d,%d",
//			taskGpsData.stopPoint.time,
//			taskGpsData.stopPoint.lat,
//			taskGpsData.stopPoint.lon,
//			sysData.sys.keepOn,
//			users[0],
//			passwords[0],
//			users[1],
//			passwords[1],
//			users[2],
//			passwords[2],
//			GSM_TCP_SERVER_IP,
//			GSM_TCP_SERVER_PORT,
//			key[0],	key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8], key[9], key[10], key[11], key[12], key[13], key[14], key[15],
//			save.flags,
//			sysData.sys.batteryPercent
//	);
//
////	console_printf(sys_consoleSpace, CONSOLE_PART_TWO, "READ: %s", tempString);
//}

































