#pragma once

#include "Arduino.h"
#include "config.h"

unsigned long T0;
double RR = 8; // respiratory rate
double PEEP = 2;
double PIP = 12;
double PPLAT = 10;

double getReference(){
  static unsigned long T3 = 60000/RR;
  static unsigned long T2 = T3/2;
  static unsigned long T1 = T2/3;
  unsigned long t = millis()-T0;
  if(t<T1) return PIP;
  if(t<T2) return PPLAT;
  if(t<T3) return PEEP;
  T0=millis();
  return PEEP;
}

void referenceSetRR   (double v) { RR    = v; }
void referenceSetPEEP (double v) { PEEP  = v; }
void referenceSetPIP  (double v) { PIP   = v; }
void referenceSetPPLAT(double v) { PPLAT = v; }
