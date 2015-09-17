#include <avr/io.h>
#include <stdlib.h>

#define FOSC 16000000 
#define BAUD 38400
#define MYUBRR FOSC/16/BAUD-1

void serialInit( void ) {
  //Set baud rate 
  UBRR0H = (MYUBRR >> 8); 
  UBRR0L = MYUBRR;
  //enable receive/send
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  //8 data, 1 stop bits, no parity
  UCSR0C = (3<<UCSZ00);
}

void serialTransmit(unsigned char data){
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

unsigned char serialReceive( void ) {
  while ( !(UCSR0A & (1<<RXC0)) );
  return UDR0;
}

void serialFlush( void ) {
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) ) dummy = UDR0; 
}

//ls dev/tty.*
//screen /dev/tty<device> 9600
