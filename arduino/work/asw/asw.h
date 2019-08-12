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
 * @brief ASW initialization configuration structure
 * @details This structure is used to define which ASW services shall be started at SW start-up.
 */
typedef struct
{
	bool isDebugActivated; /*!< Debug services activation flag */
	bool isLEDActivated; /*!< Keep-alive LED activation flag */
	bool isSensorMgtActivated; /*!< Sensor activation */
	bool isDisplayActivated; /*!< LCD display activation flag */
	bool isTimeMgtActivated; /*!< Time management activation */
}
T_ASW_init_cnf;


/*! @brief Initialization of ASW
 *  @details This function instantiates all applicative objects. Some objects are not created by this function but directly by the upper-level class.
 *           The addresses of objects are then stored in ASW_cnf_struct structure.\n
 *           The debug interface obkect is created only if the debug pin is set to logical high level.\n
 *           This function shall be called after BSW initialization function.
 *  @return Nothing
 */
void asw_init();

#endif /* WORK_ASW_ASW_H_ */
