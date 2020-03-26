#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define K_P 0.09
#define K_I 0.09
#define K_D 0.01

#define STP_PIN 7
#define DIR_PIN 4
#define ENA_PIN 8

unsigned long T0;
double RR = 8; // respiratory rate
double PEEP = 2;
double PIP = 12;
double PPLAT = 10;

double setpoint=0, input=0, output=0;
PID pid(&input, &output, &setpoint, K_P, K_I, K_D, P_ON_M, DIRECT);
PID_ATune aTune(&input, &output);
double kp=K_P,ki=K_I,kd=K_D;
int tuningMode = 0;

Adafruit_BME280 bme;

double getSetpoint(){
  static unsigned long T3 = 60000/RR;
  static unsigned long T2 = T3/2;
  static unsigned long T1 = T2/3;
  unsigned long t = millis()-T0;
  if(t<T1) return PIP;
  if(t<T2) return PPLAT;
  if(t<T3) return PEEP;
  T0=millis();
  return PEEP;
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  setupStepper();
  setupPID();
  setupBME();
  updateStepperSpeed();
}

void setupPID(){
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(20);
  pid.SetOutputLimits(-1000,1000);
  //aTune.SetNoiseBand(1);
  aTune.SetOutputStep(-20);
  //aTune.SetLookbackSec(20);
}

void setupBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Erro no sensor BME280.");
    while (1);
  }
}

void loop() {
  setpoint = getSetpoint();
  input = getPressure();

  if(tuningMode==1) {
    if(aTune.Runtime()){
      //we're done, set the tuning parameters
      tuningMode = 2;
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      Serial.print("AUTOTUNE: ");
      Serial.print(kp);
      Serial.print(" ");
      Serial.print(ki);
      Serial.print(" ");
      Serial.println(kd);
      pid.SetTunings(kp,ki,kd);
      pid.SetMode(AUTOMATIC);
    }
    updateStepperSpeed();
  } else {
    if(pid.Compute())
      updateStepperSpeed();
  }

  printState();
}

double getPressure(){
  return bme.readPressure()/100.0F - 921.0;
}

void updateStepperSpeed(){
  cli();
  int stepperFreq = 0;
  int stepperDir = 1;
  if(output==0){
    TCCR1B |= (0 << CS11) | (0 << CS10); // 64 prescaler
  }else{
    TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
    stepperDir = output<0?0:1;
    stepperFreq = map(abs(output),0,1000,1000,20000);
    digitalWrite(DIR_PIN,stepperDir);
    OCR1A = (F_CPU/(64.0*stepperFreq))/2 - 1;
    TCNT1 = 0;
  }
  sei();
}

void printState(){
  static unsigned long tic = millis();
  unsigned long tac = millis();
  if(tac-tic<100) return;
  Serial.print(setpoint); Serial.print(" ");
  Serial.print(input); Serial.print(" ");
//  Serial.print(output); Serial.print(" ");
  if(tuningMode==1){
    Serial.print("tuning mode");
  } else if(tuningMode==2){
    Serial.print("kp: ");Serial.print(pid.GetKp());Serial.print(" ");
    Serial.print("ki: ");Serial.print(pid.GetKi());Serial.print(" ");
    Serial.print("kd: ");Serial.print(pid.GetKd());Serial.print(" ");
  }
  Serial.println();
  tic = tac;
}

void setupStepper(){
  pinMode(STP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  digitalWrite(ENA_PIN,1);
  delay(2000);
  digitalWrite(ENA_PIN,0);
  
  // TIMER1
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A  = 0;
  TCCR1B |= (1 << WGM12); // CTC
  TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
  TIMSK1 |= (1 << OCIE1A); //enable
  sei();
}
ISR(TIMER1_COMPA_vect){
  static boolean v = false;
  digitalWrite(STP_PIN, v);
  v = !v;
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
