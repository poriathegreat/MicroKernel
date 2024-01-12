/*
 * debug.c
 *
 *  Created on: Dec 31, 2023
 *      Author: PoriaTheGreat
 */
/************************************************************************************/
/*                               Included Libraries                                 */
/************************************************************************************/
/* Include the system */
#include "sys.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


extern UART_HandleTypeDef DEBUG_UART_HANDLE;


/* This is the print function. It can be used as a stand alone function. It
 * will simply print without the need of using the sprintf function
 * for formating. */
void debugprintf(const char *fmt, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
#ifdef DEBUGGING
	HAL_UART_Transmit(&DEBUG_UART_HANDLE, (uint8_t*)buffer, len, -1);
#endif /* DEBUGGING */


#ifdef LCD_DEBUG_PRINTF
	char* checkPointer = strstr((const char*)buffer, (const char*)"]	");
	if(checkPointer == 0){	checkPointer = strstr((const char*)buffer, (const char*)":	");}
	checkPointer = checkPointer + 2;
	if(len > 17)	len=17;
	for(uint8_t lcd_i=0; lcd_i<len; lcd_i++){
		lcd_writeCharacter((0+lcd_i)*8, 6, au8Font8x8 + (8*(checkPointer[lcd_i]-32)));
	}
#endif

}


/* The debug of each system part should be within a maximum range.
 * The screen can have 4 parts, 0 - 20 - 40 - 60 each of 20 characters.
 * Once part can overlap into the next. Parts are erased according to characters in use.
 * The debug screen should have X lines for each subsystem. Use as needed. */
void console_printf(uint8_t line, uint8_t part, const char *fmt, ...){
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
	char buff[15] = {0};
	sprintf(buff, "\e[%d;%dH", line, (part*20) + 2);
	HAL_UART_Transmit(&DEBUG_UART_HANDLE, (uint8_t*)buff, strlen(buff), -1);
	HAL_UART_Transmit(&DEBUG_UART_HANDLE, (uint8_t*)buffer, len, -1);
}

/* This function is to clear a specific console section of a specific line */
void console_clearSection(uint8_t line, uint8_t part){
	char buff[30] = {0};
	sprintf(buff, "\e[%d;%dH                    ", line, (part*20) + 2);
	HAL_UART_Transmit(&DEBUG_UART_HANDLE, (uint8_t*)buff, strlen(buff), -1);
}

/* This function is to clear a specific console section of a specific line */
void console_clearLine(uint8_t line){
	char buff[90] = {0};
	sprintf(buff, "\e[%d;2H                                                                                ", line);
	HAL_UART_Transmit(&DEBUG_UART_HANDLE, (uint8_t*)buff, strlen(buff), -1);
}


/* Use this function when initializing the each part of the system.
 * You provide a title and how many lines are needed for your debug space.
 * It will return a line.
 * It will also create the outline of the debug space in the terminal. */
uint8_t console_linesTakenSoFar = 0;
uint8_t console_requestSpace(uint8_t neededSpace, char* title){
	uint8_t consoleLine = RESET;

	consoleLine = console_linesTakenSoFar+1;
	console_linesTakenSoFar = consoleLine + neededSpace + 1;

	debugprintf("\e[%02d;1H**********************************************************************************", consoleLine);
	debugprintf("\e[%02d;%02dH %s ", consoleLine, ( (82 - (strlen((const char*)title) + 2 )) / 2)  , title);
	debugprintf("\e[%02d;1H**********************************************************************************", console_linesTakenSoFar);

	for(uint8_t i = 0; i < neededSpace; i++){
		debugprintf( "\e[%02d;1H*", consoleLine + i + 1 );
		debugprintf( "\e[%02d;82H*", consoleLine + i + 1 );
	}

	return consoleLine + 1;
}

#ifdef LCD_DEBUG
void lcd_debugprintf(const char *fmt, ...) {

	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
	if(len > 17)	len=17;
	for(uint8_t lcd_i=0; lcd_i<len; lcd_i++){
		lcd_writeCharacter((0+lcd_i)*8, 7, au8Font8x8 + (8*(buffer[lcd_i]-32)));
	}
}
#endif







