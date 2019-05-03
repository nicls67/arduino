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

/* Inclusion of all ASW header files*/
#include "debug/debug.h"
#include "keepAliveLed/keepAliveLed.h"
#include "TempSensor/TempSensor.h"
#include "display_ift/DisplayInterface.h"
#include "display_mgt/DisplayManagement.h"

/* TODO : remove inclusions in header files: all inclusions shall be done in source files */

/*!
 * @brief ASW configuration structure
 * @details This structure contains all pointers to instanced applicative objects
 */
typedef struct
{
	UsartDebug* p_usartDebug; /*!< Pointer to USART debug object */
	keepAliveLed* p_keepAliveLed; /*!< Pointer to keepAliveLed object */
	TempSensor* p_TempSensor; /*!< Pointer to TempSensor object */
	DisplayInterface* p_DisplayInterface; /*!< Pointer to DisplayInterface object */
	DisplayManagement* p_DisplayManagement; /*!< Pointer to DisplayManagement object */
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
