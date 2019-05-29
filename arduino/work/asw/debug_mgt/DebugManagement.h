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

#define PERIOD_MS_TASK_DISPLAY_DEBUG_DATA  5000 /*!< Period for displaying temperature and humidity data */
#define PERIOD_MS_TASK_DISPLAY_CPU_LOAD 5000 /*!< Period for displaying CPU load data */

/*!
 * @brief Defines the debug states
 */
typedef enum
{
	MAIN_MENU, /*!< Init state : main menu is displayed */
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
	 * @brief Displays periodic data on usart link
	 * @details This task displays the menu and periodic data (temperature, humidity and CPU load) on usart screen. It only calls the function DisplayData.
	 * @return Nothing
	 */
	static void DisplayPeriodicData_task();

	/*!
	 * @brief Displays data on usart link
	 * @details This task displays the menu and periodic data (temperature, humidity and CPU load) on usart screen.
	 * @return Nothing
	 */
	void DisplayData();

	/*!
	 *  @brief Management of debug mode
	 *  @details This function manages the debug menu according to the following state machine :
	 *  		 	  - MAIN_MENU state : handles user choice in main menu and selects next state\n
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

	/*!
	 * @brief Menu string get function
	 * @details This function returns the pointer to the menu string to display
	 * @return Menu string pointer
	 */
	inline uint8_t* getMenuStringPtr()
	{
		return menu_string_ptr;
	}

	/*!
	 * @brief Info string get function
	 * @details This function returns the pointer to the info string to display
	 * @return Info string pointer
	 */
	inline uint8_t* getInfoStringPtr()
	{
		return info_string_ptr;
	}

	/*!
	 * @brief Info message setting function
	 * @details This functions sets the info message pointer to the given string address
	 * @param [in] addr String address
	 * @return Nothing
	 */
	inline void setInfoStringPtr(uint8_t* addr)
	{
		info_string_ptr = addr;
	}

private:

	DebugInterface* debug_ift_ptr; /*!< Pointer to the debug interface object, which is used to send data on usart link */
	uint8_t* menu_string_ptr; /*!< Pointer to the current menu string to display */
	uint8_t* info_string_ptr; /*!< Pointer to the info message to display */
	debug_state_t menu_state; /*!< Current debug state */

};

#endif /* WORK_ASW_DEBUG_MGT_DEBUGMANAGEMENT_H_ */
