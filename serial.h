#pragma once

#include "Arduino.h"

void serialRun(){
  static unsigned long tic = millis();
  unsigned long tac = millis();
  if(tac-tic<100) return;
  Serial.print(setpoint); Serial.print(" ");
  Serial.print(input); Serial.print(" ");
  Serial.println();
  tic = tac;
}


void serialEvent() {
  static String s = "";
  static char cmd = '\0';
  static float v = 0;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if(c=='P' || c=='I' || c=='D'){
      cmd = c;
    }else if(c!='\n'){
      s+=c;
    }else{
      v = s.toFloat();
      if(cmd=='P')
        kp = v;
      else if(cmd=='I')
        ki = v;
      else if(cmd=='D')
        kd = v;
      pid.SetTunings(kp,ki,kd);
      s = "";
      c = '\0';
    }
  }    
}
