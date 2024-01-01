/*
 * tasks.h
 *
 *  Created on: Jul 23, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_PROCESS_H_
#define INC_INC_PRJ_PROCESS_H_
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"
/* Include headers for each task */



/************************************************************************************/
/*                                 Exported Macros                                  */
/************************************************************************************/
/* Include needed timers for different tasks here. */
enum task_timers{
	TASKTIMER_FILL_HERE_WITH_TIMERS = 0,




	TASKTIMERS_TOTAL
};

/* Include needed flags for different tasks here. */
enum task_flags{
	TASKFLAG_FILL_HERE_WITH_FLAGS,





	TASKFLAGS_TOTAL
};

/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
/* Include global parameters here. These parameters will be accessible from anywhere
 * in the system. */
typedef struct {
	_Bool		fillThisStructureWithParameters;






}taskData_t;	extern taskData_t taskData;
/************************************************************************************/
/*                                Exported functions                                */
/************************************************************************************/

/* This will configure the tasks function. Place this BEFORE the superloop. */
void tasks_firstInit(void);


/* Functions to access flags and timers publicly. */
void tasks_addTimer(void);
uint32_t tasks_timerCheck(uint8_t i);
void tasks_timerReset(uint8_t i);
_Bool task_flagConf(uint16_t flag, _Bool state);
_Bool task_flagCheck(uint16_t flag);



#endif /* INC_INC_PRJ_PROCESS_H_ */
