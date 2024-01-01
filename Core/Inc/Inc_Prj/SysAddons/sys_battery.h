/*
 * sys_battery.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSADDONS_SYS_BATTERY_H_
#define INC_INC_PRJ_SYSADDONS_SYS_BATTERY_H_

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"


#if INIT_SYS_HAS_BATTERY
#define INIT_CHARGING_LED						(0)
#if INIT_CHARGING_LED
#define INIT_COMMON_CHARGIN_AND_STAT_LED		(0)
#if INIT_COMMON_CHARGIN_AND_STAT_LED
#define _CHARGING_LED_GPIO				_STAT_LED_GPIO
#define _CHARGING_LED_Pin				_STAT_LED_Pin
#else
#define _CHARGING_LED_GPIO				LED2_GPIO_Port
#define _CHARGING_LED_Pin				LED2_Pin
#endif /* INIT_COMMON_CHARGIN_AND_STAT_LED */
#define _CHARGER_GPIO					VIN_ON_GPIO_Port
#define _CHARGER_Pin					VIN_ON_Pin
#define WHEN_CHARGER_CONNECTED			GPIO_PIN_SET
#endif /* INIT_CHARGING_LED */


void sys_batteryFirstInit();







#endif /* INIT_SYS_HAS_BATTERY */

#endif /* INC_INC_PRJ_SYSADDONS_SYS_BATTERY_H_ */
