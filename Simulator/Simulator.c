/*
 * Simulator.c
 *
 * Created: 2020-03-09 07:26:37
 *  Author: Klas Mannberg
 */


#include <io.h>
#include <sys/termios.h>

#include <sys/uio.h>

 //for threads
#include <pthread.h>

//for write/read
#include <unistd.h>

//for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//for exit & printf
#include <stdlib.h>

//for getchar
#include <stdio.h>

void* remover(void* arg);
void* adder(void* arg);
void* output(void* arg);
void* reader(void* arg);

pthread_t reader_t;
pthread_t output_t;
pthread_t adder_t;
pthread_t remover_t;

pthread_mutex_t mu;


int northqueue = 0;
int southqueue = 0;
int north = 0;
int south = 0;
int carsonbridge = 0;
int lights = 6;
int changed = 0;
int COM1;

void send(int arg) {

	int writing;
	if (arg == 1) {
		writing = 1;//1 = 0001 = northbound car arrival 
	}
	else if (arg == 2) {
		writing = 4;//4 = 0100 = southbound car arrival
	}
	else if (arg == 3) {
		writing = 2;//2 = 0010 = northbound entered bridge
	}
	else if (arg == 4) {
		writing = 8;//8 = 1000 = southbound entered bridge
	}
	int temp = write(COM1, &writing, 1); //tell controller
}

void* output(void* arg) {
	while (1) {
		if (changed == 1) { //console output
			printf("\nNorthQueue = %d", northqueue);
			printf(" SouthQueue = %d", southqueue);
			printf(" CarsOnBridge = %d", carsonbridge);
			printf(" NorthLight = %d", north);
			printf(" SouthLight = %d", south);
			printf("\n");
			changed = 0;
		}
		sleep(0.1);
	}

}
void* reader(void* arg) { //reading traffic lights from controller 

	while (1) {
		uint8_t lights;
		int temp = read(COM1, &lights, sizeof(lights)); //ssize_t read(int fildes, void *buf, size_t nbyte); Getting lamp status

		//recieve change from butterfly and update accordingly
		if (lights == 6) { //6 = 0110 = northbound->red southbound->green
			north = 0;
			south = 1;
			changed = 1;
		}
		else if (lights == 9)
		{
			south = 0;
			north = 1;
			changed = 1;

		}
		else if (lights == 0 | lights == 15) {
			north = south = 0;
			changed = 1;

		}
	}
}
void* adder(void* arg) {
	while (1) {
		if (north == 1 && south == 0 && northqueue > 0) {
			pthread_mutex_lock(&mu); //vars being changed by other threads
			northqueue -= 1;
			carsonbridge += 1;
			pthread_mutex_unlock(&mu);

			pthread_create(&remover_t, NULL, remover, (void*)1337); //need to remove from bridge
			send(3); //communicate to butterfly
			sleep(1);
		}
		else if (north == 0 && south == 1 && southqueue > 0) {
			pthread_mutex_lock(&mu);
			southqueue -= 1;
			carsonbridge += 1;
			pthread_mutex_unlock(&mu);

			pthread_create(&remover_t, NULL, remover, (void*)1337);
			send(4);
			sleep(1);
		}
	}

}
void* remover(void* arg) {
	sleep(5); //after 5 sec car has passed
	carsonbridge -= 1;
	changed = 1;
	pthread_exit(NULL);//kill the thread
}
int init(void) {
	int COM1 = open("/dev/ttyS0", O_RDWR);
	//create a termios
	struct termios termios_p;
	//pull parameters and store into this structure
	tcgetattr(COM1, &termios_p);

	//New settings:
	termios_p.c_cflag = B9600 | CS8 | CREAD | CLOCAL | HUPCL;// CS8 should already be enabled on open. B9600 for port speed. one stop bit should be default aslong as no CSTOPB, no parity bit is default when no PARENB set.
															// CREAD enable receiver otherwise nothing read from butterfly. CLOCAL for correct line. HUPCL to end.

	termios_p.c_lflag = termios_p.c_lflag & ~(ICANON | ECHO); //ICANON kills process, echo disturbs console output

	//push new settings
	cfsetispeed(&termios_p, B9600); //int cfsetispeed(struct termios *termios_p, speed_t speed); push the speed
	cfsetospeed(&termios_p, B9600); //int cfsetospeed(struct termios *termios_p, speed_t speed);
	tcsetattr(COM1, TCSANOW, &termios_p); // TCSANOW change occurs immediately
	north = 1; //just initalize same as butterfly
	return COM1; //to set global var in main

}
void consoleloop(void) { //react to console input
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);//keyboard, controller not needed and will break everything
	select(4, &rfds, NULL, NULL, NULL); //wait
	char input;
	while (1) {
		if (FD_ISSET(0, &rfds)) { //keypress registered
			input = getchar();//input from console
			if (input == 's') {
				pthread_mutex_lock(&mu); //this var needs to be locked since adder thread keeps updating it
				southqueue += 1;
				pthread_mutex_unlock(&mu);
				changed = 1; //enable reprint
				printf("\nCar added to south queue");
				send(2); //communicate to butterfly

			}
			else if (input == 'n')
			{
				pthread_mutex_lock(&mu);
				northqueue += 1;
				pthread_mutex_unlock(&mu);
				changed = 1;
				printf("\nCar added to north queue");
				send(1);

			}
			else if (input == 'e')
			{
				printf("\nExiting...");
				sleep(1);
				exit(0); //terminate all threads and finish main (exit)
			}

		}
	}
}

int main(void)
{


	COM1 = init(); //set global port var

	//creating the threads
	pthread_create(&adder_t, NULL, adder, (void*)1337);
	pthread_create(&output_t, NULL, output, (void*)1337);
	pthread_create(&reader_t, NULL, reader, (void*)1337);

	//goto inf loop of console input
	consoleloop();
}




