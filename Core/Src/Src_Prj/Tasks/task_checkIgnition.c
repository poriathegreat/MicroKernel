/*
 * task_checkIgnition.c
 *
 *  Created on: Aug 1, 2023
 *      Author: PoriaTheGreat
 */


#include "tasks.h"
#include "t_ignition.h"
#include "mc60_private.h"
_Bool ignitionState = RESET;
uint16_t ignitionDebouncer = RESET;
void task_checkIgnition(void){
	if(tasks_timerCheck(TIMERS_TASK_IGNITION)){
		if( !HAL_GPIO_ReadPin(VIN_ON_GPIO_Port, VIN_ON_Pin) && ignitionDebouncer > 0) ignitionDebouncer--;
		else if (ignitionDebouncer < 2000) ignitionDebouncer++;

		_Bool ignitionStateCurrent = ignitionDebouncer/1000;

		if( ignitionState != ignitionStateCurrent ){
			ignitionState = ignitionStateCurrent;
			gsm_timerReset(TIMERS_GSM_SINCE_LAST_POINT);

			if(ignitionState == M_IGNITION_ON){
				//ignition on //[
				task_createPacket(PACKET_CHARGERCONNECTED);
			}else {
				//ignition off //lockSEALED
				task_createPacket(PACKET_CHARGERDISCONNECTED);
			}
		}
	}
}


void task_firstInitIgnition(void){
	if(!HAL_GPIO_ReadPin(VIN_ON_GPIO_Port, VIN_ON_Pin)){
		ignitionDebouncer = 0;
		ignitionState = 0;

	}else {
		ignitionDebouncer = 1999;
		ignitionState = 1;
	}
}
















