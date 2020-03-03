/*
 * Lab4.c
 * Dual pulse generator with a GUI using TinyTimber kernel.
 * 
 * Created: 2020-02-24 13:39:57
 *  Author: Klas Mannberg
 * 
 */

/*
 * ASYNC/SYNC(objectpointer,method,arg)
 * Async always returns message tag not actual result
 *
 *
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TinyTimber.h"
#include "PulseGen.h"
#include "GUI.h"
#include "LCD.h"
#include "Pulser.h"

int main(void)
{

	LCD_init(); //also contains other inits

	//Creating objects
	Writer Write1 = initWriter(0);
	PulseGen Gen1 = initGEN(0, 0, &Write1, 4, 0);
	PulseGen Gen2 = initGEN(0, 0, &Write1, 6, 0);
	LCD LCD1 = initLCD(&Gen1, &Gen2, &Gen1);
	Pulser Pulse1 = initPulser(0, &LCD1);
	GUI GUI1 = initGUI(0, &LCD1, &Pulse1, 0);

	//Installing method as interrupt handler with obj and interrupt source as arguments
	INSTALL(&GUI1, portB, IRQ_PCINT1); //Button down/up/press interrupt handler.
	INSTALL(&GUI1, portE, IRQ_PCINT0); //Button right/left interrupt handler.

	//Must give control to TinyTimber scheduler, starting event-handling. Method(obj,arg) is startup event
	return TINYTIMBER(&LCD1, update_screen, 0);
}