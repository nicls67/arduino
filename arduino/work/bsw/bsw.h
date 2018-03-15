/*!
 * @file bsw.h
 * @brief BSW main header file
 *
 * @date 13 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_BSW_H_
#define WORK_BSW_BSW_H_

#include <avr/io.h>
#include <stdlib.h>
#include "../lib/operators.h"

/* Inclusion of all BSW header files */
#include "usart/usart.h"
#include "dio/dio.h"

/*
 * Definition of all configurable constants of BSW
 */
#define USART_BAUDRATE (uint16_t)9600 /*!< usart connection to PC uses a baud rate of 9600 */

/*!
 * @brief BSW configuration structure
 * @details This structure contains all pointers to instanced drivers objects
 */
typedef struct
{
	usart* p_usart; /*!< Pointer to usart driver object */
	dio* p_dio; /*!< Pointer to dio driver object */
}
T_BSW_cnf_struct;

extern T_BSW_cnf_struct BSW_cnf_struct; /*!< BSW configuration structure */


/*! @brief Initialization of BSW
 *  @details This function instantiates all driver objects, leading hardware initialization.
 *           The addresses of driver objects are then stored in BSW_cnf_struct structure.
 *  @return Nothing
 */
void bsw_init();


#endif /* WORK_BSW_BSW_H_ */