/*!
 * @file operators.h
 *
 * @brief c++ operators definitions header file
 *
 * @date 14 mars 2018
 * @author nicls67
 */

#ifndef WORK_LIB_OPERATORS_H_
#define WORK_LIB_OPERATORS_H_

/*!
 * @brief Operator new
 * @details Equivalent to malloc function in C
 *          Allocates a memory zone of size a_size
 *
 * @param [in] a_size memory size to allocate
 * @return Pointer to the start of allocated memory zone
 */
void * operator new(size_t a_size);

/*!
 * @brief Operator delete
 * @details Equivalent to free function in C
 *          Free the memory zone at address ptr
 *
 * @param [in] ptr Pointer to the start of memory zone to free
 * @return Nothing
 */
void operator delete(void * ptr);


#endif /* WORK_LIB_OPERATORS_H_ */
