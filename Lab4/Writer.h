/*
 * Writer.c
 *
 * Created: 2020-02-26 15:50:56
 *  Author: Klas Mannberg
 */
#ifndef WRITER_H_
#define WRITER_H_

#include "Writer.h"
#include "TinyTimber.h"

typedef struct
{
	Object super;
	int arg;
} Writer;

void write(Writer *self, int arg);
#define initWriter(arg)   \
	{                     \
		initObject(), arg \
	}

#endif