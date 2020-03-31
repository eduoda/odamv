#pragma once

#include "Arduino.h"
#include "config.h"

volatile int pos = 0;
int stopAt = MAX_POS;
int dir = 1;
float freq = 0;

void stepperDisable(){
  digitalWrite(ENA_PIN,!ENA_VAL);
}

void stepperEnable(){
  digitalWrite(ENA_PIN,ENA_VAL);
}

void stepperSetSpeed(int d, float f){
  dir = d;
  freq = f;
  cli();
  if(freq==0){
    TCCR1B |= (0 << CS11) | (0 << CS10); // 64 prescaler
  }else{
    TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
    digitalWrite(DIR_PIN,dir);
    OCR1A = (F_CPU/(64.0*freq))/2 - 1;
    TCNT1 = 0;
  }
  sei();
}

void stepperUpdateSpeed(float output){
  int d = 0;
  float f = 0;
  if(output!=0){
    d = output<0?0:1;
    f = map(abs(output),0,1000,MIN_FREQ,MAX_FREQ);
  }
  stepperSetSpeed(d, f);
}

void stepperSetup(){
  pinMode(STP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  stepperDisable();
  delay(1000);
  stepperEnable();
  pos = 0;
  
  // TIMER1
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A  = 0;
  TCCR1B|= (1 << WGM12); // CTC
  TCCR1B|= (1 << CS11) | (1 << CS10); // 64 prescaler
  TIMSK1|= (1 << OCIE1A); //enable
  sei();
}

ISR(TIMER1_COMPA_vect){
  static boolean v = false;
  if(pos == stopAt || freq==0) return;
  digitalWrite(STP_PIN, v);
  v = !v;
  if(v) pos += dir?1:-1;
}

void stepperGoTo(int p,float f){
  stopAt = p;
  if(p<pos){
    stepperSetSpeed(0, f);
    while(p<pos);
  }else{
    stepperSetSpeed(1, f);
    while(p>pos);
  }
  stepperSetSpeed(0, 0);
  stopAt = MAX_POS;
}
