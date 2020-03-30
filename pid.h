#pragma once

#include "Arduino.h"
#include "config.h"
#include "reference.h"
#include "stepper.h"
#include "pressure.h"
#include <PID_v1.h>

double setpoint=0, input=0, output=0;
double kp=K_P,ki=K_I,kd=K_D;
PID pid(&input, &output, &setpoint, K_P, K_I, K_D, P_ON_M, DIRECT);
double reference = 0;

void pidSetup(){
  output = 0;
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(SAMPLE_TIME);
  pid.SetOutputLimits(-1000,1000);
  stepperUpdateSpeed(0);
}

void pidReset(){
  pid.SetMode(MANUAL);
  stepperGoTo(0,MIN_FREQ);
  stepperDisable();
  pid.SetMode(AUTOMATIC);
}

void pidRun(){
  reference = getReference();
  float pressure = getPressure();
  input = pressure - reference;

  if(pressure>MAX_PRESSURE){
    pidReset();
  }
  if(reference == PEEP){
    pid.SetMode(MANUAL);
    output = 0;
    stepperGoTo(0,MAX_FREQ/2);
    stepperDisable();
    pid.SetMode(AUTOMATIC);
  }else{
    stepperEnable();
    if(pid.Compute())
      stepperUpdateSpeed(output);
  }
  
}

void pidSetP(double v){
  kp = v;
  pid.SetTunings(kp,ki,kd);
}
void pidSetI(double v){
  ki = v;
  pid.SetTunings(kp,ki,kd);
}
void pidSetD(double v){
  kd = v;
  pid.SetTunings(kp,ki,kd);
}
void pidSetPID(double _p,double _i,double _d){
  kp = _p;
  ki = _i;
  kd = _d;
  pid.SetTunings(kp,ki,kd);
}
