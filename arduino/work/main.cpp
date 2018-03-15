/*!
 *  @file main.cpp
 *  @brief Background task file
 *
 *  @date 12 mars 2018
 *  @author nicls67
 */


#include <avr/io.h>
#include <util/delay.h>


#include "bsw/bsw.h"
#include "asw/asw.h"



/*!
 * @brief Background task of program
 * @details This function initializes all the software and then goes into an infinite loop.
 *          Periodic interrupt will wake up the software to perform application
 */
int main( void )
{

	bsw_init();
	asw_init();


	for( ; ; )
	{
		// set Pin #7 of Port B
		BSW_cnf_struct.p_dio->dio_setPortB(7,true);

		// Wait for 1 second
		_delay_ms( 250.0 ) ;

		BSW_cnf_struct.p_dio->dio_setPortB(7,false);

		_delay_ms( 250.0 ) ;

		ASW_cnf_struct.p_usartDebug->sendData((uint8_t*)("hello\n"));
	}
}

