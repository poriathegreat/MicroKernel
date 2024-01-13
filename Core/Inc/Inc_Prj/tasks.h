/*
 * tasks.h
 *
 *  Created on: Jan 13, 2024
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_TASKS_H_
#define INC_INC_PRJ_TASKS_H_
#include "sys.h"
/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
/* Exported variables should be placed here */



/************************************************************************************/
/*                               Exported Functions                                 */
/************************************************************************************/
/* This function will continuously be called by the sys_main() function.
 * Inside this function should be all the different tasks. */
void tasks_main(void);

/* This function will be called once every 1ms by the system_clock()
 * This function should hold counters, counting up every 1ms. */
void tasks_tick(void);

/* This function is called by the sys_init() function.
 * This function will be called only once, when the system starts. */
void tasks_init(void);









#endif /* INC_INC_PRJ_TASKS_H_ */
