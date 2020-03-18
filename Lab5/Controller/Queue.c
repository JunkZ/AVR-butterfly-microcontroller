/*
 * Queue.c
 *
 * Created: 2020-03-07 09:25:43
 *  Author: Klas Mannberg
 */
#include "Queue.h"
#include "LCD.h"
#include <avr/io.h>


void react(Queue* self, int action) {//react to the interrupt
	switch (action) {
	case 0: //north queue +1 
		self->north += 1;
		ASYNC(self, update_screen_north, self->north); //update lcd
		if (self->south == 0 && self->onbridge == 0) { //if south is just emtpy and noone on bridge switch direction
			transmitter(self, 9);
		}
		else if (self->south == 0) { //or if something is on bridge
			AFTER(SEC(self->onbridge), self, transmitter, 9); //wait seconds equal to amount of cars on bridge
		}
		break;
	case 1: //north queue -1 car
		self->north -= 1;
		self->counter += 1; //count up send from this direction to prevent infinity

		ASYNC(self, update_screen_north, self->north);

		self->onbridge += 1; //one car entered bridge
		ASYNC(self, update_screen_onbridge, self->onbridge);

		AFTER(SEC(5), self, carfinished, 0); //after 5 seconds it has passed remove from bridge

		if (self->north == 0 || self->counter >= 20) { //empty queue just switch, or if 20 cars already passed this way
			transmitter(self, 0); //stop cars before switching since we need to empty bridge
			AFTER(SEC(6), self, transmitter, 6); //after 5 sec allow in other direction
			self->counter = 0; //reset infinity counter

		}


		break;
	case 2: //south queue +1 car
		self->south += 1;

		ASYNC(self, update_screen_south, self->south);

		if (self->north == 0 && self->onbridge == 0) {
			transmitter(self, 6);
		}
		else if (self->north == 0) {
			AFTER(SEC(self->onbridge), self, transmitter, 6);
		}
		break;
	case 3: //south queue -1 car
		self->south -= 1;
		ASYNC(self, update_screen_south, self->south);

		self->counter += 1;

		self->onbridge += 1;
		ASYNC(self, update_screen_onbridge, self->onbridge);

		AFTER(SEC(5), self, carfinished, self);

		if (self->south == 0 || self->counter >= 20) {
			transmitter(self, 0);
			AFTER(SEC(6), self, transmitter, 9);
			self->counter = 0;

		}

		break;
	}
}
void carfinished(Queue* self) {
	self->onbridge -= 1;
	ASYNC(self, update_screen_onbridge, self->onbridge);
}
void testloop(Queue* self) { //method is only for testing
	UDR0 = 9;
	if (self->direction == 9 && self->north > 0) {
		ASYNC(self, react, 1);
	}
	else if (self->direction == 6 && self->south > 0) {
		ASYNC(self, react, 3);
	}


	AFTER(SEC(1), self, testloop, 0);
}
void init(Queue* self, int lights) {
	update_screen_north(self);
	update_screen_onbridge(self);
	update_screen_south(self);
	transmitter(self, 9); //initial state
	//testloop(self);
	//ASYNC(self,react,1);

}
void transmitter(Queue* self, int lights) {
	self->direction = lights; //only for test
	UDR0 = lights;//send
}