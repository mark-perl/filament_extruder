#pragma once
#include <Arduino.h>
#include "pinMap.h"
#include "parameters.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

#define OFF     0
#define AUTO    1
#define MANUAL  2

#define OVERVIEW 1
#define EDIT     2

class userInterface
{
public:
    userInterface();
    ~userInterface();

    static void encoderInterrupt();
    static void buttonPressedInterrupt();

    int readMode();

    void displayInit();
    void overviewDisplay(Parameter params[], Parameter meas_diam);
    void updateMeasDiameter(Parameter meas_diam);
    Parameter updateParameter(Parameter param);
    void resetDial();
    void offDisplay();

    static volatile bool selectPressed;
    static volatile bool enterPressed;
    static volatile int dialValue;

    int mode = OFF;
    int displayMode = OVERVIEW;

    bool updateDisplay = true;

private:
    String intToString(int value, int dp);
    String valueString;
    int stringLen;
    int decimalLoc;
    
    bool updateDisplayValue = false;
    
    static unsigned long buttonPressedMillis;

};