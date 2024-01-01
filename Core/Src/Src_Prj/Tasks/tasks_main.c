/*
 * tasks_main.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system functions */
#include "sys.h"
#include "tasks.h"


uint32_t 	tasksTimersArray[TASKTIMERS_TOTAL]	=	{0};
_Bool 		taskFlagsArray[TASKFLAGS_TOTAL] 		= 	{0};


void tasks_addTimer(void){
	for(uint8_t i = 0; i<TASKTIMERS_TOTAL; i++){
		tasksTimersArray[i]++;
	}
}

uint32_t tasks_timerCheck(uint8_t i){
	return tasksTimersArray[i];
}



void tasks_timerReset(uint8_t i){

	tasksTimersArray[i]=0;
}


_Bool task_flagConf(uint16_t flag, _Bool state){
	taskFlagsArray[flag]=state;
	return state;
}


_Bool task_flagCheck(uint16_t flag){
	return taskFlagsArray[flag];
}




































