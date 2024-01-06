/*
 * sys_addons.h
 *
 *  Created on: Jan 6, 2024
 *      Author: PoriaTheGreat
 */

#ifndef INC_INC_PRJ_SYSADDONS_SYS_ADDONS_H_
#define INC_INC_PRJ_SYSADDONS_SYS_ADDONS_H_

#include "system_setup.h"

#if INIT_SYS_HAS_BATTERY
#include "sys_battery.h"
#endif
#if INIT_STANDBY_FUNCTIONALITY
#include "sys_standby.h"
#endif
#if INIT_CONFIGURE_PIN_STATE
#include "sys_configure_io.h"
#endif
#if INIT_SAVE_TO_MEMORY
#include "sys_savetomemory.h"
#endif
#if INIT_SYS_STAT_LED
#include "sys_statusled.h"
#endif
#include "sys_loadserialnumber.h"

#include "sys_buffer.h"



typedef struct{

}s_addonsStructure;




















#endif /* INC_INC_PRJ_SYSADDONS_SYS_ADDONS_H_ */
