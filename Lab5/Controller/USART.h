/*
 * USART.h
 *
 * Created: 2020-03-07 05:39:25
 *  Author: Klas Mannberg
 */


#ifndef USART_H_
#define USART_H_

#include "TinyTimber.h"
#include "Queue.h"

typedef struct
{
	Object super;
	Queue* queue;
	int var1;

} USART;

void USART_(USART* self);

#define initUSART(queue,var1) \
	{ \
	initObject(),queue,var1 \
	}

#endif