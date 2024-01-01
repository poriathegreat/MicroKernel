/*
 * process_gps.c
 *
 *  Created on: Jul 23, 2023
 *      Author: PoriaTheGreat
 */
#include "t_gps.h"

/* Enable debug for the GPS task */
#define DEBUG_GPS_TASK					1


/* GPS task processor configurations */
#define MOVEMENT_THRESHOLD_NORMAL		80		/* in meters */
#define MOVEMENT_MINIMUM_SPEED			5

#define ANGLE_MINIMUM_TIME 				8000
#define ANGLE_MAX_BASE_NUMBER			200 /* for speed 60, and minimumAngleChnage of 9, we have (ANGLE_MAX_BASE_NUMBER/60) + 9 */
#define ANGLE_MINIMUM_CHANGE			9
#define ANGLE_SENSITIVITY_MULTIPLIER	1.5	// Min=0.5, Max=2, default=1
#define ANGLE_MINIMUM_TIME_LIMIT		5	//default 1

#define OVER_SPEED_THRESHOLD			110

#define MAXIMUM_VALUABLEPOINT_TIME		240
#define MINIMUM_STOP_TIME				240 //SECONDS
#define MINIMUM_SIGNAL_LOST_TIME		120
#define MINIMUM_OVER_SPEED_TIME			5




/* Angle change definitions. Enumerated as type. */
typedef enum{
	ANGLE_CHANGE_NOTDETECTED = 0,
	ANGLE_CHANGE_DETECTED    = 1,
}angleChange_t;

/* Movement detection definitions. Enumerated as type. */
typedef enum {
	MOVEMENT_DETECTED 		= 0,
	MOVEMENT_NOT_DETECTED 	= 1
}movement_detection_t;

/* True/False enumeration. */
enum trueFalse{
	IS_FALSE 	= 	0,
	IS_TRUE		=	1
};

/* The system will output the following type points.
 * The actual position information for that point is stored in
 * the valuablePoint structure. */
typedef enum{
	TASKGPS_USELESS_POINT,
#if TASK_GPS_DETECT_SIGNALLOST
	TASKGPS_SIGNALLOST_BEGIN,
	TASKGPS_SIGNALLOST_END,
#endif /* TASK_GPS_DETECT_SIGNALLOST */
	TASKGPS_STOPPOINT_BEGIN,	// save data
	TASKGPS_STOPPOINT_END,		// save data
#if TASK_GPS_DETECT_OVERSPEED
	TASKGPS_OVERSPEED_BEGIN,	// save data
	TASKGPS_OVERSPEED_END,
#endif /* TASK_GPS_DETECT_OVERSPEED */
	TASKGPS_ANGLE_CHANGED,
	TASKGPS_POSITION
}gpsProcessResults_t;


/* Local parameters. */
double dist = RESET;
uint16_t angleChangeAvr= RESET;
uint8_t taskGpsConsoleSpace = RESET;
m_gpsTaskDataStructure taskGpsData = {0};


/* The GPS processor definition */
gpsProcessResults_t taskGps_processGpsData(
		m_gpsMinimumDataStructure *live,
		m_gpsMinimumDataStructure *stop,
		m_gpsMinimumDataStructure *lastValid,
#if TASK_GPS_DETECT_SIGNALLOST
		m_gpsMinimumDataStructure *signalLost,
#endif /* TASK_GPS_DETECT_SIGNALLOST */
#if TASK_GPS_DETECT_OVERSPEED
		m_gpsMinimumDataStructure *overSpeed,
#endif /* TASK_GPS_DETECT_OVERSPEED */
		m_gpsMinimumDataStructure *valuablePoint
);




void task_gpsStart(void){
/* This is a valuable point. */
#if DEBUG_GPS_TASK
	taskGpsConsoleSpace = console_requestSpace(6, "GPS TASK DEBUG");
#endif /* DEBUG_GPS_TASK */
	console_printf(taskGpsConsoleSpace, CONSOLE_PART_ONE, "CURRENT TIME:  ");

}

void task_gps(void){
	/* This process will check the GPS for new data and will process it. */
	if(sysData.gps.newData == SET){
		sysData.gps.newData = RESET;

		taskGpsData.currentPoint.lat		 = sysData.gps.lat;
		taskGpsData.currentPoint.lon		 = sysData.gps.lon;
		taskGpsData.currentPoint.avrSpeed 	 = sysData.gps.avrSpeed;
		taskGpsData.currentPoint.bearing	 = sysData.gps.bearing;
		taskGpsData.currentPoint.pdop		 = sysData.gps.pdop;
		taskGpsData.currentPoint.satellite	 = sysData.gps.satellite;
		taskGpsData.currentPoint.speed		 = sysData.gps.speed;
		taskGpsData.currentPoint.unixTime	 = sysData.gps.unixTime;
		memcpy(taskGpsData.currentPoint.time , sysData.gps.time, 13);

		gpsProcessResults_t gpsResult =
				taskGps_processGpsData(
						&taskGpsData.currentPoint,
						&taskGpsData.stopPoint,
						&taskGpsData.lastValidPoint,
#if TASK_GPS_DETECT_SIGNALLOST
						&taskGpsData.signalLostPoint,
#endif /* TASK_GPS_DETECT_SIGNALLOST */
#if TASK_GPS_DETECT_OVERSPEED
						&taskGpsData.overSpeedPoint,
#endif /* TASK_GPS_DETECT_OVERSPEED */
						&taskGpsData.valuablePoint
				);



		if(gpsResult != TASKGPS_USELESS_POINT)	{angleChangeAvr = RESET;}

		switch (gpsResult){
		case TASKGPS_USELESS_POINT:
			console_printf(taskGpsConsoleSpace, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			break;

		case TASKGPS_ANGLE_CHANGED:
			console_printf(taskGpsConsoleSpace+1, CONSOLE_PART_ONE, "LAST ANGLE CHANGE: ");
			console_printf(taskGpsConsoleSpace+1, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			task_createPacket(PACKET_COURSECHANGE);
			break;
#if TASK_GPS_DETECT_OVERSPEED
		case TASKGPS_OVERSPEED_BEGIN:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			task_createPacket(PACKET_OVERSPEEDBEGIN);
			break;

		case TASKGPS_OVERSPEED_END:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			task_createPacket(PACKET_OVERSPEEDEND);
			break;
#endif /* TASK_GPS_DETECT_OVERSPEED */
		case TASKGPS_POSITION:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			task_createPacket(PACKET_LOCATION);
			break;
#if TASK_GPS_DETECT_SIGNALLOST
		case TASKGPS_SIGNALLOST_BEGIN:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			task_createPacket(PACKET_SIGNALLOST);
			break;

		case TASKGPS_SIGNALLOST_END:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			task_createPacket(PACKET_SIGNALBACK);
			break;
#endif /* TASK_GPS_DETECT_SIGNALLOST */
		case TASKGPS_STOPPOINT_BEGIN:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			if(task_flagCheck(FLAGS_TASK_DETECTMOVEMENT)){
				task_flagConf(FLAGS_TASK_SEND_STOPDETECTED_SMS, SET);
			}
			task_flagConf(FLAGS_TASK_SAVESTATUS, SET);
			task_createPacket(PACKET_STOPDETECTED);
			break;

		case TASKGPS_STOPPOINT_END:
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_ONE, "LAST POSITION POINT: ");
			console_printf(taskGpsConsoleSpace+2, CONSOLE_PART_THREE, "[%lu]", taskGpsData.currentPoint.unixTime);
			if(task_flagCheck(FLAGS_TASK_DETECTMOVEMENT)){
				task_flagConf(FLAGS_TASK_SEND_MOVEMENTDETECTED_SMS, SET);
			}
			task_flagConf(FLAGS_TASK_SAVESTATUS, SET);
			task_createPacket(PACKET_MOVEMENTDETECTED);
			break;

		}
	}
}




movement_detection_t taskGps_checkDistanceSinceLastPoint(
		double movementThreshold,
		m_gpsMinimumDataStructure *live,
		m_gpsMinimumDataStructure *lastValid
);



angleChange_t taskGps_checkAngleChange(
		m_gpsMinimumDataStructure *previous,
		m_gpsMinimumDataStructure *live
);


/**
  * @brief  This function will check if we have moved.
  * @param  gps_currentData_t is the current GPS parsed from NMEA.
  * @param  live is the current GPS data.
  * @param  lastValid is the last point we had since we had movement.
  *
  * @retval The GPS state and it's corresponding pointData_t.
  * The states can be found in the GPS_states ENUM. */
movement_detection_t taskGps_checkDistanceSinceLastPoint(
		double movementThreshold,
		m_gpsMinimumDataStructure *live,
		m_gpsMinimumDataStructure *previous
){
	movement_detection_t result = MOVEMENT_NOT_DETECTED;

	if(previous->lat == 0){
		memcpy(previous, live, sizeof(m_gpsMinimumDataStructure));
	}else {
		dist = gps_distance(previous->lat,
				previous->lon,
				live->lat,
				live->lon) * 1000;

		if(dist > movementThreshold){
			memcpy(previous, live, sizeof(m_gpsMinimumDataStructure));
			result = MOVEMENT_DETECTED;
		}

	}
	return result;
}



/* If we detect an angle change, we step in.
 *
 * This angle change shall be considered using a formula of some kind.
 * if we were to send a point where there is a 15 degree angle change with speeds over 80kph
 * and we were to send a point where there is a 20 degree change with speeds under 80kph.
 *
 * we could take this and make a formula which takes into account time as well...
 *
 * in streets where speed is low, angle change could be great... however if we set a 30 second no point rule, we might lose a lot of distance.
 * to fix this problem, lets come up with a formula connecting speed, time, and angle. (later this could be scaled to consider distance as well.)
 *
 * lets say speed is 5, then we should be able to send points on a shorter interval, plus the angle change (in city streets) will be much sharper.
 * lets say speed is above 80, then we should limit the points sent to a longer interval (in high speeds, people are usually going straight), plus the angle change, if any, is very little.
 *
 * so lets tie this all together: speed and time are closely connected. More the speed, longer the interval.
 * Minimum interval should be 5 seconds. Maximum interval with angle change, should be around 60 seconds.
 * Minimum speed is 5 (and under) and maximum speed should be 100 (and over).
 * Minimum angle change with high speeds should be 10, and the maximum angle change, with low speeds should be 50.
 *
 * so if the vehicle were to turn with 5kph, at a strict 90* junction, 40 degrees in, the device would send a point. 5 seconds later, the device would realize the rest of the 90, and send another point.
 *
 * and if a vehicle were to drive at 100kph it would be sensitive to change, and send a point where it detects 10 degrees of change, however there is a great deal of time limitation.
 * and if a vehicle were to drive at 5kph, it would not be very sensitive to change, on the other hand if change IS detected, the device has less time limitation. (Ideal for city)
 *
 * so to be able to check the angle change, first we need a time limitation:
 * the time limitation has to work closely with speed, the higher the speed, the higher the time limitation..
 * avrSpeed is a good parameter to play around with.
 * timeLimit = avrSpeed;
 * if timeLimit is less than 5, place: timeLimit = 5
 * if timeLimit is less than 10 and more than 5, place: timeLimit = avrSpeed
 * if timeLimit is more than 10, place: timeLimit = 10+((avrSpeed-10)/2)
 * so if time passed is more than "timeLimit", and an angle change has been detected we should send a point. so avrSpeeds of 30, will have a 25 second limitation and so on.
 *
 * now that we have the time limitation, we need a second rule, angel change according to speed.
 * speeds of 5 and below, angle change of 50
 * speeds of 90 and above, angle change of 10
 *
 * should the change be linear? speed of 40, should be sensitive to an angle change of 40-10/2 + 10 , 25 ? This doesn't seem like a good idea...
 * After a bit of thinking, I decided to tweak the y = 1/x plot. Resulting in ( y = (200/x) + 10 ). This particular graph has great changes in the beginning and small changes towards the end.
 * Some results of the formula are:
 * if speed < 5 then speed = 5, (to keep the formula stable).
 * (speed -> angleSensitivity + 5 = angleSensivity_new)
 * 5  -> 50+5 = 55
 * 7  -> 38+5 = 43
 * 8  -> 35+5 = 40
 * 9  -> 32+5 = 37
 * 10 -> 30+5 = 35
 * 15 -> 23+5 = 28
 * 20 -> 20+5 = 25
 * 30 -> 17+5 = 22
 * 40 -> 15+5 = 20
 * 50 -> 14+5 = 19
 * 60 -> 13+5 = 18
 * 100-> 12+5 = 17
 *
 * These look like good numbers to me...
 *
 * So first, the time limitation:
 * Then, the angle limitation. */
angleChange_t taskGps_checkAngleChange(
		m_gpsMinimumDataStructure *lastSentPoint,
		m_gpsMinimumDataStructure *live
){

	angleChange_t result = ANGLE_CHANGE_NOTDETECTED;

	/* We only check angle change if the speed is more than 5 KM/h */
	if(	live->avrSpeed > (MOVEMENT_MINIMUM_SPEED-1) ){

		/* We calculate the angle change. */
		uint16_t angle = abs(lastSentPoint->bearing - live->bearing);
		angle =  (360 - angle) < (angle) ? (360 - angle) : (angle);
		angleChangeAvr = 0.5*angle + 0.5*angleChangeAvr;

		/* Limit time based on avrSpeed. */
		uint16_t timeLimitationParameter = live->avrSpeed;
		/* if the speed is less than ANGLE_MINIMUM_TIME_LIMIT, we should keep the limitation parameter ANGLE_MINIMUM_TIME_LIMIT */
		if (timeLimitationParameter < ANGLE_MINIMUM_TIME_LIMIT) 		timeLimitationParameter = ANGLE_MINIMUM_TIME_LIMIT;


		uint8_t angleChangeLimit = 		( ( ( (ANGLE_MAX_BASE_NUMBER)/(timeLimitationParameter) ) + (ANGLE_MINIMUM_CHANGE/ANGLE_SENSITIVITY_MULTIPLIER) ) / ANGLE_SENSITIVITY_MULTIPLIER ) ;
		if(timeLimitationParameter > 5)		timeLimitationParameter = ((timeLimitationParameter-ANGLE_MINIMUM_TIME_LIMIT)/4) + ANGLE_MINIMUM_TIME_LIMIT;
		timeLimitationParameter = timeLimitationParameter*SECONDS;	/* Convert seconds to miliseconds */

		if(angleChangeAvr > angleChangeLimit){
			uint16_t angleOverChange = ( ( SECONDS*(angleChangeAvr - angleChangeLimit) ) / 2 );
			if(timeLimitationParameter > angleOverChange) timeLimitationParameter = timeLimitationParameter-angleOverChange;
			else timeLimitationParameter = 0;	/* send data ASAP (no less than ANGLE_MINIMUM_TIME mili-seconds are allowed) */
		}

		uint32_t timeSinceLastSent = abs(live->unixTime - lastSentPoint->unixTime);
		if(
				(angleChangeAvr > angleChangeLimit)
				&&
				( timeSinceLastSent*SECONDS > (timeLimitationParameter + (ANGLE_MINIMUM_TIME/ANGLE_SENSITIVITY_MULTIPLIER) ) )
		) result = ANGLE_CHANGE_DETECTED;

		if(taskGpsConsoleSpace) console_printf(taskGpsConsoleSpace+5, CONSOLE_PART_ONE, "AvrSpeed:%02d Time:%04d/%04d Ang:%02d/%02d Dist:%02f       ",
				live->avrSpeed,
				timeSinceLastSent,
				timeLimitationParameter/SECONDS,
				angleChangeAvr,
				angleChangeLimit,
				dist
				);
	}
	return result;
}


taskGps_data t_gpsData = {0};



void taskGps_refreshLocation(
		m_gpsMinimumDataStructure *previous,
		m_gpsMinimumDataStructure *now
){
	previous->lat	=	(now->lat);
	previous->lon	=	(now->lon);
}

void taskGps_refreshTime(
		m_gpsMinimumDataStructure *previous,
		m_gpsMinimumDataStructure *now
){
	memcpy((char*)(previous->time), (const char*)(now->time),13);

}

void taskGps_refreshPoint(
		m_gpsMinimumDataStructure *previous,
		m_gpsMinimumDataStructure *now
){
	memcpy(previous, now, sizeof(m_gpsMinimumDataStructure));
}



/**
  * @brief  Processes the gps data and gives an understandable output.
  * @param  gps_currentData_t is the current GPS parsed from NMEA.
  * @param  gps_lastValidData_t is the last valid data we had.
  * @param  gps_stopPointData_t is the last point we had since we had movement.
  *
  * @retval The GPS state and it's corresponding pointData_t.
  * The states can be found in the GPS_states ENUM. */
gpsProcessResults_t taskGps_processGpsData(
		m_gpsMinimumDataStructure *live,
		m_gpsMinimumDataStructure *stop,
		m_gpsMinimumDataStructure *lastMovement,
#if TASK_GPS_DETECT_SIGNALLOST
		m_gpsMinimumDataStructure *signalLost,
#endif /* TASK_GPS_DETECT_SIGNALLOST */
#if TASK_GPS_DETECT_OVERSPEED
		m_gpsMinimumDataStructure *overSpeed,
#endif /* TASK_GPS_DETECT_OVERSPEED */
		m_gpsMinimumDataStructure *valuablePoint
		){
	gpsProcessResults_t result = TASKGPS_USELESS_POINT;

	/* If there is 2D or 3D fixed GPS data, we move forward. */
	if(
			(live->lat != 0)
			&& (live->pdop != 0)
			&& (live->pdop < 3.0)
			&& (live->satellite > 5)
	){

		/* We check to see if we have moved MOVEMENT_THRESHOLD_NORMAL meters.
		 * If we have moved, we reset the movement detection timer, showing that we have moved recently. */
		if(taskGps_checkDistanceSinceLastPoint(MOVEMENT_THRESHOLD_NORMAL, live, lastMovement) == MOVEMENT_DETECTED){
			/* We update the lastMovement structure (confirming there is movement). */

			/* Since there's movement, if we're not in a stop, we refresh the stop parameters.
			 * However if we are in a stop, we keep the data intact so we could send it to the server in the next step.
			 * Once the data is sent out (and/or saved) we refresh the stopPoint. */
			if(t_gpsData.systemHasStoppedMoving == IS_FALSE) taskGps_refreshPoint(stop, live);
		}

#if TASK_GPS_DETECT_SIGNALLOST
		/* Since we are here, it means we have a fix of the GPS signal.
		 * We reach here once and if the signal returns and we have confirmed whether we were at a stop or not
		 * we only send the GPS lost time if we are currently not in stop mode.
		 * We reset the GPS fix timer
		 * if we are not in a signal lost state, we refresh the signal back parameters
		 * or else, we keep those parameters to send to the server now that we're back.
		 *
		 * this means: for example we move the car into the garage, obviously once the signal is lost,
		 * we no longer detect movement, so once the signal is back we check to see if there has been movement,
		 * if no movement, we send stop_begin point.
		 * HOWEVER what if we are parked near a window? over night the signal could come and go many times,
		 * sending many signalLost, signalBack events. This is not what we want....
		 * so both here and where we send the POINT_gpsLost, we add another limitation, to only care for signal lost IF
		 * the device is not in stop mode. */
		t_gpsData.timeWhenWeLastHadSignal = live->unixTime;
		if(t_gpsData.systemHasLostSignal == IS_FALSE) {
			/* Refresh the last point where we had a good signal. */
			taskGps_refreshPoint(signalLost, live);

		}else if(t_gpsData.systemHasLostSignal == IS_TRUE){
			if(t_gpsData.systemHasStoppedMoving == IS_FALSE){
				t_gpsData.systemHasLostSignal = IS_FALSE;
				taskGps_refreshPoint(valuablePoint, live);
				return TASKGPS_SIGNALLOST_END;
			}else{
				t_gpsData.systemHasLostSignal = IS_FALSE;
			}
		}
#endif /* TASK_GPS_DETECT_SIGNALLOST */

		/* If no movement has been detected for more than STOP_MODE_AFTER minutes, we assume the vehicle has stopped. */
		uint32_t timeSinseMovementDetected = abs(lastMovement->unixTime - live->unixTime);

		if(timeSinseMovementDetected > MINIMUM_STOP_TIME){
			/* If we have not sent the stop_begin point, we will do so at once. */
			if(t_gpsData.systemHasStoppedMoving == IS_FALSE){
				t_gpsData.systemHasStoppedMoving = IS_TRUE;
				taskGps_refreshLocation(stop, live);
				taskGps_refreshPoint(valuablePoint, live);
				return TASKGPS_STOPPOINT_BEGIN;
			}
		/* If there is movement: */
		}else {
			uint32_t secondsSinceLastValuablePoint = abs(valuablePoint->unixTime - live->unixTime);

			/* If the device is currently in STOP mode, exit stop mode and report back. */
			if(t_gpsData.systemHasStoppedMoving == IS_TRUE){
				t_gpsData.systemHasStoppedMoving = IS_FALSE;
				taskGps_refreshPoint(valuablePoint, live);
				return TASKGPS_STOPPOINT_END;
			}

#if TASK_GPS_DETECT_OVERSPEED
			/* If we are detecting over speed for the first time. */
			else if(
					(live->avrSpeed > OVER_SPEED_THRESHOLD)
					&&(t_gpsData.systemIsOverSpeeding == IS_FALSE)
					&&(secondsSinceLastValuablePoint > 10)
			){
				t_gpsData.systemIsOverSpeeding = IS_TRUE;
				taskGps_refreshPoint(overSpeed, live);
				taskGps_refreshPoint(valuablePoint, live);
				return TASKGPS_OVERSPEED_BEGIN;
			}

			/* If we detect normal speed after over speed is detected */
			else if(
					(live->avrSpeed < OVER_SPEED_THRESHOLD)
					&&(t_gpsData.systemIsOverSpeeding == IS_TRUE)
			){
				t_gpsData.systemIsOverSpeeding = IS_FALSE;
				uint32_t secondsSinseOverSpeedBegan = abs(overSpeed->unixTime - live->unixTime);
				/* We only send the over speed time if its over 5 seconds. */
				if(secondsSinseOverSpeedBegan > MINIMUM_OVER_SPEED_TIME){
					taskGps_refreshPoint(valuablePoint, live);
					return TASKGPS_OVERSPEED_END;
				}
			}
#endif /* TASK_GPS_DETECT_OVERSPEED */
			/* If angle change is detected, angle change point will be sent.
			 * The detection is done based on an algorithm explained elsewhere. */
			else if( taskGps_checkAngleChange(valuablePoint, live) == ANGLE_CHANGE_DETECTED ){
				taskGps_refreshPoint(valuablePoint, live);
				return TASKGPS_ANGLE_CHANGED;
			}

			/* If no change has been detected in the system and we are moving and the car is not lifted
			 * and its been POINT_POSITION_TIMING seconds since the last point was sent to the server
			 * thats when we send a normal position point.  */
			else if(
				(secondsSinceLastValuablePoint > MAXIMUM_VALUABLEPOINT_TIME)
			){
				taskGps_refreshPoint(valuablePoint, live);
				return TASKGPS_POSITION;
			}
		}
	/* If signal is lost: */
	}
#if TASK_GPS_DETECT_SIGNALLOST
	else{
		/* If the GPS signal is not fixed:
		 * AND we are not in a full stop mode,
		 * At such event, we need to see if the clock is correctly fixed.
		 * If the clock is not correctly fixed, we are doomed :')  */
		uint32_t secondsSinceSignalLost = abs(valuablePoint->unixTime - signalLost->unixTime);
		if(
				(t_gpsData.systemHasLostSignal == IS_FALSE)
				&&(secondsSinceSignalLost > MINIMUM_SIGNAL_LOST_TIME)
				&&(t_gpsData.systemHasStoppedMoving == IS_FALSE)
		){
			/* send signal lost*/
			t_gpsData.systemHasLostSignal = IS_TRUE;
#if TASK_GPS_DETECT_OVERSPEED
			t_gpsData.systemIsOverSpeeding = IS_FALSE;
#endif /* TASK_GPS_DETECT_OVERSPEED */
			return TASKGPS_SIGNALLOST_BEGIN;
		}
	}
#endif
	return result;
}



