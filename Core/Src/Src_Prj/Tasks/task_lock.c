/*
 * sys.c
 *
 *  Created on: Jul 11, 2023
 *      Author: PoriaTheGreat
 */
#include "t_lock.h"
#include "t_gps.h"
#include "gps.h"

uint32_t lockTimer = RESET;
uint32_t lockLedTimer = RESET;
_Bool lockFlagsArray[NUMBER_OF_LOCK_FLAGS] = {0};
m_lockStat_t lockStat = M_LOCK_IDLE;


typedef enum{
	LOCK_IDLE,
	LOCK_BOOST_ENABLE,
	LOCK_WAIT,
	LOCK_BOOST_DISABLE,
	LOCK_TURN_OFF

}m_lockMovement_t;

m_lockMovement_t lockStep = LOCK_IDLE;
m_lockMovement_t lockMode = LOCK_IDLE;
uint16_t lockDelay = RESET;

m_gpsMinimumDataStructure unlockLocation = {0};

typedef enum{
	LOCKER_OUT_OF_RANGE,
	LOCKER_IN_RANGE
}unlockRange_t;
unlockRange_t lockRange = LOCKER_OUT_OF_RANGE;

void lock_checkLocation(void){

	gps_distance(unlockLocation.lat, unlockLocation.lon, taskGpsData.lastValidPoint.lat , taskGpsData.lastValidPoint.lon);

//	if(){
//
//	}

}













#define TIMING_BOOST_ENABLE				150
#define TIMING_BOOST_DISABLE			200
#define TIMING_LOCK_OPEN_HOLD			10000
void lock_toggleLock(void){


	if(lock_flagCheck(FLAGS_LOCK_TOGGLE) == SET){
		lock_flagConf(FLAGS_LOCK_TOGGLE, RESET);
		if(task_flagCheck(FLAGS_TASK_LOCKOPEN)){
			HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, SET);
			lockMode = LOCK_BOOST_ENABLE;
			lockStat = M_LOCK_BUSY;
		}
	}


	if(lockMode != LOCK_IDLE) lockStat = M_LOCK_BUSY;

	if( lockTimer > lockDelay )	{lockStep = lockMode;}
	else 						{lockStep = LOCK_IDLE;}

	if(lockStat != M_LOCK_IDLE){

		if((HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)){
			// Don't mess with the LED
		}else if(lockStat != M_LOCK_IDLE){
			if(lockLedTimer > 50){
				lockLedTimer = RESET;
				HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			}
		}else if(task_flagCheck(FLAGS_TASK_LOCKOPEN) && (lockLedTimer > 2500)){
			lockLedTimer = RESET;
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, SET);
		}else if ((lockLedTimer > 100) && (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) != GPIO_PIN_RESET)){
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, RESET);
		}
	}

	switch(lockStep){
	case LOCK_IDLE:
		break;

	case LOCK_BOOST_ENABLE:
		HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, SET);
		if(taskData.adcVoltage > 12000){
			lockTimer = RESET;
			lockDelay = TIMING_BOOST_ENABLE;
			lockMode = LOCK_WAIT;
		}
		break;

	case LOCK_WAIT:

		if(
				(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)
				&&
				(taskData.adcVoltage > 13000)
		){
			HAL_GPIO_WritePin(COIL_EN_GPIO_Port, COIL_EN_Pin, SET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, SET);
			lockTimer = RESET;
		}else if((HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) != GPIO_PIN_RESET)) {
			HAL_GPIO_WritePin(COIL_EN_GPIO_Port, COIL_EN_Pin, RESET);
		}

		if(lockTimer > 15000){
			lockMode = LOCK_BOOST_DISABLE;
		}
		break;

	case LOCK_BOOST_DISABLE:
		HAL_GPIO_WritePin(COIL_EN_GPIO_Port, COIL_EN_Pin, RESET);
		HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, RESET);

		lockTimer = RESET;
		lockDelay = TIMING_BOOST_DISABLE;
		lockMode = LOCK_TURN_OFF;
		break;

	case LOCK_TURN_OFF:
		lockTimer = RESET;
		lockMode = LOCK_IDLE;
		lockStat = M_LOCK_IDLE;
		break;

	default:
		lockMode = LOCK_BOOST_ENABLE;
		break;
	}
}


void lock_addTimer(void){
	lockTimer++;
	lockLedTimer++;
}

_Bool lock_flagConf(uint16_t flag, _Bool state){
	lockFlagsArray[flag]=state;
	return state;
}


_Bool lock_flagCheck(uint16_t flag){
	return lockFlagsArray[flag];
}





