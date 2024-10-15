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
    void updateDisplay(String text, float value);
    Parameter updateParameter(Parameter param);

    static volatile bool selectPressed;
    static volatile bool enterPressed;
    static volatile int dialValue;

    int mode = OFF;
    int displayMode = OVERVIEW;

private:
    String floatToString(float value);

    int lastParamIndex = -1;
    static unsigned long buttonPressedMillis;


};