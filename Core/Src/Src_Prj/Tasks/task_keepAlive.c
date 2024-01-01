/*
 * task_keepAlive.c
 *
 *  Created on: Jul 24, 2023
 *      Author: PoriaTheGreat
 */

#include "tasks.h"
#include "t_gps.h"




_Bool deviceFirstOn = SET;
void task_keepAlive(void){



	if(
		(abs(taskGpsData.valuablePoint.unixTime - taskGpsData.currentPoint.unixTime) > 3*3600)
		&&(taskGpsData.valuablePoint.unixTime < taskGpsData.currentPoint.unixTime)
	){
		memcpy(&(taskGpsData.valuablePoint), &(taskGpsData.currentPoint), sizeof(taskGpsData.valuablePoint));
		task_createPacket(PACKET_ALIVE);
	}
}



















