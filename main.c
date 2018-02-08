/*
 * Problem3.c
 *
 * Created: 2/7/2018 6:31:03 PM
 * Author : Ryan
 */ 

#include <avr/io.h>
#include "EmSys.h"
#include "SWSerial.h"

char convert_case(char);

int main(void) {
	long baudrate = 9600L;  // initially--can change after this works
	int tx_pin = 12;
	int rx_pin = 11;  // for example
	int framing = SERIAL_8N1;
	
	init_sw_serial(rx_pin, tx_pin, baudrate, framing); 
	init_sw_serial_getc_test(baudrate, framing);   

	debug_init();
	sw_serial_getc();  // throw away first char
	while(1) {
		char c = sw_serial_getc();
		c = convert_case(c);
		sw_serial_putc(c);
		test_sw_serial_getc();   // library test function */
	}
}

char convert_case(char c) {
	if (c >= 65 && c <= 90)
		return c + 32;
	if (c >= 97 && c <= 122)
		return c - 32;
	
	return c;	
}


