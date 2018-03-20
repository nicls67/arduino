/*!
 * @file asw.cpp
 *
 * @brief ASW main file
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include <avr/io.h>

/* Inclusion of all ASW header files*/
#include "log/log.h"
#include "keepAliveLed/keepAliveLed.h"

#include "asw.h"


T_ASW_cnf_struct ASW_cnf_struct;


void asw_init()
{
	ASW_cnf_struct.p_usartDebug = new UsartDebug();
	ASW_cnf_struct.p_keepAliveLed = new keepAliveLed();
}
