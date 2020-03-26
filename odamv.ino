#include "config.h"
#include "pid.h"
#include "serial.h"


void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  stepperSetup();
  pressureSetup();
  pidSetup();
}

void loop() {
  pidRun();
  serialRun();
}
