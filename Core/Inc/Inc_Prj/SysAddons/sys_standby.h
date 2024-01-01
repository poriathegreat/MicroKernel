/*
 * sys_checkwakeup.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSADDONS_SYS_CHECKWAKEUP_C_
#define INC_INC_PRJ_SYSADDONS_SYS_CHECKWAKEUP_C_

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"
#if INIT_STANDBY_FUNCTIONALITY

/* The return type of the sys_checkSleep function */
typedef enum {
	SLEEP = 1,
	AWAKE = 0
}syswakeup_t;

/* This function is called to make the system enter standby mode*/
void sys_enterStandby(void);
/************************************************************************************/







#endif /* INIT_STANDBY_FUNCTIONALITY */

#endif /* INC_INC_PRJ_SYSADDONS_SYS_CHECKWAKEUP_C_ */
