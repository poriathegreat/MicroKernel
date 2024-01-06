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
#include "interface.h"
/* This function allows us to port SYSTICK (or any 1ms timer) to
 * different parts of the system. Unlocking BareMetal multitasking.
 * Add timer functions here: */
_Bool tick = RESET;
void sys_systemClock(void){
	if(tick){
		/* Here we should see counters, counting up every 1ms. */
		system_tick();
		s_buffer_tick();
		tasks_tick();
		interface_tick();
#if INIT_SYS_STAT_LED
		s_statLed_tick();
#endif /* INIT_SYS_STAT_LED */
#if INIT_SAVE_TO_MEMORY
		s_ROM_tick();
#endif /* INIT_SAVE_TO_MEMORY */
#if INIT_SYS_HAS_BATTERY
		s_battery_tick();
#endif /* INIT_SYS_HAS_BATTERY */


	}
}
/* This is the timer callback. Feel free to use a different timer.
 * This functions is platform specific. Adjust to your needs. */
void HAL_SYSTICK_Callback(void){tick = SET;}
