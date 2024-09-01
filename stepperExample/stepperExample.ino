#include <Arduino.h>
#include "AccelStepper.h"
// Ensure AccelStepper lib installed

// Arduino Pinouts for CNC shield
const int xStep = 2;
const int yStep = 3;
const int zStep = 4;
const int aStep = 12;
const int xDir = 5;
const int yDir = 6;
const int zDir = 7;
const int aDir = 13;

// Define motor objects
AccelStepper motor_x (1, xStep, xDir);
AccelStepper motor_y (1, yStep, yDir);


void setup() {

  pinMode(xStep, OUTPUT);
  pinMode(xDir, OUTPUT);
  pinMode(yStep, OUTPUT);
  pinMode(yDir, OUTPUT);

  // Set parameters for motor
  motor_x.setMaxSpeed(800); // steps/s
  motor_x.setAcceleration(10000); // steps/s^2
  motor_y.setMaxSpeed(800); // steps/s
  motor_y.setAcceleration(10000); // steps/s^2

}

void loop() {
  // Suggestion for controlling motor
  // using nonblocking code

  // Avoid blocking code to allow system to run on single controller
  
  // Speed based control
  motor_x.runSpeed();                   // Needs to be called constantly for motor to run

  int speed = 100;
  motor_x.setSpeed(speed);              // Sets speed to run. Can update as required

  
  // Position based control
  motor_y.runSpeedToPosition();         // Needs to be called constantly for motor to run

  motor_y.moveTo(1000);                 // Sets goal absolute position (Use .move for relative position)
  motor_y.setSpeed(speed);              // Must be called after moveTo for speed control

}
