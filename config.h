#pragma once

#define SAMPLE_TIME 50

#define MAX_PRESSURE 60

#define ODAHW     1
#define INSPIREHW 2

#define HW INSPIREHW

#if HW == ODAHW
  #define K_P 10.00
  #define K_I  0.12
  #define K_D  0.06  
  #define STP_PIN 2
  #define DIR_PIN 5
  #define ENA_PIN 8
  #define ENA_VAL 0
  #define MAX_FREQ   19000
  #define MIN_FREQ    1000
  #define HOMING_FREQ 3000
  #define MAX_POS  19200
  #define MIN_POS -19200
#elif HW == INSPIREHW
  #define K_P  40.70
  #define K_I   0.16
  #define K_D   0.00
  #define STP_PIN 4
  #define DIR_PIN 3
  #define ENA_PIN 2
  #define ENA_VAL 1
  #define MAX_FREQ    3600
  #define MIN_FREQ     200
  #define HOMING_FREQ 1200
  #define MAX_POS   920
  #define MIN_POS  -920
#else
  #error "Unknown HARDWARE"
#endif

#define BUZZER_PIN 12
#define LED_PIN    13
