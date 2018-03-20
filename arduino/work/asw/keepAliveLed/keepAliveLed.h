/*!
 * @file keepAliveLed.h
 *
 * @brief Class keepAliveLed header file
 *
 * @date 17 mars 2018
 * @author nicls67
 */

#ifndef WORK_ASW_KEEPALIVELED_KEEPALIVELED_H_
#define WORK_ASW_KEEPALIVELED_KEEPALIVELED_H_


#define PERIOD_MS_TASK_LED SW_PERIOD_MS /*!< Period for led blinking */

/*!
 * @brief Class for keep-alive LED blinking
 * @details This class defines all functions to make keep-alive LED blink
 */
class keepAliveLed {
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes the class keppAliveLed
	 *
	 * @return Nothing
	 */
	keepAliveLed();

	/*!
	 * @brief Task for LED blinking
	 * @details This function is inserted into the scheduler. It changes the state of the LED output to make it blink
	 *
	 * @return Nothing
	 */
	static void blinkLed_task();

private:

};

#endif /* WORK_ASW_KEEPALIVELED_KEEPALIVELED_H_ */
