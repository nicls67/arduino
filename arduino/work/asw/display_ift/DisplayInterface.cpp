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
#include "../../scheduler/scheduler.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../debug/debug.h"
#include "../TempSensor/TempSensor.h"
#include "../display_ift/DisplayInterface.h"
#include "../display_mgt/DisplayManagement.h"
#include "../keepAliveLed/keepAliveLed.h"

#include "../asw.h"

#include "DisplayInterface.h"

DisplayInterface::DisplayInterface()
{
	uint8_t i;

	dummy = 0;

	/* Instantiate new LCD driver and attach it to BSW structure */
	if (BSW_cnf_struct.p_lcd == 0)
	{
		BSW_cnf_struct.p_lcd = new LCD(&LCD_init_cnf);
	}
	p_lcd = BSW_cnf_struct.p_lcd;

	/* All lines are empty at startup (LCD driver has cleared the screen) */
	for (i=0; i<LCD_SIZE_NB_LINES; i++)
		lineEmptyTab[i] = true;

	/* Linked list is not created yet */
	LL_shift_data_ptr = 0;

}

bool DisplayInterface::DisplayFullLine(uint8_t* str, uint8_t size, uint8_t line, T_DisplayInterface_LineDisplayMode mode)
{
	uint8_t i;
	T_Display_shift_data* LLElement = 0;

	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return false;

	/* If all characters can be displayed on one line, switch mode to NORMAL */
	if (size <= LCD_SIZE_NB_CHAR_PER_LINE)
		mode = NORMAL;
	/* If there are too many characters and mode is NORMAL, switch mode to LINE_SHIFT */
	else if ((size > LCD_SIZE_NB_CHAR_PER_LINE) && (mode == NORMAL))
		mode = LINE_SHIFT;

	/* Find DDRAM address of the start of the requested line */
	p_lcd->SetDDRAMAddress(FindFirstCharAddr(line));

	switch (mode)
	{
	case NORMAL:
		/* Write each character on the screen */
		for (i=0; i<size; i++)
		{
			p_lcd->WriteInRam(str[i], LCD_DATA_DDRAM);
		}
		lineEmptyTab[line] = false;
		break;

	case LINE_SHIFT:
		/* First write the line in normal mode */
		DisplayFullLine(str, LCD_SIZE_NB_CHAR_PER_LINE, line, NORMAL);

		/* Create the linked list for shifting data if it doesn't exist */
		if (LL_shift_data_ptr == 0)
			LL_shift_data_ptr = new LinkedList();

		/* Create a new element in the linked list for this line */
		LLElement = new T_Display_shift_data;
		LLElement->line = line;
		LLElement->size = size;
		LLElement->str_start_ptr = str;
		LLElement->str_cur_ptr = str;
		LLElement->temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;
		LL_shift_data_ptr->AttachNewElement((void*)LLElement);

		/* Then add periodic task to scheduler to shift display */
		p_scheduler->addPeriodicTask((TaskPtr_t)(&DisplayInterface::shiftLine_task), DISPLAY_LINE_SHIFT_PERIOD_MS);
		break;

	case GO_TO_NEXT_LINE:
		size -= LCD_SIZE_NB_CHAR_PER_LINE;

		/* Write each character on the screen */
		for (i=0; i<LCD_SIZE_NB_CHAR_PER_LINE; i++)
		{
			p_lcd->WriteInRam(str[i], LCD_DATA_DDRAM);
		}
		lineEmptyTab[line] = false;

		/* Call the function recursively to display the remaining characters on the next line */
		DisplayFullLine(&str[LCD_SIZE_NB_CHAR_PER_LINE], size, line + 1, GO_TO_NEXT_LINE);
		break;
	}

	return true;
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

	/* Remove the line from the linked list, if this line was not a shifted line,
	 * we don't care, the function will only return false */
	dummy = LL_shift_data_ptr->RemoveElement((CompareFctPtr_t)&DisplayInterface::LLElementCompare, (void*)&line);

	/* Check if there is still some lines to shift, if no, remove the periodic task */
	if(LL_shift_data_ptr->IsLLEmpty())
		dummy = p_scheduler->removePeriodicTask((TaskPtr_t)(&DisplayInterface::shiftLine_task));

	/* Find DDRAM address of the start of the requested line */
	p_lcd->SetDDRAMAddress(FindFirstCharAddr(line));

	/* Set data to ' ' (space character) in DDRAM */
	for (i=0; i<LCD_SIZE_NB_CHAR_PER_LINE; i++)
	{
		p_lcd->WriteInRam(' ', LCD_DATA_DDRAM);
	}

	lineEmptyTab[line] = true;

	return true;
}

bool DisplayInterface::IsLineEmpty(uint8_t line)
{
	/* Check that the line number is correct, if it's incorrect, exit the function */
	if (line >= LCD_SIZE_NB_LINES)
		return true;

	return lineEmptyTab[line];
}

bool DisplayInterface::LLElementCompare(void* LLElement, void* CompareElement)
{
	T_Display_shift_data* display_shift_data_ptr = (T_Display_shift_data*)LLElement;
	uint8_t line = *((uint8_t*)CompareElement);

	if(display_shift_data_ptr->line == line)
		return true;
	else
		return false;
}

void DisplayInterface::shiftLine_task()
{
	LinkedList* LL_ptr = ASW_cnf_struct.p_DisplayInterface->getLLShiftDataPtr();
	T_Display_shift_data* data_ptr;

	/* Go the the beginning of the chain */
	LL_ptr->ResetElementPtr();

	/* Process all lines of the screen */
	do
	{
		/* Update data structure */
		data_ptr = (T_Display_shift_data*)LL_ptr->getCurrentElement();

		/* Increment pointer and if we are at the end of the line, go back at the beginning */
		if (data_ptr->str_cur_ptr >= (data_ptr->str_start_ptr + data_ptr->size - LCD_SIZE_NB_CHAR_PER_LINE))
		{
			if(data_ptr->temporization == 0)
			{
				data_ptr->str_cur_ptr = data_ptr->str_start_ptr;
				data_ptr->temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;
			}
			else
				data_ptr->temporization--;
		}
		else if(data_ptr->str_cur_ptr == data_ptr->str_start_ptr)
		{
			if(data_ptr->temporization == 0)
			{
				data_ptr->str_cur_ptr ++;
				data_ptr->temporization = DISPLAY_LINE_SHIFT_TEMPO_TIME;
			}
			else
				data_ptr->temporization--;
		}
		else
			data_ptr->str_cur_ptr ++;

		/* Display the line */
		ASW_cnf_struct.p_DisplayInterface->DisplayFullLine(data_ptr->str_cur_ptr, LCD_SIZE_NB_CHAR_PER_LINE, data_ptr->line, NORMAL);
	}
	while(LL_ptr->MoveToNextElement());

}
