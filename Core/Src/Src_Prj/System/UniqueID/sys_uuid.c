/*
 * unique_id.c
 *
 *  Created on: Aug 6, 2023
 *      Author: Kian Emami
 */


#include "sys.h"
#include "unique_id.h"
#include <stdint.h>
#include <string.h>

#if INIT_SYS_LOCK_UUID

#ifndef UUID_LOCKED_TO
extern IWDG_HandleTypeDef hiwdg;
#endif /* UUID_LOCKED_TO */


int32_t read_mcu_unique_id( uint8_t * uuid ){
    int32_t result = 0 ;
    memcpy((uint8_t *) uuid , (uint8_t *) UUID_BASE_ADDRESS , UUID_SIZE_BYTE);
    return result ;
}


void s_readUuid(void){
	read_mcu_unique_id(sysData.sys.uuid);
#ifndef UUID_LOCKED_TO
	uint8_t sys_uuidSpace = console_requestSpace(2, "MCU Unique ID");
	console_printf(sys_uuidSpace, CONSOLE_PART_ONE, "Define this as the new UUID_LOCKED_TO: ");
	char tempBuff[50] = {0};
	for(uint8_t i = RESET; i < 12; i++){sprintf(tempBuff+strlen(tempBuff), "%02X,", sysData.sys.uuid[i]);}
	console_printf(sys_uuidSpace+1, CONSOLE_PART_ONE, "#define UUID_LOCKED_TO    {%s}", tempBuff);
	while (1){HAL_IWDG_Refresh(&hiwdg);}
#else
	uint8_t hardCoded[12] = UUID_LOCKED_TO;
	if(memcmp(sysData.sys.uuid, hardCoded, 12) != 0){
		while(1);
	}
#endif
}

#endif /* INIT_SYS_LOCK_UUID */
