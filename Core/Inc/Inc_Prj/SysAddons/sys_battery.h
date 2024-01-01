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

/* Check if this part of the system is enabled. */
#if INIT_SYS_HAS_BATTERY

/* This is the limitation set for checking the battery.
 * The battery will be checked every RECHECK_BATTERY_INTERVAL */
#define RECHECK_BATTERY_INTERVAL		(1000) /* milliseconds */

/* If battery is read from the ADC, this part of the system can be enabled. */
#define BATTERY_READ_FROM_ADC			(1)
#if BATTERY_READ_FROM_ADC
/* Choose the ADC handle that is used to read the battery voltage. */
#define BATTERYADC_HANDLE				hadc1
/* If a voltage diver is used, the ratio of the divider should be set here. */
#define VOLTAGE_DIVIDER					(10)



/* This is the ADC reference voltage */
#define ADC_MAX_VOLTAGE					(3300)

/* The ADC accuracy. */
#define ADC_MAX_DIGITAL_VALUE			(4095)


/* These values are used to calculate the current battery percentage.
 * If the voltage is between MAX_HIGH and MAX_LOW, the percentage will be set to 100
 * while VOLTAGE_THRESH_MIN is the voltage for 0 percent.
 * Anything in between is calculated accordingly.
 *
 * The offset will add that voltage to the calculated voltage (there might be drop before the voltage
 * reaches the MCU ADC)
 *  */
#define VOLTAGE_THRESH_MAX_HIGH			(4150)
#define VOLTAGE_THRESH_MAX_LOW			(3950)


#define VOLTAGE_THRESH_MIN				(3600)
#define VOLTAGE_OFFSET					(100)
#endif /* BATTERY_READ_FROM_ADC */


/* This function is automatically added to the sys_init() function. Nothing more to do here.*/
void s_battery_init();

/* This function is automatically added to the sys() function. Nothing more to do here.*/
void s_checkbattery_main(void);












#endif /* INIT_SYS_HAS_BATTERY */
#endif /* INC_INC_PRJ_SYSADDONS_SYS_BATTERY_H_ */
