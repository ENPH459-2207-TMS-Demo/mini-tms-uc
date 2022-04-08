/* Mini-TMS Main Code
 * 
 * This `main.cpp` file is meant to run on a Teensy 3.5 (https://www.pjrc.com/store/teensy35.html) and provides functionalities for
 * the mini-TMS project created as part of an ENPH 459 capstone project.
 * 
 * Requirements:
 *  - PlatformIO (https://platformio.org/) with Teensy add-on (https://registry.platformio.org/platforms/platformio/teensy)
 *  - Arduino framework
 * 
 * Please contact clee125@student.ubc.ca for any questions about the code.
 */

#include <Arduino.h>
#include <Bounce.h>

/* To implement the 180 out of phase waveforms, a timer interrupt was utilized so that two digital pins can be turned on/off at a given frequency.
 * (Essentially bit-bang)
 * 
 * The timer is measured in us, but (likely due to hardware limits) it gets unclear at higher frequencies (proven for above 300 kHz).
 * 100 kHz is approximately a value of 5, so for kHz range, use value/100*5 for desired value
 * Enter the floating point value for period from outside calculations: do not rely on microcontroller operation for the math
 * 
 * More information: https://www.pjrc.com/teensy/td_timing_IntervalTimer.html
*/

// Frequency definitions for different timers
#define driver_period 1.76056338028169014 // approximately 283 kHz
#define proto10 50000 // 10 Hz
#define proto1 500000 // 1 Hz

// IntervalTimer objects for the PWM waveforms and the TMS protocol timers 
IntervalTimer pwmTimer;
IntervalTimer tmsTimer;

// Digital states for bit-bang PWM and TMS protocols
byte pwm1 = false;
byte pwm2 = true;
byte sd_state = true;

uint8_t tms_state = 0; // Variable for simple state machine

const int ledPin = LED_BUILTIN;  // built-in LED

// Button pin
const int control = 11;

// Timer pins
const int reg_HIGH = 3;
const int reg_LOW = 4;

// Gate driver pin
const int SD_pin = 6;

// Buzzer pin
const int buzzer = 35;


// Timer interrupt functions for the PWM output (gate_driver) and the SD pin output (tms_protocol)
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
    tone(buzzer, 264, 10);
  } else {
    noTone(buzzer);
  }
}

void setup() {
  Serial.begin(9600);

  // Pin definitions
  pinMode(control, INPUT_PULLUP);
  pinMode(SD_pin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(reg_HIGH, OUTPUT);
  pinMode(reg_LOW, OUTPUT);
  
  // Set up PWM timer
  pwmTimer.begin(gate_driver, driver_period);
  pwmTimer.priority(100);

  digitalWrite(ledPin, 1); // Turns on built-in LED
  
  // The current code does not turn on the mini-TMS when power is first supplied and a button press is requirement for it to enter the 3 states.
  // We decided to make this a feature. If you'd like to change it, add the commented line below.

  // digitalWriteFast(SD_pin, 0);
}

// Configure button pin for software debouncing
Bounce button = Bounce(control, 10);

void loop() {
  Serial.print("Current state: ");
  Serial.println(tms_state);
  if (button.update()){
    if (button.fallingEdge()){
      tms_state++;
      if (tms_state == 0){
        // State 0: Constant on
        Serial.println("TMS: Const");
      } else if (tms_state ==1){
        // State 1: 1 Hz protocol
        tmsTimer.begin(tms_protocol, proto1);
        Serial.println("TMS: 1Hz");
      } else if (tms_state == 2) {
        // State 2: 10 Hz protocol
        tmsTimer.update(proto10);
        Serial.println("TMS: 10Hz");
      } else {
        // State 3: Reset to state 0
        tms_state = 0;
        tmsTimer.end();
        digitalWriteFast(SD_pin, 1); // turn off SD pin
        noTone(buzzer);
      }
    }
  }
}

  
