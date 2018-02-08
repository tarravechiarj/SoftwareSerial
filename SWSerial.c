/*
 * SWSerial.c
 *
 * Created: 2/4/2018 4:08:50 PM
 *  Author: Ryan
 */ 

#include <avr/io.h>
#include "EmSys.h"
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

/* bit corresponding to receive pin and a mask to read it */
static char rx_bit;
static char rx_mask;

/* framing parameters */
static char data_bits;
static char parity;
static char stop_bits;

void delay_usec(unsigned int);
static unsigned int get_bit_time(long);
static char pin_to_port_bit(int);
static char compute_parity_bit(char);

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

	rx_bit = pin_to_port_bit(rx_pin);
	rx_mask = 1 << rx_bit;
	DDRB &= ~(1 << rx_bit);
	
	data_bits = ((frame & 0x7) >> DATABITS) + 5;
	parity = frame >> PARITYBITS;
	stop_bits = (frame >> STOPBITS) & 1;
}

/* Transmit a char in a data frame using TTL serial transmission
 */
void sw_serial_putc(char c) {
	char curr_bit;
	char parity_bit;

	/* transmit start bit */
	PORTB &= tx_low_mask;   
	
	/* transmit data bits */
	for (int i = 0; i < data_bits; i++) {
		delay_usec(bit_time - 2);	
		curr_bit = (c >> i) & 1;
		PORTB &= tx_low_mask;
		PORTB |= curr_bit << tx_bit;
	}
	
	/* transmit 0 or 1 parity bits */
	delay_usec(bit_time);
	if (parity) {
		parity_bit = compute_parity_bit(c);
		PORTB &= tx_low_mask;
		PORTB |= parity_bit << tx_bit;
		delay_usec(bit_time - 17);
	}
	
	/* transmit 1 or 2 stop bits */
	do {
		PORTB |= tx_high_mask;
		delay_usec(bit_time);
	} while (stop_bits--);  
}

/* Read a char from a data frame using TTL serial transmission
 */
char sw_serial_getc(void) {
	char curr_bit;
	char c = 0;

	/* wait until start bit is read and delay half a bit time before reading */
	while (PINB & rx_mask) delay_usec(1);
	delay_usec(bit_time >> 1);

	/* read data bits */
	for (int i = 0; i < data_bits; i++) {
		delay_usec(bit_time - 2);
		curr_bit = (PINB & rx_mask) >> rx_bit;
		c |= curr_bit << i;
	}
	delay_usec(bit_time >> 1);

	/* wait for parity bit */
	if (parity) {
		delay_usec(bit_time);
	}

	/* wait for  1 or 2 stop bits */
	do {
		delay_usec(bit_time);
	} while (stop_bits--); 

	return c;
}

/* Transmit each char in str, excluding terminating null.
 */
void sw_serial_puts(char *str) {
	char c;
	
	while ((c = *str++)) {
		sw_serial_putc(c);
	}
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

/* Compute parity bit by XORing each bit of c and parity parameter.
 */
static char compute_parity_bit(char c) {
	char curr_bit;
	char parity_bit = parity & 1;
	
	for (int i = 0; i < 8; i++) {
		curr_bit = (c >> i) & 1;
		parity_bit ^= curr_bit;
	}
	
	return parity_bit;
}
