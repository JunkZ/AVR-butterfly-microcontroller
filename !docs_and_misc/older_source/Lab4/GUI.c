/*
 * GUI.c
 *
 * Created: 2020-02-24 17:10:24
 *  Author: Klas Mannberg
 */
#include "GUI.h"
#include "TinyTimber.h"
#include <avr/io.h> //for the pins
#include "LCD.h"

/*
Joystick is connected to certain bits of PORTB and PORTE.
PORTB I/O bits correspond to press down or up
PORTE I/O bits correspond to left or right

must have a been a easier way to figure the bits out??
*/
void portB(GUI *self)
{
	//BIT 4:  press
	if ((PINB & 16) == 0)
	{
		if (self->LCD->curgen->freq == 0)
		{
			self->LCD->curgen->freq = self->LCD->curgen->saved_freq;						 //save the freq
			self->LCD->curgen->saved_freq = 0;												 //and reset
			AFTER(MSEC(1000 / self->LCD->curgen->freq / 2), self->LCD->curgen, infinity, 0); //need to resume pulsing after retriveing freq
		}
		else if (self->LCD->curgen->freq != 0)
		{
			ASYNC(self->LCD->curgen, save, 0); //otherwise save the freq
		}
		ASYNC(self->LCD, update_screen, 0); //and update the screen
	}

	//BIT 6:  up
	else if ((PINB & 64) == 0)
	{
		SYNC(self->LCD->curgen, update_F, 1); //SYNC since screen update needs proper value
		ASYNC(self->LCD, update_screen, 0);
		AFTER(MSEC(500), self, checkhold, 1); //bit of start delay for hold
	}

	//BIT 7:  down
	else if ((PINB & 128) == 0)
	{
		SYNC(self->LCD->curgen, update_F, -1); //same as up but negative arg
		ASYNC(self->LCD, update_screen, 0);
		AFTER(MSEC(500), self->Pulse, metronome, 1);
	}
}
void checkhold(GUI *self)
{
	self->checking_hold = 1; //kinda shit way of locking, tried to prevent multiple clicks acting as hold
	if (((PINB & 64) == 0 || (PINB & 128) == 0))
	{									  //if any of the buttons still being held after the delay calling this method
		ASYNC(self->Pulse, metronome, 1); //start the hold loop
		self->checking_hold = 0;
	}
	else
	{
		self->checking_hold = 0;
		return; //not holding
	}
}
void portE(GUI *self)
{
	//BIT 2 = left
	if ((PINE & 4) == 0) //swapping generator look at shift arg
	{
		ASYNC(self->LCD, shift, 1);
	}
	else if ((PINE & 8) == 0)
	{
		ASYNC(self->LCD, shift, 2);
	}
	ASYNC(self->LCD, update_screen, 0);
}