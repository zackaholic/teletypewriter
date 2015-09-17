/*
Notes: 
In CTC mode with OCR1A set as TOP the overflow interrupt WILL NOT trigger.
Use the OCR1A compare match interrupt. I guess the overflow interrupt is only for
legit overflows of the maximum register value.
*/
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

volatile uint64_t _ms = 0;

void timer1Init(void) {
  TCCR1A = (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << WGM11)|(0 << WGM10);
  //CTC mode, OCR1A = TOP, prescaler = 64: 250000clock
  TCCR1B = (0 << ICNC1)|(0 << ICES1)|(0 << WGM13)|(1 << WGM12)|(0 << CS12)|(1 << CS11)|(1 << CS10);
  //TOP = 250: TOIE interrupt every 1ms
  OCR1A = 250;
  OCR1B = 0;
  TCNT1 = 0;
  //overflow interrupt enabled
  TIMSK1 = (0 << ICIE1)|(0 << OCIE1B)|(1 << OCIE1A)|(0 << TOIE1);
}


ISR(TIMER1_COMPA_vect) {
  _ms += 1;
}

uint64_t getTime(void) {
  return _ms;
}