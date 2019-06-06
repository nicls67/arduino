/*!
 * @file Watchdog.h
 *
 * @brief Class Watchdog header file
 *
 * @date 6 juin 2019
 * @author nicls67
 */

#ifndef WORK_BSW_WDT_WATCHDOG_H_
#define WORK_BSW_WDT_WATCHDOG_H_

/*!
 * @brief Watchdog management class
 * @details This class provides services to manage the watchdog HW module. The watchdog shall be reset periodically to avoid a hardware reset of the system.
 */
class Watchdog
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes the watchdog class. It enables the HW watchdog with a default timeout value.
	 *
	 * @return Nothing
	 */
	Watchdog();

	/*!
	 * @brief Overloaded class constructor
	 * @details This function initializes the watchdog class. It enables the HW watchdog with the given timeout value.
	 *
	 * @param [in] timeout Timeout value requested for the watchdof
	 * @return Nothing
	 */
	Watchdog(uint8_t timeout);

	/*!
	 * @brief Watchdog reset function
	 * @details This function resets the watchdog timer by calling wdt_reset macro
	 *
	 * @return Nothing
	 */
	void reset();

	/*!
	 * @brief Watchdog timeout value update function
	 * @details This function updates the timeout value of the watchdog. It disables then re-enables the watchdog.
	 *
	 * @param [in] value New timeout value
	 * @return Nothing
	 */
	void timeoutUpdate(uint8_t value);

	/*!
	 * @brief System reset function
	 * @details This function provokes a system reset by going in an infinite loop. Thus the watchdog will reset the CPU when the timeout occurs.
	 *
	 * @return Nothing
	 */
	void SystemReset();

private:

	/*!
	 * @brief Watchdog enabling function
	 * @details This function enables the watchdog by calling wdt_enable macro.
	 *
	 * @param [in] value Timeout value
	 * @return Nothing
	 */
	void enable(uint8_t value);

	/*!
	 * @brief Watchdog disabling function
	 * @details This function disables the watchdog by calling wdt_disable macro.
	 *
	 * @return Nothing
	 */
	void disable();
};

extern Watchdog* p_global_BSW_wdg; /*!< Pointer to Watchdog driver object */

#endif /* WORK_BSW_WDT_WATCHDOG_H_ */
