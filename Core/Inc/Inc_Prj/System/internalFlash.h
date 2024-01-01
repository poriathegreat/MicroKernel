/*
 * internalFlash.h
 *
 *  Created on: Jun 22, 2022
 *      Author: HP
 */

#ifndef INC_INTERNALFLASH_H_
#define INC_INTERNALFLASH_H_


/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"


/* Functions to interact with the internal flash memory. */
uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords);
void Convert_To_Str (uint32_t *Data, char *Buf);
void Flash_Write_NUM (uint32_t StartSectorAddress, float Num);
float Flash_Read_NUM (uint32_t StartSectorAddress);



/********************  FLASH_Error_Codes   ***********************//*
HAL_FLASH_ERROR_NONE      0x00U  // No error
HAL_FLASH_ERROR_PROG      0x01U  // Programming error
HAL_FLASH_ERROR_WRP       0x02U  // Write protection error
HAL_FLASH_ERROR_OPTV      0x04U  // Option validity error
*/


#endif /* INC_INTERNALFLASH_H_ */
