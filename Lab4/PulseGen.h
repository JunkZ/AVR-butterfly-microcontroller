/*
 * PulseGen.h
 *
 * Created: 2020-02-24 17:09:55
 *  Author: Klas Mannberg
 */
#ifndef PULSEGEN_H_
#define PULSEGEN_H_

#include "PulseGen.h"
#include "TinyTimber.h"
#include "Writer.h"

typedef struct
{
    Object super;
    int freq;
    int saved_freq;
    Writer *Write1;
    int portnr; //also corresponds to 4=gen1, 6=gen2
    int first_call;
} PulseGen;

void update_F(PulseGen *self, int val);
void save(PulseGen *self);
void infinity(PulseGen *self);

#define initGEN(freq, saved_freq, Write1, portnr, first_call)      \
    {                                                              \
        initObject(), freq, saved_freq, Write1, portnr, first_call \
    }

#endif