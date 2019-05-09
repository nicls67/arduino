/*!
 * @file DebugManagement.h
 *
 * @brief Debug management class header file
 *
 * @date 8 mai 2019
 * @author nicls67
 */

#ifndef WORK_ASW_DEBUG_MGT_DEBUGMANAGEMENT_H_
#define WORK_ASW_DEBUG_MGT_DEBUGMANAGEMENT_H_

#define PERIOD_MS_TASK_DISPLAY_SENSORS  5000 /*!< Period for displaying temperature and humidity data */
#define PERIOD_MS_TASK_DISPLAY_CPU_LOAD 5000 /*!< Period for displaying CPU load data */

/*!
 * @brief Defines the debug states
 */
typedef enum
{
	INIT, /*!< Init state : main menu has been displayed, wait for a received character */
	DISPLAY_DATA, /*!< Display sensor data in continuous */
	DISPLAY_CPU_LOAD, /*!< Display CPU load in continuous */

}debug_state_t;

/*!
 * @brief Debug management class
 * @details This class manages the debug menu available on USART interface. It allows to display SW informations like sensors data, CPU load...
 */
class DebugManagement {

public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes the class. If needed, it creates a new instance of debug interface object.
	 *
	 * @return Nothing
	 */
	DebugManagement();

	/*!
	 * @brief Displays sensors data on usart link
	 * @details This task sends sensors data (temperature and humidity) on usart link every 5 seconds
	 * @return Nothing
	 */
	static void DisplaySensors_task();

	/*!
	 * @brief Displays CPU load data on usart link
	 * @details This task sends CPU load data (current and average load) on usart link every 5 seconds
	 * @return Nothing
	 */
	static void DisplayCPULoad_task();

	/*!
	 *  @brief Management of debug mode
	 *  @details This function manages the debug mode according to the following state machine :
	 *  		 	  - INIT state : handles user choice in main menu and selects next state\n
	 *  		 	  - DISPLAY_DATA state : display sensor data periodically\n
	 *  		 	  - DISPLAY CPU LOAD : display CPU load periodically\n\n
	 *
	 *  		 It is called each time a data is received on USART and debug mode is active.
	 *
	 *  @return True if the debug mode shall be closed, false otherwise
	 */
	bool DebugModeManagement();

	/*!
	 * @brief Interface pointer get function
	 * @details This function returns the pointer to the debug interface object
	 *
	 * @return Pointer to debug interface
	 */
	inline DebugInterface* getIftPtr()
	{
		return debug_ift_ptr;
	}

private:

	DebugInterface* debug_ift_ptr; /*!< Pointer to the debug interface object, which is used to send data on usart link */

	debug_state_t debug_state; /*!< Current debug state */
};

#endif /* WORK_ASW_DEBUG_MGT_DEBUGMANAGEMENT_H_ */
