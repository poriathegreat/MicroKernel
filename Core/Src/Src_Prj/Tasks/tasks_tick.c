/*
 * sys_clock.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the interface functions */
#include "interface.h"
#include "tasks.h"
/* This function allows us to port SYSTICK (or any 1ms timer) to
 * different parts of the system. Unlocking BareMetal multitasking.
 * For deeper detail, look into sys_clock.c
 *
 * Add timer functions here: */
void tasks_tick(void){
	/* Counters should be placed here
	 * This function is called every 1ms. */








}
