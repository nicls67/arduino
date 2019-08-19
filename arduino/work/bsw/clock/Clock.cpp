/*!
 * @file Clock.cpp
 *
 * @brief Clock class source code file
 *
 * @date 7 aout 2019
 * @author nicls67
 */

#include <avr/io.h>

#include "../timer/timer.h"

#include "Clock.h"

Clock* p_global_BSW_Clock;

Clock::Clock()
{
	/* Reset interrupt counter */
	resetCounter();

	/* Create timer object if it is still not initialized */
	if(p_global_BSW_timer == 0)
		p_global_BSW_timer = new timer();

	/* Configure and start timer */
	p_global_BSW_timer->configureTimer1(CLOCK_TIMER_PRESCALER_VALUE, CLOCK_TIMER_CTC_VALUE);
	p_global_BSW_timer->startTimer1();
}

