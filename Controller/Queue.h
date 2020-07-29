/*
 * Queue.h
 *
 * Created: 2020-03-07 05:39:25
 *  Author: Klas Mannberg
 */


#ifndef QUEUE_H_
#define QUEUE_H_

#include "TinyTimber.h"
#include "Queue.h"

typedef struct
{
	Object super;
	int north;
	int south;
	int direction; /*only for testing*/
	int counter; /*prevent infinity*/
	int onbridge;
} Queue;

void testloop(Queue* self);
void transmitter(Queue* self, int lights);
void carfinished(Queue* self);
void react(Queue* self, int action);
void init(Queue* self, int lights);

#define initQUEUE(north,south,direction,counter,onbridge)	\
	{														\
	initObject(),north,south,direction,counter,onbridge		\
	}

#endif