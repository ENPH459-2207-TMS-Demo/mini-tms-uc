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
#define proto1 1000000
#define proto10 10000000

//Display disp;

byte tms_state = false;
byte pwm1 = false;
byte pwm2 = true;
byte sd_state = true;

// Create an IntervalTimer object 
IntervalTimer pwmTimer;
IntervalTimer tmsTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED

// TODO: Test these buttons
const int button_const = 11;
const int button_1hz = 12;
const int button_10hz = 13;

// Timer pins
const int reg_HIGH = 3;
const int reg_LOW = 4;

// Buzzer pin
const int buzzer = 9;

const int SD_pin = 6; // SD pin for gate drivers

const int power = 34;

void gate_driver() {
  digitalWriteFast(reg_LOW, pwm1);
  digitalWriteFast(reg_HIGH, pwm2);
  pwm1 = !pwm1;
  pwm2 = !pwm2;
}

void tms_protocol() {
  digitalWriteFast(SD_pin, sd_state);
  sd_state = !sd_state;
}

// Code below is used for ISR implementation of buttons (debouncing done outside)
// void ISR_const(){
//   tmsTimer.end();
// }

// void ISR_1hz(){
//   tmsTimer.begin(tms_protocol,proto1);
// }

// void ISR_10hz(){
//   tmsTimer.begin(tms_protocol,proto10);
// }void ISR_const(){
//   tmsTimer.end();
// }

// void ISR_1hz(){
//   tmsTimer.begin(tms_protocol,proto1);
// }

// void ISR_10hz(){
//   tmsTimer.begin(tms_protocol,proto10);
// }

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(reg_HIGH, OUTPUT);
  pinMode(reg_LOW, OUTPUT);

  pinMode(power, INPUT_PULLDOWN);

  pinMode(buzzer, OUTPUT);

  // Thing
  pinMode(SD_pin, OUTPUT);
  
  pwmTimer.begin(gate_driver, driver_period); // 5 is 100 kHz, adjust accordingly
  tmsTimer.priority(100);
  // disp.setup();
  // TODO: below is inplementation of interrupt pins
  // attachInterrupt(digitalPinToInterrupt(button_const), ISR_const,RISING);
  // attachInterrupt(digitalPinToInterrupt(button_1hz), ISR_1hz,RISING);
  // attachInterrupt(digitalPinToInterrupt(button_10hz), ISR_10hz,RISING);
}

// TODO: test this; debouncing coded at 10 ms atm
Bounce constButton = Bounce(button_const, 10);
Bounce oneButton = Bounce(button_1hz, 10);
Bounce tenButton = Bounce(button_10hz, 10);

void loop() {
  volatile byte state_copy; // copy of sd_state
  if (constButton.update()){
    if (constButton.fallingEdge()) { // this is for pin button to gnd, pressed
      //disp.println("TMS Protocol: OFF");
      state_copy = false;
      tmsTimer.end();
    }
  } else if (oneButton.update()){
    if (oneButton.fallingEdge()){
      //disp.println("TMS Protocol: 1 Hz");
      tmsTimer.begin(tms_protocol,proto1);
    }
  } else if (tenButton.update()){
    if (tenButton.fallingEdge()){
      //disp.println("TMS Protocol: 10 Hz");
      tmsTimer.begin(tms_protocol,proto10);
    }
  }
  //disp.clear();
  
  // TODO: test if this causes some delay in the uc that is non-negligible
  noInterrupts();
  state_copy = sd_state;
  interrupts();

  if (state_copy){
    analogWriteFrequency(buzzer, 1000);
  } else {
    analogWriteFrequency(buzzer, 0);
  }
}

  