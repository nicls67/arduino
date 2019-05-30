/*!
 * @file LinkedList.c
 *
 * @brief Linked List library source file
 *
 * @date 27 avr. 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../string/String.h"
#include "../operators/operators.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../../asw/TempSensor/TempSensor.h"
#include "../../asw/debug_ift/DebugInterface.h"
#include "../../asw/debug_mgt/DebugManagement.h"
#include "../../asw/display_ift/DisplayInterface.h"
#include "../../asw/keepAliveLed/keepAliveLed.h"
#include "../../asw/display_mgt/DisplayManagement.h"

#include "../../asw/asw.h"

#include "LinkedList.h"

LinkedList::LinkedList()
{
	firstElement = 0;
	curElement_ptr = 0;
}

LinkedList::~LinkedList()
{
	T_LL_element* prev = firstElement;
	T_LL_element* cur = firstElement;

	/* Delete all elements of the list */
	if(IsLLEmpty() == false)
	{
		while(cur != 0)
		{
			free(prev->data_ptr);
			free(prev);
			prev = cur;
			cur = cur->nextElement;
		}
	}
}

void LinkedList::AttachNewElement(void* data_ptr)
{
	T_LL_element* new_element;
	T_LL_element* cur_element;

	/* Create a new element */
	new_element = new T_LL_element;
	new_element->data_ptr = data_ptr;
	new_element->nextElement = 0;

	cur_element = firstElement;

	if(cur_element == 0)
		firstElement = new_element;
	else
	{
		/* Find the last element of the chain */
		while(cur_element->nextElement != 0)
			cur_element = cur_element->nextElement;

		/* Link the new element to the chain */
		cur_element->nextElement = new_element;
	}
}

bool LinkedList::RemoveElement(CompareFctPtr_t comparisonFct_ptr, void* reference_ptr)
{
	T_LL_element* cur_element = firstElement;
	T_LL_element* prev_element = firstElement;
	bool ret_val;

	/* First find the element into the chain */
	while ((cur_element != 0) && (comparisonFct_ptr(cur_element->data_ptr, reference_ptr) == false))
	{
		prev_element = cur_element;
		cur_element = cur_element->nextElement;
	}

	/* If pointer is equal to 0, the requested element does not exist, exit the function */
	if (cur_element == 0)
		ret_val = false;
	else
	{
		/* The element exists, remove it from chain */
		if (cur_element == firstElement)
		{
			firstElement = cur_element->nextElement;

			/* If we try to delete the current element of the chain, update the value of the pointer */
			if(cur_element == curElement_ptr)
				curElement_ptr = firstElement;
		}
		else
		{
			prev_element->nextElement = cur_element->nextElement;

			/* If we try to delete the current element of the chain, update the value of the pointer */
			if(cur_element == curElement_ptr)
				curElement_ptr = prev_element;
		}

		delete cur_element;

		ret_val = true;
	}

	return ret_val;
}

bool LinkedList::MoveToNextElement()
{
	if (curElement_ptr->nextElement != 0)
	{
		curElement_ptr = curElement_ptr->nextElement;
		return true;
	}
	else
	{
		curElement_ptr = firstElement;
		return false;
	}
}

bool LinkedList::IsLLEmpty()
{
	if (firstElement == 0)
		return true;
	else
		return false;
}

bool LinkedList::FindElement(CompareFctPtr_t comparisonFct_ptr, void* reference_ptr, void** chainElement_ptr)
{
	T_LL_element* cur_element = firstElement;
	bool elementFound = false;

	/* First find the element into the chain */
	while ((cur_element != 0) && (comparisonFct_ptr(cur_element->data_ptr, reference_ptr) == false))
		cur_element = cur_element->nextElement;

	/* The element has been found into the chain */
	if(cur_element != 0)
	{
		*chainElement_ptr = cur_element->data_ptr;
		elementFound = true;
	}

	return elementFound;
}
