/*
 * interface.h
 *
 *  Created on: Dec 31, 2023
 *      Author: HP
 */

#ifndef INC_INC_PRJ_SYSTEM_INTERFACE_H_
#define INC_INC_PRJ_SYSTEM_INTERFACE_H_
#include "sys.h"




void interface_firstInit(void);



HAL_StatusTypeDef interface_passDataToNextLayer(uint8_t *data, size_t dataSize);


/* This is the entry point function.
 * This function should be placed in the SUPER LOOP.
 * Inside this function should be all the different tasks. */
void interface_tasks(void);


void interface_fetchData(void);



void interface_clock(void);








#endif /* INC_INC_PRJ_SYSTEM_INTERFACE_H_ */
