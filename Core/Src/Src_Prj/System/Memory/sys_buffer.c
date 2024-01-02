/*
 * bufferBuffer.c
 *
 *  Created on: Apr 10, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system functions */
#include "sys_buffer.h"

#if BUFFER_USE_ENCRYPTION
#include "tinyAesCtr.h"
#endif /* BUFFER_USE_ENCRYPTION */

#if BUFFER_USE_EXTERNAL_FLASH
#include "flashMemoryController.h"
#endif /* BUFFER_USE_EXTERNAL_FLASH */


/************************************************************************************/
/*                               Private Definitions                                */
/************************************************************************************/
enum eventTimers{
	TIMERS_BUFFER_CHECKCELLS = 0,
	TIMERS_BUFFER_WAIT_TIME,

	BUFFERTIMERS_TOTAL
};

#if BUFFER_USE_EXTERNAL_FLASH
	#define BUFFER_CELLS_IN_RAM 			((FCTR_SECTOR_SIZE/256) - 1)
#else
	#define BUFFER_CELLS_IN_RAM 			BUFFER_CELLS_IN_RAM_IF_NO_FLASH 		/* Each 256 bytes!*/
#endif /* BUFFER_USE_EXTERNAL_FLASH */
/************************************************************************************/
/*                                 Private Variables                                */
/************************************************************************************/

uint32_t bufferTimersArray	[BUFFERTIMERS_TOTAL]	= {0};
uint8_t bufferCells			[ ( (BUFFER_CELLS_IN_RAM+1)*256 ) ] 		= {0};	/* This format was used instead of 2D arrays. This format could be written in the flashMemory directly. */
uint8_t* bufferCellsSize = bufferCells+(BUFFER_CELLS_IN_RAM*256);
bufferDataStructure_t bufferData = {0};

uint8_t key[16] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
uint8_t iv[16] = {
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

/************************************************************************************/
/*                                Private functions                                 */
/************************************************************************************/
/* This function will encrypt the data (if feature enabled), before passing
 * to the next layer. */
HAL_StatusTypeDef buffer_encryptBeforePassing(uint8_t *data, size_t dataSize){
	HAL_StatusTypeDef ret = HAL_ERROR;

#if BUFFER_USE_ENCRYPTION
	if(bufferData.encryptionEnabled == SET){
		size_t newDataSize[1] = {0};
		newDataSize[0] = dataSize;

		// Encrypt the input text
		uint8_t ciphertext[256] = {0};

		aes_ctr_encrypt(key, iv, data, ciphertext+8, newDataSize);
		memcpy(ciphertext, sysData.sys.serialNumber, 8);
		newDataSize[0] = newDataSize[0] + 8 ;

		if(interface_passDataToNextLayer(ciphertext,newDataSize[0]) == HAL_OK){
			ret = HAL_OK;
		}else {
			ret = HAL_ERROR;
		}
	}
	else {


	    if(interface_passDataToNextLayer(data,dataSize) == HAL_OK){
	    	ret = HAL_OK;
	    }else {
	    	ret = HAL_ERROR;
	    }
	}
#else
    if(interface_passDataToNextLayer(data,dataSize) == HAL_OK){
    	ret = HAL_OK;
    }else {
    	ret = HAL_ERROR;
    }
#endif /* BUFFER_USE_ENCRYPTION */
    return ret;
}
/************************************************************************************/
/* This function will add the packet to the queue to be sent later.
 * The queue is checked by the buffer() function.
 * If a packet is available, it will then be [encrypted then] sent. */
HAL_StatusTypeDef buffer_addToBuffer(uint8_t *data, size_t dataSize){
	HAL_StatusTypeDef result = HAL_ERROR;
#if BUFFER_DATA_ENABLED
	uint8_t cell = BUFFER_CELLS_IN_RAM;

	for(uint8_t i = RESET ; i < BUFFER_CELLS_IN_RAM ; i++){
		if(bufferCellsSize[i] == RESET){
			cell =  i;
			break;
		}
	}

	if(cell < BUFFER_CELLS_IN_RAM){
		bufferCellsSize[ cell ] = dataSize;
		memcpy(bufferCells + (cell*256) , data , dataSize );
		bufferData.dataWaiting = SET;
		result = HAL_OK;
	}

#if BUFFER_USE_EXTERNAL_FLASH
	else {
		/* if we have filled enough RAM to fit a sector in the FLASH MEMORY,
		 * we copy the data into the next free sector and clear the RAM.
		 */

		if(fctr_pushToFlash( bufferCells ) == FCTRSTAT_OK){
			memset(bufferCells, 0, sizeof(bufferCells));

			/* Since we just emptied the RAM into the FLASH, every cell is EMPTY.
			 * We can use any cell, I used the first cell, cell zero. */
			bufferCellsSize[ 0 ] = dataSize;
			memcpy(bufferCells + (0*256), data , dataSize );
			bufferData.dataWaiting = SET;
			result = HAL_OK;
		}
	}
#endif /* BUFFER_USE_EXTERNAL_FLASH */
#endif /* BUFFER_DATA_ENABLED */
#if !BUFFER_DATA_ENABLED
	if(buffer_encryptBeforePassing(data, dataSize) == HAL_OK){
		result = HAL_OK;
	}else {
		result = HAL_ERROR;
	}
#endif /* BUFFER_DATA_ENABLED */
	return result;
}
/************************************************************************************/
/* This function is run once before entering the super loop. */
void s_buffer_init(void){

#if BUFFER_USE_ENCRYPTION
	/* If KEY and IV are to be changed,
	 * they have to be set here. */
#if INIT_SYS_LOCK_UUID
	 memcpy(iv, sysData.sys.uuid, 12);
#endif /* INIT_SYS_LOCK_UUID */
#endif /* BUFFER_USE_ENCRYPTION */


#if BUFFER_USE_EXTERNAL_FLASH
	  /* This function will initialize the EXTERNAL FLASH buffer. */
	  fctr_firstInit();
#endif /* BUFFER_USE_EXTERNAL_FLASH */

}
/************************************************************************************/
/* This function returns the cell which holds data.
 * if the cell is equal to buffer_CELLS_IN_RAM , it means we have reached the end of the buffer, with no empty cells found.
 * to check validity of the result : if bufferBuffer_cellWithData < buffer_CELLS_IN_RAM -> number is valid */
uint8_t buffer_cellWithData(void){
	uint8_t result = BUFFER_CELLS_IN_RAM;
	for(uint8_t i = RESET ; i < BUFFER_CELLS_IN_RAM ; i++){
		if(bufferCellsSize[i] != RESET){
			result =  i;
//			break;
			return result;
		}
	}

#if BUFFER_USE_EXTERNAL_FLASH
	/* Once this is reached, it should mean there was no buffered data waiting on RAM.
	 * Now is the perfect time to load the FLASH memory into the RAM. */

	/* We need to check if there is anything waiting for us on the flash memory: */
	if(fctr_popFromFlash(bufferCells) == FCTRSTAT_OK){
		/* Since we just loaded a full sector into the RAM, every cell is full.
		 * We can return any cell, I returned the first cell, cell zero. */
		result =  0;
	}


#endif /* BUFFER_USE_EXTERNAL_FLASH */
	return result;
}
/************************************************************************************/
/* This function is to be placed inside the super loop.
 * It will check for a cell with data, if a cell with data is found,
 * it will [encrypt then] send the given package. */
void s_buffer_main(void){
#if BUFFER_DATA_ENABLED
	if(
	    	(bufferTimersArray[TIMERS_BUFFER_CHECKCELLS] > CHECK_BUFFER_INTERVAL_MS)
	){
		bufferTimersArray[TIMERS_BUFFER_CHECKCELLS] = RESET;

		uint8_t cellWithData = buffer_cellWithData();
		if(cellWithData < BUFFER_CELLS_IN_RAM){
			if(buffer_encryptBeforePassing(bufferCells + (cellWithData*256), bufferCellsSize[cellWithData]) == HAL_OK){
				memset(bufferCells + (cellWithData*256), 0, 256);
				bufferCellsSize[cellWithData] = RESET;
			}
			bufferData.dataWaiting = SET;
		}else {
			bufferTimersArray[TIMERS_BUFFER_WAIT_TIME] = RESET;
			bufferData.dataWaiting = RESET;
		}
	}
#endif /* BUFFER_DATA_ENABLED */
}
/************************************************************************************/
/* Add this function to a 1ms timer call back to enable timed multitasking */
void s_buffer_tick(void){
	for(uint8_t timer = RESET ; timer < BUFFERTIMERS_TOTAL ; timer++){
		bufferTimersArray[timer]++;
	}
}

/* Use this function to check if a specific timer has reached a specific number */
uint32_t buffer_timerCheck(uint8_t timer){
	return bufferTimersArray[timer];
}

/* Use this function to reset a specific timer */
void buffer_timerReset(uint8_t timer){
	bufferTimersArray[timer] = RESET;
}



















