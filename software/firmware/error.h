#pragma once

#include "Arduino.h"
#include "config.h"
#include "pid.h"
#include "reference.h"
#include "stepper.h"
#include "pressure.h"

const int NO_ERROR          = 0;
const int ERR_HIGH_PRESSURE = 1;
const int ERR_LOW_PRESSURE  = 2;
const int ERR_ENDSTOP       = 3;

int error = NO_ERROR;
void errorRun(){
  if(input>=MAX_PRESSURE){
    error = ERR_HIGH_PRESSURE;
  }else if(input<PEEP*0.9){
    error = ERR_LOW_PRESSURE;
  }else if(pos>=MAX_POS || pos<=MIN_POS){
    error = ERR_ENDSTOP;
  }
  if(error){
    tone(BUZZER_PIN, 50, 500);
    digitalWrite(LED_PIN,HIGH);
    // Serial.print('ERROR ');
    // Serial.println(error);
  }
}

void errorReset(){
  error = NO_ERROR;
}
