/*
 * Pulser.c
 *
 * Created: 2020-02-26 15:50:56
 *  Author: Klas Mannberg
 */
#include "LCD.h"
#include "Pulser.h"
#include "avr/io.h"
void write(Writer *self, int arg)
{
	if (arg == 4)//generator 1
	{ 
		LCDDR1 = LCDDR1 ^ 0xFFFF; //test for pulsing without oscilloscope
		PORTE = PORTE ^ 16; //XOR toggles between xxx1 xxxx and xxx0 xxxx. also need to keep old value from port 6, not overwrite
	}
	else if (arg == 6)//generator 2
	{ 
		PORTE = PORTE ^ 64;
	}
}