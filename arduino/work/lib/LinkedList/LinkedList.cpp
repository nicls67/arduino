/*!
 * @file LinkedList.c
 *
 * @brief Linked List library source file
 *
 * @date 27 avr. 2019
 * @author nicls67
 */

#include "LinkedList.h"

LinkedList::LinkedList()
{
	firstElement = 0;
	curElement_ptr = 0;
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

	/* First find the element into the chain */
	while ((cur_element != 0) && (comparisonFct_ptr(cur_element->data_ptr, reference_ptr) == false))
	{
		prev_element = cur_element;
		cur_element = cur_element->nextElement;
	}

	/* If pointer is equal to 0, the requested element does not exist, exit the function */
	if (cur_element == 0)
		return false;

	/* The element exists, remove it from chain */
	if (cur_element == firstElement)
		firstElement = cur_element->nextElement;
	else
		prev_element->nextElement = cur_element->nextElement;

	delete cur_element;

	return true;
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
