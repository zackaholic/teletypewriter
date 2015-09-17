#ifndef timers_h
#define timers_h

#include <avr/interrupt.h>

void timer1Init(void);

uint64_t getTime(void);

ISR(TIMER1_COMPA_vect);

#endif