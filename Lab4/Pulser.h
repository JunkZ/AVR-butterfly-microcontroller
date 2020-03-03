/*
 * Pulser.h
 *
 * Created: 2020-02-26 15:50:56
 *  Author: Klas Mannberg
 */ 
#ifndef PULSER_H_
#define PULSER_H_

#include "Pulser.h"
#include "TinyTimber.h"
#include "LCD.h"

typedef struct {
	Object super;
	int cont;
	LCD *LCD;
} Pulser;

void metronome(Pulser *self);

#define initPulser(cont, LCD) {initObject(),cont, LCD};
	
#endif