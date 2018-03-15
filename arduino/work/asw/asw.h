/*!
 * @file asw.h
 *
 * @brief ASW main header file
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#ifndef WORK_ASW_ASW_H_
#define WORK_ASW_ASW_H_

#include <avr/io.h>
#include <stdlib.h>
#include "../lib/operators.h"

/* Inclusion of all ASW header files */
#include "log/log.h"


/*!
 * @brief ASW configuration structure
 * @details This structure contains all pointers to instanced applicative objects
 */
typedef struct
{
	UsartDebug* p_usartDebug; /*!< Pointer to usart debug object */
}
T_ASW_cnf_struct;

extern T_ASW_cnf_struct ASW_cnf_struct; /*!< ASW configuration structure */


/*! @brief Initialization of ASW
 *  @details This function instantiates all applicative objects.
 *           The addresses of objects are then stored in ASW_cnf_struct structure.
 *           This function shall be called after BSW initialization function.
 *  @return Nothing
 */
void asw_init();

#endif /* WORK_ASW_ASW_H_ */
