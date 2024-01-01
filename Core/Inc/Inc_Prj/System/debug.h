#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"
/************************************************************************************/
/*                                 Exported Macros                                  */
/************************************************************************************/
/* Define the UART handle connected to debug console here. */
#define DEBUG_UART_HANDLE	huart2
/************************************************************************************/
/* Un-comment each part to enable or disable debug of the given part of the         */
/* system. WARNING: Un-commenting will effect overall size of the code!             */
/************************************************************************************/
/* Disabling debugging will effectively stop ALL debugging
 * This is used to disable debugging before production so that the end
 * user may not see sensitive information transmitted on the UART line.
 *
 * The rest of the system could use this macro as a preprocessor to keep the
 * debug lines from being compiled. */
#define DEBUGGING			(1)
/*****************************************************************************/
/* This is where the debug of each part of the system is controlled.
 * There should be TWO instances of each of these MACROS,
 * one here, all gathered in one place for easy access, and
 * one in the header file of the specific library for modular use.
 * (I personally enable all the needed debug from here.) */
#define DEBUGFLASH			(0)
#define DEBUGFCTR			(1)

/*****************************************************************************/
/* The deep debug is used to spit out low level data straight into the terminal
 * its not meant to be clean, its meant to be effective for short uses.
 * Any time a debugprintf() line is written in the code, it should be along with
 * this preprocessor and its specific library macro. */
#define DEEPDEBUG			(0)


/* Example usage: */
#if DEBUGGING
#if DEBUGFLASH
#if DEEPDEBUG
void deepDebugTestFunction (void) {debugprintf("Deep Debug Enabled!");};
#endif //DEEPDEBUG
#endif //DEBUGFLASH
#endif //DEBUGGING
/*****************************************************************************/
/*#####################################################################################*/
/*****************************************************************************/
/* The functions are MACROS needed to use this library.                      */
/*****************************************************************************/
/* Each line is split into 4 sections, each section 20 characters long.
 * Each section WILL overflow into the next section if above 20 character
 * is printed *this was done intentionally*.
 * The four parts are as follows: */
enum debug_console_parts{
	CONSOLE_PART_ONE,
	CONSOLE_PART_TWO,
	CONSOLE_PART_THREE,
	CONSOLE_PART_FOUR
};
/*****************************************************************************/
/* Use this function when initializing the each part of the system.
 * You provide a title and how many lines are needed for your debug space.
 * It will return a line.
 * It will also create the outline of the debug space in the terminal. */
uint8_t console_requestSpace(uint8_t neededSpace, char* title);
/*****************************************************************************/
/* Use this function to print into the console space you requested before.
 * The first parameter is the console line returned by the requestSpace
 * function.
 * The second parameter is one of the four CONSOLE_PART macros.
 * The last parameter is a simple print function with easy access formatting.*/
void console_printf(uint8_t line, uint8_t part, const char *fmt, ...);
/*****************************************************************************/
/*Use the following two functions to clear a line or a section of the console*/
void console_clearSection(uint8_t line, uint8_t part);
void console_clearLine(uint8_t line);
/*****************************************************************************/
/* This is the print function. It can be used as a stand alone function. It
 * will simply print without the need of using the sprintf function
 * for formating. */
void debugprintf(const char *fmt, ...);
/*****************************************************************************/
#endif /* INC_DEBUG_H_ */
