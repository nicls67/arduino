/*!
 * @file Watchdog.cpp
 *
 * @brief Class Watchdog source code file
 *
 * @date 6 juin 2019
 * @author nicls67
 */

/* TODO : debug menu to manage watchdog  : on/off */
/* TODO : add management of reset source */
/* TODO : keep timeout value after of soft reset */

#include <avr/wdt.h>
#include <util/delay.h>

#include "Watchdog.h"

#define WDG_TIMEOUT_DEFAULT_MS WDG_TMO_500MS /*!< Default timeout value is set to 500 ms */

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
	tmo_value = value;
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

uint16_t Watchdog::getTMOValue()
{
	uint16_t retval;

	switch(tmo_value)
	{
	case 0:
	default:
		retval = 15;
		break;
	case 1:
		retval = 30;
		break;
	case 2:
		retval = 60;
		break;
	case 3:
		retval = 120;
		break;
	case 4:
		retval = 250;
		break;
	case 5:
		retval = 500;
		break;
	case 6:
		retval = 1000;
		break;
	case 7:
		retval = 2000;
		break;
	case 8:
		retval = 4000;
		break;
	case 9:
		retval = 8000;
		break;
	}

	return retval;
}
