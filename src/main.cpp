#include <Arduino.h>

#define driver_period 50/3

int tms_state = false;
int pwmState = false;

// Create an IntervalTimer object 
IntervalTimer pwmTimer;
IntervalTimer tmsTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED

// Timer pins
const int reg_HIGH = 3;
const int reg_LOW = 4;

const int proto_HIGH = 5;
const int SD_pin1 = 6; // SD pin for one of the gate drivers
const int proto_LOW = 7;
const int SD_pin2 = 8;

const int control = 34;

void gate_driver() {
  digitalWrite(reg_LOW, pwmState);
  pwmState = !pwmState;
  digitalWrite(reg_HIGH, pwmState);
}

void tms_protocol() {
  digitalWrite(proto_LOW, tms_state);
  tms_state = !tms_state;
  digitalWrite(proto_HIGH, tms_state);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(reg_HIGH, OUTPUT);
  pinMode(reg_LOW, OUTPUT);

  pinMode(control, INPUT_PULLDOWN);
  pinMode(proto_HIGH, OUTPUT);
  pinMode(proto_LOW, OUTPUT);

  // Thing
  pinMode(SD_pin1, OUTPUT);
  pinMode(SD_pin2, OUTPUT);
  
  pwmTimer.begin(gate_driver, driver_period); // 5 is 100 kHz, adjust accordingly
  tmsTimer.begin(tms_protocol, driver_period);
}

// Variables for timed outputs
elapsedMicros protocol1;
elapsedMicros protocol2;

const int proto_1_period = 10000000;
const int proto_2_period = 100000000;

void loop() {
  // put your main code here, to run repeatedly:
  if (control == HIGH){
    if (protocol1 >= proto_1_period) {
      digitalWrite(SD_pin1, HIGH);
      protocol1 = protocol1 - proto_1_period;
    } else {
      digitalWrite(SD_pin1, LOW);
    }

    if (protocol1 >= proto_2_period) {
      digitalWrite(SD_pin2, HIGH);
      protocol2 = protocol2 - proto_2_period;
    } else {
      digitalWrite(SD_pin2, LOW);
    }
  } else {
    digitalWrite(SD_pin1, HIGH);
    digitalWrite(SD_pin2, HIGH);
  }
}
