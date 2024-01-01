/*
 * sys_clock.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system functions */
#include "system_internal.h"
/* This function allows us to port SYSTICK (or any 1ms timer) to
 * different parts of the system. Unlocking BareMetal multitasking.
 * Add timer functions here: */
void sys_systemClock(void){
	/* Here we should see counters, counting up every 1ms. */
	sys_addTimer();
	buffer_addTimer();
	tasks_addTimer();
	interface_clock();

#if INIT_SYS_STAT_LED
	s_statLed_tick();
#endif /* INIT_SYS_STAT_LED */

}
/* This is the timer callback. Feel free to use a different timer.
 * This functions is platform specific. Adjust to your needs. */
void HAL_SYSTICK_Callback(void){sys_systemClock();}
