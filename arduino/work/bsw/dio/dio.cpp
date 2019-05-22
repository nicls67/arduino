/*!
 * @file dio.cpp
 * @brief DIO library
 *
 * @date 13 mars 2018
 * @author nicls67
 */


#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/string/String.h"
#include "../../scheduler/scheduler.h"

#include "../usart/usart.h"
#include "../timer/timer.h"
#include "../I2C/I2C.h"
#include "../lcd/LCD.h"
#include "../dio/dio.h"
#include "../dht22/dht22.h"
#include "../cpuLoad/CpuLoad.h"

#include "../bsw.h"

#include "../../asw/debug_ift/DebugInterface.h"
#include "../../asw/debug_mgt/DebugManagement.h"
#include "../../asw/TempSensor/TempSensor.h"
#include "../../asw/display_ift/DisplayInterface.h"
#include "../../asw/display_mgt/DisplayManagement.h"
#include "../../asw/keepAliveLed/keepAliveLed.h"

#include "../../asw/asw.h"

void dio::ports_init()
{
	/* Initialize ports as input or output */
	DDRB = PORTB_CNF_DDRB;

	/* Set initial value for output pins */
	PORTB = PORTB_CNF_PORTB;
}

uint8_t* dio::getPORTxAddress(uint8_t portcode)
{
	uint8_t port_idx = DECODE_PORT(portcode);
	uint8_t* port_addr;

	/* Find port address according to index */
	switch(port_idx)
	{
	case PORT_A:
		port_addr = (uint8_t*)PORTA_PTR;
		break;
	case PORT_B:
		port_addr = (uint8_t*)PORTB_PTR;
		break;
	case PORT_C:
		port_addr = (uint8_t*)PORTC_PTR;
		break;
	case PORT_D:
		port_addr = (uint8_t*)PORTD_PTR;
		break;
	default:
		/* default case : normally not reachable */
		port_addr = (uint8_t*)PORTA_PTR;
		break;
	}

	return port_addr;
}

uint8_t* dio::getPINxAddress(uint8_t portcode)
{
	uint8_t port_idx = DECODE_PORT(portcode);
	uint8_t* port_addr;

	/* Find port address according to index */
	switch(port_idx)
	{
	case PORT_A:
		port_addr = (uint8_t*)PINA_PTR;
		break;
	case PORT_B:
		port_addr = (uint8_t*)PINB_PTR;
		break;
	case PORT_C:
		port_addr = (uint8_t*)PINC_PTR;
		break;
	case PORT_D:
		port_addr = (uint8_t*)PIND_PTR;
		break;
	default:
		/* default case : normally not reachable */
		port_addr = (uint8_t*)PINA_PTR;
		break;
	}

	return port_addr;
}

uint8_t* dio::getDDRxAddress(uint8_t portcode)
{
	uint8_t port_idx = DECODE_PORT(portcode);
	uint8_t* port_addr;

	/* Find port address according to index */
	switch(port_idx)
	{
	case PORT_A:
		port_addr = (uint8_t*)DDRA_PTR;
		break;
	case PORT_B:
		port_addr = (uint8_t*)DDRB_PTR;
		break;
	case PORT_C:
		port_addr = (uint8_t*)DDRC_PTR;
		break;
	case PORT_D:
		port_addr = (uint8_t*)DDRD_PTR;
		break;
	default:
		/* default case : normally not reachable */
		port_addr = (uint8_t*)DDRA_PTR;
		break;
	}

	return port_addr;
}

dio::dio()
{
	ports_init();

	/* Initialize class members */
	PINx_addr_mem = getPINxAddress(PORT_A);
	PINx_idx_mem = (uint8_t)0;
}

void dio::dio_setPort(uint8_t portcode, bool state)
{
	uint8_t mask;
	uint8_t* port = getPORTxAddress(portcode);

	mask = ~(1 << DECODE_PIN(portcode));
	*port &= mask;
	*port |= (state << DECODE_PIN(portcode));
}

void dio::dio_invertPort(uint8_t portcode)
{
	uint8_t* port = getPORTxAddress(portcode);

	*port ^= (1 << DECODE_PIN(portcode));
}


bool dio::dio_getPort(uint8_t portcode)
{
	bool res;
	uint8_t port_value = *(getPINxAddress(portcode));

	res = (port_value >> DECODE_PIN(portcode)) & 0x1;

	return res;
}

void dio::dio_changePortPinCnf(uint8_t portcode, uint8_t cnf)
{
	uint8_t mask = ~(1 << DECODE_PIN(portcode));
	uint8_t* port = getDDRxAddress(portcode);

	/* If cnf is different from 0 or 1, set it by default to 0 (input) */
	if ((cnf != PORT_CNF_OUT) && (cnf != PORT_CNF_IN))
		cnf = PORT_CNF_IN;

	/* reset selected pin */
	*port &= mask;

	/* set selected pin to cnf */
	*port |= (cnf << DECODE_PIN(portcode));
}

void dio::dio_memorizePINaddress(uint8_t portcode)
{
	PINx_addr_mem = getPINxAddress(portcode);
	PINx_idx_mem = DECODE_PIN(portcode);
}

bool dio::dio_getPort_fast(void)
{
	bool res;

	uint8_t port_value = *PINx_addr_mem;

	res = (port_value >> PINx_idx_mem) & 0x1;

	return res;
}
