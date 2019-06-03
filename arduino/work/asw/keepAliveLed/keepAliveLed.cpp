/*!
 * @file keepAliveLed.cpp
 *
 * @brief Definition of function for class keepAliveLed
 *
 * @date 17 mars 2018
 * @author nicls67
 */

#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"

#include "../../bsw/dio/dio.h"

#include "keepAliveLed.h"


keepAliveLed* p_global_ASW_keepAliveLed;

keepAliveLed::keepAliveLed()
{
	/* Add task blinLed_task to scheduler with period of 500ms */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&keepAliveLed::blinkLed_task), PERIOD_MS_TASK_LED);
}

void keepAliveLed::blinkLed_task()
{
	/* Call DIO driver to invert LED pin state */
	p_global_BSW_dio->dio_invertPort(LED_PORT);
}
