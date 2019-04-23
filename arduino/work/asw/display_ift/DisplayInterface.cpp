/*!
 * @file DisplayInterface.cpp
 *
 * @brief Source code file for display services
 *
 * @date 23 avr. 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "DisplayInterface.h"

DisplayInterface::DisplayInterface()
{
	uint8_t i;
	uint8_t data = 65;

	/* Instantiate new LCD driver and attach it to BSW structure */
	p_lcd = new LCD(&LCD_init_cnf);
	BSW_cnf_struct.p_lcd = p_lcd;

	/* Test */
	for (i=0; i<80; i++)
	{
		p_lcd->WriteInRam(data, LCD_DATA_DDRAM);
		data++;
	}
}

