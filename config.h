#pragma once

#define SAMPLE_TIME 50
#define K_P 10.00
#define K_I  0.12
#define K_D  0.06

#define MAX_PRESSURE 60

#define ODAHW     1
#define INSPIREHW 2

#define HW ODAHW

#if HW == ODAHW
  #define STP_PIN 2
  #define DIR_PIN 5
  #define ENA_PIN 8
  #define ENA_VAL 0
  #define MAX_FREQ 19000
  #define MIN_FREQ  1000
  #define MAX_POS  19200
  #define MIN_POS -19200
#elif HW == INSPIREHW
  #define STP_PIN 2
  #define DIR_PIN 5
  #define ENA_PIN 8
  #define ENA_VAL 1
  #define MAX_FREQ 3900
  #define MIN_FREQ  200
  #define MAX_POS  1000
  #define MIN_POS -1000
#else
  #error "Unknown HARDWARE"
#endif


#define BUZZER_PIN 12
#define LED_PIN    13
