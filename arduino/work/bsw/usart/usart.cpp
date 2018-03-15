/*!
 * @file usart.cpp
 * @brief BSW library for USART
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */


#include <avr/io.h>
#include "usart.h"


usart::usart(uint16_t a_BaudRate)
{
	/* Set value of baud rate and then call harware initialization function */
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

	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) ;

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0) | (3<<UCSZ00) ;

}



void usart::usart_sendString(uint8_t *str)
{
	uint16_t i = 0;

	/* Check if input string is different of null */
	if(str != 0)
	{
		/* Send each character of the string on usart bus */
		while(str[i] != '\0')
		{
			if(str[i]=='\n')
				usart_transmit('\r');

			usart_transmit(str[i]);
			i++;
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
