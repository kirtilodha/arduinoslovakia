/**
 * ATTiny85 Analog Comparator Interrupt + Internal reference
 * v. 1.0
 * Copyright (C) 2014 Robert Ulbricht
 *
 * On change triggers interrupt.
 * ACSR.ACO = Internal ref>AIN1
 * LED on = ACSR.ACO
 * Interrupt handler must be fast, so we just indicate into
 * main loop, that we want TX on serial port.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ 

#include <wiring_private.h>
#include <SoftwareSerial.h>

// ATMEL ATTINY85
//
//                  +-\/-+
//      (D 5) PB5  1|    |8  Vcc
//   tx (D 3) PB3  2|    |7  PB2 (D 2) led
//   rx (D 4) PB4  3|    |6  PB1 (D 1) AIN1
//            GND  4|    |5  PB0 (D 0)
//                  +----+ 

const int dig_led = 2;
const int dig_tx = 3;
const int dig_rx = 4;
SoftwareSerial s85(dig_rx, dig_tx); // RX, TX
volatile boolean detected_change=false;

///
/// Setup ports
///
void setup() {                
  s85.begin(9600);
  s85.println("ATTiny85 Analog Comparator Interrupt, internal");
  //sbi(ACSR,ACD);
  pinMode(dig_led, OUTPUT);
  cbi(ACSR,ACIE); // disable interrupt
  sbi(ACSR,ACBG); // internal reference
  //interrupt on toggle event
  cbi(ACSR,ACIS1);
  cbi(ACSR,ACIS0);
  sbi(ACSR,ACIE); // enable interrupt 
}

///
/// Main loop
///
void loop() {
  if(detected_change) {
    s85.println("toggle");
    detected_change=false;
  }
}

///
/// Analog comparator interrupt handler
///
ISR(ANA_COMP_vect) {
  int value=ACSR&(1<<ACO);
  digitalWrite(dig_led, value);
  detected_change=true;
}
