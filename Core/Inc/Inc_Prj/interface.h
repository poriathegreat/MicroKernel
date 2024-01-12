/*
 * interface.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSTEM_INTERFACE_H_
#define INC_INC_PRJ_SYSTEM_INTERFACE_H_
#include "sys.h"
/************************************************************************************/
/*                               Exported Variables                                 */
/************************************************************************************/
/* The main system structure.
 * All the data from the different parts of the system will be saved here.
 * There should be ONE parameter per system part.
 * All the information related to that specific part of the system should be
 * written in its own structure, within this structure. */
typedef struct {
	/* This is how every part of the system should be added here */
	sysInfoStructure_t*      sys;
	/***************************************
	 * Sub-system structures should each be
	 * added here as they are added to the
	 * project.
	 ***************************************/

	/* The memory exported data structure */
}interfaceDataStructure_t;
extern interfaceDataStructure_t interfaceData;

/* This is the entry point function.
 * This function should be placed in the SUPER LOOP.
 * Inside this function should be all the different tasks. */
void interface_tasks(void);
void interface_tick(void);
void interface_firstInit(void);
HAL_StatusTypeDef interface_passDataToNextLayer(uint8_t *data, size_t dataSize);




#endif /* INC_INC_PRJ_SYSTEM_INTERFACE_H_ */
