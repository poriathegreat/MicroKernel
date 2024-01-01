/*
 * task_checkButton.c
 *
 *  Created on: Aug 8, 2023
 *      Author: HP
 */

#include "tasks.h"
#include "t_lock.h"
#include "mc60_public.h"

uint32_t buttonCheckTimer = RESET;

void task_button(void){
	static _Bool buttonPrev = RESET;
	static _Bool buttonStat = RESET;

	buttonStat = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);

	if((buttonStat == RESET) && (buttonPrev == SET)){
		if(buttonCheckTimer>2000){
			buttonPrev = RESET;
			if( (lockStat == M_LOCK_IDLE) ) {
				lock_flagConf(FLAGS_LOCK_TOGGLE , SET);
				task_createPacket(PACKET_BUTTONPRESSED);
			}
			if(
				(smsAdmin[0] != '1')
			){
				char buff[170] = {0};
				sprintf(buff, "S/N: (%s) \nBattery: (%d%%)\nVoltage: (%dmv)\nSeal: (%d)\nhttps://maps.google.com/maps?daddr=%2.6f,%2.6f",
						sysData.sys.serialNumber,
						sysData.sys.batteryPercent,
						sysData.sys.batteryVoltage,
						!task_flagCheck(FLAGS_TASK_LOCKOPEN),
						taskGpsData.lastValidPoint.lat, taskGpsData.lastValidPoint.lon);

				mc60_smsAddToSendQueue((const char*)buff, (const char*)smsAdmin, strlen(buff));
			}

		}
	}else if((buttonStat == RESET) && (buttonPrev == RESET)){
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, SET);
	}else if(buttonStat == SET){
		buttonPrev = SET;
		buttonCheckTimer = RESET;
	}
}



void buttonCheck_addTimer(void){
	buttonCheckTimer++;
}











