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
	WDG_MENU,  /*!< Watchdog state : watchdog menu is displayed */
}
debug_mgt_main_menu_state_t;

/*!
 * @brief Defines possible states for watchdog management
 */
typedef enum
{
	WDG_MAIN, /*!< Main menu of watchdog management */
	WDG_TMO_UPDATE, /*!< Timeout update mode */
}
debug_mgt_wdg_state_t;

/*!
 * @brief Structure containing all debug states
 */
typedef struct
{
	debug_mgt_main_menu_state_t main_state; /*!< Current main menu state */
	debug_mgt_wdg_state_t wdg_state; /*!< Current state of watchdog management */
}
debug_mgt_state_struct_t;

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
	 *				  - WDG_MENU state : handles user choice in watchdog menu and selects next state\n
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
	inline String* getInfoStringPtr()
	{
		return info_string_ptr;
	}

	/*!
	 * @brief Info message setting function
	 * @details This functions sets the info message pointer to the given string address
	 * @param [in] addr String address
	 * @return Nothing
	 */
	inline void setInfoStringPtr(String* addr)
	{
		info_string_ptr = addr;
	}

private:

	DebugInterface* debug_ift_ptr; /*!< Pointer to the debug interface object, which is used to send data on usart link */
	TempSensor* tempSensor_ptr; /*!< Pointer to the temperature sensor object */
	HumSensor* humSensor_ptr; /*!< Pointer to the humidity sensor object */
	uint8_t* menu_string_ptr; /*!< Pointer to the current menu string to display */
	String* info_string_ptr; /*!< Pointer to the info message to display */
	debug_mgt_state_struct_t debug_state; /*!< Structure containing debug states for each menu */
	bool isInfoStringDisplayed; /*!< Value defining if the info string has been already displayed one complete cycle of not */


	/*!
	 * @brief Debug menu exit function
	 * @details This function prepares the exit of debug menu. It writes the message "Bye !" on the screen and removes the periodic task from the scheduler.
	 *
	 * @return Nothing.
	 */
	void exitDebugMenu();

	/*!
	 * @brief System reset function
	 * @details This function provokes a reset of the system. It displays a message on the screen and calls the reset function from watchdog class.
	 *
	 * @return Nothing.
	 */
	void systemReset();

	/*!
	 * @brief Watchdog menu management function
	 * @details This function manages the watchdog menu. It handles the character received on USART bus and execute the requested action.
	 * 			It also manages the display of the watchdog menu.
	 *
	 * @param [in] rcv_char Character received on USART bus.
	 * @return Nothing.
	 */
	void WatchdogMenuManagement(uint8_t rcv_char);

	/*!
	 * @brief Main menu management
	 * @details This function manages the main debug menu. It handles the character received on USART bus and execute the requested action.
	 * 			It also manages the display of the main menu.
	 *
	 * @param [in] rcv_char Character received on USART bus.
	 * @return True if the debug mode shall be exited, false otherwise.
	 */
	bool MainMenuManagement(uint8_t rcv_char);
};

extern DebugManagement* p_global_ASW_DebugManagement; /*!< Pointer to the DebugManagement object */

#endif /* WORK_ASW_DEBUG_MGT_DEBUGMANAGEMENT_H_ */
