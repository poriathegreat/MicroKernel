/*
 * memory_saveToInternalFlash.c
 *
 *  Created on: Jul 26, 2023
 *      Author: PoriaTheGreat
 */
#include "tasks.h"
#include "sys.h"
#include "w25qxx.h"


extern char GSM_TCP_SERVER_IP[16];
extern char GSM_TCP_SERVER_PORT[6];
char smsAdmin[20] = {0};

extern char users[MAX_NUMBER_OF_DEVICE_ADMINS][20];
extern char passwords[MAX_NUMBER_OF_DEVICE_ADMINS][20];
char keySave[33] = {0};

extern uint8_t key[16];
void task_ROMsaveCurrentStatusNow(void){
#if INIT_SAVE_INTERNAL_MEMORY
	sprintf(keySave, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			key[0],	key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8], key[9], key[10], key[11], key[12], key[13], key[14], key[15]);


	/*
	 * save the stop data:
	 * stopBeginTime,stopBeginLat,stopBeginLon,keepOn,user1,user2,user3,pass1,pass2,pass3,serverIp,serverPort,smsConfig
	 * 170423112300,53.786587,32.523875,1,+989127953012,poria,kian,123456,654321,987654321,192.168.128.100,12345,smsConfig
	 * 39
	 */

	uint8_t flags = 0;
	if ( task_flagCheck(FLAGS_TASK_DETECTMOVEMENT) )	 	flags |= 0b00000001;
	if ( task_flagCheck(FLAGS_TASK_ENCRYPTION_ENABLED) ) 	flags |= 0b00000010;
	if ( task_flagCheck(FLAGS_TASK_LOCKOPEN) )				flags |= 0b00000100;

	char tempString[224] = {0};
	sprintf((char *)tempString,"%s,%2.6f,%2.6f,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d",
			taskGpsData.stopPoint.time,
			taskGpsData.stopPoint.lat,
			taskGpsData.stopPoint.lon,
			sysData.sys.keepOn,
			users[0],
			passwords[0],
			users[1],
			passwords[1],
			users[2],
			passwords[2],
			GSM_TCP_SERVER_IP,
			GSM_TCP_SERVER_PORT,
			keySave,
			flags,
			sysData.sys.batteryPercent
	);
	//	console_printf(sys_consoleSpace+2, CONSOLE_PART_ONE, "Saving: {%s}", tempString);
	sys_ROMSave(tempString, strlen(tempString));
#endif


#if INIT_SAVE_EXTERNAL_MEMORY
	sys_EXTROMSave();
#endif
}




void task_ROMsave(void){
	if((tasks_timerCheck(TIMERS_TASK_SAVE) > 5*60000)
			||(task_flagCheck(FLAGS_TASK_SAVESTATUS))
	){

		tasks_timerReset(TIMERS_TASK_SAVE);
		task_flagConf(FLAGS_TASK_SAVESTATUS, RESET);
		task_ROMsaveCurrentStatusNow();

	}
}

extern char GSM_TCP_SERVER_IP[16];
extern char GSM_TCP_SERVER_PORT[6];


typedef struct {
	uint8_t stopPointTime[13];
	float stopPointLat;
	float stopPointLon;
	_Bool keepOn;
	char users[3][20];
	char passwords[3][20];
	char serverIp[16];
	char serverPort[6];
	uint8_t key[16];
	uint8_t batt;
	uint8_t flags;
}saveToFlashStruct;


void task_ROMreadPreviousStatus(char* string){
	/*
	 * save the stop data:
	 * stopBeginTime,stopBeginLat,stopBeginLon,keepOn,adminNum,serverIp,serverPort,smsConfig
	 * 170423112300,53.786587,32.523875,1,+989127953012,192.168.128.100,12345,smsConfig
	 * 39
	 */
	//	console_printf(sys_consoleSpace, CONSOLE_PART_ONE, "Read: {%s}", string);
	//	console_printf(sys_consoleSpace+4, CONSOLE_PART_ONE, "TEST", string);

	char *flashReadArray = strtok(string, (const char*)",");
	memcpy(taskGpsData.stopPoint.time,flashReadArray,12);
	memcpy(taskGpsData.signalLostPoint.time,taskGpsData.stopPoint.time,12);

	flashReadArray = strtok(NULL, (const char*)",");
	taskGpsData.stopPoint.lat = atof(flashReadArray);
	taskGpsData.lastValidPoint.lat = taskGpsData.stopPoint.lat;
	taskGpsData.signalLostPoint.lat = taskGpsData.stopPoint.lat;

	flashReadArray = strtok(NULL, (const char*)",");
	taskGpsData.stopPoint.lon = atof(flashReadArray);
	taskGpsData.lastValidPoint.lon = taskGpsData.stopPoint.lon;
	taskGpsData.signalLostPoint.lon =  taskGpsData.stopPoint.lon;

	flashReadArray = strtok(NULL, (const char*)",");
	sysData.sys.keepOn = atoi(flashReadArray);

	//########################################################################################
	/* User and password space in ROM */
	flashReadArray = strtok(NULL, (const char*)",");					//user1
	if((strlen(flashReadArray) > 2) && (flashReadArray[0] == '+')){
		strcpy(users[0],flashReadArray);
		flashReadArray = strtok(NULL, (const char*)",");				//pass1
		strcpy(passwords[0],flashReadArray);
	}else flashReadArray = strtok(NULL, (const char*)",");				//pass1


	flashReadArray = strtok(NULL, (const char*)",");					//user2
	if((strlen(flashReadArray) > 2) && (flashReadArray[0] == '+')){
		strcpy(users[1],flashReadArray);
		flashReadArray = strtok(NULL, (const char*)",");				//pass2
		strcpy(passwords[1],flashReadArray);
	}else flashReadArray = strtok(NULL, (const char*)",");				//pass2


	flashReadArray = strtok(NULL, (const char*)",");					//user3
	if((strlen(flashReadArray) > 2) && (flashReadArray[0] == '+')){
		strcpy(users[2],flashReadArray);
		flashReadArray = strtok(NULL, (const char*)",");				//pass3
		strcpy(passwords[2],flashReadArray);
	}else flashReadArray = strtok(NULL, (const char*)",");				//pass3

	//	console_printf(sys_consoleSpace+4, CONSOLE_PART_ONE, "ADM1[%s],[%s]\tADM2[%s],[%s]\tADM3[%s],[%s]", users[0], passwords[0], users[1], passwords[1], users[2], passwords[2] );


	//####################################################################################

	flashReadArray = strtok(NULL, (const char*)",");
	if(strlen(flashReadArray) > 5){
		strcpy(GSM_TCP_SERVER_IP,flashReadArray);
	}
	//####################################################################################


	flashReadArray = strtok(NULL, (const char*)",");
	if(strlen(flashReadArray) > 3){
		strcpy(GSM_TCP_SERVER_PORT,flashReadArray);
	}
	//####################################################################################
	flashReadArray = strtok(NULL, (const char*)",");
	if(strlen(flashReadArray) > 3){
		strcpy(keySave,flashReadArray);

		char hexChar[3] = {0};
		for(uint8_t i = 0; i < 16; i++){
			memset(hexChar, 0, 3);
			memcpy(hexChar, keySave + (i*2), 2);
			key[i] = (long)strtol((const char*)hexChar, NULL, 16);
		}

	}


	//####################################################################################

	flashReadArray = strtok(NULL, (const char*)",");
	uint8_t tempBuff = atoi(flashReadArray);
	if(tempBuff & 0b00000001){
		task_flagConf(FLAGS_TASK_DETECTMOVEMENT, SET);
	}else 		task_flagConf(FLAGS_TASK_DETECTMOVEMENT, RESET);

	if(tempBuff & 0b00000010){
		task_flagConf(FLAGS_TASK_ENCRYPTION_ENABLED, SET);
	}else 		task_flagConf(FLAGS_TASK_ENCRYPTION_ENABLED, RESET);

	if(tempBuff & 0b00000100){
		task_flagConf(FLAGS_TASK_LOCKOPEN, SET);
	}else 		task_flagConf(FLAGS_TASK_LOCKOPEN, RESET);

	//####################################################################################

	flashReadArray = strtok(NULL, (const char*)",");
	sysData.sys.batteryPercent = atoi(flashReadArray);



}



typedef struct {
	uint8_t stopPointTime[13];
	float stopPointLat;
	float stopPointLon;
	_Bool keepOn;
	char users[3][20];
	char passwords[3][20];
	char serverIp[16];
	char serverPort[6];
	uint8_t key[16];
	uint8_t batt;
	uint8_t flags;
}saveToFlashStruct_t;
saveToFlashStruct_t save = {0};







uint8_t tempBuff[sizeof(saveToFlashStruct_t)] = {0};

void sys_EXTROMSave(void){
	memset(tempBuff, 0, sizeof(tempBuff));

	uint8_t flags = 0;
	if ( task_flagCheck(FLAGS_TASK_DETECTMOVEMENT) )	 	flags |= 0b00000001;
	if ( task_flagCheck(FLAGS_TASK_ENCRYPTION_ENABLED) ) 	flags |= 0b00000010;
	if ( task_flagCheck(FLAGS_TASK_LOCKOPEN) )				flags |= 0b00000100;


	memcpy(save.stopPointTime, taskGpsData.stopPoint.time, sizeof(taskGpsData.stopPoint.time));
	save.stopPointLat = taskGpsData.stopPoint.lat;
	save.stopPointLon = taskGpsData.stopPoint.lon;
	save.keepOn = sysData.sys.keepOn;
	memcpy(save.users, users, sizeof(users));
	memcpy(save.passwords, passwords, sizeof(passwords));
	memcpy(save.serverIp, GSM_TCP_SERVER_IP, sizeof(GSM_TCP_SERVER_IP));
	memcpy(save.serverPort, GSM_TCP_SERVER_PORT, sizeof(GSM_TCP_SERVER_PORT));
	memcpy(save.key, key, sizeof(key));
	save.batt = sysData.sys.batteryPercent;
	save.flags = flags;
	memcpy(tempBuff, &save, sizeof(saveToFlashStruct_t));

	if(W25qxx_IsEmptySector(0, 0, w25qxx.SectorSize)){
		W25qxx_WriteSector(tempBuff, 0, 0, sizeof(saveToFlashStruct_t));
	}else{
		W25qxx_EraseSector(0);
		W25qxx_WriteSector(tempBuff, 0, 0, sizeof(saveToFlashStruct_t));
	}
	char tempString[224] = {0};
	sprintf((char *)tempString,"%s,%2.6f,%2.6f,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d",
			taskGpsData.stopPoint.time,
			taskGpsData.stopPoint.lat,
			taskGpsData.stopPoint.lon,
			sysData.sys.keepOn,
			users[0],
			passwords[0],
			users[1],
			passwords[1],
			users[2],
			passwords[2],
			GSM_TCP_SERVER_IP,
			GSM_TCP_SERVER_PORT,
			keySave,
			flags,
			sysData.sys.batteryPercent
	);

//	console_printf(sys_consoleSpace+1, CONSOLE_PART_ONE, "WROTE: %s", tempString);
}



void sys_EXTROMRead(void){
	memset(tempBuff, 0, sizeof(saveToFlashStruct_t));

	if(W25qxx_IsEmptySector(0, 0, w25qxx.SectorSize)){

		uint8_t flags = 0;
		if ( task_flagCheck(FLAGS_TASK_DETECTMOVEMENT) )	 	flags |= 0b00000001;
		if ( task_flagCheck(FLAGS_TASK_ENCRYPTION_ENABLED) ) 	flags |= 0b00000010;
		if ( task_flagCheck(FLAGS_TASK_LOCKOPEN) )				flags |= 0b00000100;


		memcpy(save.stopPointTime, taskGpsData.stopPoint.time, sizeof(taskGpsData.stopPoint.time));
		save.stopPointLat = taskGpsData.stopPoint.lat;
		save.stopPointLon = taskGpsData.stopPoint.lon;
		save.keepOn = sysData.sys.keepOn;
		memcpy(save.users, users, sizeof(users));
		memcpy(save.passwords, passwords, sizeof(passwords));
		memcpy(save.serverIp, GSM_TCP_SERVER_IP, sizeof(GSM_TCP_SERVER_IP));
		memcpy(save.serverPort, GSM_TCP_SERVER_PORT, sizeof(GSM_TCP_SERVER_PORT));
		memcpy(save.key, key, sizeof(key));
		save.batt = sysData.sys.batteryPercent;
		save.flags = flags;

		memcpy(tempBuff, &save, sizeof(saveToFlashStruct_t));
		W25qxx_EraseSector(0);
		W25qxx_WriteSector(tempBuff, 0, 0, sizeof(saveToFlashStruct_t));
//		console_printf(sys_consoleSpace, CONSOLE_PART_ONE, "1ST INIT");

		/**************************************************************/
	}else {


		memset(tempBuff, 0, sizeof(saveToFlashStruct_t));
		W25qxx_ReadSector(tempBuff, 0, 0, sizeof(saveToFlashStruct_t));
		memcpy(&save, tempBuff, sizeof(saveToFlashStruct_t));

		memcpy(taskGpsData.stopPoint.time, save.stopPointTime, sizeof(taskGpsData.stopPoint.time));
		memcpy(taskGpsData.signalLostPoint.time,taskGpsData.stopPoint.time,12);


		taskGpsData.stopPoint.lat = save.stopPointLat;
		taskGpsData.lastValidPoint.lat = taskGpsData.stopPoint.lat;
		taskGpsData.signalLostPoint.lat = taskGpsData.stopPoint.lat;

		taskGpsData.stopPoint.lon = save.stopPointLon;
		taskGpsData.lastValidPoint.lon = taskGpsData.stopPoint.lon;
		taskGpsData.signalLostPoint.lon =  taskGpsData.stopPoint.lon;

		sysData.sys.keepOn = save.keepOn;
		memcpy(users, save.users, sizeof(users));
		memcpy(passwords, save.passwords, sizeof(passwords));
		memcpy(GSM_TCP_SERVER_IP, save.serverIp, sizeof(GSM_TCP_SERVER_IP));
		memcpy(GSM_TCP_SERVER_PORT, save.serverPort, sizeof(GSM_TCP_SERVER_PORT));
		memcpy(key, save.key, sizeof(key));
		sysData.sys.batteryPercent = save.batt;

		if(save.flags & 0b00000001){
			task_flagConf(FLAGS_TASK_DETECTMOVEMENT, SET);
		}else 		task_flagConf(FLAGS_TASK_DETECTMOVEMENT, RESET);

		if(save.flags & 0b00000010){
			task_flagConf(FLAGS_TASK_ENCRYPTION_ENABLED, SET);
		}else 		task_flagConf(FLAGS_TASK_ENCRYPTION_ENABLED, RESET);

		if(save.flags & 0b00000100){
			task_flagConf(FLAGS_TASK_LOCKOPEN, SET);
		}else 		task_flagConf(FLAGS_TASK_LOCKOPEN, RESET);
//		console_printf(sys_consoleSpace, CONSOLE_PART_ONE, "NOT FIRST");

	}


	char tempString[224] = {0};
	sprintf((char *)tempString,"%s,%2.6f,%2.6f,%d,%s,%s,%s,%s,%s,%s,%s,%s,(%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X),%d,%d",
			taskGpsData.stopPoint.time,
			taskGpsData.stopPoint.lat,
			taskGpsData.stopPoint.lon,
			sysData.sys.keepOn,
			users[0],
			passwords[0],
			users[1],
			passwords[1],
			users[2],
			passwords[2],
			GSM_TCP_SERVER_IP,
			GSM_TCP_SERVER_PORT,
			key[0],	key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8], key[9], key[10], key[11], key[12], key[13], key[14], key[15],
			save.flags,
			sysData.sys.batteryPercent
	);

//	console_printf(sys_consoleSpace, CONSOLE_PART_TWO, "READ: %s", tempString);
}



void taskRom_addTimer(){



}
















