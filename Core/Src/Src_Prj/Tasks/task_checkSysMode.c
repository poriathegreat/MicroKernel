/*
 * task_checkSysMode.c
 *
 *  Created on: Aug 1, 2023
 *      Author: PoriaTheGreat
 */

#include "tasks.h"
#include "mc60_private.h"



void task_checkSystemMode(void){

	if(ignitionState == M_IGNITION_ON){
		gsmData.powerMode = MC60_OPERATION_HIGHPERFORMANCE;

	}else if(
			(t_gpsData.systemHasStoppedMoving)
#if TASK_GPS_DETECT_SIGNALLOST
			|| (t_gpsData.systemHasLostSignal)
#endif /* TASK_GPS_DETECT_SIGNALLOST */
			){
		gsmData.powerMode = MC60_OPERATION_LOWPOWER;
	}else{
		gsmData.powerMode = MC60_OPERATION_NORMAL;
	}
}

























