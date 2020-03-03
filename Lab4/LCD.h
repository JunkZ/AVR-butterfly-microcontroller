/*
 * LCD.h
 *
 * Created: 2020-02-24 17:10:24
 *  Author: Klas Mannberg
 */ 
#include "TinyTimber.h"
#include "PulseGen.h"

#ifndef LCD_H_
#define LCD_H_

typedef struct {
    Object super;
	PulseGen *Gen1;
	PulseGen *Gen2;
	PulseGen *curgen;
} LCD;

void LCD_init(void);
void writeChar(char ch, int pos);
void printAt(long num, int pos);
void update_screen(LCD *self,int gen);
void shift(LCD *self, int gen);


#define initLCD(Gen1,Gen2,curgen) {initObject(),Gen1,Gen2,curgen}
	
#endif