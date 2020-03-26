#pragma once

#include "Arduino.h"
#include "config.h"

void stepperUpdateSpeed(float output){
  cli();
  int stepperFreq = 0;
  int stepperDir = 1;
  if(output==0){
    TCCR1B |= (0 << CS11) | (0 << CS10); // 64 prescaler
  }else{
    TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
    stepperDir = output<0?0:1;
    stepperFreq = map(abs(output),0,1000,1000,20000);
    digitalWrite(DIR_PIN,stepperDir);
    OCR1A = (F_CPU/(64.0*stepperFreq))/2 - 1;
    TCNT1 = 0;
  }
  sei();
}

void stepperSetup(){
  pinMode(STP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  digitalWrite(ENA_PIN,1);
  delay(2000);
  digitalWrite(ENA_PIN,0);
  
  // TIMER1
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A  = 0;
  TCCR1B |= (1 << WGM12); // CTC
  TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
  TIMSK1 |= (1 << OCIE1A); //enable
  sei();
}

ISR(TIMER1_COMPA_vect){
  static boolean v = false;
  digitalWrite(STP_PIN, v);
  v = !v;
}

void stepperDisable(){
  digitalWrite(ENA_PIN,1);
}
void stepperEnable(){
  digitalWrite(ENA_PIN,0);
}
