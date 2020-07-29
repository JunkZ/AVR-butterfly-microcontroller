/*
 * Lab1.c
 *
 * Created: 1/29/2020 3:10:35 AM
 *  Author: Klas Mannberg
 */ 


#include <avr/io.h>

void LCD_init(void) {
	/* 1/3 bias, 1/4 duty cycle, 25 segments */
	LCDCRB = (1<<LCDCS) | (1<<LCDMUX1) | (1<<LCDMUX0) | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);

	/* 16 prescaler (no change needed), 8 clock divide*/
	LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);

	/*300 Mu s driver time and output voltage 3.35V*/
	LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);

	/*enable lcd with low power waveform and no interrupt and no blanking(on update)*/
	LCDCRA = (1<<LCDEN) | (1<<LCDAB);
	
}
void writeLong(long i) {
	int x = 5;
	while (x>=0 && i>0) {
		writechar((char)(i % 10),x);
		i = i / 10;
		x--;
	}
	
	
}

void writechar(char ch, int pos) {
	int numbers[10] = {
		0x1551,
		0x2080,
		0x1E11,
		0x1B11,
		0x0B50,
		0x1B41,
		0x1F41,
		0x0111,
		0x1F51,
		0x0B51
	};
	
	if (pos >= 0 && pos <= 5) {
		if (ch >= 0 && ch <= 9) {
			int mask = 0xF;
			int mask2 = 0xF0;
			int towrite = numbers[ch];
			
			if (pos == 0) {
				LCDDR0 = (mask & towrite) | LCDDR0;
				towrite = (towrite >> 4);
				LCDDR5 = (mask & towrite) | LCDDR5;
				towrite = (towrite >> 4);
				LCDDR10 = (mask & towrite) | LCDDR10;
				towrite = (towrite >> 4);
				LCDDR15 = (mask & towrite) | LCDDR15;
			}
			if (pos == 1) {
				LCDDR0 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR5 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR10 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR15 = (mask & towrite) << 4;
			}
			if (pos == 2) {
				LCDDR1 = (mask & towrite) | LCDDR1;
				towrite = (towrite >> 4);
				LCDDR6 = (mask & towrite) | LCDDR6;
				towrite = (towrite >> 4);
				LCDDR11 = (mask & towrite) | LCDDR11;
				towrite = (towrite >> 4);
				LCDDR16 = (mask & towrite) | LCDDR16;
			}
			if (pos == 3) {
				LCDDR1 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR6 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR11 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR16 = (mask & towrite) << 4;
			}
			if (pos == 4) {
				LCDDR2 = (mask & towrite) | LCDDR2 ;
				towrite = (towrite >> 4);
				LCDDR7 = (mask & towrite) | LCDDR7;
				towrite = (towrite >> 4);
				LCDDR12 = (mask & towrite)| LCDDR12;
				towrite = (towrite >> 4);
				LCDDR17 = (mask & towrite)| LCDDR17;
			}
			if (pos == 5) {
				LCDDR2 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR7 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR12 = (mask & towrite) << 4;
				towrite = (towrite >> 4);
				LCDDR17 = (mask & towrite) << 4;
			}
			/*if (pos == 5) {
				LCDDR3 = (mask & towrite) <<4;
				towrite = (towrite >> 4);
				LCDDR8 = (mask & towrite)<<4;
				towrite = (towrite >> 4);
				LCDDR13 = (mask & towrite)<<4;
				towrite = (towrite >> 4);
				LCDDR18 = (mask & towrite)<<4;
			}*/

		}
	}
}
void primes(long i) {
	while(i<430) {
		if (is_prime(i) == 1) {
			writeLong(i);
		}
		i++;
	}
	
	
}
void blink(void) {
	TCCR1B = (CS12 << 1) | (0 << CS11) | (0 << CS10);
	TCNT1 = 0x0000;
	int limit = 8000000/256;
	int nexttime = 0;
	int ticked = 0;
	while (1) {
		
		if (TCNT1 >= nexttime && ticked == 0) {
			ticked = 1;
			if (nexttime> 0xFFFF) {
				nexttime = nexttime - 0xFFFF;
			} else {
				nexttime = nexttime + limit;
			}
			if (LCDDR1 == 0x0) {
				LCDDR1 = 0x2;
			} else {
				LCDDR1 = 0x0;
			}
			
		}
		if (TCNT1 < nexttime) {
			ticked = 0;
		}
		
	}
	}
void button(void) {
	PORTB = (128 | PORTB);
	int old = (PINB & 128);
	while(1) {
		if ((PINB & 128) != old) {
			if (LCDDR3 == 1) {
				LCDDR3 = 0;
				LCDDR1 = 32;
			} else {
				LCDDR3 = 1;
				LCDDR1 = 0;
			}
			while ((PINB & 128) != old) {
				int dummy = 0;		//catch joystick being held, just dummy code to halt execution.
			}
		}

	}
	
}
void primescont(int i) {
	if (is_prime(i) == 1) {
		writeLong(i);
	}
}
int buttoncont(int old) {
	if ((PINB & 128) != old) {
		if ((PINB & 128) == 128) {
			if (LCDDR13 == 1) {
					LCDDR13 = 0;
					LCDDR18 = 1;
				} else
				{
					LCDDR13 = 1;
					LCDDR18 = 0;
				}
			}

		}
	return (PINB & 128);
}
void button2(void) {
	PORTB = (128 | PORTB);
	int old = (PINB & 128);
	//int switch = 0;
	while(1) {
		if ((PINB & 128) != old) {
			//switch = 1;
			
			if ((PINB & 128) == 128) {
				if (LCDDR3 == 1) {
					LCDDR3 = LCDDR3 ^ 1;
					LCDDR1 = LCDDR1 | 32;
				} else
				{
					LCDDR3 = LCDDR3 | 1;
					LCDDR1 = LCDDR1 ^ 32;
				}
			}
			old = (PINB & 128);
			//if (PINB7 == 0)
		}
	}
}
void blinkcont(void) {
	if (TCNT1 >= 0xFF) {
		if (LCDDR1 == 0x0) {
			LCDDR1 = (0x2 | LCDDR1);
			} else {
			LCDDR1 = (LCDDR1 ^ 0x2);
		}
		TCNT1 = 0x0;
		}
		
	}
void combined(void) {
	//INIT PRIMES
	int i = 25000;
	//INIT BUTTON
	PORTB = (128 | PORTB);
	int old = (PINB & 128);
	//INIT BLINK

	TCCR1B = (CS12 << 1) | (0 << CS11) | (0 << CS10);
	TCNT1 = 0x0000;
	int limit = 8000000/256;
	int nexttime = 0;
	int ticked = 0;
	
	while (1) {
		old = buttoncont(old);
				
		if (TCNT1 >= nexttime && ticked == 0) {
			ticked = 1;
			if (nexttime> 0xFFFF) {
				nexttime = nexttime - 0xFFFF;
				} else {
				nexttime = nexttime + limit;
			}
			if (LCDDR1 == 0x0) {
				LCDDR1 = 0x2;
				} else {
				LCDDR1 = 0x0;
			}
			
		}
		if (TCNT1 < nexttime) {
			ticked = 0;
		}
		if (i < 100000) {
			primescont(i++);
		}
		
	}
}



