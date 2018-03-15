/*!
 * @file operators.cpp
 *
 * @brief c++ operators definitions
 *
 * @date 14 mars 2018
 * @author nicls67
 */

#include <stdlib.h>
#include "operators.h"

void * operator new(size_t a_size)
{
  return malloc(a_size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

