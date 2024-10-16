#include "userInterface.h"

RotaryEncoder dial(Encoder_Dial_A, Encoder_Dial_B, RotaryEncoder::LatchMode::FOUR3);
LiquidCrystal_I2C display(0x27, 4, 20);

volatile bool userInterface::selectPressed = false;
volatile bool userInterface::enterPressed = false;
volatile int userInterface::dialValue = 0;
unsigned long userInterface::buttonPressedMillis = 0;

userInterface::userInterface()
{ 
    pinMode(SW_Select, INPUT);
    pinMode(SW_Enter, INPUT);
    pinMode(SW_Manual, INPUT);
    pinMode(SW_Auto, INPUT);

    attachInterrupt(digitalPinToInterrupt(Encoder_Dial_A), encoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Encoder_Dial_B), encoderInterrupt, CHANGE);
    
    attachInterrupt(digitalPinToInterrupt(SW_Select), buttonPressedInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(SW_Enter), buttonPressedInterrupt, RISING);
}

userInterface::~userInterface()
{

}

void userInterface::displayInit()
{
    display.init();
    display.backlight(); 
    display.clear();
    display.setCursor(1,1);
    display.print("Hello");
}

void userInterface::overviewDisplay(Parameter params[])
{
    if (updateDisplay) {
        updateDisplay = false;

        display.clear();

        for (int i = 0; i < 4; i++) {
            display.setCursor(0,i);
            display.print(params[i].name + ": ");
            display.print(intToString(params[i].value, params[i].divisor));
        }
    }
}

void userInterface::offDisplay()
{
    if (updateDisplay) {
        updateDisplay = false;

        display.clear();
        display.setCursor(0,1);
        display.print("OFF");
    }
}

Parameter userInterface::updateParameter(Parameter param)
{ 
    if (updateDisplay) {
        updateDisplay = false;
        
        display.clear();
        display.setCursor(0,0);
        display.print("EDIT");

        display.setCursor(0,2);
        display.print(param.name);

        display.setCursor(0,3);
        display.print(intToString(param.value, param.divisor));
        display.print(" "+param.units);
    }

    if (dialValue != 0){
        // Update value from dial input
        param.value += dialValue;
        // Reset dial to 0
        resetDial();
        // Prompt to update display
        updateDisplay = true;   
    }

    return param;
}

void userInterface::encoderInterrupt()
{
    dial.tick();
    dialValue = dial.getPosition();
}

void userInterface::buttonPressedInterrupt()
{
    if ((millis() - buttonPressedMillis) > 300){
        if (digitalRead(SW_Select)) {selectPressed = true;}
        if (digitalRead(SW_Enter))  {enterPressed = true;}
        buttonPressedMillis = millis();
    }
}

int userInterface::readMode()
{
    if (digitalRead(SW_Auto)) {
        // Serial.println("Mode: Auto");
        return AUTO;
    }
    else if (digitalRead(SW_Manual)) {
        // Serial.println("Mode: Manual");
        return MANUAL;
    }
    else {
        // Serial.println("Mode: Off");
        return OFF;
    }
}

String userInterface::intToString(int value, int divisor)
{
    valueString = String(value);
    if (divisor == 0) {
        return valueString;
    }
    else {
        stringLen = valueString.length();
        decimalLoc = valueString.length() - divisor;
        return valueString.substring(0,decimalLoc) + '.' + valueString.substring(decimalLoc, stringLen);
    }
}

void userInterface::resetDial()
{
    dial.setPosition(0);
    dialValue = 0;
}