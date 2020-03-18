/*
 * Lab5.c
 *
 * Created: 2020-03-07 04:38:48
 *  Author: Klas Mannberg
 */


#include <avr/io.h>
#include "TinyTimber.h"
#include "Queue.h"
#include "USART.h"
#include "LCD.h"

Queue Queue1 = initQUEUE(0, 0, 0, 0, 0);
USART usart1 = initUSART(&Queue1, 0);

int main(void)
{
	initialize(); //also usart & interrupt inits

	INSTALL(&usart1, USART_, IRQ_USART0_RX); //interrupt handler for port

	return TINYTIMBER(&Queue1, init, 9);
}
