/*
 * USART.c
 *
 * Created: 2020-03-07 09:14:44
 *  Author: Klas Mannberg
 */
#include <avr/io.h>
#include "Queue.h"
#include "USART.h"

void USART_(USART* self) {

	switch (UDR0 & 15) {
	case 1: //northbound car arrival
		ASYNC(self->queue, react, 0);//react to interrupt
		break;
	case 2://Northbound bridge entry
		ASYNC(self->queue, react, 1);
		break;
	case 4://Southbound car arrival
		ASYNC(self->queue, react, 2);
		break;
	case 8://Southbound bridge entry
		ASYNC(self->queue, react, 3);
		break;
	}
}