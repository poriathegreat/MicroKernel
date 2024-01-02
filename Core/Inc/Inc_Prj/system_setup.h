/*
 * systemSetup.h
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
#ifndef INC_INC_PRJ_SYSTEMSETUP_H_
#define INC_INC_PRJ_SYSTEMSETUP_H_
/* To add a library to the project:
 *
 * 1. add the library_addTimer() function to the ../Core/Src/Interface/interface_systemClock() function.
 * 2. add the library_dataStructure to the interfaceDataStructure_t of ../Core/Src/Interface/int_fetchData
 * 3. add the fetchData function to add a copy of the data structure to the interfaceDataStructure_t of ../Core/Src/Interface/int_fetchData
 * 4. add the main function to the ../Core/Src/Interface/int_task.
 * 5. add the initialization function to the ../Core/Src/Interface/int_firstInit.
 * 6. add the interrupt handlers as stated in the library.
 */
/*######################################################################################*/
/*       THIS PART OF THE CODE MUST BE EDITED TO SUIT THE NEEDS OF THE PROJECT          */
/*######################################################################################*/
/************************************************************************************/
/*                               System Information                                 */
/************************************************************************************/
/* The project name. */
#define SYS_PROJECT_NAME 						"PUT SYSTEM NAME HERE"
/* The project version, change with every update to the project. */
#define SYS_VERSION 							"0.1"
/************************************************************************************/
/* If INIT_LOAD_SERIAL_FROM_MEMORY is enabled, the sys_loadserialnumber .c/.h
 * files must be edited to read the serial number from a specific memory space.
 *
 * If INIT_LOAD_SERIAL_FROM_MEMORY is disabled, the serialNumber of the device
 * will be set to this hard coded serial number. */
#define INIT_LOAD_SERIAL_FROM_MEMORY			(0)
#if !INIT_LOAD_SERIAL_FROM_MEMORY
#define SYS_HARDCODED_SERIAL					"11228866"
#endif
/************************************************************************************/
/*                                   System Debug                                   */
/************************************************************************************/
#define INIT_SYS_REQUEST_CONSOLE_DEBUG			(0)
#if INIT_SYS_REQUEST_CONSOLE_DEBUG
#define CONSOLE_DEBUG_LINES_NEEDED				(15)
extern uint8_t sys_consoleSpace;
#endif /* INIT_SYS_REQUEST_DEBUG */


/*######################################################################################*/
/*                                   SYSTEM ADDONS                                      */
/*######################################################################################*/


/* If INIT_SYS_STAT_LED is enabled, the sys_statusled .c/.h files need to be edited
 * to reflect the correct status on LED */
#define INIT_SYS_STAT_LED						(1)
/************************************************************************************/
/* If STANDBY_FUNCTIONALITY is enabled, the sys_standby .h/.c files needs to be edited
 * in that file, we need to define what will keep the system from entering standby. */
#define INIT_STANDBY_FUNCTIONALITY 				(1)
/************************************************************************************/
/* If INIT_CONFIGURE_PIN_STATE is enabled, the sys_configure_io .c/.h files needs to be edited
 * in those files, we need to re-define the initial PIN states.
 * This function is not mandatory, as I/O initial state is configured in the .ioc of the
 * project (and likewise in other MCU).
 * However, I do not know how pin states move across sleep states of a MCU, and this
 * is to be sure and make things more accessible. */
#define INIT_CONFIGURE_PIN_STATE				(1)
/************************************************************************************/
/* If INIT_SAVE_TO_MEMORY is enabled, the sys_savetomemory .c/.h files need to be edited.
 * In these files, we save the status of the system before shutting off or going to sleep.
 * This status is checked when the system wakes up. */
#define INIT_SAVE_TO_MEMORY						(1)
/************************************************************************************/
/* If INIT_SYS_HAS_BATTERY is enabled, the sys_battery .c/.h files need to be edited.
 * needs to be edited. */
#define INIT_SYS_HAS_BATTERY					(1)
/************************************************************************************/
/* If INIT_SYS_USE_ENCYPTION is enabled, the interface_passDataToNextLayer will received
 * encrypted data. For more detail look into the buffer.h file. */
#define INIT_SYS_USE_ENCYPTION					(1)
/************************************************************************************/
/* If BUFFER_USE_EXTERNAL_FLASH is enabled, the data will be buffered in the external
 * flash memory. For more detail, look into the flashMemoryController.h */
#define INIT_SYS_BUFFER							(1)
/************************************************************************************/
/* If INIT_SYS_LOCK_UUID is enabled, the mcu will not boot unless that given UUID is
 * loaded from MCU hardware. In other words, the code is locked to a single MCU.
 * To check the current UUID, comment the UUID_LOCKED_TO line, then the MCU will not
 * boot, but will show the current UUID. */
#define INIT_SYS_LOCK_UUID						(1)
#if INIT_SYS_LOCK_UUID
#define UUID_LOCKED_TO			{0x52,0xFF,0x6D,0x06,0x48,0x65,0x84,0x49,0x12,0x22,0x19,0x67}
#endif /* INIT_SYS_LOCK_UUID */
/************************************************************************************/














































#endif /* INC_INC_PRJ_SYSTEMSETUP_H_ */
