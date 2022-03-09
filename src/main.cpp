#include <Arduino.h>

#define tms_PULSE 100000 // in us (1e6 is 1 Hz, 1e5 is 10 Hz)
#define driver_period 100/3

void gate_driver() {
  digitalWrite(pin2, pwmState);
  pwmState = !pwmState;
  digitalWrite(pin1, pwmState);
}

// Create an IntervalTimer object 
IntervalTimer pwmTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED
const int pin1 = 1;
const int pin2 = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  Serial.begin(9600);
  pwmTimer.begin(gate_driver, driver_period); // 5 is 100 kHz, adjust accordingly
}

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int tmsState = LOW;
int pwmState = LOW;

void loop() {
  // put your main code here, to run repeatedly:
}