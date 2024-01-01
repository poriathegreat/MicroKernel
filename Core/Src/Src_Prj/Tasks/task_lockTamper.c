/*
 * task_checkIgnition.c
 *
 *  Created on: Aug 1, 2023
 *      Author: PoriaTheGreat
 */


#include "tasks.h"
#include "t_tamper.h"
#include "mc60_private.h"


_Bool tamperState = RESET;

void task_checkTamper(void){
	if( tamperState != HAL_GPIO_ReadPin(_TAMPER_PORT, _TAMPER_PIN) ){
		tamperState = HAL_GPIO_ReadPin(_TAMPER_PORT, _TAMPER_PIN);
		gsm_timerReset(TIMERS_GSM_SINCE_LAST_POINT);

		if(tamperState == M_TAMPER_OPEN){
			//tamper open
			task_createPacket(PACKET_TAMPEROPEN);
		}else {
			//tamper closed
			task_createPacket(PACKET_TAMPERCLOSED);
		}
	}
}


void task_firstInitTamper(void){
	tamperState = HAL_GPIO_ReadPin(_TAMPER_PORT, _TAMPER_PIN);
}
















