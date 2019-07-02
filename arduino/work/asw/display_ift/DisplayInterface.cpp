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

#include "../../lib/String/String.h"
#include "../../lib/LinkedList/LinkedList.h"

#include "../../scheduler/scheduler.h"

#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"

#include "DisplayInterface.h"


DisplayInterface* p_global_ASW_DisplayInterface;

DisplayInterface::DisplayInterface(const T_LCD_conf_struct * LCD_init_cnf)
{
	uint8_t i;

	dummy = 0;

	/* Instantiate new LCD driver and attach it to BSW structure */
	if (p_global_BSW_lcd == 0)
	{
		p_global_BSW_lcd = new LCD(LCD_init_cnf);
	}
	p_lcd = p_global_BSW_lcd;

	/* Initialize display data */
	for(i = 0; i < LCD_SIZE_NB_LINES; i++)
	{
		display_data[i].mode = NORMAL;
		display_data[i].isEmpty = true;
		display_data[i].alignment = LEFT;

		ClearStringInDataStruct(i);

		display_data[i].shift_data.str_cur_ptr = 0;
		display_data[i].shift_data.str_ptr = 0;
		display_data[i].shift_data.temporization = 0;

	}

	isShiftInProgress = false;

}

bool DisplayInterface::DisplayFullLine(uint8_t* str, uint8_t size, uint8_t line, T_DisplayInterface_LineDisplayMode mode, T_DisplayInterface_LineAlignment alignment)
{
	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return false;

	/* Size can't be zero */
	if(size == 0)
		return false;

	/* If all characters can be displayed on one line, switch mode to NORMAL */
	if (size <= LCD_SIZE_NB_CHAR_PER_LINE)
		mode = NORMAL;
	/* If there are too many characters and mode is NORMAL, switch mode to LINE_SHIFT */
	else if ((size > LCD_SIZE_NB_CHAR_PER_LINE) && (mode == NORMAL))
		mode = LINE_SHIFT;
	/* If next line mode is requested and we are on the last line, switch mode to line shift */
	else if((mode == GO_TO_NEXT_LINE) && (line == LCD_SIZE_NB_LINES - 1))
		mode = LINE_SHIFT;


	/* If the requested mode is LINE SHIFT and the line is already in LINE SHIFT mode,
	 * do not execute the function completely : only the string in shift data structure shall be updated */
	if((mode == LINE_SHIFT) && (display_data[line].mode == LINE_SHIFT))
	{
		display_data[line].shift_data.str_ptr->Clear();
		display_data[line].shift_data.str_ptr->appendString(str);

		/* The address of the string may have changed, replace pointer inside the range */
		if(display_data[line].shift_data.str_cur_ptr > (display_data[line].shift_data.str_ptr->getString() + display_data[line].shift_data.str_ptr->getSize() - LCD_SIZE_NB_CHAR_PER_LINE))
			display_data[line].shift_data.str_cur_ptr = display_data[line].shift_data.str_ptr->getString() + display_data[line].shift_data.str_ptr->getSize() - LCD_SIZE_NB_CHAR_PER_LINE;
		else if (display_data[line].shift_data.str_cur_ptr < display_data[line].shift_data.str_ptr->getString())
			display_data[line].shift_data.str_cur_ptr = display_data[line].shift_data.str_ptr->getString();

		return true;
	}

	/* Clear the line to avoid issues in case a line shift is in progress on this line */
	ClearLine(line);

	/* Update data */
	display_data[line].mode = mode;
	display_data[line].isEmpty = false;
	display_data[line].alignment = alignment;



	switch (mode)
	{
	case NORMAL:
	default:
		updateLineAndRefresh(str, size, line);
		break;

	case LINE_SHIFT:
		/* First write the first characters normally */
		updateLineAndRefresh(str, LCD_SIZE_NB_CHAR_PER_LINE, line);

		/* Update shift data structure */
		display_data[line].shift_data.str_ptr = new String((const uint8_t*)str);
		display_data[line].shift_data.str_cur_ptr = display_data[line].shift_data.str_ptr->getString();
		display_data[line].shift_data.temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;

		/* If no shift is in progress on another line, add periodic task to scheduler */
		if(isShiftInProgress == false)
		{
			p_global_scheduler->addPeriodicTask((TaskPtr_t)(&DisplayInterface::shiftLine_task), DISPLAY_LINE_SHIFT_PERIOD_MS);
			isShiftInProgress = true;
		}

		break;

	case GO_TO_NEXT_LINE:
		/* First write the first characters normally */
		updateLineAndRefresh(str, LCD_SIZE_NB_CHAR_PER_LINE, line);

		/* Call the function recursively to write the remaining characters on the next lines */
		DisplayFullLine(&str[LCD_SIZE_NB_CHAR_PER_LINE], size - LCD_SIZE_NB_CHAR_PER_LINE, line + 1, GO_TO_NEXT_LINE, alignment);
		break;
	}

	return true;
}

bool DisplayInterface::DisplayFullLine(String* str, uint8_t line, T_DisplayInterface_LineDisplayMode mode, T_DisplayInterface_LineAlignment alignment)
{
	return DisplayFullLine(str->getString(), str->getSize(), line, mode, alignment);
}

void DisplayInterface::updateLineAndRefresh(uint8_t* str, uint8_t size, uint8_t line)
{
	uint8_t i;

	/* Write each character in data structure */
	for (i=0; i<size; i++)
		display_data[line].display_str[i] = str[i];

	/* Update alignment */
	if(size < LCD_SIZE_NB_CHAR_PER_LINE)
		setLineAlignment(line);

	/* Refresh line */
	RefreshLine(line);
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
	bool dummy, isNextLineMode = false;

	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return false;

	if(display_data[line].mode == GO_TO_NEXT_LINE)
		isNextLineMode = true;

	/* Free shift data string */
	if(display_data[line].mode == LINE_SHIFT)
		free(display_data[line].shift_data.str_ptr);

	/* Set line mode to NORMAL */
	display_data[line].mode = NORMAL;

	/* Check if there is still some lines to shift, if no, remove the periodic task */
	isShiftInProgress = false;

	for(i=0; i<LCD_SIZE_NB_LINES; i++)
	{
		if(display_data[i].mode == LINE_SHIFT)
			isShiftInProgress = true;
	}

	if(isShiftInProgress == false)
		dummy = p_global_scheduler->removePeriodicTask((TaskPtr_t)(&DisplayInterface::shiftLine_task));

	/* Mark line as empty */
	display_data[line].isEmpty = true;

	/* Clear string in data structure and refresh display */
	ClearStringInDataStruct(line);
	RefreshLine(line);

	/* If the line was in next line mode, clear also the next line */
	if(isNextLineMode)
		ClearLine(line + 1);

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
	T_display_data* display_data_ptr = p_global_ASW_DisplayInterface->getDisplayDataPtr();
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
			p_global_ASW_DisplayInterface->updateLineAndRefresh(display_shift_data_ptr->str_cur_ptr, LCD_SIZE_NB_CHAR_PER_LINE, i);
		}

	}

}


void DisplayInterface::setLineAlignment(uint8_t line)
{
	uint8_t char_idx, new_str_idx, new_str_idx_mem, first_char_idx, last_char_idx, size;
	uint8_t tmp_str[LCD_SIZE_NB_CHAR_PER_LINE];

	uint8_t* str = display_data[line].display_str;

	switch(display_data[line].alignment)
	{
	case LEFT:
	default:
		/* Find the first character displayed */
		char_idx = 0;
		while((str[char_idx] == ' ') && (char_idx < LCD_SIZE_NB_CHAR_PER_LINE))
			char_idx++;

		/* No character have been found or the string is already aligned */
		if((char_idx >= LCD_SIZE_NB_CHAR_PER_LINE) || (char_idx == 0))
			return;

		/* Create the new string in a temporary table */
		for(new_str_idx = 0; new_str_idx < (LCD_SIZE_NB_CHAR_PER_LINE - char_idx); new_str_idx++)
			tmp_str[new_str_idx] = str[char_idx + new_str_idx];

		/* Complete the string with spaces */
		for(new_str_idx = (LCD_SIZE_NB_CHAR_PER_LINE - char_idx); new_str_idx < LCD_SIZE_NB_CHAR_PER_LINE; new_str_idx++)
			tmp_str[new_str_idx] = ' ';

		/* Copy the result in the displayed string */
		for(char_idx = 0; char_idx < LCD_SIZE_NB_CHAR_PER_LINE; char_idx++)
			str[char_idx] = tmp_str[char_idx];

		break;

	case CENTER:
		/* Find the first character displayed */
		first_char_idx = 0;
		while((str[first_char_idx] == ' ') && (first_char_idx < LCD_SIZE_NB_CHAR_PER_LINE))
			first_char_idx++;

		/* No character have been found */
		if(first_char_idx >= LCD_SIZE_NB_CHAR_PER_LINE)
			return;

		/* Find the last character displayed */
		last_char_idx = LCD_SIZE_NB_CHAR_PER_LINE - 1;
		while((str[last_char_idx] == ' ') && (last_char_idx >= first_char_idx))
			last_char_idx--;

		/* If all the line is written no alignment is needed */
		if ((first_char_idx == 0) && (last_char_idx == LCD_SIZE_NB_CHAR_PER_LINE - 1))
			return;

		/* Compute the number of displayed characters */
		size = last_char_idx - first_char_idx + 1;

		/* Compute the start position of the string */
		new_str_idx = (LCD_SIZE_NB_CHAR_PER_LINE - size) / 2;

		/* Fill the start of the string with spaces */
		for(char_idx = 0; char_idx < new_str_idx; char_idx++)
			tmp_str[char_idx] = ' ';

		/* Copy the string */
		for(char_idx = new_str_idx; char_idx <= new_str_idx + size; char_idx++)
			tmp_str[char_idx] = str[first_char_idx + char_idx - new_str_idx];

		/* Fill the end of the line with spaces */
		for(char_idx = new_str_idx + size + 1; char_idx < LCD_SIZE_NB_CHAR_PER_LINE; char_idx++)
			tmp_str[char_idx] = ' ';

		/* Copy the result in the displayed string */
		for(char_idx = 0; char_idx < LCD_SIZE_NB_CHAR_PER_LINE; char_idx++)
			str[char_idx] = tmp_str[char_idx];

		break;

	case RIGHT:
		/* Find the first character displayed */
		char_idx = 0;
		while((str[char_idx] == ' ') && (char_idx < LCD_SIZE_NB_CHAR_PER_LINE))
			char_idx++;

		/* No character have been found */
		if(char_idx >= LCD_SIZE_NB_CHAR_PER_LINE)
			return;
		/* Find the first space character after the text */
		else
		{
			while((str[char_idx] != ' ') && (char_idx < LCD_SIZE_NB_CHAR_PER_LINE))
				char_idx++;

			/* The text goes until the end of the screen, no alignment is needed */
			if(char_idx >= LCD_SIZE_NB_CHAR_PER_LINE)
				return;
		}

		/* Fill the start of the line with spaces */
		for(new_str_idx = 0; new_str_idx < (LCD_SIZE_NB_CHAR_PER_LINE - char_idx); new_str_idx++)
			tmp_str[new_str_idx] = ' ';

		new_str_idx_mem = new_str_idx;

		/* Copy the text in the temporary table */
		for(new_str_idx = (LCD_SIZE_NB_CHAR_PER_LINE - char_idx); new_str_idx < LCD_SIZE_NB_CHAR_PER_LINE; new_str_idx++)
			tmp_str[new_str_idx] = str[new_str_idx - new_str_idx_mem];

		/* Copy the result in the displayed string */
		for(char_idx = 0; char_idx < LCD_SIZE_NB_CHAR_PER_LINE; char_idx++)
			str[char_idx] = tmp_str[char_idx];


		break;
	}

}


void DisplayInterface::setLineAlignmentAndRefresh(uint8_t line, T_DisplayInterface_LineAlignment alignment)
{
	/* If the line is in line shift mode, do nothing
	 * Changes are made only if the new alignment is different than the current one
	 * If the line is empty, do nothing */
	if((display_data[line].mode != LINE_SHIFT)
			&& (alignment != display_data[line].alignment)
			&& (display_data[line].isEmpty == false))
	{
		/* Update data structure */
		display_data[line].alignment = alignment;

		/* Update string and refresh display */
		setLineAlignment(line);
		RefreshLine(line);
	}
}
