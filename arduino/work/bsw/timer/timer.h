/*!
 * @file timer.h
 *
 * @brief Timer class header file
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_TIMER_TIMER_H_
#define WORK_BSW_TIMER_TIMER_H_




/*!
 * @brief Class defining a timer
 * @details This class defines a timer/counter. The selected timer is configured in CTC mode and interrupts are enabled.
 * 			The prescaler value and CTC value can both be configured by user.
 *
 */
class timer
{
public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes class attributes
	 * @return Nothing
	 */
	timer();

	/*!
	 * @brief Configures Timer #1
	 * @details This function configures hardware timer #1 in CTC mode, enables its interrupts, sets prescaler to a_prescaler and CTC value to a_ctcValue
	 * @param [in] a_prescaler prescaler value
	 * @param [in] a_ctcValue Value to which the counter will compare before raising an interrupt
	 * @return Nothing
	 */
	void configureTimer1(uint16_t a_prescaler, uint16_t a_ctcValue);

	/*!
	 * @brief Start Timer #1
	 * @details This functions starts Timer #1. Timer shall be initialized before this function is called.
	 * @return Nothing
	 */
	void startTimer1();

	/*!
	 * @brief Stops Timer #1
	 * @details This functions stops  timer #1 by resetting bits 0-2 of TCCR1B
	 * @return Nothing
	 */
	void stopTimer1();

	/*!
	 * @brief Reads current value of timer #1
	 * @details This function reads the value of of timer #1 using register TCNT1. The function is inlined to speed up SW execution.
	 *
	 * @return Current timer value
	 */
	inline uint16_t getTimer1Value()
	{
		return TCNT1;
	}

private:
	uint8_t prescaler;

};


#endif /* WORK_BSW_TIMER_TIMER_H_ */
