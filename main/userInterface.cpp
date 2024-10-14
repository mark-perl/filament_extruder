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

void userInterface::updateDisplay(String text, float value)
{
    display.clear();

    display.setCursor(0,0);
    display.print(text);

    display.print(": ");

    char valueStr[10];
    dtostrf(value, 3, 0, valueStr);
    display.print(value);
}

Parameter userInterface::updateParameter(Parameter param)
{   
    if (lastParamIndex == -1){
        dial.setPosition(0);
        dialValue = 0;
    }

    if (param.index != lastParamIndex){
        lastParamIndex = param.index;
        
        display.clear();
        display.setCursor(0,1);
        display.print(param.name);

        display.setCursor(0,2);
        display.print(floatToString(param.value));
        display.print(" "+param.units);
    }

    if (dialValue != 0){
        param.value += dialValue;
        lastParamIndex = -1;
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
    if ((millis() - buttonPressedMillis) > 500){
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

String userInterface::floatToString(float value)
{
    char valueString[10];
    dtostrf(value, 0, 0, valueString);
    return valueString;
}