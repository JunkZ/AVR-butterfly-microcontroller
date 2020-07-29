/*
 * PulseGen.c
 *
 * Created: 2020-02-24 17:09:55
 *  Author: Klas Mannberg
 */ 
#include "PulseGen.h"
#include "avr/io.h"

void update_F(PulseGen *self,int val) {
	
    if (val<0 && self->freq>0) {
        self->freq += val;
    } else if(val>0 && self->freq<100 && self->freq>0) {
        self->freq += val;
    } else if(self->freq==0 && val==1){
		if(self->first_call == 0) {
			self->first_call = 1;
			AFTER(MSEC(1000)/2,self,infinity,0); //since from 0 its guaranteed 1hz so call after 1sec 2),/,/
		}
		self->freq += val;
	}
}

void save(PulseGen *self) {
    self->saved_freq = self->freq;
    self->freq = 0;
	self->first_call = 0; //so the pulse loop starts again
}

void infinity(PulseGen *self) {
	if (self->freq>0) {
		SYNC(self->Write1,write,self->portnr);//pulse once
		AFTER(MSEC(1000/self->freq/2),self,infinity,0);
		
	}/* else {
		if (self->portnr == 4 && (PORTE & 16) == 1) { //make sure its outputting low not high
			PORTE = PORTE ^ 16;
		} else if (self->portnr == 6 && (PORTE &64) == 1) {
			PORTE = PORTE ^ 64;
		}
	}
	
	/*if (self->freq==0)  { //since freq 00 = stop pulse
		if (self->portnr == 4 && (PORTE & 16) == 0) { //if gen1 is currently outputting low
			AFTER(MSEC(1000/self->freq/2),self,infinity,0);//we do not wanna pulse just loop again
		} else if (self->portnr == 6 && (PORTE & 64) == 0) { //same for gen2
			AFTER(MSEC(1000/self->freq/2),self,infinity,0);
		} else { //this means frequency is 0 but gen is currently outputting high...
			SYNC(self->Write1,write,self->portnr);//...so we need to pulse one last time
		}
		
		 
	} else {
		SYNC(self->Write1,write,self->portnr);//pulse once
	
	}*/
}