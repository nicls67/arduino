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
 * @brief Type definition for reset source
 * @details This enumeration defines all possible reset sources : JTAG reset, watchdog reset, brown-out reset, external reset and power-on reset.
 */
typedef enum
{
	JTAG_RESET_TYPE,
	WDG_RESET_TYPE,
	BROWN_OUT_RESET_TYPE,
	EXTERNAL_RESET_TYPE,
	POWER_ON_RESET_TYPE
}
T_WDG_Reset_type;

/*!
 * @brief Definition of available timeout values
 */
#define WDG_TMO_15MS WDTO_15MS /*!< Timeout value is 15 ms */
#define WDG_TMO_30MS WDTO_30MS /*!< Timeout value is 30 ms */
#define WDG_TMO_60MS WDTO_60MS /*!< Timeout value is 60 ms */
#define WDG_TMO_120MS WDTO_120MS /*!< Timeout value is 120 ms */
#define WDG_TMO_250MS WDTO_250MS /*!< Timeout value is 250 ms */
#define WDG_TMO_500MS WDTO_500MS /*!< Timeout value is 500 ms */
#define WDG_TMO_1S WDTO_1S /*!< Timeout value is 1 s */
#define WDG_TMO_2S WDTO_2S /*!< Timeout value is 2 s */
#define WDG_TMO_4S WDTO_4S /*!< Timeout value is 4 s */
#define WDG_TMO_8S WDTO_8S /*!< Timeout value is 8 s */

extern uint8_t mcusr_mirror __attribute__ ((section(".noinit")));
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

	/*!
	 * @brief Watchdog timeout get value.
	 * @details This function returns the current watchdog timeout value in ms. It has to convert the value of tmo_value into a numeric value of the timeout.
	 *
	 * @return Timeout value.
	 */
	uint16_t getTMOValue();

	/*!
	 * @brief Watchdog status function
	 * @details This function returns the current status of the watchdog : enabled or disabled.
	 *
	 * @return True if the watchdog is enabled, false otherwise.
	 */
	inline bool isEnabled()
	{
		return isActive;
	}

	/*!
	 * @brief Watchdog switching function
	 * @details This function switches the state of the watchdog. It it was enabled, the function disables the watchdog, and if it was disabled, the function enables it with the memorized timeout value.
	 * 			The function returns the new status of the watchdog.
	 *
	 * @return New status of the watchdog : True if enabled, false if disabled.
	 */
	bool SwitchWdg();

	/*!
	 * @brief Reset reason get function.
	 * @details This function returns the reason of the last reset.
	 *
	 * @return Reason of the last reset.
	 */
	inline T_WDG_Reset_type getResetReason()
	{
		return reset_reason;
	}

private:

	uint8_t tmo_value; /*!< Current timeout value */
	bool isActive; /*!< Watchdog activation flag */
	T_WDG_Reset_type reset_reason; /*!< Variable used to store the reason if the last reset */

	/*!
	 * @brief Watchdog disabling function
	 * @details This function disables the watchdog by calling wdt_disable macro.
	 *
	 * @return Nothing
	 */
	void disable();

	/*!
	 * @brief Watchdog enabling function
	 * @details This function enables the watchdog by calling wdt_enable macro.
	 *
	 * @param [in] value Timeout value
	 * @return Nothing
	 */
	void enable(uint8_t value);

	/*!
	 * @brief Reset reason reading function.
	 * @details This function reads the reason of the last reset in register \c MCUSR and clears the register. The reset reason is stored in variable \c reset_reason.
	 *
	 * @return Nothing
	 */
	void extractResetReason();
};

extern Watchdog* p_global_BSW_wdg; /*!< Pointer to Watchdog driver object */

#endif /* WORK_BSW_WDT_WATCHDOG_H_ */
