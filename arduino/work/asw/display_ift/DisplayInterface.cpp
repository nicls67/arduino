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
#include <util/delay.h>

#include "DisplayInterface.h"

DisplayInterface::DisplayInterface()
{
	/* Instantiate new LCD driver and attach it to BSW structure */
	if (BSW_cnf_struct.p_lcd == 0)
	{
		BSW_cnf_struct.p_lcd = new LCD(&LCD_init_cnf);
	}
	p_lcd = BSW_cnf_struct.p_lcd;

	/* Test */
	p_lcd->command(LCD_CMD_CLEAR_DISPLAY);
	_delay_us(500000);
	DisplayFullLine((uint8_t*)"toto", 4, 0);
	_delay_us(500000);
	DisplayFullLine((uint8_t*)"azerty", 6, 1);
	_delay_us(500000);
	DisplayFullLine((uint8_t*)"nbvcxwmlkjhftyjbvbffcdvhjhkknhytfhhjukjnbhgg", 30, 2);
	_delay_us(500000);
	DisplayFullLine((uint8_t*)"SALUT", 8, 3);
	_delay_us(500000);

}

void DisplayInterface::DisplayFullLine(uint8_t* str, uint8_t size, uint8_t line)
{
	uint8_t ram_addr;
	uint8_t i;

	/* Size is limited to the screen size for now */
	if (size > LCD_SIZE_NB_CHAR_PER_LINE)
		size = LCD_SIZE_NB_CHAR_PER_LINE;

	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return;

	/* Find DDRAM address of the start of the requested line */
	ram_addr = FindFirstCharAddr(line);

	/* Write each character on the screen */
	for (i=0; i<size; i++)
	{
		p_lcd->SetDDRAMAddress(ram_addr);
		p_lcd->WriteInRam(str[i], LCD_DATA_DDRAM);
		ram_addr++;
	}

}

uint8_t DisplayInterface::FindFirstCharAddr(uint8_t line)
{
	uint8_t start_addr = 0;
	uint8_t ram_offset;

	/* Parameter line is not checked against limits, it's assumed that the caller
	 * function has already checked the value */

	/* If we are in 1-line mode */
	if (p_lcd->GetLineNumberCnf() == LCD_CNF_ONE_LINE)
	{
		/* TODO : 1-line mode not managed for now */
	}
	else
	{
		/* First find in which line bank we are,
		 * calculate DDRAM offset and update line number to make the bank transparent */
		if ((line % 2) == 0)
		{
			/* We are in the first bank */
			ram_offset = LCD_RAM_2_LINES_MIN_1;
		}
		else
		{
			/* We are in the second bank */
			ram_offset = LCD_RAM_2_LINES_MIN_2;
		}

		line /= 2;

		/* Now we consider the RAM address goes from 0 to LCD_SIZE_NB_CHAR_PER_LINE * LCD_SIZE_NB_LINES / 2
		 * The start address is the product of the line number and the size of a line */
		start_addr = line * LCD_SIZE_NB_CHAR_PER_LINE;
		start_addr += ram_offset;
	}

	return start_addr;
}
