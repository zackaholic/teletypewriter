/* Name: main.c
 * Author: Zack Lewis
 * Copyright: 
 * License: 
 */

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"
#include "timers.h"

  uint16_t readPulsePins() {
    uint16_t pulse = (PIND & 0b11111100)|(PINB & 0b00000011);
    return pulse;
  }

  uint16_t readSensePins() {
    uint16_t sense = ((PINB & 0b00000100) << 5)|((PINC & 0b00111111) << 1);  
    return sense;
  }

  void writeSensePins(uint8_t code) {
    PORTC = ((code & 0b01111110) >> 1);
    if(!(code & 0b10000000)) {
      PORTB &= ~(1 << 3);
    }
    _delay_us(10);

    PORTC = (1 << PC0)|(1 << PC1)|(1 << PC2)|(1 << PC3)|(1 << PC4)|(1 << PC5);
    PORTB |= (1 << 3);
  }

int main(void)
{
  //inputs all around
  DDRD &= ~(1 << PD2)|(1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)|(1 << PD7);
  DDRB &= ~(1 << PB0)|(1 << PB1)|(1 << PB2)|(1 << PB3)|(1 << PB4)|(1 << PB5);
  DDRC &= ~(1 << PC0)|(1 << PC1)|(1 << PC2)|(1 << PC3)|(1 << PC4)|(1 << PC5);
  
  //pullups enabled 
  PORTD = (1 << PD2)|(1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)|(1 << PD7);  
  PORTB = (1 << PB0)|(1 << PB1)|(1 << PB2)|(1 << PB3)|(1 << PB4)|(1 << PB5);
  PORTC = (1 << PC0)|(1 << PC1)|(1 << PC2)|(1 << PC3)|(1 << PC4)|(1 << PC5);

  SREG = (1 << 7);
  serialInit();
  timer1Init();

  uint16_t pKeypress = 0;
  uint16_t keypress;
  uint8_t keyTriggered = 0;
  uint8_t state = 2;
  uint16_t pulseState;
  uint16_t senseState;
  uint8_t pulseStart = 0b11111011;
  uint16_t sequences[8];
  uint8_t i = 0;

/*******************main loop******************/  
  for(;;){
    switch (state) {
      case 0: //wait(at the moment forever) for a keystroke
        keypress = 0;
        //wait for first pulse
        while((pulseState = readPulsePins()) != pulseStart);

        for(i = 0; i < 8; i++) {
          pulseState = readPulsePins();
          senseState = readSensePins();
          sequences[i] = (senseState << 8) | pulseState;    //for testing

          if(senseState < 254) {
            keypress = (senseState << 8) | pulseState;
            keyTriggered = 1;                               //for testing
          }
          while(pulseState == readPulsePins());
        }
        state = 1;

        //fell through the sequence without a keystroke
      break;

      case 1: //print last keystroke over serial
        if(keypress != pKeypress) {
          pKeypress = keypress;

/*
          switch (keypress) {
            case 0:
            //do nothing...FOR NOW!!!
            break;
            case 64639:
              serialTransmit('a');
            break;
            case 57086:
              serialTransmit('b');
            break;
            case 63230:
              serialTransmit('c');
            break;
            case 63103:
              serialTransmit('d');
            break;
            case 63167:
              serialTransmit('e');
            break;
            case 61055:
              serialTransmit('f');
            break;
            case 56959:
              serialTransmit('g');
            break;
            case 48767:
              serialTransmit('h');
            break;
            case 64759:
              serialTransmit('i');
            break;
            case 48887:
              serialTransmit('j');
            break;
            case 32507:
              serialTransmit('k');
            break;
            case 64763:
              serialTransmit('l');
            break;
            case 61179:
              serialTransmit('m');
            break;
            case 48894:
              serialTransmit('n');
            break;
            case 64247:
              serialTransmit('o');
            break;
            case 63223:
              serialTransmit('p');
            break;
            case 64703:
              serialTransmit('q');
            break;
            case 61119:
              serialTransmit('r');
            break;
            case 64127:
              serialTransmit('s');
            break;
            case 57023:
              serialTransmit('t');
            break;
            case 32503:
              serialTransmit('u');
            break;
            case 61182:
              serialTransmit('v');
            break;
            case 64191:
              serialTransmit('w');
            break;
            case 64254:
              serialTransmit('x');
            break;
            case 48831:
              serialTransmit('y');
            break;
            case 64766:
              serialTransmit('z');
            break;
            case 64735:
              serialTransmit('1');
            break;
            case 64223:
              serialTransmit('2');
            break;
            case 63199:
              serialTransmit('3');
            break;
            case 61151:
              serialTransmit('4');
            break;
            case 57055:
              serialTransmit('5');
            break;
            case 48863:
              serialTransmit('6');
            break;
            case 32495:
              serialTransmit('7');
            break;
            case 64751:
              serialTransmit('8');
            break;
            case 64239:
              serialTransmit('9');
            break;
            case 63215:
              serialTransmit('0');
            break;
            case 61167:
              serialTransmit('-');
            break;
            case 57071:
              serialTransmit('=');
            break;
            case 64251:
              serialTransmit(';');
            break;
            case 63227:
              serialTransmit('\'');
            break;
            case 57083:
              serialTransmit(',');
            break;
            case 48891:
              serialTransmit('.');
            break;
            case 57079:
              serialTransmit(13); //CR
              serialTransmit(10); //LF
            break;
            case 48879:
              serialTransmit(8); //BS
            break;
            case 64765:
              serialTransmit('/');
            break;
            case 61181:
              serialTransmit(' ');
            break;                        
            case 61175:
            //  serialTransmit(); it's the 1/2 key
            break;   
            default:
              serialTransmit(keypress);
            break;
          }
          */
          //testing routine
          if(keyTriggered = 1) {
            for(i = 0; i < 8; i++) {
              serialTransmit(sequences[i] >> 8);
              serialTransmit(sequences[i]);
            }
            keyTriggered = 0;
          }
        }
        state = 0;
      break;

      case 2: //trigger keystroke

          while((pulseState = readPulsePins()) != 0b11111011);

          _delay_us(8);         //delay value untested! maybe this was sometimes getting read too???
          DDRC |= (1 << PC2);   //is the read on the falling edge of the pulsing pins or at 
          PORTC &= ~(1 << PC2);   //some arbitrary time? I don't know.

          while((pulseState = readPulsePins()) != 0b11110111);
          _delay_us(10);  //this delay value SEEMS to make a difference.

          DDRC &= ~(1 << PC2);          
          PORTC |= (1 << PC2); 
          _delay_ms(500);
//          serialTransmit(PINC);   //00111111

//          state = 0;

      break;

      default:
      break;
    }

  }
  return 0;   /* never reached */
}

/*
!!!right Shift, space bar and question mark keys aren't being read!!!
a: 64639
b: 57086
c: 63230
d: 63103
e: 63167
f: 61055
g: 56959
h: 48767
i: 64759
j: 48887
k: 32507
l: 64763
m: 61179
n: 48894
o: 64247
p: 63223
q: 64703
r: 61119
s: 64127
t: 57023
u: 32503
v: 61182
w: 64191
x: 64254
y: 48831
z: 64766
space: 61181
return: 57079
left shift: 32510
right shift: 32509
1: 64735
2: 64223
3: 63199
4: 61151
5: 57055
6: 48863
7: 32495
8: 64751
9: 64239
0: 63215
-: 61167
=: 57071
backspace: 48879
,: 57083
.: 48891
/: 64765
1/2: 61175
*/
