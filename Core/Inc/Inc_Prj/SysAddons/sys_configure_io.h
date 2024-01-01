/*
 * sys_configure_io.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSADDONS_SYS_CONFIGURE_IO_H_
#define INC_INC_PRJ_SYSADDONS_SYS_CONFIGURE_IO_H_

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"
#if INIT_CONFIGURE_PIN_STATE


/* This function will configure I/Os to the desired levels upon initialization. */
void s_configureio_init(void);











#endif /* INIT_CONFIGURE_PIN_STATE */
#endif /* INC_INC_PRJ_SYSADDONS_SYS_CONFIGURE_IO_H_ */
