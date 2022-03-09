#include <Arduino.h>

#define tms_PULSE 100000 // in us (1e6 is 1 Hz, 1e5 is 10 Hz)
#define driver_period 100/3

void gate_driver() {
  digitalWrite(reg_LOW, pwmState);
  pwmState = !pwmState;
  digitalWrite(reg_HIGH, pwmState);
}

// Create an IntervalTimer object 
IntervalTimer pwmTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED

// Timer pins
const int reg_HIGH = 3;
const int reg_LOW = 4;

const int proto_HIGH = 5;
const int proto_LOW = 6;

const int control = 34;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(reg_HIGH, OUTPUT);
  pinMode(reg_LOW, OUTPUT);

  pinMode(control, INPUT_PULLDOWN);
  pinMode(proto_HIGH, OUTPUT);
  pinMode(proto_LOW, OUTPUT);
  
  pwmTimer.begin(gate_driver, driver_period); // 5 is 100 kHz, adjust accordingly
}

int pwmState = LOW;

// Variables for timed outputs
elapsedMicros protocol1;
elapsedMicros protocol2;

void loop() {
  // put your main code here, to run repeatedly:
  if (control == HIGH){
    if (sinceTest1 >= 1000) {
      sinceTest1 = sinceTest1 - 1000;
    }
  } else {

  }
}