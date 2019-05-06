/*!
 * @file usart.cpp
 * @brief BSW library for USART
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */


#include <avr/io.h>

#include "../../lib/string/String.h"

#include "usart.h"


usart::usart(uint16_t a_BaudRate)
{
	/* Set value of baud rate and then call hardware initialization function */
	BaudRate = a_BaudRate;
	usart_init();
}

void usart::usart_init()
{
	uint32_t ui32UBRR = F_CPU ;

	ui32UBRR >>= 4 ; // (=) to ui32UBRR /= 16 but more efficient
	ui32UBRR /= BaudRate ;
	ui32UBRR -= 1 ;

	/* Set baud rate */
	UBRR0H = (uint8_t) ( (ui32UBRR>>8) & (0x000000FF) ) ;
	UBRR0L = (uint8_t) ( ui32UBRR & (0x000000FF) ) ;

	/* Enable receiver and transmitter
	 * Enable Rx complete interrupt    */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) ;

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0) | (3<<UCSZ00) ;

}



void usart::usart_sendString(String *str)
{
	uint16_t i = 0;
	uint8_t* data;

	/* Check if input string is not empty */
	if(str->getSize() != 0)
	{
		data = str->getString();
		/* Send each character of the string on usart bus */
		for(i=0; i<str->getSize(); i++)
		{
			if(data[i]=='\n')
				usart_transmit('\r');

			usart_transmit(data[i]);
		}
	}
}


inline void usart::setBaudRate(uint16_t a_BaudRate)
{
	BaudRate = a_BaudRate;
}



void usart::usart_transmit( uint8_t Data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) ) ;

	/* Put data into buffer, sends the data */
	UDR0 = Data ;
}

uint8_t usart::usart_read()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );

	/* Get and return received data from buffer */
	return UDR0;
}


