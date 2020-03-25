#include <PID_v1.h>

unsigned long T0;
double RR = 10; // respiratory rate
double PEEP = 5;
double PIP = 22;
double PPLAT = 20;
double stepperDirPin = 1;
double stepperStpPin = 2;

double setpoint=0, input=0, output=128;
double Kp=0,Ki=15,Kd=0.;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, P_ON_M, DIRECT);

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
  pinMode(stepperDirPin, OUTPUT);
  pinMode(stepperStpPin, OUTPUT);
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(10);
  Serial.begin(9600);
  while(!Serial);
  delay(100);
}

void loop() {
  setpoint = getSetpoint();
  input = getPressure();
  pid.Compute();
  stepper();
  printState();
}

double getPressure(){
  return output/4;
}

int stepperDelay = 0;
int stepperDir = 1;
void updateStepperSpeed(){
  double d = output-128;
  if(d==0){
    stepperDelay = 0;
  }else{
    stepperDir = d<0?-1:1;
    digitalWrite(stepperDirPin,stepperDir);
    stepperDelay = map(abs(d),1,128,20000,1000);
  }
}
void stepper(){
  updateStepperSpeed();
  static unsigned long tic = micros();  
  if(!stepperDelay){
    digitalWrite(stepperStpPin,LOW);
    return;
  }
  unsigned long tac = micros();
  if(tac<tic+200)
    digitalWrite(stepperStpPin,HIGH);
  else if(tac<tic+stepperDelay)
    digitalWrite(stepperStpPin,LOW);
  else
    tic = tac;
}

void printState(){
  static unsigned long tic = millis();
  unsigned long tac = millis();
  if(tac-tic<50) return;
  Serial.print("setpoint: ");Serial.print(setpoint); Serial.print(" ");
  Serial.print("input: ");Serial.print(input); Serial.print(" ");
//  Serial.print("output: ");Serial.print(output/12.8); Serial.print(" ");
//  Serial.print("stepperDelay: ");Serial.print(stepperDir*stepperDelay/1000); Serial.print(" ");
  Serial.println();
  tic = tac;
}
