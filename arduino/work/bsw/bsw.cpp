/*!
 * @file bsw.cpp
 * @brief BSW main file
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>

/* Inclusion of all BSW header files */
#include "usart/usart.h"
#include "dio/dio.h"
#include "timer/timer.h"

#include "bsw.h"

T_BSW_cnf_struct BSW_cnf_struct;

void bsw_init()
{
	BSW_cnf_struct.p_usart = new usart(USART_BAUDRATE);
	BSW_cnf_struct.p_dio = new dio();
	BSW_cnf_struct.p_timer = new timer();
}

