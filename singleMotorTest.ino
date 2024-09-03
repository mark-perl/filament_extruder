// Test code to control single stepper motor and control speed via laptop serial port

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

int speed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(xStep, OUTPUT);
  pinMode(xDir, OUTPUT);

  // Set parameters for motor
  motor_x.setMaxSpeed(1600*10); // steps/s

  motor_x.setSpeed(speed);      

}

void loop() {
  
  if (Serial.available()){
    speed = (Serial.readString()).toInt();
    speed = speed;
    Serial.print("New Speed: ");
    Serial.println(speed);
    motor_x.setSpeed(speed * 8);  
  }

  motor_x.runSpeed();       
      
}
