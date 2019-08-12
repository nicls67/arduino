/*!
 * @file Clock.h
 *
 * @brief Clock class header file
 *
 * @date 7 aout 2019
 * @author nicls67
 */

#ifndef WORK_BSW_CLOCK_CLOCK_H_
#define WORK_BSW_CLOCK_CLOCK_H_

#define CLOCK_INTERRUPT_PERIOD_MS 10 /*!< Period of the timer interrupt */
#define CLOCK_TIMER_PRESCALER_VALUE 64 /*!< Value of prescaler to use for timer */
#define CLOCK_TIMER_CTC_VALUE ((F_CPU/CLOCK_TIMER_PRESCALER_VALUE)/(1000/CLOCK_INTERRUPT_PERIOD_MS)) /*!< Compare value for periodic timer */

/*!
 * @brief Clock management class
 * @details This class defines the clock used for time management (actual time and date or chronometer).
 * 			It uses a timer to generate a periodic interrupt (usually 10ms) which will increment a counter used for time computation.
 * 			The class uses a different timer than the scheduler to be more precise and be independent from scheduler period updates.
 */
class Clock
{
public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes the class Clock.
	 * 			The timer used for periodic interrupt is configured and started.
	 *
	 * @return Nothing.
	 */
	Clock();

	/*!
	 * @brief Interrupt counter get function.
	 * @details This function returns the value of the interrupt counter.
	 *
	 * @return Counter value.
	 */
	inline uint16_t getCounterValue()
	{
		return counter;
	}

	/*!
	 * @brief Interrupt counter reset function
	 * @details This function resets the interrupt counter.
	 *
	 * @return Nothing
	 */
	inline void resetCounter()
	{
		counter = 0;
	}

	/*!
	 * @brief Interrupt counter increment function
	 * @details This function increments the interrupt counter by 1.
	 *
	 * @return Nothing.
	 */
	inline void incrementCounter()
	{
		counter++;
	}

private:
	uint16_t counter; /*!< Interrupt counter : is incremented each time the timer interrupt is raised */
};

extern Clock* p_global_BSW_Clock; /*!< Pointer to clock driver object */

#endif /* WORK_BSW_CLOCK_CLOCK_H_ */
