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

#include "../../asw/asw.h"
#include "../../bsw/bsw.h"

String::String(uint8_t* str)
{
	uint8_t i;

	/* Compute size of the given string */
	size = ComputeStringSize(str);

	/* Allocate memory for the string (add one more byte for the \0 character) */
	string = (uint8_t*)malloc((size + 1) * sizeof(uint8_t));

	/* Copy the input string */
	for(i=0; i <= size; i++)
	{
		string[i] = str[i];
	}

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

void String::append(uint8_t* str)
{
	uint8_t new_size;
	uint8_t* new_string;
	uint8_t i;

	/* Compute size of the new string */
	new_size = ComputeStringSize(str);

	/* Allocate memory for the updated string (add one more byte for the \0 character) */
	new_string = (uint8_t*)malloc((size + new_size + 1) * sizeof(uint8_t));

	/* Copy the old string at the beginning of the updated string */
	for(i=0; i < size; i++)
	{
		new_string[i] = string[i];
	}

	/* Copy the new string after the old one */
	for(i=0; i <= new_size; i++)
	{
		new_string[size + i] = str[i];
	}

	/* Delete old string */
	free(string);

	/* Switch pointer */
	string = new_string;

	/* Update string size */
	size += new_size;
}
