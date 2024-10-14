#pragma once
#include "pinMap.h"
#include "parameters.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

#define OFF 0
#define AUTO 1
#define MANUAL 2

class userInterface
{
public:
    userInterface();
    ~userInterface();

    static void encoderInterrupt();
    // User Inputs
    static void buttonPressedInterrupt();
    int readMode();

    // LCD Display
    void displayInit();
    void updateDisplay(String text, float value);
    Parameter updateParameter(Parameter param);
    // void displayOverview(int mode);

    static volatile bool selectPressed;
    static volatile bool enterPressed;
    // static volatile bool limitPressed;
    
    int mode = OFF;
    static volatile int dialValue;

private:
    String floatToString(float value);

    int lastParamIndex = -1;
    static unsigned long buttonPressedMillis;


};