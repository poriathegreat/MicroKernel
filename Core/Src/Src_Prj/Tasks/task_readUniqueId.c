/*
 * task_readUniqueId.c
 *
 *  Created on: Aug 6, 2023
 *      Author: Kian Emami
 */

#include "tasks.h"
#include "unique_id.h"
#include "sys.h"


void task_readUuid(void){
	read_mcu_unique_id(sysData.sys.uuid);
	uint8_t hardCoded[12] = {0x52,0xFF,0x6D,0x06,0x48,0x65,0x84,0x49,0x12,0x22,0x19,0x67};


	char tempBuff[50] = {0};
	for(uint8_t i = RESET; i < 12; i++){
		sprintf(tempBuff+strlen(tempBuff), "%02X-", sysData.sys.uuid[i]);
	}
//	console_printf(sys_consoleSpace, CONSOLE_PART_ONE, "UniqueID: (%s)", tempBuff);
	memset(tempBuff, 0, sizeof(tempBuff));
	for(uint8_t i = RESET; i < 12; i++){
		sprintf(tempBuff+strlen(tempBuff), "%02X-", hardCoded[i]);
	}
//	console_printf(sys_consoleSpace+1, CONSOLE_PART_ONE, "HARDCODE: (%s)", tempBuff);


	if(memcmp(sysData.sys.uuid, hardCoded, 12) != 0){
		while(1);
	}
}






















