/*
 * Problem2.c
 *
 * Created: 2/4/2018 4:03:30 PM
 * Author : Ryan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "EmSys.h"
#include "SWSerial.h"

int main(void) {

	// init transmission parameters
	int rx_pin = 11;
	int tx_pin = 12;
	long baudrate = 9600;
	int framing = SERIAL_8N1;
	
	init_sw_serial(rx_pin, tx_pin, baudrate, framing);  // this is your function
	init_sw_serial_puts_test(baudrate, framing);   // library function: note we need the baudrate and framing here

	while(1) {
		sw_serial_puts("Hello,World!\n");
		test_sw_serial_puts();   // library test function
		_delay_ms(1000);
	}
}


