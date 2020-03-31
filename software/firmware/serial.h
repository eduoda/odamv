#pragma once

#include "Arduino.h"

void serialRun(){
  static unsigned long tic = millis();
  unsigned long tac = millis();
  if(tac-tic<100) return;
  Serial.print(reference); Serial.print(" ");
  Serial.print(input+reference); Serial.print(" ");
  Serial.print(output);
  Serial.println();
  tic = tac;
}


void serialEvent() {
  static String s = "";
  static char cmd = '\0';
  static float v = 0;
  while (Serial.available()) {
    char c = (char)Serial.read();
    double RR = 8; // respiratory rate
    double PEEP = 2;
    double PIP = 12;
    double PPLAT = 10;

    if(c=='P' || c=='I' || c=='D' || c == 'E' || c == 'W' || c == 'X' || c == 'Y' || c == 'Z'){
      cmd = c;
    }else if(c!='\n'){
      s+=c;
    }else{
      v = s.toFloat();
      if(cmd=='P'){
        pidSetP(v);
      }else if(cmd=='I'){
        pidSetI(v);
      }else if(cmd=='D'){
        pidSetD(v);
      }else if(cmd=='E'){
        errorReset();
      }else if(cmd=='W'){
        referenceSetRR(v);
      }else if(cmd=='X'){
        referenceSetPEEP(v);
      }else if(cmd=='Y'){
        referenceSetPIP(v);
      }else if(cmd=='Z'){
        referenceSetPPLAT(v);
      }
      s = "";
      c = '\0';
    }
  }    
}
