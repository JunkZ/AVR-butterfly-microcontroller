/*
 * Part1.c
 *
 * Created: 2/15/2020 3:10:35 AM
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
int is_prime(long i) {
	int n = 2;
	while (n < i) {
		if ((i % n) == 0) {
			return 0;
		}
		n++;
	}
	return 1;
}
void writeChar(char ch, int pos) {
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
		if (ch < 0 || ch > 9){
			ch = ch - '0';
			}
			if (ch>= 0 && ch <9) {
			int mask = 0xF;
			int mask2 = 0xF0;
			int towrite = numbers[ch];
			
			if (pos == 0) {
				LCDDR0 = (mask & towrite) | (LCDDR0) & mask2;
				towrite = (towrite >> 4);
				LCDDR5 = (mask & towrite) | (LCDDR5) & mask2;
				towrite = (towrite >> 4);
				LCDDR10 = (mask & towrite) | (LCDDR10) & mask2;
				towrite = (towrite >> 4);
				LCDDR15 = (mask & towrite) | (LCDDR15) & mask2;
			}
			if (pos == 1) {
				LCDDR0 = (mask & towrite) << 4 | LCDDR0 & mask;
				towrite = (towrite >> 4);
				LCDDR5 = (mask & towrite) << 4 | LCDDR5 & mask;
				towrite = (towrite >> 4);
				LCDDR10 = (mask & towrite) << 4 | LCDDR10 & mask;
				towrite = (towrite >> 4);
				LCDDR15 = (mask & towrite) << 4 | LCDDR15 & mask;
			}
			if (pos == 2) {
				LCDDR1 = (mask & towrite) | (LCDDR1) & mask2;
				towrite = (towrite >> 4);
				LCDDR6 = (mask & towrite) | (LCDDR6) & mask2;
				towrite = (towrite >> 4);
				LCDDR11 = (mask & towrite) | (LCDDR11) & mask2;
				towrite = (towrite >> 4);
				LCDDR16 = (mask & towrite) | (LCDDR16) & mask2;
			}
			if (pos == 3) {
				LCDDR1 = (mask & towrite) << 4| LCDDR1 & mask;
				towrite = (towrite >> 4);
				LCDDR6 = (mask & towrite) << 4| LCDDR6 & mask;
				towrite = (towrite >> 4);
				LCDDR11 = (mask & towrite) << 4| LCDDR11 & mask;
				towrite = (towrite >> 4);
				LCDDR16 = (mask & towrite) << 4| LCDDR16 & mask;
			}
			if (pos == 4) {
				LCDDR2 = (mask & towrite) | LCDDR2 & mask2 ;
				towrite = (towrite >> 4);
				LCDDR7 = (mask & towrite) | LCDDR7 & mask2;
				towrite = (towrite >> 4);
				LCDDR12 = (mask & towrite)| LCDDR12 & mask2;
				towrite = (towrite >> 4);
				LCDDR17 = (mask & towrite)| LCDDR17 & mask2;
			}
			if (pos == 5) {
				LCDDR2 = (mask & towrite) << 4| LCDDR2 & mask;
				towrite = (towrite >> 4);
				LCDDR7 = (mask & towrite) << 4| LCDDR7 & mask;
				towrite = (towrite >> 4);
				LCDDR12 = (mask & towrite) << 4| LCDDR12 & mask;
				towrite = (towrite >> 4);
				LCDDR17 = (mask & towrite) << 4| LCDDR17 & mask;
			}
		}
	}
}
void computePrimes(int pos) {
	long n;

	for(n = 1;; n++) {
		if (is_prime(n) == 1) {
			printAt(n, pos);
		}
	}
}
void printAt(long num, int pos) {
    int pp = pos;
    writeChar( (char)num % 100 / 10 + '0', pp);
    pp++;
    writeChar( (char)num % 10 + '0', pp);
}
void blink(void) {
	//TCCR1B = (CS12 << 1) | (0 << CS11) | (0 << CS10);
	while (1) {
		
		if (checktint() >= 20) {
			resettint();
			if (LCDDR1 == 0x0) {
				LCDDR1 = 0x2;
			} else {
				LCDDR1 = 0x0;
			}
			
		}
		
	}
	}
void button(void) {
	PORTB = (128 | PORTB);
	int old = (PINB & 128);
	int press = 0;
	while(1) {
		if ((PINB & 128) != old) {
			if ((PINB & 128) == 128) {
				press += 1;
				printAt(press,4);
			}
			old = (PINB & 128);
		}
	}
}

int main(void)
{
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	LCD_init();
	spawn(computePrimes, 0);
	spawn(button);
	blink();
	
    /*while(1)
    {
		
		//computePrimes(0);
		//spawn(blink);
		//blink();
	}*/

}


