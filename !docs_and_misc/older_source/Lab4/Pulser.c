/*
 * Pulser.c
 *
 * Created: 2020-02-26 15:50:56
 *  Author: Klas Mannberg
 */
#include "LCD.h"
#include "Pulser.h"
#include "avr/io.h"
void metronome(Pulser *self)
{

	if (self->cont == 1)
	{ //another holding check
		if ((PINB & 64) == 0)
		{
			SYNC(self->LCD->curgen, update_F, 1); //freq+1
		}
		else if ((PINB & 128) == 0)
		{
			SYNC(self->LCD->curgen, update_F, -1); //freq-1
		}
		else
		{
			self->cont = 0; //not holding anymore
			return;
		}
	}
	else
	{
		self->cont = 1; //holding
	}

	SYNC(self->LCD, update_screen, 1);	//update the screen after every tick
	AFTER(MSEC(200), self, metronome, 0); //recursion with 200msec delay
}