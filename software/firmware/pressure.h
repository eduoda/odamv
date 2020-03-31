#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#if HW != INSPIREHW
  Adafruit_BME280 bme;
  float envPressure = 0;
#endif

void pressureSetup(){
  #if HW != INSPIREHW
    if (!bme.begin(0x76)) {
      Serial.println("Erro no sensor BME280.");
      while (1);
    }
    for(int i = 0; i<100; i++){
      envPressure += bme.readPressure()/100.0F;
    }
    envPressure /= 100;
  #endif
}

float getPressure(){
  #if HW == INSPIREHW
    return map(analogRead(PRESSURE_PIN),219,552,0,20);
  #else
    return bme.readPressure()/100.0F - envPressure;
  #endif
}
