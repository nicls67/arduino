/*!
 * @file Watchdog.cpp
 *
 * @brief Class Watchdog source code file
 *
 * @date 6 juin 2019
 * @author nicls67
 */

/* TODO : debug menu to manage watchdog  : on/off, change timeout */
/* TODO : add management of reset source */

#include <avr/wdt.h>
#include <util/delay.h>

#include "Watchdog.h"

#define WDG_TIMEOUT_DEFAULT_MS WDTO_500MS /*!< Default timeout value is set to 500 ms */

Watchdog* p_global_BSW_wdg;

Watchdog::Watchdog()
{
	enable(WDG_TIMEOUT_DEFAULT_MS);
	reset(); /* The reset is not mandatory here because the enable function already make a reset, done here by precaution */
}

Watchdog::Watchdog(uint8_t timeout)
{
	enable(timeout);
}


void Watchdog::enable(uint8_t value)
{
	wdt_enable(value);
}

void Watchdog::disable()
{
	wdt_disable();
}

void Watchdog::reset()
{
	wdt_reset();
}

void Watchdog::timeoutUpdate(uint8_t value)
{
	disable();
	_delay_us(10);
	enable(value);
}

void Watchdog::SystemReset()
{
	while(1);
}
