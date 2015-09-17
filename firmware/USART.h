#ifndef USART_h
#define USART_h

#include <avr/io.h>
#include <stdlib.h>

void serialInit();

void serialTransmit(unsigned char data);

unsigned char serialReceive( void );

void serialFlush( void );

#endif