/*
 * SWSerial.c
 *
 * Created: 2/4/2018 4:08:50 PM
 *  Author: Ryan
 */ 

#include <avr/io.h>
#include "SWSerial.h"

/* pin numbers for transmit and receive pins */
static int rx_pin;
static int tx_pin;

/* serial transmission parameters */
static long baudrate;
static int framing;
static unsigned int bit_time;

/* bit corresponding to transmit pin and masks to set and clear it */
static char tx_bit;
static char tx_low_mask;
static char tx_high_mask;

static unsigned int get_bit_time(long baudrate);
static char pin_to_port_bit(int pin);

/* set receive/transmit pins and transmission parameters
 */
void init_sw_serial(int rxPin, int txPin, long baud, int frame) {
	rx_pin = rxPin;
	tx_pin = txPin;
	
	baudrate = baud;
	framing = frame;
	bit_time = get_bit_time(baud);
	
	tx_bit = pin_to_port_bit(tx_pin);
	tx_high_mask = 1 << tx_bit;
	tx_low_mask = ~tx_high_mask;
	
	DDRB |= (1 << tx_bit);
}

/* Transmit a char using TTL serial transmission
 */
void sw_serial_putc(char c) {
	char curr_bit;

	PORTB &= tx_low_mask;    // Start bit
	delay_usec(bit_time);
	
	for (int i = 0; i < 8; i++) {
		curr_bit = (c >> i) & 1;
		PORTB &= tx_low_mask;
		PORTB |= curr_bit << tx_bit;
		delay_usec(bit_time);
	}

	PORTB |= tx_high_mask;    // Stop bit
	delay_usec(bit_time);
}

/* Transmit each char in str, including terminating null.
 */
void sw_serial_puts(char *str) {
	char c;
	
	while ((c = *str++)) {
		sw_serial_putc(c);
	}
	
	sw_serial_putc(0x0);
}

/* Return the reciprocal of the baudrate, converted to usecs.
 */
static unsigned int get_bit_time(long baudrate) {
	switch (baudrate) {
		case 1200:
		case 2400:
		case 4800:
		case 9600:
		case 14400:
		case 19200:
		case 28800:
		case 38400:
		case 57600:
		case 115200:
		case 250000:
		case 500000:
		case 1000000:
			return (unsigned int) ((1 / (double) baudrate) * 1000000);
		default:
			return 0;
	}
}

/* Convert a digital pin number to the corresponding bit in PORTB.
 */
static char pin_to_port_bit(int pin) {
	switch (pin) {
		case 10:
			return PB4;
		case 11:
			return PB5;
		case 12:
			return PB6;
		case 13:
			return PB7;
		case 50:
			return PB3;
		case 51:
			return PB2;
		case 52:
			return PB1;
		case 53:
			return PB0;
		default:
			return PB6;
	}
}


