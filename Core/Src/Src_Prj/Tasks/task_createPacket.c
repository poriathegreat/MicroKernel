/*
 * task_createPacket.c
 *
 *  Created on: Jul 24, 2023
 *      Author: PoriaTheGreat
 */
#include "tasks.h"
#include "t_ignition.h"


#define PAYLA			0
#define PORIA			1
#define JSON			2

#define PROTOCOL		PORIA



extern uint8_t adcPercent;
/* This function will create the packets and pass it to the memory buffer for further processing. */
void task_createPacket(uint8_t packetType){

	char packetBuff[255];
	size_t packetSize = RESET;

#if PROTOCOL == PAYLA
	memset(packetBuff, 0 , 255);

	sprintf((char*)packetBuff,"0000%c%c%c%c%c,%s,%s,%s,%02d,%d,%d,%s,%2.6f,%2.6f,%d,%s,%5.0f,%2.2f,%d,%d,%d,%d,%d,%lu,%d,%d,%d,%.0lf,%03d,%lu,%d,%2.2f,%d,%1.2f,%s$",

			(sysData.sys.serialNumber[4]),				//	0												//1
			(sysData.sys.serialNumber[5]),				//	0												//2
			(sysData.sys.serialNumber[6]),				//	0												//3
			(sysData.sys.serialNumber[7]),				//	0												//4
			('P'), 										//	%c				// Packet Type					//5
			(sysData.sys.serialNumber),					//	%s				// Device ID					//6
			(sysData.sys.serialNumber),					//	%s				// Vehicle ID					//7
			(sysData.sys.serialNumber),					//	%s				// Vehicle ID					//8
			(packetType),								//	%02d			// Event Code					//9
			(ignitionState),							//	%d				// Ignition Status				//10
			(0), 										//	%d				// GPS Noise					//11
			(taskGpsData.currentPoint.time),			//	%s				// GPS Time						//12
			(taskGpsData.currentPoint.lat),				//	%2.6f			// GPS LAT						//13
			(taskGpsData.currentPoint.lon),				//	%2.6f			// GPS LON						//14
			(int)(taskGpsData.currentPoint.bearing), 	//	%d				// GPS Bearing					//15
			("999999999"),								//	%s				// Smartcard ID					//16
			(0.0), 										//	%5.0f			// Driving Minutes				//17
			(float)(12),								//	%2.2f			// Main system voltage			//18
			(sysData.sys.batteryVoltage),				//	%2.2f			// Battery Voltage				//19
			(700),										//	%d				// CAN RPM						//20
			(taskData.adcPercent),						//	%d				// CAN FUEL						//21
			(task_flagCheck(FLAGS_TASK_LOCKOPEN)),		//	%d				// I/O Port Stat				//22
			(taskGpsData.currentPoint.speed),			//	%d				// preferedSpeed				//23
			(0LU),										//	%lu				// VehicleSelectedMileage		//24 //26
			(taskGpsData.currentPoint.speed),			//	%d				// GPS Speed					//25 //27
			(120),										//	%d				// OverSpeedValue				//26 //28
			(int)(0),									//	%d				// CanSpeed						//27 //29
			(0.0),										//	%.0lf			// GPSMileage/1000				//28 //30
			(0),										//	%03d%03d%03d	// GSENSOR						//29 //31
			(0LU),										//	%lu				// CAN KM						//30 //32
			(taskGpsData.currentPoint.satellite),		//	%d				// GPS SAT						//31 //33
			(taskGpsData.currentPoint.pdop),			//	%2.2f			// GPS PDOP						//32 //34
			(0),
			// last two
			(5.5),										//	%1.2f			// SYS BOOT VER					//33 //35
			(sysData.sys.version)						//	%s				// SYS MAIN VER					//34 //36
			);



	packetSize = strlen(packetBuff);

	packetBuff[0] = (uint8_t)(packetSize-4);
	packetBuff[1] = (uint8_t)((packetSize-4) >> 8 & 0xff) ;
	packetBuff[2] = (uint8_t)((packetSize-4) >> 16 & 0xff) ;
	packetBuff[3] = (uint8_t)((packetSize-4) >> 24 & 0xff) ;
#elif PROTOCOL == PORIA
	memset(packetBuff, 0 , 255);
	sprintf((char*)packetBuff,"@%s,%02d,%d,%s,%2.6f,%2.6f,%d,%d,%d,%d,%d,%d,%2.2f,%s&",

			(sysData.sys.serialNumber),					//	%s				// Vehicle ID					//0
			(packetType),								//	%02d			// Event Code					//1
			(ignitionState),							//	%d				// Ignition Status				//2
			(taskGpsData.currentPoint.time),			//	%s				// GPS Time						//3
			(taskGpsData.currentPoint.lat),				//	%2.6f			// GPS LAT						//4
			(taskGpsData.currentPoint.lon),				//	%2.6f			// GPS LON						//5
			(int)(taskGpsData.currentPoint.bearing), 	//	%d				// GPS Bearing					//6
			(sysData.sys.batteryVoltage),				//	%d				// Battery Voltage				//7
			sysData.sys.batteryPercent,
			(task_flagCheck(FLAGS_TASK_LOCKOPEN)),		//	%d				// I/O Port Stat				//9
			(taskGpsData.currentPoint.speed),			//	%d				// GPS Speed					//10
			(taskGpsData.currentPoint.satellite),		//	%d				// GPS SAT						//11
			(taskGpsData.currentPoint.pdop),			//	%2.2f			// GPS PDOP						//12
			(sysData.sys.version)						//	%s				// SYS MAIN VER					//13
			);

	packetSize = strlen(packetBuff);

#endif /* PROTOCOL */
	buffer_addToBuffer((uint8_t*)packetBuff, packetSize);
}

































