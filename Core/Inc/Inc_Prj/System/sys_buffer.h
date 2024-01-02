/*
 * memoryBuffer.h
 *
 *  Created on: Apr 10, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_BUFFER_H_
#define INC_BUFFER_H_

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"
/************************************************************************************/
/*                                 Exported Macros                                  */
/************************************************************************************/
/* number of buffer cells in ram. Each buffer cell is 256 bytes of data. */
#define BUFFER_DATA_ENABLED					INIT_SYS_BUFFER
#define CHECK_BUFFER_INTERVAL_MS			500		/* Milliseconds */
#define CHECK_BUFFER_WAIT_TIME				240000 	/* Milliseconds*/
#define BUFFER_CELLS_IN_RAM_IF_NO_FLASH		15
#define BUFFER_USE_EXTERNAL_FLASH			1
#define BUFFER_USE_ENCRYPTION				INIT_SYS_USE_ENCYPTION	// controlled by the system_setup.h file
/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
typedef struct{
	_Bool dataWaiting;
	_Bool encryptionEnabled;
}bufferDataStructure_t;	extern bufferDataStructure_t bufferData;
/************************************************************************************/
/*                                Exported functions                                */
/************************************************************************************/
/* This function must be edited by the user to properly handle the next layer.
 * The memory() function, reads data from queue, [encrypts the data] and sends
 * the data to the next layer. */
HAL_StatusTypeDef buffer_passDataToNextLayer(uint8_t *data, size_t dataSize);

/* Use this function to add data to queue, to be sent ASAP */
HAL_StatusTypeDef buffer_addToBuffer(uint8_t *data, size_t dataSize);

/* Add this function to the system 1ms timer.
 * if using the sys.h/sys.c file, add this function to sys_systemClock(). */
void s_buffer_tick(void);

/* Add this function to the Super Loop.  */
void s_buffer_main(void);

/* Place this function before the Super Loop. */
void s_buffer_init(void);



#endif /* INC_MEMORYBUFFER_H_ */
