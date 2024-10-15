// Test code to control single stepper motor and control speed via laptop serial port

#include <Arduino.h>
#include "AccelStepper.h"
// Ensure AccelStepper lib installed

// Arduino Pinouts for CNC shield
const int xStep = 4;
const int yStep = 3;
const int zStep = 4;
const int xDir = 7;
const int yDir = 6;
const int zDir = 7;
const int Manual = 12;
const int Auto = 11;


// Define motor objects
AccelStepper motor_x (1, zStep, zDir);
AccelStepper motor_y (1, yStep, yDir);

int speed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(xStep, OUTPUT);
  pinMode(xDir, OUTPUT);
  pinMode(Auto,INPUT);
  pinMode(Manual,INPUT);

  // Set parameters for motor
  motor_x.setMaxSpeed(1600*10); // steps/s

  motor_x.setSpeed(speed);      
  Serial.println("begin");
}

void loop() {
  
  if (Serial.available()){
    speed = (Serial.readString()).toInt();
    speed = speed;
    Serial.print("New Speed: ");
    Serial.println(speed);
    motor_x.setSpeed(speed);  
  }
  // if (digitalRead(Auto)){
  //   Serial.println("auto");
  // }
  // if (digitalRead(Manual)){
  //   Serial.println("manual");
  // }
  // delay(50);
  motor_x.runSpeed();       
      
}
