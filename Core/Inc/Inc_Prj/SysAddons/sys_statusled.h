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

#if INIT_SYS_HAS_BATTERY
#define INIT_CHARGING_LED						0
#if INIT_CHARGING_LED
#define INIT_COMMON_CHARGIN_AND_STAT_LED		0
#if INIT_COMMON_CHARGIN_AND_STAT_LED
#define _CHARGING_LED_GPIO						_STAT_LED_GPIO
#define _CHARGING_LED_Pin						_STAT_LED_Pin
#else
#define _CHARGING_LED_GPIO						CHARGER_LED_GPIO
#define _CHARGING_LED_Pin						CHARGER_LED_Pin
#endif /* INIT_COMMON_CHARGIN_AND_STAT_LED */
#define _CHARGER_GPIO							DETECT_CHARGER_GPIO
#define _CHARGER_Pin							DETECT_CHARGER_Pin
#define WHEN_CHARGER_CONNECTED					GPIO_PIN_SET
#endif /* INIT_CHARGING_LED */
#endif /* INIT_SYS_HAS_BATTERY */

/* The device state. The deviceState will choose the led blinks.
 * This number can be between one to five. Look into the getBlinksFromStatus() function
 * for more detail.
 * The anti parameter will set whether the led is normally on/off. Zero meaning normally off. */
typedef struct{
	uint8_t 	deviceState;
	_Bool 		anti;
}s_statLed_struct;
extern s_statLed_struct s_statLed_data;




/* This function shows the status of the system using a defined led.
 * This function is automatically added to the project. Nothing more to it. */
void s_statusLed_main(void);
/* This function will handle the non blocking timing.
 * It will be added to the system automatically, nothing more to it. */
void s_statLed_tick(void);



#endif /* INIT_SYS_STAT_LED */

















#endif /* INC_INC_PRJ_SYSADDONS_SYS_STATUSLED_H_ */
