/*
 * sys_savetomemory.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSADDONS_SYS_SAVETOMEMORY_H_
#define INC_INC_PRJ_SYSADDONS_SYS_SAVETOMEMORY_H_
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"

#if INIT_SAVE_TO_MEMORY

/* Set this to 1 for external memory or to zero for internal memory. */
#define INIT_SAVE_EXTERNAL_MEMORY				1
#if !INIT_SAVE_EXTERNAL_MEMORY
#define INTERNALMEMORY_ENDADRESS				0x08020000
#endif /* INIT_SAVE_EXTERNAL_MEMORY */

#if INIT_SAVE_EXTERNAL_MEMORY
#include "w25qxx.h"
#else
#include "internalFlash.h"
#endif /* INIT_SAVE_EXTERNAL_MEMORY */

/************************************************************************************/

typedef struct{
	_Bool forceSave;
}s_ROMdataStruct;
extern s_ROMdataStruct s_ROMdata;



/* This function is called when the system starts.
 * This function will load the saved data from the internal memory,
 * the data read is then parsed in a different function.
 * This function is automatically added. Nothing more to do here. */
void s_ROM_init(void);

/* This function is added to the sys function that will call it every 1ms.
 * This function is automatically added. Nothing more to do here.*/
void s_ROM_main(void);


/* This function is added to the sys clock function that will call it every 1ms.
 * This function is automatically added. Nothing more to do here.*/
void s_ROM_tick(void);



/************************************************************************************/
#endif /* INIT_SAVE_TO_MEMORY */












#endif /* INC_INC_PRJ_SYSADDONS_SYS_SAVETOMEMORY_H_ */
