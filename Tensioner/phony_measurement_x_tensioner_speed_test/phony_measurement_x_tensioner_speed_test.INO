//Input fake measurement data in the serial monitor to adjust motor speed. Target measurement is 175mm, default speed at target is 500 step/s

#include <Arduino.h>
#include "AccelStepper.h"

// Arduino Pinouts for CNC shield
const int dirPin = 2;   // Direction pin
const int stepPin = 3;  // Step pin

// Define motor object
AccelStepper motor_x (1, stepPin, dirPin);    //Initialise motor pins

int speed = 1550;  // Initial speed in steps/s (AT 1/16 STEPPING MODE)                      FINE TUNE THIS
float newSpeed = speed;
int maxSpeed = 5000; // Max speed limit

const int targetMeasurement = 175;  // Base measurement in mm
const float speedIncrementFactor = 0.1;  // 10% speed increment     FINE TUNE THIS 
const float measurementIncrement = 0.05;  // 

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);     // Set the maximum milliseconds to wait for serial data
  // No delay for manual input

  // Set up the motor pins
  motor_x.setMaxSpeed(maxSpeed);  // Max speed in steps per second
  motor_x.setSpeed(speed);      // Initial test speed
  Serial.println("Motor ready");
}

void loop() {

  // Read measurement input
  if (Serial.available()) {

    String measurementInput = Serial.readString();
    float measurement = measurementInput.toFloat();  // Read measurement

    // Calculate the number of increments above the base measurement
    float increments = (measurement - targetMeasurement) / measurementIncrement;
    // Calculate the adjusted speed from the base speed
    float speedAdjustment = increments * speedIncrementFactor;
    Serial.print("speedAdjustment value: ");
    Serial.print(speedAdjustment);
    Serial.print(", ");

    if (measurement > targetMeasurement) {
      // Calculate the new speed based on speedAdjustment
      newSpeed = speed + speedAdjustment;
    }

    // Ensure that the speed does not fall below a certain threshold when at base measurement
    if (measurement < targetMeasurement) {
      newSpeed = speed - (-1*speedAdjustment);
    }

    // Limit new speed to max speed
    newSpeed = constrain(newSpeed, 0, maxSpeed);
    motor_x.setSpeed(newSpeed);  // Update motor speed
    // speed = newSpeed;  // Update the current speed variable
    Serial.print("Measurement: ");
    Serial.print(measurement);
    Serial.print(" mm, New Speed: ");
    Serial.println(newSpeed);

    newSpeed = speed;
  }

  motor_x.runSpeed();  // Run the motor at constant speed
}
