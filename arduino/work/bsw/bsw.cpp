/*!
 * @file bsw.cpp
 * @brief BSW main file
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */


#include "bsw.h"

T_BSW_cnf_struct BSW_cnf_struct;

void bsw_init()
{
	BSW_cnf_struct.p_usart = new usart(USART_BAUDRATE);
	BSW_cnf_struct.p_dio = new dio();
}

