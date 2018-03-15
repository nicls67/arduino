/*!
 * @file asw.cpp
 *
 * @brief ASW main file
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include "asw.h"


T_ASW_cnf_struct ASW_cnf_struct;


void asw_init()
{
	ASW_cnf_struct.p_usartDebug = new UsartDebug();
}
