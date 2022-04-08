#include <Arduino.h>
#include <Bounce.h>
// #include <Display.h>

/*
How to measure frequency:
- The timer is measured in us, but (likely due to hardware limits) it gets unclear at higher frequencies.

100 kHz is approximately a value of 5

For kHz range: use value/100*5 for desired value

Enter the floating point exactly: do not rely on microcontroller operation for the math
*/
#define driver_period 1.76056338028169014 // approximately 283 kHz
#define proto10 50000 // 10 Hz
#define proto1 500000 // 1 Hz

//Display disp;

byte pwm1 = false;
byte pwm2 = true;
byte sd_state = true;

uint8_t tms_state = 0;

// Create an IntervalTimer object 
IntervalTimer pwmTimer;
IntervalTimer tmsTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED

const int control = 11;

// Timer pins
const int reg_HIGH = 3;
const int reg_LOW = 4;

// Buzzer pin
const int SD_pin = 6; // SD pin for gate drivers

void gate_driver() {
  digitalWriteFast(reg_LOW, pwm1);
  digitalWriteFast(reg_HIGH, pwm2);
  pwm1 = !pwm1;
  pwm2 = !pwm2;
}

void tms_protocol() {
  digitalWriteFast(SD_pin, sd_state);
  sd_state = !sd_state;
  if (sd_state) {
    tone(35, 264, 10);
  } else {
    noTone(35);
  }
}

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

  pinMode(35,OUTPUT);

  Serial.begin(9600);

  //pinMode(buzzer, OUTPUT);

  // Thing
  pinMode(SD_pin, OUTPUT);
  pinMode(control, INPUT_PULLUP);
  
  pwmTimer.begin(gate_driver, driver_period); // 5 is 100 kHz, adjust accordingly
  tmsTimer.priority(100);

  digitalWrite(ledPin, 1);
  
  // digitalWriteFast(SD_pin, 0); // this should fix the thing where it needs a button press to run
}

Bounce button = Bounce(control, 10);

void loop() {
  Serial.print("Current state: ");
  Serial.println(tms_state);
  if (button.update()){
    if (button.fallingEdge()){
      tms_state++;
      if (tms_state == 0){
        // tmsTimer.end();
        Serial.println("TMS: Const");
      } else if (tms_state ==1){
        tmsTimer.begin(tms_protocol, proto1);
        Serial.println("TMS: 1Hz");
      } else if (tms_state == 2) {
        tmsTimer.update(proto10);
        Serial.println("TMS: 10Hz");
      } else {
        tms_state = 0;
        tmsTimer.end();
        digitalWriteFast(SD_pin, 1); // turn off SD pin
        noTone(35);
      }
    }
  }
}

  
