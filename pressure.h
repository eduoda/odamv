#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;
float envPressure = 0;

void pressureSetup(){
  if (!bme.begin(0x76)) {
    Serial.println("Erro no sensor BME280.");
    while (1);
  }
  for(int i = 0; i<100; i++){
    envPressure += bme.readPressure()/100.0F;
  }
  envPressure /= 100;
}

double getPressure(){
  return bme.readPressure()/100.0F - envPressure;
}
