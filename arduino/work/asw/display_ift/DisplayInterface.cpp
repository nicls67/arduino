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

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/string/String.h"
#include "../../scheduler/scheduler.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../debug_ift/DebugInterface.h"
#include "../debug_mgt/DebugManagement.h"
#include "../TempSensor/TempSensor.h"
#include "../display_ift/DisplayInterface.h"
#include "../display_mgt/DisplayManagement.h"
#include "../keepAliveLed/keepAliveLed.h"
#include "../asw.h"

#include "DisplayInterface.h"



DisplayInterface::DisplayInterface(const T_LCD_conf_struct * LCD_init_cnf)
{
	uint8_t i;

	dummy = 0;

	/* Instantiate new LCD driver and attach it to BSW structure */
	if (BSW_cnf_struct.p_lcd == 0)
	{
		BSW_cnf_struct.p_lcd = new LCD(LCD_init_cnf);
	}
	p_lcd = BSW_cnf_struct.p_lcd;

	/* Initialize display data */
	for(i = 0; i < LCD_SIZE_NB_LINES; i++)
	{
		display_data[i].mode = NORMAL;
		display_data[i].isEmpty = true;

		ClearStringInDataStruct(i);

		display_data[i].shift_data.str_cur_ptr = 0;
		display_data[i].shift_data.str_ptr = 0;
		display_data[i].shift_data.temporization = 0;

	}

	isShiftInProgress = false;

}

bool DisplayInterface::DisplayFullLine(uint8_t* str, uint8_t size, uint8_t line, T_DisplayInterface_LineDisplayMode mode, bool isCallRecursive)
{
	uint8_t i;

	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return false;

	/* If all characters can be displayed on one line, switch mode to NORMAL */
	if (size <= LCD_SIZE_NB_CHAR_PER_LINE)
		mode = NORMAL;
	/* If there are too many characters and mode is NORMAL, switch mode to LINE_SHIFT */
	else if ((size > LCD_SIZE_NB_CHAR_PER_LINE) && (mode == NORMAL))
		mode = LINE_SHIFT;

	/* If the requested mode is LINE SHIFT and the line is already in LINE SHIFT mode,
	 * do not execute the function completely : only the string in shift data structure shall be updated */
	if((mode == LINE_SHIFT) && (display_data[line].mode == LINE_SHIFT))
	{
		display_data[line].shift_data.str_ptr->Clear();
		display_data[line].shift_data.str_ptr->appendString(str);

		/* The address of the string may have changed, replace pointer inside the range */
		if(display_data[line].shift_data.str_cur_ptr > (display_data[line].shift_data.str_ptr->getString() + display_data[line].shift_data.str_ptr->getSize() - LCD_SIZE_NB_CHAR_PER_LINE))
			display_data[line].shift_data.str_cur_ptr = display_data[line].shift_data.str_ptr->getString() + display_data[line].shift_data.str_ptr->getSize();
		else if (display_data[line].shift_data.str_cur_ptr < display_data[line].shift_data.str_ptr->getString())
			display_data[line].shift_data.str_cur_ptr = display_data[line].shift_data.str_ptr->getString();

		return true;
	}

	/* If the function is called from outside the class, update display data */
	if(isCallRecursive == false)
	{
		/* Clear the line to avoid issues in case a line shift is in progress on this line */
		ClearLine(line);

		/* Update data */
		display_data[line].mode = mode;
		display_data[line].isEmpty = false;
	}

	switch (mode)
	{
	case NORMAL:
	default:
		/* Clear data structure */
		ClearStringInDataStruct(line);

		/* Write each character in data structure */
		for (i=0; i<size; i++)
			display_data[line].display_str[i] = str[i];

		/* Refresh line */
		RefreshLine(line);
		break;

	case LINE_SHIFT:
		/* First write the line in normal mode */
		DisplayFullLine(str, LCD_SIZE_NB_CHAR_PER_LINE, line, NORMAL, true);

		/* Update shift data structure */
		display_data[line].shift_data.str_ptr = new String((const uint8_t*)str);
		display_data[line].shift_data.str_cur_ptr = display_data[line].shift_data.str_ptr->getString();
		display_data[line].shift_data.temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;

		/* If no shift is in progress on another line, add periodic task to scheduler */
		if(isShiftInProgress == false)
		{
			p_scheduler->addPeriodicTask((TaskPtr_t)(&DisplayInterface::shiftLine_task), DISPLAY_LINE_SHIFT_PERIOD_MS);
			isShiftInProgress = true;
		}

		break;

	case GO_TO_NEXT_LINE:
		/* Compute size for the overlapped line */
		size -= LCD_SIZE_NB_CHAR_PER_LINE;

		/* Write each character in data structure */
		for (i=0; i<LCD_SIZE_NB_CHAR_PER_LINE; i++)
			display_data[line].display_str[i] = str[i];

		/* Refresh line */
		RefreshLine(line);

		/* Call the function recursively to display the remaining characters on the next line */
		DisplayFullLine(&str[LCD_SIZE_NB_CHAR_PER_LINE], size, line + 1, GO_TO_NEXT_LINE, true);
		break;
	}

	return true;
}

void DisplayInterface::RefreshLine(uint8_t line)
{
	uint8_t i;

	/* Find DDRAM address of the start of the requested line */
	p_lcd->SetDDRAMAddress(FindFirstCharAddr(line));

	/* Write each character on the screen */
	for (i=0; i<LCD_SIZE_NB_CHAR_PER_LINE; i++)
		p_lcd->WriteInRam(display_data[line].display_str[i], LCD_DATA_DDRAM);

}

void DisplayInterface::ClearStringInDataStruct(uint8_t line)
{
	uint8_t i;

	for (i=0; i<LCD_SIZE_NB_CHAR_PER_LINE; i++)
		display_data[line].display_str[i] = ' ';
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

bool DisplayInterface::ClearLine(uint8_t line)
{
	uint8_t i;
	bool dummy;

	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return false;

	/* Set line mode to NORMAL */
	display_data[line].mode = NORMAL;

	/* Check if there is still some lines to shift, if no, remove the periodic task */
	isShiftInProgress = false;

	for(i=0; i<LCD_SIZE_NB_LINES; i++)
	{
		if(display_data[i].mode == LINE_SHIFT)
		{
			isShiftInProgress = true;
			free(display_data[line].shift_data.str_ptr);
		}
	}

	if(isShiftInProgress == false)
		dummy = p_scheduler->removePeriodicTask((TaskPtr_t)(&DisplayInterface::shiftLine_task));

	/* Mark line as empty */
	display_data[line].isEmpty = true;

	/* Clear string in data structure and refresh display */
	ClearStringInDataStruct(line);
	RefreshLine(line);

	return true;
}

void DisplayInterface::ClearFullScreen()
{
	uint8_t i;

	for(i=0; i<LCD_SIZE_NB_LINES; i++)
		ClearLine(i);
}

bool DisplayInterface::IsLineEmpty(uint8_t line)
{
	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return true;

	return display_data[line].isEmpty;
}

void DisplayInterface::shiftLine_task()
{
	T_display_data* display_data_ptr = ASW_cnf_struct.p_DisplayInterface->getDisplayDataPtr();
	T_Display_shift_data* display_shift_data_ptr;
	uint8_t i;

	/* Process all lines of the screen */
	for(i=0; i<LCD_SIZE_NB_LINES; i++)
	{
		/* Check if the line shall be shifted */
		if(display_data_ptr[i].mode == LINE_SHIFT)
		{
			/* Update shift data pointer */
			display_shift_data_ptr = &(display_data_ptr[i].shift_data);

			/* Increment pointer and if we are at the end of the line, go back at the beginning */
			if (display_shift_data_ptr->str_cur_ptr >= (display_shift_data_ptr->str_ptr->getString() + display_shift_data_ptr->str_ptr->getSize() - LCD_SIZE_NB_CHAR_PER_LINE))
			{
				if(display_shift_data_ptr->temporization == 0)
				{
					display_shift_data_ptr->str_cur_ptr = display_shift_data_ptr->str_ptr->getString();
					display_shift_data_ptr->temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;
				}
				else
					display_shift_data_ptr->temporization--;
			}
			else if(display_shift_data_ptr->str_cur_ptr == display_shift_data_ptr->str_ptr->getString())
			{
				if(display_shift_data_ptr->temporization == 0)
				{
					display_shift_data_ptr->str_cur_ptr ++;
					display_shift_data_ptr->temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;
				}
				else
					display_shift_data_ptr->temporization--;
			}
			else
				display_shift_data_ptr->str_cur_ptr ++;

			/* Display the line */
			ASW_cnf_struct.p_DisplayInterface->DisplayFullLine(display_shift_data_ptr->str_cur_ptr, LCD_SIZE_NB_CHAR_PER_LINE, i, NORMAL, true);
		}

	}

}
