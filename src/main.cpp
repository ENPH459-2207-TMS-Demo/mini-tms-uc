#include <Arduino.h>

#define driver_period 1000

uint8_t tms_state = false;
uint16_t tms_1hz = 1e6; // for 1 second timers (timers are measured in microseconds)
uint16_t tms_10hz = 10e6;

uint8_t pwm1 = false;
uint8_t pwm2 = true;

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
  digitalWrite(reg_LOW, pwm1);
  digitalWrite(reg_HIGH, pwm2);
  pwm1 = !pwm1;
  pwm2 = !pwm2;
}

void tms_protocol() {
  
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
}

// Variables for timed outputs
elapsedMillis protocol1;
elapsedMillis protocol2;

const int proto_1_period = 1000;
const int proto_2_period = 10000;

void loop() {

  // put your main code here, to run repeatedly:
  // if (control == LOW){
  //   if (protocol1 >= proto_1_period) {
  //     digitalWrite(SD_pin1, HIGH);
  //     Serial.println("Protocol 1 on");
  //     protocol1 = protocol1 - proto_1_period;
  //   } else {
  //     digitalWrite(SD_pin1, LOW);
  //     Serial.println("Protocol 1 off");
  //   }

  //   if (protocol1 >= proto_2_period) {
  //     digitalWrite(SD_pin2, HIGH);
  //     protocol2 = protocol2 - proto_2_period;
  //     Serial.println("Protocol 2 on");
  //   } else {
  //     digitalWrite(SD_pin2, LOW);
  //     Serial.println("Protocol 2 off");
  //   }
  // } else {
  //   digitalWrite(SD_pin1, HIGH);
  //   digitalWrite(SD_pin2, HIGH);
  // }

  // // TODO: fix this for SD pins
  // if (protocol1 >= proto_1_period) {
  //     digitalWrite(SD_pin1, HIGH);
  //     Serial.println("Protocol 1 on");
  //     protocol1 = protocol1 - proto_1_period;
  //   } else {
  //     digitalWrite(SD_pin1, LOW);
  //     Serial.println("Protocol 1 off");
  //   }

  // if (protocol1 >= proto_2_period) {
  //   digitalWrite(SD_pin2, HIGH);
  //   protocol2 = protocol2 - proto_2_period;
  //   Serial.println("Protocol 2 on");
  // } else {
  //   digitalWrite(SD_pin2, LOW);
  //   Serial.println("Protocol 2 off");
  // }
}
