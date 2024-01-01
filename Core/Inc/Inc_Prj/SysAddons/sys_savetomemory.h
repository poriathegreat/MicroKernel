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
#define INIT_SAVE_EXTERNAL_MEMORY				0



#if INIT_SAVE_EXTERNAL_MEMORY
#include "w25qxx.h"
#else
#include "internalFlash.h"
#endif /* INIT_SAVE_EXTERNAL_MEMORY */

/************************************************************************************/

typedef struct{
	_Bool saveNow;
}s_ROMdataStruct;
extern s_ROMdataStruct s_ROMdata;


#if INIT_SAVE_EXTERNAL_MEMORY
void sys_EXTROMSave(void);
void sys_EXTROMRead(void);
#else /******************************************************************************/
void sys_ROMSave(char* bufferToSave, size_t sizeOfTheBuffer);
void sys_ROMRead(void);
#endif /* INIT_SAVE_INTERNAL_MEMORY */
/************************************************************************************/
#endif












#endif /* INC_INC_PRJ_SYSADDONS_SYS_SAVETOMEMORY_H_ */
