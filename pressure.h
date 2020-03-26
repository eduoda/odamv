#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void pressureSetup(){
  if (!bme.begin(0x76)) {
    Serial.println("Erro no sensor BME280.");
    while (1);
  }
}

double getPressure(){
  return bme.readPressure()/100.0F - 921.0;
}
