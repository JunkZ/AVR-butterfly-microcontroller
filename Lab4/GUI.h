/*
 * GUI.h
 *
 * Created: 2020-02-24 17:10:24
 *  Author: Klas Mannberg
 */

#ifndef GUI_H_
#define GUI_H_

#include "TinyTimber.h"
#include "Pulser.h"
#include "LCD.h"

typedef struct
{
	Object super;
	int hold;
	LCD *LCD;
	Pulser *Pulse;
	int checking_hold;
} GUI;

void portB(GUI *self);
void portE(GUI *self);
void checkhold(GUI *self);
#define initGUI(hold, LCD, Pulse, checking_hold)      \
	{                                                 \
		initObject(), hold, LCD, Pulse, checking_hold \
	}

#endif