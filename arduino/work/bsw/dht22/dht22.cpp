/*!
 * @file dht22.cpp
 *
 * @brief This file defines classes for DHT22 driver
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../lib/string/string.h"

#include "../usart/usart.h"
#include "../timer/timer.h"
#include "../I2C/I2C.h"
#include "../lcd/LCD.h"
#include "../dio/dio.h"
#include "../dht22/dht22.h"
#include "../cpuLoad/CpuLoad.h"

#include "../bsw.h"





#define MAX_WAIT_TIME_US 100

dht22::dht22(uint8_t port)
{
	dht22_port = port;
	p_dio = BSW_cnf_struct.p_dio;

	initializeCommunication();
}

bool dht22::read(uint16_t* raw_humidity, uint16_t* raw_temperature)
{
	uint8_t wait_cpt;
	uint8_t bit_cpt = 32;

	uint8_t rcv_buf_data[32];
	uint8_t rcv_buf_crc[8];

	/* To speed-up register reading time, the registers addresses are computed now and only once
	 * and memorized into DIO driver.
	 * Then the registers reads will be done with the "fast" reading function using the memorized addresses */
	p_dio->dio_memorizePINaddress(dht22_port);

	/* Disable interrupt during the communication */
	cli();

	/* Set pin to LOW for 1 ms at least */
	p_dio->dio_setPort(dht22_port, false);
	_delay_us(1100);

	/* Set pin to HIGH for 35 us */
	p_dio->dio_setPort(dht22_port, true);
	_delay_us(35);

	/* Re-configure pin as input */
	p_dio->dio_changePortPinCnf(dht22_port, PORT_CNF_IN);

	/* Sensor will now set pin state to LOW for 80 us
	 * Wait for 40 us (half time) and check that the pin is LOW */
	_delay_us(40);
	if (p_dio->dio_getPort_fast() != false)
		return false;

	/* Sensor will set pin state to HIGH for 80 us
	 * Wait for 80 us (2nd half of the 80 us LOW + 1st half time of the 80 us HIGH) and check that the pin is HIGH */
	_delay_us(80);
	if (p_dio->dio_getPort_fast() != true)
		return false;

	/* Wait until signal goes to LOW (bit transmission)
	 * If the signal never goes to LOW, report transmission failure */
	wait_cpt = 0;
	while(p_dio->dio_getPort_fast() != false)
	{
		_delay_us(1);
		wait_cpt++;
		if(wait_cpt > MAX_WAIT_TIME_US)
			return false;
	}

	/* Wait until signal goes to HIGH (bit transmission)
	 * If the signal never goes to HIGH, report transmission failure */
	wait_cpt = 0;
	while(p_dio->dio_getPort_fast() != true)
	{
		_delay_us(1);
		wait_cpt++;
		if(wait_cpt > MAX_WAIT_TIME_US)
			return false;
	}

	/* There are 32 bits of data to receive */
	while (bit_cpt > 0)
	{
		/* Count the number of cycles until the signal goes LOW */
		wait_cpt = 0;
		while(p_dio->dio_getPort_fast() != false)
		{
			_delay_us(1);
			wait_cpt++;
			if(wait_cpt > MAX_WAIT_TIME_US)
				return false;
		}

		/* Update bit counter and memorize time */
		bit_cpt--;
		rcv_buf_data[bit_cpt] = wait_cpt;

		/* Wait until signal goes to HIGH (bit transmission)
		 * If the signal never goes to HIGH, report transmission failure */
		wait_cpt = 0;
		while(p_dio->dio_getPort_fast() != true)
		{
			_delay_us(1);
			wait_cpt++;
			if(wait_cpt > MAX_WAIT_TIME_US)
				return false;
		}
	}

	/* Start receiving checksum -> 8 bits to receive */
	bit_cpt = 8;
	while (bit_cpt > 0)
	{
		/* Count the number of cycles until the signal goes LOW */
		wait_cpt = 0;
		while(p_dio->dio_getPort_fast() != false)
		{
			_delay_us(0.5);
			wait_cpt++;
			if(wait_cpt > MAX_WAIT_TIME_US)
				return false;
		}

		/* Update bit counter and memorize time */
		bit_cpt--;
		rcv_buf_crc[bit_cpt] = wait_cpt;

		/* Wait until signal goes to HIGH (bit transmission)
		 * If the signal never goes to HIGH, report transmission failure */
		wait_cpt = 0;
		while(p_dio->dio_getPort_fast() != true)
		{
			_delay_us(1);
			wait_cpt++;
			if(wait_cpt > MAX_WAIT_TIME_US)
				return false;
		}
	}

	/* Re-enable interrupts at the end of communication */
	sei();

	/* Initialize communication for next time */
	initializeCommunication();


	/* Convert timing into binary data */
	uint32_t raw_rcv_data = 0;
	uint8_t  raw_rcv_crc = 0;

	for (uint8_t i = 32; i > 0; i--)
	{
		uint32_t b;
		if (rcv_buf_data[i-1] > 10)
			b = 1;
		else
			b = 0;

		raw_rcv_data |= (b << (i-1));
	}
	for (uint8_t i = 8; i > 0; i--)
	{
		uint32_t b;
		if (rcv_buf_crc[i-1] > 10)
			b = 1;
		else
			b = 0;

		raw_rcv_crc |= (b << (i-1));
	}

	/* Check checksum */
	uint8_t crc1, crc2, crc3, crc4, crc;
	crc1 = (raw_rcv_data >> 24) & 0xFF;
	crc2 = (raw_rcv_data >> 16) & 0xFF;
	crc3 = (raw_rcv_data >>  8) & 0xFF;
	crc4 = raw_rcv_data & 0xFF;
	crc = crc1 + crc2 + crc3 + crc4;
	if(crc != raw_rcv_crc)
		return false;


	/* Convert data in humidity and temperature */
	*raw_humidity = (raw_rcv_data >> 16) & 0xFFFF;
	*raw_temperature = raw_rcv_data & 0xFFFF;


	return true;
}

void dht22::initializeCommunication()
{
	/* Configures pin at output for the beginning of communication */
	p_dio->dio_changePortPinCnf(dht22_port, PORT_CNF_OUT);

	/* Set pin at level HIGH */
	p_dio->dio_setPort(dht22_port, true);

}
