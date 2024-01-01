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
extern ADC_HandleTypeDef hadc1;

void sys_batteryFirstInit(){
	HAL_ADC_Start(&hadc1);

}

#define VOLTAGE_THRESH_MAX_HIGH			12500
#define VOLTAGE_THRESH_MAX_LOW			11500
#define VOLTAGE_THRESH_MIN				3300
#define VOLTAGE_OFFSET					0
#define VOLTAGE_DIVIDER					10
#define ADC_MAX_VOLTAGE					3300
#define ADC_MAX_DIGITAL_VALUE			4095
void task_checkAdc(void){
	if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK){
		uint32_t voltageRaw = HAL_ADC_GetValue(&hadc1);
		uint32_t voltageTemp = ( (voltageRaw * ADC_MAX_VOLTAGE * VOLTAGE_DIVIDER) / ADC_MAX_DIGITAL_VALUE) + VOLTAGE_OFFSET;
		taskData.adcVoltage = ( (taskData.adcVoltage*95) + (voltageTemp)*5) / 100;

		if( taskData.adcVoltage > VOLTAGE_THRESH_MAX_LOW ){
			taskData.adcPercent = 100;
		}else if ( taskData.adcVoltage < VOLTAGE_THRESH_MIN ){
			taskData.adcPercent = 0;
		}else{
			taskData.adcPercent =
					( ( (taskData.adcVoltage - VOLTAGE_THRESH_MIN)*100) / (VOLTAGE_THRESH_MAX_LOW - VOLTAGE_THRESH_MIN) );
		}
//		console_printf(sys_consoleSpace+3, CONSOLE_PART_ONE, "ADC VOLT: [%dmv]     ", ((taskData.adcVoltage)));
//		console_printf(sys_consoleSpace+3, CONSOLE_PART_THREE, "ADC PERC: [%d%%]     ", taskData.adcPercent);
	}
}


void task_showBattery(void){
	const uint32_t battLedRepeatTimer = 5000;
	const uint16_t battLedBlinkTimer = 2500;
	static uint8_t battLedToggleTotal = 0;
	static uint8_t battLedToggleCounter = 0;

	if(lockStat == M_LOCK_IDLE){

		battLedToggleTotal = (uint8_t)(sysData.sys.batteryPercent / 10);
		battLedToggleTotal = battLedToggleTotal * 2;
		if(tasks_timerCheck(TIMERS_TASK_BATTLED_EXT) > battLedRepeatTimer){
			if(battLedToggleCounter > battLedToggleTotal){
				tasks_timerReset(TIMERS_TASK_BATTLED_EXT);
				battLedToggleCounter = RESET;

			}else if(tasks_timerCheck(TIMERS_TASK_BATTLED_INT) > (battLedBlinkTimer / battLedToggleTotal)){
				battLedToggleCounter++;
				if(!(battLedToggleCounter > battLedToggleTotal)){
					HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
				}
				tasks_timerReset(TIMERS_TASK_BATTLED_INT);
			}
		}else {
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, RESET);
		}
	}
}


void sys_checkBattery(void){
	if(gsm_flagCheck(FLAGS_GSM_INIT) && (sys_timerCheck(TIMERS_SYS_CHECKBATTERY) > 1000)){
		sys_timerReset(TIMERS_SYS_CHECKBATTERY);

		if(gsmData.batteryVoltage > 2900 ) 	sysData.sys.batteryVoltage = gsmData.batteryVoltage;
		if(gsmData.batteryPercent != RESET) {
			sysData.sys.batteryPercent = gsmData.batteryPercent;
			if( abs(sysData.sys.batteryPercent - gsmData.batteryPercent) > 5){
				task_flagConf(FLAGS_TASK_SAVESTATUS, SET);
			}
		}

		console_printf(sys_infoSpace+3, CONSOLE_PART_TWO, "[%d]", sysData.sys.batteryVoltage);
		console_printf(sys_infoSpace+4, CONSOLE_PART_TWO, "[%d]", sysData.sys.batteryPercent);
	}
}

void sys_checkBatteryInit(void){
	console_printf(sys_infoSpace+3, CONSOLE_PART_ONE, "System Voltage");
	console_printf(sys_infoSpace+4, CONSOLE_PART_ONE, "System Battery");
}


#if INIT_CHARGING_LED
/* Blink led to show charging level of device. */
void sys_chargingLed(uint8_t batteryPercent){
	const uint16_t chargeLedRepeatTimer = 5000;
	const uint8_t  chargeLedBlinkTimer  = 250; /* ( 5000 / ( (100 - 0) / 5 ) ) Max blink time, when battery = 0*/
	static uint8_t chargeLedToggleCounter = RESET;
	uint8_t chargeLedToggleTotal = ( (100 - batteryPercent) / 5 );
	if ( chargeLedToggleTotal == 0 )  {
		HAL_GPIO_WritePin(_CHARGING_LED_GPIO, _CHARGING_LED_Pin, SET);
	}else{

		if(sys_timerCheck(TIMERS_SYS_CHARGING_LED_BLINK_EXT) > chargeLedRepeatTimer){
			if(chargeLedToggleCounter > chargeLedToggleTotal){
				sys_timerReset(TIMERS_SYS_CHARGING_LED_BLINK_EXT);
				chargeLedToggleCounter = RESET;

			}else if(sys_timerCheck(TIMERS_SYS_CHARGING_LED_BLINK_INT) > chargeLedBlinkTimer){
				HAL_GPIO_TogglePin(_CHARGING_LED_GPIO, _CHARGING_LED_Pin);
				chargeLedToggleCounter++;
				sys_timerReset(TIMERS_SYS_CHARGING_LED_BLINK_INT);
			}
		}else {
			HAL_GPIO_WritePin(_CHARGING_LED_GPIO, _CHARGING_LED_Pin, SET);
		}
	}
}
#endif /* INIT_CHARGING_LED */


#endif /*INIT_SYS_HAS_BATTERY */
