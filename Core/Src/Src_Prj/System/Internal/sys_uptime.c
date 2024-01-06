/*
 * sys_uptime.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
#include "system_internal.h"
uint8_t sysSeconds = RESET;
uint8_t sysMinutes = RESET;
uint32_t sysHours = RESET;
void sys_upTime(void){
	if(sys_timerCheck(TIMERS_SYS_UPTIME) > 1000){
		sys_timerReset(TIMERS_SYS_UPTIME);
		sysSeconds++;

		if(sysSeconds > 59)  {
			sysSeconds = RESET;
			sysMinutes++;
		}

		if(sysMinutes > 59){
			sysMinutes = RESET;
			sysHours++;
		}
#if DEBUGGING
		console_printf(sys_infoSpace+2, CONSOLE_PART_TWO, "[%d:%02d:%02d]   ", sysHours, sysMinutes, sysSeconds );
#endif /* DEBUGGING */
	}
}






























