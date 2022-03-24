#include <Arduino.h>
#include <Audio.h>

/*
How to measure frequency:

- The timer is measured in us, but (likely due to hardware limits) it gets unclear at higher frequencies.

100 kHz is approximately a value of 5

For kHz range: use value/100*5 for desired value

Enter the floating point exactly: do not rely on microcontroller operation for the math
*/
#define driver_period 1.76056338028169014 // approximately 283 kHz

uint8_t tms_state = false;

uint8_t pwm1 = false;
uint8_t pwm2 = true;

uint8_t sd_state = true;

// Create an IntervalTimer object 
IntervalTimer pwmTimer;
IntervalTimer tmsTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED

// Timer pins
const int reg_HIGH = 3;
const int reg_LOW = 4;

const int SD_pin = 6; // SD pin for one of the gate driversz

const int power = 34;

void gate_driver() {
  digitalWrite(reg_LOW, pwm1);
  digitalWrite(reg_HIGH, pwm2);
  pwm1 = !pwm1;
  pwm2 = !pwm2;
}

void tms_protocol() {
  digitalWrite(SD_pin, sd_state);
  sd_state = !sd_state;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(reg_HIGH, OUTPUT);
  pinMode(reg_LOW, OUTPUT);

  pinMode(power, INPUT_PULLDOWN);

  // Thing
  pinMode(SD_pin, OUTPUT);
  
  pwmTimer.begin(gate_driver, driver_period); // 5 is 100 kHz, adjust accordingly
}

// Variables for timed outputs
elapsedMillis protocol1;
elapsedMillis protocol2;

const int proto_1_period = 1000;
const int proto_2_period = 10000;

elapsedMillis sinceTest1;
elapsedMillis sinceTest2;
elapsedMillis sinceTest3;

void loop() {
  if (sinceTest1 >= proto_1_period) {
    sinceTest1 = sinceTest1 - proto_1_period;
    digitalWrite(SD_pin, sd_state);
    sd_state = !sd_state;
  }
}

  