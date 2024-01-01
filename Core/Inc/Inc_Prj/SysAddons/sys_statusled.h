/*
 * sys_statusled.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSADDONS_SYS_STATUSLED_H_
#define INC_INC_PRJ_SYSADDONS_SYS_STATUSLED_H_

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"

#if INIT_SYS_STAT_LED
#define _STAT_LED_GPIO							LED_GPIO_Port
#define _STAT_LED_Pin							LED_Pin




/* This function shows the status of the system usign a defined led */
void sys_statusLED(void);









#endif /* INIT_SYS_STAT_LED */

















#endif /* INC_INC_PRJ_SYSADDONS_SYS_STATUSLED_H_ */
