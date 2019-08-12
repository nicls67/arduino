/*!
 * @file TimeManagement.cpp
 *
 * @brief TimeManagement class source code file
 *
 * @date 7 aout 2019
 * @author nicls67
 */

#include <avr/io.h>

#include "../../lib/string/String.h"
#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/clock/Clock.h"

#include "../debug_ift/DebugInterface.h"

#include "TimeManagement.h"

TimeManagement* p_global_ASW_TimeManagement;

#define TIME_MANAGEMENT_RESOLUTION_CENTISEC CLOCK_INTERRUPT_PERIOD_MS/10 /*!< Resolution of the time computation */

TimeManagement::TimeManagement()
{
	/* Create clock object if it is still not initialized */
	if(p_global_BSW_Clock == 0)
		p_global_BSW_Clock = new Clock();

	/* Initialize current time to 0 */
	current_time.centiSeconds = 0;
	current_time.seconds = 0;
	current_time.minutes = 0;
	current_time.hours = 0;

	/* Start periodic task */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&TimeManagement::TimeComputation_task), PERIOD_TIME_COMPUTATION_TASK);
}

void TimeManagement::TimeComputation_task()
{
	p_global_ASW_TimeManagement->UpdateCurrentTime();
}

void TimeManagement::UpdateCurrentTime()
{
	uint16_t count = p_global_BSW_Clock->getCounterValue();
	p_global_BSW_Clock->resetCounter();

	uint16_t increment_cnt = count * TIME_MANAGEMENT_RESOLUTION_CENTISEC;

	/* Update hundredth of seconds */
	while(increment_cnt >= 100)
	{
		increment_cnt -= 100;
		current_time.seconds++;
	}

	current_time.centiSeconds += increment_cnt;

	if(current_time.centiSeconds >= 100)
	{
		current_time.centiSeconds -= 100;
		current_time.seconds++;
	}

	/* Update seconds */
	while(current_time.seconds >= 60)
	{
		current_time.seconds -= 60;
		current_time.minutes++;
	}

	/* Update minutes */
	while(current_time.minutes >= 60)
	{
		current_time.minutes -= 60;
		current_time.hours++;
	}
}

void TimeManagement::FormatTimeString(String* str, uint8_t separator, bool isSecondsDisplayed, bool isCentiDisplayed)
{
	str->appendInteger(current_time.hours,10);
	str->appendChar(separator);
	str->appendInteger(current_time.minutes,10);

	if(isSecondsDisplayed)
	{
		str->appendChar(separator);
		str->appendInteger(current_time.seconds,10);
	}

	if(isCentiDisplayed)
	{
		str->appendChar(separator);
		str->appendInteger(current_time.centiSeconds,10);
	}
}
