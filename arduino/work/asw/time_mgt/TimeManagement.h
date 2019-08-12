/*!
 * @file TimeManagement.h
 *
 * @brief TimeManagement class header file
 *
 * @date 7 aout 2019
 * @author nicls67
 */

#ifndef WORK_ASW_TIME_MGT_TIMEMANAGEMENT_H_
#define WORK_ASW_TIME_MGT_TIMEMANAGEMENT_H_

#define PERIOD_TIME_COMPUTATION_TASK 500 /*!< Time computation task is called every 500 ms */

/*!
 * @brief Time memorization structure
 * @details This structure memorizes a time value : hour, minutes, seconds, hundredth of seconds.
 */
typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t centiSeconds;
}
T_TimeManagement_TimeStruct;

/*!
 * @brief Time management class.
 * @details This class manages time services (current time and date).
 */
class TimeManagement
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes the class. It creates a new Clock object if needed and adds periodic task in the scheduler.
	 *
	 * @return Nothing
	 */
	TimeManagement();

	/*!
	 * @brief Time management periodic task
	 * @details This function is called periodically by the scheduler. It calls the time computation function.
	 *
	 * @return Nothing.
	 */
	static void TimeComputation_task();

	/*!
	 * @brief Time computation function
	 * @details This function retrieves the clock counter value and resets it.
	 * 			The current time is computed according to this timer value. The lowest possible resolution is 10ms.
	 *
	 * @return Nothing.
	 */
	void UpdateCurrentTime();

	/*!
	 * @brief Current time get function
	 * @details This function returns the current time.
	 *
	 * @return Pointer to the current time structure.
	 */
	inline T_TimeManagement_TimeStruct* getCurrentTime()
	{
		return &current_time;
	}

	/*!
	 * @brief String formatting function
	 * @details This function formats the time value into a string with the given separator character.
	 * 			The display of hundredth of seconds is optional.
	 * 			For example if the separator is ':', the returned string will be "hh:mm:ss:cc".
	 *
	 * @param [out] str Pointer to the string to fill.
	 * @param [in] separator Separating char
	 * @param [in] isSecondsDisplayed Flag indicating of the seconds are displayed or not
	 * @param [in] isCentiDisplayed Flag indicating of the hundredth of seconds are displayed or not
	 *
	 * @return Nothing
	 */
	void FormatTimeString(String* str, uint8_t separator, bool isSecondsDisplayed = true, bool isCentiDisplayed = false);

private:
	T_TimeManagement_TimeStruct current_time; /*!< Current time */
};

extern TimeManagement* p_global_ASW_TimeManagement; /*!< Pointer to the TimeManagement object */

#endif /* WORK_ASW_TIME_MGT_TIMEMANAGEMENT_H_ */
