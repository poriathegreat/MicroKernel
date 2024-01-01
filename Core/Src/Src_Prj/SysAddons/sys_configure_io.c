/*
 * sys_configure_io.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
#include "sys_configure_io.h"


/************************************************************************************/
#if INIT_CONFIGURE_PIN_STATE
/* This function will configure I/Os to the desired levels. */
void sys_configureI_O(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
}
#endif /* INIT_CONFIGURE_PIN_STATE */














