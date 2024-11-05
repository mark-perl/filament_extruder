#include <Arduino.h>
#include "AccelStepper.h"

// Pin definitions
#define CLOCK_PIN 12
#define DATA_PIN 11
#define ZERO_BUTTON_PIN 4 // Pin connected to the zero button
const int dirPin = 2;   // Direction pin for the motor
const int stepPin = 3;  // Step pin for the motor

// Motor setup
AccelStepper motor_x (1, stepPin, dirPin);
int baseSpeed = 500;  // Speed in steps/s at the target measurement
float newSpeed = baseSpeed;
int maxSpeed = 5000; // Max speed limit

// Target measurement settings
const int targetMeasurement = 1.75;  // OR 2.85mm ... Base measurement in mm 
const float speedIncrementFactor = 0.1;  // 10% speed increment per measurement increment
const float measurementIncrement = 5.0;  // Speed adjusts every 5mm

// Caliper variables
int sign;
long value;
float result = 0;
float offset = 0; // Offset value for zeroing
float displayValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10); // Set the maximum milliseconds to wait for serial data
  
  // Set up the motor pins
  motor_x.setMaxSpeed(maxSpeed);  // Max speed in steps per second
  motor_x.setSpeed(baseSpeed);    // Initial test speed

  // Set up the caliper pins
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  pinMode(ZERO_BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor

  Serial.println("System ready");
}

void loop() {
  // Check if zero button is pressed
  if (digitalRead(ZERO_BUTTON_PIN) == LOW) {
    zeroCaliper();  // Call function to zero the caliper
    delay(200);     // Simple debounce delay for button press
  }

  // Read data from the digital caliper
  if (readCaliper()) {
    // Apply the offset to zero the reading
    displayValue = result - offset;
    Serial.print("Measurement: ");
    Serial.print(displayValue);
    Serial.println(" mm");

    // Adjust motor speed based on measurement
    adjustMotorSpeed(displayValue);
  }

  motor_x.runSpeed();  // Run the motor at the adjusted speed
}

// Reads data from the caliper
bool readCaliper() {
  // Wait for the clock pin to go high
  while (digitalRead(CLOCK_PIN) == LOW) {}

  unsigned long tmpTime = micros();  // Record the time when the clock goes high

  // Wait for the clock pin to go low
  while (digitalRead(CLOCK_PIN) == HIGH) {}

  // Check if the signal is valid (debounce)
  if ((micros() - tmpTime) < 500) return false;

  sign = 1;
  value = 0;

  // Read 24 bits from the caliper
  for (int i = 0; i < 24; i++) {
    // Wait for the clock pin to go high
    while (digitalRead(CLOCK_PIN) == LOW) {}
    // Wait for the clock pin to go low
    while (digitalRead(CLOCK_PIN) == HIGH) {}

    // Read the data bit
    if (digitalRead(DATA_PIN) == HIGH) {
      if (i < 20) value |= (1 << i);  // Build the value from bits 0-19
      if (i == 20) sign = -1;         // Set the sign bit
    }
  }

  // Convert the raw value to a measurement in millimeters
  result = (value * sign) / 100.0;
  return true;
}

// Adjusts the motor speed based on the caliper measurement
void adjustMotorSpeed(float measurement) {
  // Calculate the number of increments above or below the base measurement
  float increments = (measurement - targetMeasurement) / measurementIncrement;
  // Calculate the speed adjustment
  float speedAdjustment = increments * speedIncrementFactor * baseSpeed;

  if (measurement > targetMeasurement) {
    newSpeed = baseSpeed + speedAdjustment;
  } else if (measurement < targetMeasurement) {
    newSpeed = baseSpeed - speedAdjustment;
  } else {
    newSpeed = baseSpeed;
  }

  // Limit new speed to max speed
  newSpeed = constrain(newSpeed, 0, maxSpeed);
  motor_x.setSpeed(newSpeed);  // Update motor speed
  Serial.print("New Speed: ");
  Serial.println(newSpeed);
}

void zeroCaliper() {
  offset = result;  // Set the offset to the current value to zero out future measurements
  Serial.println("Caliper zeroed.");
}
