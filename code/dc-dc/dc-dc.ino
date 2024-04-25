// DC-DC Converter
// Nate Seibold and Owen Cook

#include <myTimer.h> // include library for PWM

#define feedbackPin A1 // define output pins
#define outPin 9

#define period 25 // define period

float pulseWidth = 100.0; // declare starting pulse width

float targetVoltage = 1.2; // set target voltage here, 1.2 V is chosen for motor output

float voltageDivided = (targetVoltage * 0.4); // convert desired voltage to feedback voltage based on voltage divider

void setup() {
  initPWM_fast(period*16); // begin PWM
  Serial.begin(9600); // begin serial monitor
}

void loop() {
  int currentOutput = analogRead(feedbackPin); // read feedback voltage as analog value
  float voltage = currentOutput * (5.0 / 1023.0); // convert from analog value to true voltage
  Serial.println(voltage / 0.4); // print output voltage (for testing)

  float difference = voltage - voltageDivided; // calculate difference between desired output and current output
 
/*
  Below if statements adjust pulse width based on difference from desired voltage.
  The pulse width is raised if a higher output is required, and lowered for a lower output.
  When the voltage is within 0.075 V of target, pulse width is incremented at 1/1000th of the scale.
  Outside of 0.075 V, the pulse width is adjusted in larger increments to prioritize speed over accuracy.
  Finally, a protection statement is included at the end, to reset the pulse width to a predetermined safe value to prevent accidental high voltages.
 */

  if (difference <= 0.075 && difference > 0) {
    pulseWidth += -0.001;
  }

  if (difference >= -0.075 && difference < 0) {
    pulseWidth += 0.001;
  }

  if (difference < -0.075) {
    pulseWidth += 1.00;
  }

  if (difference > 0.075) {
    pulseWidth += -1.00;
  }

  if (voltage > 4.0) {
    pulseWidth = 100.0;
  }

  pwmPW_PER_fast(pulseWidth, period*16); // set PWM to new pulse width value

}