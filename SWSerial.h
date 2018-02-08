/*
 * SWSerial.h
 *
 * Created: 2/4/2018 4:09:15 PM
 *  Author: Ryan
 */ 


#ifndef SWSERIAL_H_
#define SWSERIAL_H_

void init_sw_serial(int, int, long, int);
void sw_serial_putc(char c);
void sw_serial_puts(char *str);
char sw_serial_getc(void);


#endif /* SWSERIAL_H_ */