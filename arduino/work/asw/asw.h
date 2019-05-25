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


/*!
 * @brief ASW configuration structure
 * @details This structure contains all pointers to instanced applicative objects
 */
typedef struct
{
	DebugInterface* p_DebugInterface; /*!< Pointer to USART debug interface object */
	keepAliveLed* p_keepAliveLed; /*!< Pointer to keepAliveLed object */
	TempSensor* p_TempSensor; /*!< Pointer to TempSensor object */
	DisplayInterface* p_DisplayInterface; /*!< Pointer to DisplayInterface object */
	DisplayManagement* p_DisplayManagement; /*!< Pointer to DisplayManagement object */
	DebugManagement* p_DebugManagement; /*!< Pointer to the DebugManagement object */
}
T_ASW_cnf_struct;

extern T_ASW_cnf_struct ASW_cnf_struct; /*!< ASW configuration structure */



/*! @brief Initialization of ASW
 *  @details This function instantiates all applicative objects. Some objects are not created by this function but directly by the upper-level class.
 *           The addresses of objects are then stored in ASW_cnf_struct structure.\n
 *           The debug interface obkect is created only if the debug pin is set to logical high level.\n
 *           This function shall be called after BSW initialization function.
 *  @return Nothing
 */
void asw_init();

#endif /* WORK_ASW_ASW_H_ */
