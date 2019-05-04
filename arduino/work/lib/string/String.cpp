/*!
 * @file String.cpp
 *
 * @brief String class source file
 *
 * @date 2 mai 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "String.h"

String::String(const uint8_t* str)
{
	uint8_t i;

	/* Compute size of the given string */
	size = ComputeStringSize((uint8_t*)str);

	/* Allocate memory for the string (add one more byte for the \0 character) */
	string = (uint8_t*)malloc((size + 1) * sizeof(uint8_t));

	/* Copy the input string */
	for(i=0; i <= size; i++)
	{
		string[i] = str[i];
	}

}

String::String()
{
	string = 0;
	size = 0;
}

uint8_t String::ComputeStringSize(uint8_t* str)
{
	uint8_t size = 0;

	while((*str != '\0') && (size < 256))
	{
		str++;
		size++;
	}

	return size;
}

void String::appendString(uint8_t* str)
{
	uint8_t new_size;
	uint8_t* new_string;
	uint8_t i;

	/* Compute size of the new string */
	new_size = ComputeStringSize(str);

	/* Allocate memory for the updated string (add one more byte for the \0 character) */
	new_string = (uint8_t*)malloc((size + new_size + 1) * sizeof(uint8_t));

	/* Copy the old string at the beginning of the updated string */
	if(size > 0)
	{
		for(i=0; i < size; i++)
		{
			new_string[i] = string[i];
		}
	}

	/* Copy the new string after the old one */
	for(i=0; i <= new_size; i++)
	{
		new_string[size + i] = str[i];
	}

	/* Delete old string */
	if(size > 0)
		free(string);

	/* Switch pointer */
	string = new_string;

	/* Update string size */
	size += new_size;
}

void String::appendInteger(uint16_t value, uint8_t base)
{
	uint8_t* int_str;

	/* If the base in not between 2 and 36, 10 is used as default */
	if((base > 36) && (base < 2))
		base = 10;

	/* First convert the integer value into a chain of characters */
	int_str = (uint8_t*)malloc(17 * sizeof(uint8_t));
	int_str = (uint8_t*)itoa(value, (char*)int_str, base);

	/* Add the new characters to the string */
	appendString(int_str);

	free(int_str);
}

void String::Clear()
{
	if(size > 0)
		free(string);

	size = 0;
}
