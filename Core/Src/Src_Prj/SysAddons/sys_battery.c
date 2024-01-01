/*
 * sys_battery.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
#include "sys_battery.h"
#if INIT_SYS_HAS_BATTERY

uint8_t s_batteryConsoleSpace = RESET;

#if BATTERY_READ_FROM_ADC
extern ADC_HandleTypeDef BATTERYADC_HANDLE;

void s_batteryReadAdc(void){
	if(HAL_ADC_PollForConversion(&BATTERYADC_HANDLE, 100) == HAL_OK){
		uint32_t voltageRaw = HAL_ADC_GetValue(&BATTERYADC_HANDLE);
		uint32_t voltageTemp = ( (voltageRaw * ADC_MAX_VOLTAGE * VOLTAGE_DIVIDER) / ADC_MAX_DIGITAL_VALUE) + VOLTAGE_OFFSET;
		sysData.sys.batteryVoltage = ( (sysData.sys.batteryVoltage*95) + (voltageTemp)*5) / 100;

		if( sysData.sys.batteryVoltage > VOLTAGE_THRESH_MAX_LOW ){
			sysData.sys.batteryPercent = 100;
		}else if ( sysData.sys.batteryVoltage < VOLTAGE_THRESH_MIN ){
			sysData.sys.batteryPercent = 0;
		}else{
			sysData.sys.batteryPercent =
					( ( (sysData.sys.batteryVoltage - VOLTAGE_THRESH_MIN)*100) / (VOLTAGE_THRESH_MAX_LOW - VOLTAGE_THRESH_MIN) );
		}
	}
}
#else
void s_getBatteryFromAnotherSource(void){
	uint8_t percent = RESET;	/* in % */
	uint16_t voltage = RESET;	/* in mV */
	/* Logic to fetch/calculate battery data. */




	/* Put the values fetched in their corresponding variables. */
	sysData.sys.batteryPercent = percent;
	sysData.sys.batteryVoltage = voltage;
}
#endif /* BATTERY_READ_FROM_ADC */
uint32_t recheckBattery = RESET;
void s_battery_tick(void){
	recheckBattery++;
}


void s_checkbattery_main(void){
	if(recheckBattery > RECHECK_BATTERY_INTERVAL){
		recheckBattery = RESET;
#if BATTERY_READ_FROM_ADC
		s_batteryReadAdc();
#else
		s_getBatteryFromAnotherSource();
#endif /* BATTERY_READ_FROM_ADC */
		console_printf(s_batteryConsoleSpace, CONSOLE_PART_TWO, "[%d]", sysData.sys.batteryVoltage);
		console_printf(s_batteryConsoleSpace+1, CONSOLE_PART_TWO, "[%d]", sysData.sys.batteryPercent);
	}
}


void s_battery_init(void){
#if BATTERY_READ_FROM_ADC
	HAL_ADC_Start(&hadc1);
#endif
	s_batteryConsoleSpace = console_requestSpace(2, "Battery Information");
	console_printf(s_batteryConsoleSpace, CONSOLE_PART_ONE, "System Voltage");
	console_printf(s_batteryConsoleSpace+1, CONSOLE_PART_ONE, "System Battery");
}


#endif /*INIT_SYS_HAS_BATTERY */
